#ifndef GLOBVAR_HPP
#define GLOBVAR_HPP
/*
* Provides declaration of the parameters
* captured in the config/parameters.txt file
*/

#include <vector>
#include "parameters.hpp"

template <typename T>
using vec1d = std::vector<T>;

template <typename T>
using vec2d = std::vector<std::vector<T>>;

typedef struct Config {
   
    /*=============================
     * Model Parameters
     * ============================
     */

    /* width of the model */
    int Nx;
    /* height of the model */
    int Nz;
    /* spatial grid spacing */
    float dx;
    float dz;
    /* extension of the model by samples */
    int extent;
    float factor;

    /* =============================
     * Modelling Parameters
     *==============================
     */

    /* time samples of the simulation */
    int Nt;
    /* time grid spacing of the simulation*/
    float dt;
    /* position in x for the source */
    int pos0;
    /* position in y for the source */
    int posf;
    /* max frequency of the ricker wavelet */
    float fmax;

    /* activate snapshots in the simulation */
    bool snap_bool;
    /* numbers of snapshots per simulation*/
    int snap_num;

    /* array of frequencies for the ormsby wavelet */
    std::vector<float> freqs;

    /* modify parameters from struct by acessing the parameters hash map*/
    void extractParameters(Parameters& params) {
        Nx = params.get<int>("Nx");
        Nz = params.get<int>("Nz");
        dx = params.get<float>("dx");
        dz = params.get<float>("dz");

        extent = params.get<int>("Extent");
        factor = params.get<float>("Factor");

        Nt = params.get<int>("Nt");
        dt = params.get<float>("dt");
        pos0 = params.get<int>("pos0");
        posf = params.get<int>("posf");
        fmax = params.get<float>("fmax");

        snap_bool = params.get<bool>("snap_bool");
        snap_num = params.get<int>("snap_num");

        freqs = params.get<std::vector<float>>("freqs");
    }

} config;

#endif // !GLOBVAR_HPP

