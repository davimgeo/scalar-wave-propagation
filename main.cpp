#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "include/FD.hpp"
#include "lib/SeisMath/include/SeisMath.hpp"

int main() {
    uint16_t Nx = 1150, Nz = 648, Nt = 5001;
    float dx = 10.0f, dz = 10.0f, dt = 0.001f;
    int pos0 = 20, posf = Nx * 0.5;
    float fmax = 10.0f;
    vec1d<float> freqs = {5.0f, 10.0f, 40.0f, 45.0f};

    bool snap_bool = true;
    uint16_t snap_num = 101;

    size_t rows = 648;
    size_t cols = 1150;

    seismath::Wavelets wavelet_generator(Nt, dt, fmax, freqs);

    vec1d<float> ricker = wavelet_generator.generateRicker();

    vec1d<float> model = seismath::loadFlattened2dVec<float>(rows, cols, "data/model_vp_2d_1150x648.bin");

    vec1d<float> result = compute_2D(ricker, Nx, Nz, Nt, dx, dz, dt, pos0, posf, model, snap_bool, snap_num);

    std::string output_file = "data/wavefield_result.bin";

    seismath::write1dVecAs2d(result, Nx, Nz, output_file);

    std::cout << "Wavefield result saved to " << output_file << '\n';

    return 0;
}

