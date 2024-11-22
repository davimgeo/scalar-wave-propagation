#pragma once

#include "../lib/SeisMath/include/SeisMath.hpp"

vec1d<float> compute_2D(const vec1d<float>& property, uint16_t Nx, uint16_t Nz, uint16_t Nt,
                        float dx, float dz, float dt, int pos0, int posf, const vec1d<float>& model,
                        bool snap_bool, uint16_t snap_num);

