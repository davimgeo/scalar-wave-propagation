#ifndef FD_HPP
#define FD_HPP

#include <vector>

std::vector<std::vector<float>> compute_2D(
    const std::vector<float>& property, int Nx, int Nz, 
    int Nt, float dx, float dz, float dt, int pos0, int posf, 
    const std::vector<std::vector<float>>& model, bool snap_bool, int snap_num
);
#endif // !FD_HPP


