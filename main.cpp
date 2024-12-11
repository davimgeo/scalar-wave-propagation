#include <string>
#include <iostream>
#include "include/parameters.hpp"
#include "include/modelling.hpp"
#include "include/globvar.hpp"

int main() {
    // Load parameters from your configuration file
    Parameters params("config/parameters.txt");
    params.load();

    // Create configuration structs and extract parameters
    WaveletConfig wconfig;
    wconfig.extractParameters(params);

    ModelConfig mconfig;
    mconfig.extractParameters(params);

    ModellingConfig modconfig;
    modconfig.extractParameters(params);

    ConfigTypes ctype;
    ctype.extractTypeParameters(params);

    // Initialize the factory
    Wavefield2DFactory factory;
    factory.setConfigs(&wconfig, &mconfig, &modconfig, &ctype);
    factory.initialize();

    // Generate wavelet and compute FD
    vec::vec1d<float> w = factory.generateWavelet();
    vec::vec2d<float> result = factory.computeFD();

    std::cout << "Computation completed." << std::endl;

    return 0;
}

