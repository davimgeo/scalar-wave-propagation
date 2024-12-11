#include "../include/modelling.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <omp.h>

static void getSnapshots(
    const std::vector<std::vector<float>>& Upre, 
    int n, 
    int Nx, 
    int Nz, 
    int extent,
    Utils* utils
) {
    std::ostringstream filename_stream;
    filename_stream << "data/snapshots/snapshot_" << std::setw(5) << std::setfill('0') << n << ".bin";
    std::string data_path = filename_stream.str();
    utils->writePaddedArray(Upre);
    std::cout << "Successfully saved " << data_path << '\n'; 
}

vec::vec2d<float> FD::compute2d() const {
    int Nx = modelConfig->Nx + modelConfig->extent;
    int Nz = modelConfig->Nz + modelConfig->extent;
    int pos0 = modellingConfig->pos0 + modelConfig->extent/2; 
    int posf = modellingConfig->posf + modelConfig->extent/2; 
    Utils* utils;
    vec::vec2d<float> model_pad = utils->pad2d(model);
    vec::vec2d<float> Upas(Nz, vec::vec1d<float>(Nx, 0.0f));
    vec::vec2d<float> Upre(Nz, vec::vec1d<float>(Nx, 0.0f));
    const float const_dx = 1.0f / (5040.0f * modelConfig->dx * modelConfig->dx);
    const float const_dz = 1.0f / (5040.0f * modelConfig->dz * modelConfig->dz);
    const int snap_ratio = (modellingConfig->snap_num > 0) ? modellingConfig->Nt / modellingConfig->snap_num : 0;

    for (int n = 0; n < modellingConfig->Nt; ++n) {
        if (!wavelet.empty())
            Upre[pos0][posf] += wavelet[n] / (modelConfig->dx * modelConfig->dx);

#pragma omp parallel for collapse(2)
        for (int i = 4; i < Nz - 4; ++i) {
            for (int j = 4; j < Nx - 4; ++j) {
                const float d2u_dx2 =
                    (-9.0f   * Upre[i-4][j] + 128.0f  * Upre[i-3][j] - 1008.0f * Upre[i-2][j] +
                     8064.0f * Upre[i-1][j] - 14350.0f* Upre[i][j]   + 8064.0f  * Upre[i+1][j] -
                     1008.0f * Upre[i+2][j] + 128.0f  * Upre[i+3][j] - 9.0f     * Upre[i+4][j]) * const_dx;

                const float d2u_dz2 =
                    (-9.0f   * Upre[i][j-4] + 128.0f  * Upre[i][j-3] - 1008.0f * Upre[i][j-2] +
                     8064.0f * Upre[i][j-1] - 14350.0f* Upre[i][j]   + 8064.0f  * Upre[i][j+1] -
                     1008.0f * Upre[i][j+2] + 128.0f  * Upre[i][j+3] - 9.0f     * Upre[i][j+4]) * const_dz;

                Upas[i][j] = (d2u_dx2 + d2u_dz2) * (modellingConfig->dt * modellingConfig->dt * model_pad[i][j] *
                    model_pad[i][j]) + 2.0f * Upre[i][j] - Upas[i][j];            
            }
        }

        if (modellingConfig->snap_bool && snap_ratio > 0 && (n % snap_ratio == 0)) {
            getSnapshots(Upre, n, Nx, Nz, modelConfig->extent, utils);
        }

        std::swap(Upas, Upre);
    }

    return Upre; 
}

