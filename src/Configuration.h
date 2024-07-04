#pragma once
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <typeinfo>

class Configuration {
public:
    static Configuration& getInstance();

    void setValue(const std::string& key, const std::string& value);

    template<typename T>
    T getValue(const std::string& key, const T& defaultValue) const;

    void loadFromFile(const std::string& filename);

private:
    Configuration() = default;
    std::unordered_map<std::string, std::string> config;

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    template<typename T>
    T convertStringToValue(const std::string& str, const T& defaultValue) const;
};

template<typename T>
T Configuration::getValue(const std::string& key, const T& defaultValue) const {
    auto it = config.find(key);
    if (it != config.end()) {
        return convertStringToValue<T>(it->second, defaultValue);
    }
    std::cout << "Warning: Key '" << key << "' not found in configuration. Using default value." << std::endl;
    return defaultValue;
}

template<typename T>
T Configuration::convertStringToValue(const std::string& str, const T& defaultValue) const {
    T value;
    std::istringstream iss(str);
    
    if (iss >> value) {
        return value;
    } else {
        std::cerr << "Error: Failed to convert value '" << str << "' to type " 
                  << typeid(T).name() << ". Using default value." << std::endl;
        return defaultValue;
    }
}