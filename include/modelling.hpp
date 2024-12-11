#ifndef MODELLING_HPP
#define MODELLING_HPP

#include <vector>
#include <cmath>
#include "globvar.hpp"

class IWavefield2D {
protected:
    WaveletConfig* waveletConfig;
    ModelConfig* modelConfig;
    ModellingConfig* modellingConfig;
    ConfigTypes* configTypes;

public:
    IWavefield2D() 
        : waveletConfig(nullptr), modelConfig(nullptr), modellingConfig(nullptr), configTypes(nullptr) {}
    virtual ~IWavefield2D() = default;

    virtual vec::vec1d<float> generateWavelet() const = 0;
    virtual vec::vec2d<float> computeFD() const = 0;

    void setConfigs(WaveletConfig* wavelet, ModelConfig* model, ModellingConfig* modelling, ConfigTypes* config) {
        waveletConfig = wavelet;
        modelConfig = model;
        modellingConfig = modelling;
        configTypes = config;
    }
};

class Utils {
public:
    // Load a 2D vector from some source (dummy implementation)
    vec::vec2d<float> load2dvec(int rows, int cols) const;

    // Write a 2D vector somewhere (dummy implementation)
    void write2dvec(const vec::vec2d<float>& v) const;

    vec::vec2d<float> pad2d(const vec::vec2d<float>& arr) const;

    void writePaddedArray(const std::vector<std::vector<float>>& Upre);
};

class Wavelets : public IWavefield2D {
public:
    vec::vec1d<float> generateRicker() const;
    vec::vec1d<float> generateOrmsby() const;

    vec::vec1d<float> generateWavelet() const override {
        if (configTypes->waveletType == 0) return generateRicker();
        if (configTypes->waveletType == 1) return generateOrmsby();
        return {};
    }

    vec::vec2d<float> computeFD() const override {
        // Dummy: no FD computation here
        return {};
    }
};

class FD : public IWavefield2D {
protected:
    vec::vec1d<float> wavelet;
    vec::vec2d<float> model;

public:
    virtual ~FD() = default;
    vec::vec1d<float> generateWavelet() const override;

    void setWavelet() {
        wavelet = generateWavelet();  
    }

    void setModel() {
        model = load2dvec(modelConfig->Nz, modelConfig->Nx);
    }

    vec::vec2d<float> compute2d() const;

    vec::vec2d<float> computeFD() const override {
        return compute2d();
    }
};

class Wavefield2DFactory {
public:
    Wavefield2DFactory() : configTypes(nullptr), waveletConfig(nullptr), modelConfig(nullptr), modellingConfig(nullptr), waveletInterface(nullptr) {}
    ~Wavefield2DFactory() {
        delete waveletInterface;
    }

    void setConfigs(WaveletConfig* w, ModelConfig* m, ModellingConfig* mo, ConfigTypes* c) {
        waveletConfig = w;
        modelConfig = m;
        modellingConfig = mo;
        configTypes = c;
    }

    void initialize() {
        waveletInterface = _chooseWavelet();
        if (waveletInterface) {
            waveletInterface->setConfigs(waveletConfig, modelConfig, modellingConfig, configTypes);
        }
    }

    vec::vec1d<float> generateWavelet() {
        if (waveletInterface) return waveletInterface->generateWavelet();
        return {};
    }

    vec::vec2d<float> computeFD() {
        if (waveletInterface) return waveletInterface->computeFD();
        return {};
    }

private:
    ConfigTypes* configTypes;
    WaveletConfig* waveletConfig;
    ModelConfig* modelConfig;
    ModellingConfig* modellingConfig;
    IWavefield2D* waveletInterface;

    IWavefield2D* _chooseWavelet() {
        switch (configTypes->waveletType) {
        case 0:
            return new Wavelets();
        case 1:
            return new FD();
        default:
            return nullptr;
        }
    }
};

#endif // MODELLING_HPP

