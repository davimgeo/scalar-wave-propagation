#include <omp.h> 
#include <iostream>
#include <string>
#include <utility>
#include <iomanip> 
#include <sstream>
#include "../include/aux.hpp"

static void getSnapshots(const std::vector<std::vector<float>>& Upre, int n, int Nx, int Nz) {
    std::ostringstream filename_stream;
    filename_stream << "data/snapshots/snapshot_" << std::setw(5) << std::setfill('0') << n << ".bin";

    std::string data_path = filename_stream.str();

    write2dVec(Upre, data_path);
    std::cout << "Successfully saved " << data_path << '\n'; 
}

// ensure that the user only uses float type
std::vector<std::vector<float>> compute_2D(const std::vector<float>& property, int Nx, int Nz, int Nt,
                                           float dx, float dz, float dt, int pos0, int posf,
                                           const std::vector<std::vector<float>>& model,
                                           bool snap_bool, int snap_num) {

    std::vector<std::vector<float>> Upas(Nz, std::vector<float>(Nx, 0.0f));
    std::vector<std::vector<float>> Upre(Nz, std::vector<float>(Nx, 0.0f));

    const float const_dx = 1 / (5040.0f * dx * dx);
    const float const_dz = 1 / (5040.0f * dz * dz);

    const int snap_ratio = Nt / snap_num;

    for (int n = 0; n < Nt; ++n) {
        Upre[pos0][posf] += property[n] / (dx * dx);

        #pragma omp parallel for collapse(2)
        for (int i = 4; i < Nz - 4; ++i) {
            for (int j = 4; j < Nx - 4; ++j) {

                const float d2u_dx2 =
                    (-9.0f * Upre[i - 4][j] + 128.0f * Upre[i - 3][j] - 1008.0f * Upre[i - 2][j] +
                     8064.0f * Upre[i - 1][j] - 14350.0f * Upre[i][j] + 8064.0f * Upre[i + 1][j] -
                     1008.0f * Upre[i + 2][j] + 128.0f * Upre[i + 3][j] - 9.0f * Upre[i + 4][j]) *
                    const_dx;

                const float d2u_dz2 =
                    (-9.0f * Upre[i][j - 4] + 128.0f * Upre[i][j - 3] - 1008.0f * Upre[i][j - 2] +
                     8064.0f * Upre[i][j - 1] - 14350.0f * Upre[i][j] + 8064.0f * Upre[i][j + 1] -
                     1008.0f * Upre[i][j + 2] + 128.0f * Upre[i][j + 3] - 9.0f * Upre[i][j + 4]) *
                    const_dz;

                Upas[i][j] = (d2u_dx2 + d2u_dz2) * (dt * dt * model[i][j] * model[i][j]) +
                             2 * Upre[i][j] - Upas[i][j];            
            }
        }

        if (snap_bool && (n % snap_ratio == 0)) {
            getSnapshots(Upre, n, Nx, Nz);
        }

        std::swap(Upas, Upre);
    }

    return Upre; 
}

