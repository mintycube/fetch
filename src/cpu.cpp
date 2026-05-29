#include "cpu.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

auto GetCpuModel() -> std::string {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::string model = "Unknown CPU";
    int threads = 0;

    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            if (model == "Unknown CPU") {
                model = line.substr(line.find(":") + 2);
                // Clean up common CPU model strings
                if (model.find(" Intel(R) Core(TM) ") != std::string::npos) {
                    model.replace(model.find(" Intel(R) Core(TM) "), 19, " ");
                }
                if (model.find(" CPU @ ") != std::string::npos) {
                    model = model.substr(0, model.find(" CPU @ "));
                }
                // Trim leading/trailing spaces
                model.erase(0, model.find_first_not_of(" "));
                model.erase(model.find_last_not_of(" ") + 1);
            }
        }
        if (line.find("processor") != std::string::npos) {
            threads++;
        }
    }

    std::string freq_str = "";
    std::ifstream freq_file("/sys/devices/system/cpu/cpu0/cpufreq/bios_limit");
    if (!freq_file.is_open()) {
        freq_file.open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq");
    }

    if (freq_file.is_open()) {
        long freq_khz;
        if (freq_file >> freq_khz) {
            double freq_ghz = static_cast<double>(freq_khz) / 1000000.0;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << freq_ghz << "G";
            freq_str = " @ " + ss.str();
        }
    }

    // return model + " (" + std::to_string(threads) + ")" + freq_str;
    return model;
}
