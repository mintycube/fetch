#include "battery.hpp"

#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

static auto ReadFile(const std::string& path) -> std::string {
    std::ifstream file(path);
    std::string line;
    if (std::getline(file, line)) {
        return line;
    }
    return "";
}

auto GetBatteryCycleCount() -> std::string {
    std::string path = "/sys/class/power_supply/BAT0/cycle_count";
    if (fs::exists(path)) {
        return ReadFile(path);
    }
    return "N/A";
}

auto ReadLong(const std::string& path) -> long {
    if (fs::exists(path)) {
        return std::stol(ReadFile(path));
    }
    return -1;
}

auto GetBatteryHealth() -> std::string {
    long full = -1;
    long design = -1;
    double full_wh = 0.0;
    double design_wh = 0.0;

    // Case 1: energy_* exists
    if (fs::exists("/sys/class/power_supply/BAT0/energy_full") &&
        fs::exists("/sys/class/power_supply/BAT0/energy_full_design")) {

        full = ReadLong("/sys/class/power_supply/BAT0/energy_full");
        design = ReadLong("/sys/class/power_supply/BAT0/energy_full_design");

        full_wh = full / 1e6;
        design_wh = design / 1e6;
    }
    // Case 2: fallback to charge_* (your case)
    else if (fs::exists("/sys/class/power_supply/BAT0/charge_full") &&
             fs::exists("/sys/class/power_supply/BAT0/charge_full_design")) {

        long charge_full = ReadLong("/sys/class/power_supply/BAT0/charge_full");
        long charge_design = ReadLong("/sys/class/power_supply/BAT0/charge_full_design");
        long voltage = ReadLong("/sys/class/power_supply/BAT0/voltage_min_design");

        if (charge_full <= 0 || charge_design <= 0 || voltage <= 0) {
            return "N/A";
        }

        // Convert to Wh
        full_wh = (charge_full / 1e6) * (voltage / 1e6);
        design_wh = (charge_design / 1e6) * (voltage / 1e6);
    }
    else {
        return "N/A";
    }

    double health = (full_wh / design_wh) * 100.0;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << full_wh
       << "/" << std::fixed << std::setprecision(0) << design_wh
       << " Wh ("
       << std::fixed << std::setprecision(1) << health << "%)";

    return ss.str();
}
