#ifndef GLOBVAR_HPP
#define GLOBVAR_HPP

/*
* Provides declaration of the parameters
* captured in the config/parameters.txt file
* and declaration of common alias.
*/

#include <vector>
#include "parameters.hpp"

namespace vec
{

template <typename T>
using vec1d = std::vector<T>;

template <typename T>
using vec2d = std::vector<std::vector<T>>;

} // namespace vec

struct WaveletConfig
{
    /* time samples of the wavelet */
    int wNt;
    /* max frequency of the ricker wavelet */
    float fmax;
    /* array of frequencies for the ormsby wavelet */
    std::vector<float> freqs;

    void extractParameters(const Parameters& params)
    {
        wNt = params.get<int>("Nt");
        fmax = params.get<float>("fmax");
        freqs = params.get<std::vector<float>>("freqs");
    }
};

struct ModelConfig
{
    /* path where the model is located */
    std::string PATH;

    /* width of the model */
    int Nx;
    /* height of the model */
    int Nz;
    /* spatial grid spacing */
    float dx;
    float dz;
    /* extension of the model by samples */
    int extent;
    /* factor which pressure will be multiplied each iteration */
    float factor;

    void extractParameters(const Parameters& params)
    {
        PATH = params.get<std::string>("Path");
        Nx = params.get<int>("Nx");
        Nz = params.get<int>("Nz");
        dx = params.get<float>("dx");
        dz = params.get<float>("dz");
        extent = params.get<int>("Extent");
        factor = params.get<float>("Factor");
    }
};

struct ModellingConfig
{
    /* time samples of the simulation */
    int Nt;
    /* time grid spacing of the simulation */
    float dt;
    /* position in x for the source */
    int pos0;
    /* position in y for the source */
    int posf;

    /* activate snapshots in the simulation */
    bool snap_bool;
    /* numbers of snapshots per simulation */
    int snap_num;

    void extractParameters(const Parameters& params)
    {
        Nt = params.get<int>("Nt");
        dt = params.get<float>("dt");
        pos0 = params.get<int>("pos0");
        posf = params.get<int>("posf");
        snap_bool = params.get<bool>("snap_bool");
        snap_num = params.get<int>("snap_num");
    }
};

struct ConfigTypes
{
    int waveletType;

    void extractTypeParameters(const Parameters& params)
    {
        waveletType = params.get<int>("Wavelet_Type");
    }
};

#endif // !GLOBVAR_HPP

