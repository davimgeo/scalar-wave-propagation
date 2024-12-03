#ifndef AUX_HPP
#define AUX_HPP

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

template <typename T>
void write2dVec(const std::vector<std::vector<T>>& vec, const std::string& PATH) {
    std::ofstream fout(PATH, std::ios::out | std::ios::binary);

    if (!fout.is_open()) {
        throw std::runtime_error("Could not open file. Please verify the file path.");
    }

    for (const auto& row : vec) {
        for (const auto& val : row) {
            fout.write(reinterpret_cast<const char*>(&val), sizeof(T));
        }
    }

    fout.close();
}

template <typename T>
void write1dVecAs2d(const std::vector<T>& vec, int rows, int cols, const std::string& PATH) {
    std::ofstream fout(PATH, std::ios::out | std::ios::binary);

    if (!fout.is_open()) {
        throw std::runtime_error("Could not open file. Please verify the file path.");
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fout.write(reinterpret_cast<const char*>(&vec[i * cols + j]), sizeof(T));
        }
    }

    fout.close();
}

template <typename T>
std::vector<std::vector<T>> load2dVec(int rows, int cols, const std::string& PATH) {
    std::fstream fin;
    fin.open(PATH, std::ios::in | std::ios::binary);

    if (!fin.is_open()) {
        throw std::runtime_error("Could not open file. Please verify the file path.");
    }

    std::vector<std::vector<T>> result(rows, std::vector<T>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fin.read(reinterpret_cast<char*>(&result[i][j]), sizeof(T));
        }
    }

    fin.close();
    return result;
}

template <typename T>
std::vector<std::vector<float>> pad2d(const std::vector<std::vector<T>>& arr, int extent) {
    /* ensures the grid extent is even to avoid error */
    if (extent % 2) { 
        throw std::invalid_argument("Extent must be an even integer."); 
    }

    /* get arr dimensions */
    int rows = static_cast<int>(arr.size()); 
    int cols = static_cast<int>(arr[0].size()); 
    int half_ext = extent / 2;

    /* Defines result array of the pad */
    std::vector<std::vector<float>> padded_arr(rows + extent, std::vector<float>(cols + extent, 0.0f));

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
#endif // AUX_HPP

