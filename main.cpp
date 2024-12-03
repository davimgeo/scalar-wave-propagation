#include <string>
#include "src/globvar.hpp"
#include "src/parameters.hpp"
#include "include/wavelet.hpp"
#include "include/aux.hpp"
#include "include/FD.hpp"

int main() {
    std::string PATH = "config/parameters.txt";
    Parameters params(PATH);
    params.load();

    Config c;
    c.extractParameters(params);

    Wavelets wavelets(&c);

    vec1d<float> ricker = wavelets.generateRicker();

    vec2d<float> model = load2dVec<float>(c.Nx, c.Nz, "data/model_vp_2d_1150x648.bin");
    vec2d<float> model_extent = pad2d(model, c.extent);

    vec2d<float> result = compute_2D(
        ricker, c.Nx, c.Nz, c.Nt, c.dx, 
        c.dz, c.dt, c.pos0, c.posf, 
        model_extent, c.snap_bool, c.snap_num
    );

    write1dVecAs2d(result, c.Nx, c.Nz, "data/wavelefiled_result.bin");

    return 0;
}

