#ifndef WAVELET_HPP
#define WAVELET_HPP

#include <vector>
#include "../src/globvar.hpp"

class Wavelets {
public:
    explicit Wavelets(Config* c);

    std::vector<float> generateRicker() const;

    std::vector<float> generateOrmsby() const;

private:
    Config* _config;  
};

#endif // !WAVELET_HPP

