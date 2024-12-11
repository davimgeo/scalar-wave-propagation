#include "../include/modelling.hpp"
#include <fstream>
#include <stdexcept>

void Utils::write2dvec(const vec::vec2d<float>& vec) const {
    std::ofstream fout(modelConfig->PATH, std::ios::out | std::ios::binary);

    if (!fout.is_open()) {
        throw std::runtime_error("Could not open file. Please verify the file path.");
    }

    for (const auto& row : vec) {
        for (const auto& val : row) {
            fout.write(reinterpret_cast<const char*>(&val), sizeof(float));
        }
    }

    fout.close();
}

vec::vec2d<float> Utils::load2dvec(int rows, int cols) const {
    std::ifstream fin(modelConfig->PATH, std::ios::in | std::ios::binary);

    if (!fin.is_open()) {
        throw std::runtime_error("Could not open file. Please verify the file path.");
    }

    vec::vec2d<float> result(rows, vec::vec1d<float>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fin.read(reinterpret_cast<char*>(&result[i][j]), sizeof(float));
        }
    }

    fin.close();
    return result;
}

vec::vec2d<float> Utils::pad2d(const vec::vec2d<float>& arr) const {
    /* ensures the grid extent is even to avoid error */
    if (modelConfig->extent % 2) {
        throw std::invalid_argument("Extent must be an even integer.");
    }

    int extent = modelConfig->extent;
    extent *= 2;

    /* get arr dimensions */
    int rows = static_cast<int>(arr.size());
    int cols = static_cast<int>(arr[0].size());
    int half_ext = modelConfig->extent / 2;

    /* Defines result array of the pad */
    vec::vec2d<float> padded_arr(rows + extent, vec::vec1d<float>(cols + extent, 0.0f));

    /* Fills the padded array with the original array */
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            padded_arr[i + half_ext][j + half_ext] = arr[i][j];
        }
    }

    /* Fills the top of the padded array */
    for (int i = 0; i < half_ext; i++) {
        for (int j = 0; j < cols; j++) {
            /* uses the first line to pad */
            padded_arr[i][j + half_ext] = arr[0][j];
        }
    }

    /* Fills the bottom of the padded array */
    for (int i = rows + half_ext; i < rows + extent; i++) {
        for (int j = 0; j < cols; j++) {
            /* uses the last line to pad */
            padded_arr[i][j + half_ext] = arr[rows - 1][j];
        }
    }

    /* Fills the right of the padded array */
    for (int i = 0; i < rows; i++) {
        for (int j = cols + half_ext; j < cols + extent; j++) {
            /* uses the last column to pad */
            padded_arr[i + half_ext][j] = arr[i][cols - 1];
        }
    }

    /* Fills the left of the padded array */
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < half_ext; j++) {
            /* uses the first column to pad */
            padded_arr[i + half_ext][j] = arr[i][0];
        }
    }

    /* Fills the bottom right corner of the padded array */
    for (int i = rows + half_ext; i < rows + extent; i++) {
        for (int j = cols + half_ext; j < cols + extent; j++) {
            /* uses the bottom right corner element pad */
            padded_arr[i][j] = arr[rows - 1][cols - 1];
        }
    }

    /* Fills the bottom left corner of the padded array */
    for (int i = rows + half_ext; i < rows + extent; i++) {
        for (int j = 0; j < half_ext; j++) {
            /* uses the bottom left corner element pad */
            padded_arr[i][j] = arr[rows - 1][0];
        }
    }

    /* Fills the top right corner of the padded array */
    for (int i = 0; i < half_ext; i++) {
        for (int j = cols + half_ext; j < cols + extent; j++) {
            /* uses the top right corner element pad */
            padded_arr[i][j] = arr[0][cols - 1];
        }
    }

    /* Fills the top left corner of the padded array */
    for (int i = 0; i < half_ext; i++) {
        for (int j = 0; j < half_ext; j++) {
            /* uses the top left corner element pad */
            padded_arr[i][j] = arr[0][0];
        }
    }

    return padded_arr;
}

void Utils::writePaddedArray(const std::vector<std::vector<float>>& Upre) {
    std::ofstream fout(modelConfig->PATH, std::ios::out | std::ios::binary);

    if (!fout.is_open()) {
        throw std::runtime_error("Could not open file. Please verify the file path.");
    }

    int half_ext = modelConfig->extent / 2;
    for (size_t i = 0; i < Upre.size() - modelConfig->extent; i++) {
        for (size_t j = 0; j < Upre[0].size() - modelConfig->extent; j++) {
            fout.write(reinterpret_cast<const char*>(&Upre[i + half_ext][j + half_ext]), sizeof(float));
        }
    }

    fout.close();
}

