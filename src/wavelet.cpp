#include <cmath>
#include "../include/modelling.hpp"

// Define a free-function sinc
template <typename T>
static constexpr T sinc(T x) {
    if (x == T(0)) {
        return T(1);
    }
    return std::sin(x) / x;
}

// generateRicker and generateOrmsby implementations as per the WaveletConfig and ModellingConfig pointers
vec::vec1d<float> Wavelets::generateRicker() const {
    vec::vec1d<float> wavelet(waveletConfig->wNt, 0.0f);
    float t0 = 2.0f * M_PI / waveletConfig->fmax;

    for (int i = 0; i < waveletConfig->wNt; i++) {
        float t = i * modellingConfig->dt - t0;
        float arg = (M_PI * waveletConfig->fmax * t) * (M_PI * waveletConfig->fmax * t);
        wavelet[i] = (1.0f - 2.0f * arg) * std::exp(-arg);
    }

    return wavelet;
}

vec::vec1d<float> Wavelets::generateOrmsby() const {
    vec::vec1d<float> wavelet(waveletConfig->wNt, 0.0f);

    float t0 = (waveletConfig->wNt * modellingConfig->dt) / 2.0f;

    // Ensure waveletConfig->freqs has the required size and is set
    // Assuming waveletConfig->freqs: [f1, f2, f3, f4], with f1 < f2 < f3 < f4
    float f1 = waveletConfig->freqs[0];
    float f2 = waveletConfig->freqs[1];
    float f3 = waveletConfig->freqs[2];
    float f4 = waveletConfig->freqs[3];

    for (int i = 0; i < waveletConfig->wNt; i++) {
        float t = i * modellingConfig->dt - t0;

        float arg1 = std::pow(sinc(M_PI * f4 * t), 2) * std::pow(M_PI * f4, 2) / (M_PI * (f4 - f3));
        float arg2 = std::pow(sinc(M_PI * f3 * t), 2) * std::pow(M_PI * f3, 2) / (M_PI * (f4 - f3));
        float arg3 = std::pow(sinc(M_PI * f2 * t), 2) * std::pow(M_PI * f2, 2) / (M_PI * (f2 - f1));
        float arg4 = std::pow(sinc(M_PI * f1 * t), 2) * std::pow(M_PI * f1, 2) / (M_PI * (f2 - f1));

        wavelet[i] = (arg1 - arg2) - (arg3 - arg4);
    }

    return wavelet;
}

