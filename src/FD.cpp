#include <omp.h> 
#include "../lib/SeisMath/include/SeisMath.hpp"
#include <string>
#include <utility>
#include <iomanip> 
#include <sstream>

void getSnapshots(const vec1d<float> &Upre, uint16_t n, uint16_t Nx, uint16_t Nz) {
    std::ostringstream filename_stream;
    filename_stream << "data/snapshots/snapshot_" << std::setw(5) << std::setfill('0') << n << ".bin";

    std::string data_path = filename_stream.str();

    seismath::write1dVecAs2d(Upre, Nx, Nz, data_path);
    std::cout << "Successfully saved " << data_path << std::endl; 
}

// ensure that the user only use float type
vec1d<float> compute_2D(const vec1d<float>& property, uint16_t Nx, uint16_t Nz, uint16_t Nt,
                        float dx, float dz, float dt, int pos0, int posf, const vec1d<float>& model,
                        bool snap_bool, uint16_t snap_num) {

    vec1d<float> Upas(Nx * Nz, 0.0f);
    vec1d<float> Upre(Nx * Nz, 0.0f);

    const float const_dx = 1 / (5040.0f * dx * dx);
    const float const_dz = 1 / (5040.0f * dz * dz);

    const uint16_t snap_ratio = Nt / snap_num;
    const size_t source_idx = pos0 * Nx + posf;

    for (size_t n = 0; n < Nt; ++n) {
        Upre[source_idx] += property[n] / (dx * dx);

        #pragma omp parallel for collapse(2)
        for (size_t i = 4; i < static_cast<size_t>(Nz) - 4; ++i) {
            for (size_t j = 4; j < static_cast<size_t>(Nx) - 4; ++j) {
                size_t idx = i * Nx + j;

                const float d2u_dx2 =
                    (-9.0f * Upre[idx - 4 * Nx] + 128.0f * Upre[idx - 3 * Nx] - 1008.0f * Upre[idx - 2 * Nx] +
                     8064.0f * Upre[idx - Nx] - 14350.0f * Upre[idx] + 8064.0f * Upre[idx + Nx] -
                     1008.0f * Upre[idx + 2 * Nx] + 128.0f * Upre[idx + 3 * Nx] - 9.0f * Upre[idx + 4 * Nx]) *
                    const_dx;

                const float d2u_dz2 =
                    (-9.0f * Upre[idx - 4] + 128.0f * Upre[idx - 3] - 1008.0f * Upre[idx - 2] +
                     8064.0f * Upre[idx - 1] - 14350.0f * Upre[idx] + 8064.0f * Upre[idx + 1] -
                     1008.0f * Upre[idx + 2] + 128.0f * Upre[idx + 3] - 9.0f * Upre[idx + 4]) *
                    const_dz;

                Upas[idx] = (d2u_dx2 + d2u_dz2) * (dt * dt * model[idx] * model[idx]) +
                            2 * Upre[idx] - Upas[idx];            
            }
        }

        if (snap_bool && (n % snap_ratio == 0)) {
            getSnapshots(Upre, n, Nx, Nz);
        }

        std::swap(Upas, Upre);
    }

    return Upre; 
}


