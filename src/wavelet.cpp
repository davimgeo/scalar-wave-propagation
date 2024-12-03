#include <cmath>
#include "../include/wavelet.hpp"

template <typename T>
static constexpr T sinc(T x) {
    if (x == T(0)) {
        return T(1);
    }
    return std::sin(x) / x;
}

Wavelets::Wavelets(Config* c) {
    _config = c; 
}

std::vector<float> Wavelets::generateRicker() const {
    std::vector<float> wavelet(_config->Nt, 0.0f);  

    float t0 = 2.0f * M_PI / _config->fmax;

    for (int i = 0; i < _config->Nt; i++) {
        float t = i * _config->dt - t0;
        float arg = (M_PI * _config->fmax * t) * (M_PI * _config->fmax * t);
        wavelet[i] = (1.0f - 2.0f * arg) * std::exp(-arg);  
    }

    return wavelet;
}

std::vector<float> Wavelets::generateOrmsby() const {
    float t0 = (_config->Nt * _config->dt) / 2.0f;  
    std::vector<float> wavelet(_config->Nt, 0.0f);

    for (int i = 0; i < _config->Nt; i++) {
        float t = i * _config->dt - t0;

        float arg1 = pow(sinc(M_PI * _config->freqs[3] * t), 2) * pow(M_PI * _config->freqs[3], 2) / 
                     (M_PI * _config->freqs[3] - M_PI * _config->freqs[2]);
        float arg2 = pow(sinc(M_PI * _config->freqs[2] * t), 2) * pow(M_PI * _config->freqs[2], 2) / 
                     (M_PI * _config->freqs[3] - M_PI * _config->freqs[2]);
        float arg3 = pow(sinc(M_PI * _config->freqs[1] * t), 2) * pow(M_PI * _config->freqs[1], 2) / 
                     (M_PI * _config->freqs[1] - M_PI * _config->freqs[0]);
        float arg4 = pow(sinc(M_PI * _config->freqs[0] * t), 2) * pow(M_PI * _config->freqs[0], 2) / 
                     (M_PI * _config->freqs[1] - M_PI * _config->freqs[0]);

        wavelet[i] = (arg1 - arg2) - (arg3 - arg4);
    }

    return wavelet;
}

