#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <filesystem>

Configuration& Configuration::getInstance() {
    static Configuration instance;
    return instance;
}

void Configuration::setValue(const std::string& key, const std::string& value) {
    config[key] = value;
}

void Configuration::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file: " << filename << std::endl;
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            setValue(key, value);
        }
    }
}