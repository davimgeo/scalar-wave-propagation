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

    bool snap_bool = true;
    uint16_t snap_num = 20;

    size_t rows = 1150;
    size_t cols = 648;

    vec1d<float> property = generateRicker(Nt, dt, fmax);

    vec1d<float> model = loadFlattened2dVec<float>(rows, cols, "data/model_vp_2d_1150x648.bin");

    vec1d<float> result = compute_2D(property, Nx, Nz, Nt, dx, dz, dt, pos0, posf, model, snap_bool, snap_num);

    std::string output_file = "data/wavefield_result.bin";
  
    write1dVecAs2d(result, Nx, Nz, output_file);

    std::cout << "Wavefield result saved to " << output_file << std::endl;

    return 0;
}

