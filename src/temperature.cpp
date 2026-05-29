#include "temperature.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

auto GetTemperature() -> std::string {
    return GetCpuTemp();
}

auto GetCpuTemp() -> std::string {
    for (const auto& entry : fs::directory_iterator("/sys/class/hwmon/")) {
        std::string path = entry.path().string();
        if (fs::exists(path + "/name")) {
            std::ifstream name_file(path + "/name");
            std::string name;
            std::getline(name_file, name);
            if (name == "coretemp") {
                if (fs::exists(path + "/temp1_input")) {
                    std::ifstream temp_file(path + "/temp1_input");
                    int temp;
                    if (temp_file >> temp) {
                        return std::to_string(temp / 1000) + "°C";
                    }
                }
            }
        }
    }
    return "N/A";
}

auto GetNvmeTemp() -> std::string {
    for (const auto& entry : fs::directory_iterator("/sys/class/hwmon/")) {
        std::string path = entry.path().string();
        if (fs::exists(path + "/name")) {
            std::ifstream name_file(path + "/name");
            std::string name;
            std::getline(name_file, name);
            if (name == "nvme") {
                if (fs::exists(path + "/temp1_input")) {
                    std::ifstream temp_file(path + "/temp1_input");
                    int temp;
                    if (temp_file >> temp) {
                        return std::to_string(temp / 1000) + "°C";
                    }
                }
            }
        }
    }
    return "N/A";
}

auto GetPchTemp() -> std::string {
    for (const auto& entry : fs::directory_iterator("/sys/class/hwmon/")) {
        std::string path = entry.path().string();
        if (fs::exists(path + "/name")) {
            std::ifstream name_file(path + "/name");
            std::string name;
            std::getline(name_file, name);
            if (name == "pch_cannonlake" || name == "pch_skylake") {
                if (fs::exists(path + "/temp1_input")) {
                    std::ifstream temp_file(path + "/temp1_input");
                    int temp;
                    if (temp_file >> temp) {
                        return std::to_string(temp / 1000) + "°C";
                    }
                }
            }
        }
    }
    return "N/A";
}
