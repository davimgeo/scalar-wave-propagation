#include <omp.h> 
#include "../lib/SeisMath/include/SeisMath.hpp"
#include <string>
#include <utility>

void getSnapshots(bool snap_bool, uint16_t snap_ratio, const vec1d<float> &Ufut, uint16_t n, uint16_t Nx, uint16_t Nz) {
  std::string data_path = "data/snapshots/snapshot_" + std::to_string(n) + ".bin";
  if (snap_bool && (!(n % snap_ratio))) {
    write1dVecAs2d(Ufut, Nx, Nz, data_path);
    std::cout << "Successfully saved " << data_path << std::endl; 
  }
}

// ensure that the user only use float type
vec1d<float> compute_2D(const vec1d<float>& property, uint16_t Nx, uint16_t Nz, uint16_t Nt,
                        float dx, float dz, float dt, int pos0, int posf, const vec1d<float>& model,
                        bool snap_bool, uint16_t snap_num) {

    vec1d<float> Upas(Nx * Nz, 0.0f);
    vec1d<float> Upre(Nx * Nz, 0.0f);
    vec1d<float> Ufut(Nx * Nz, 0.0f);

    float const_dx = 1 / (5040.0f * dx * dx);
    float const_dz = 1 / (5040.0f * dz * dz);

    uint16_t snap_ratio = Nt / snap_num;

    for (size_t n = 0; n < Nt; ++n) {
        size_t source_idx = pos0 * Nx + posf;
        Upre[source_idx] += property[n] / (dx * dx);

        #pragma omp parallel for collapse(2)
        for (size_t i = 4; i < Nz - 4; ++i) {
            for (size_t j = 4; j < Nx - 4; ++j) {
                size_t idx = i * Nx + j;

                float d2u_dx2 =
                    (-9 * Upre[idx - 4 * Nx] + 128 * Upre[idx - 3 * Nx] - 1008 * Upre[idx - 2 * Nx] +
                     8064 * Upre[idx - Nx] - 14350 * Upre[idx] + 8064 * Upre[idx + Nx] -
                     1008 * Upre[idx + 2 * Nx] + 128 * Upre[idx + 3 * Nx] - 9 * Upre[idx + 4 * Nx]) *
                    const_dx;

                float d2u_dz2 =
                    (-9 * Upre[idx - 4] + 128 * Upre[idx - 3] - 1008 * Upre[idx - 2] +
                     8064 * Upre[idx - 1] - 14350 * Upre[idx] + 8064 * Upre[idx + 1] -
                     1008 * Upre[idx + 2] + 128 * Upre[idx + 3] - 9 * Upre[idx + 4]) *
                    const_dz;

                Upas[idx] = (d2u_dx2 + d2u_dz2) * (dt * dt * model[idx] * model[idx]) +
                            2 * Upre[idx] - Upas[idx];
            }
        }

        if (snap_bool) {
            getSnapshots(snap_bool, snap_ratio, Upre, n, Nx, Nz);
        }

        std::swap(Upas, Upre);
    }

    return Upre; 
}


