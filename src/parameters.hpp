#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

/*
 * Extracts parameters from the parameters file
 * and adds them into a map where the key is the
 * name of the variable and the value is the variable's value.
 */

#include <string>
#include <map>
#include <any>
#include <stdexcept>
#include <regex>
#include <fstream>
#include <vector>

class Parameters {
public:
    std::map<std::string, std::any> parameters;

    explicit Parameters(const std::string& path) : path(path) {}

    void load() {
        std::regex word_regex(R"((\w+)\s*=\s*([^#]+?)\s*(?:#|$).*\((.*?)\))");

        std::ifstream param_file(path);
        if (!param_file.is_open()) {
            throw std::runtime_error("Unable to open file: " + path);
        }

        std::string line;
        while (std::getline(param_file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, word_regex)) {
                std::string key = match[1].str();
                std::string value = match[2].str();
                std::string type = match[3].str();
                parameters[key] = typeConverter(type, value);
            }
        }

        param_file.close();
    }

    template <typename T>
    T get(const std::string& key) const {
        auto it = parameters.find(key);
        if (it == parameters.end()) {
            throw std::runtime_error("Key '" + key + "' not found in parameters.");
        }
        return std::any_cast<T>(it->second);
    }

private:
    std::string path;

    std::any typeConverter(const std::string& type, const std::string& value) const {
        std::string uppercase_type = type;
        for (auto& c : uppercase_type) c = toupper(c);

        if (uppercase_type == "INT") {
            return std::stoi(value);
        } else if (uppercase_type == "FLOAT") {
            return std::stof(value);
        } else if (uppercase_type == "BOOL") {
            return (value == "1" || value == "True" || value == "true");
        } else if (uppercase_type == "STR") {
            return value;
        } else if (uppercase_type == "IARRAY") {
            return parseIntegerArray(value);
        } else if (uppercase_type == "FARRAY") {
            return parseFloatArray(value);
        } else {
            throw std::invalid_argument("Unsupported type: " + type);
        }
    }

    std::vector<int> parseIntegerArray(const std::string& value) const {
        std::regex array_regex(R"(\d+)");
        std::sregex_iterator words_begin(value.begin(), value.end(), array_regex);
        std::sregex_iterator words_end;

        std::vector<int> result;
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            result.push_back(std::stoi(i->str()));
        }
        return result;
    }

    std::vector<float> parseFloatArray(const std::string& value) const {
        std::regex array_regex(R"(\d+(\.\d+)?)");
        std::sregex_iterator words_begin(value.begin(), value.end(), array_regex);
        std::sregex_iterator words_end;

        std::vector<float> result;
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            result.push_back(std::stof(i->str()));
        }
        return result;
    }
};

#endif // PARAMETERS_HPP

