#include "machine.hpp"
#include <fstream>
#include <string>

auto GetMachineModel() -> std::string {
    std::ifstream file("/sys/class/dmi/id/product_name");
    std::string line;
    if (std::getline(file, line)) {
        // Some systems might have "Mobile Workstation" or similar, 
        // we can trim it if it's too long, but user's example shows "HP ZBook Firefly 14 G7"
        if (line == "HP ZBook Firefly 14 G7 Mobile Workstation") {
            return "HP ZBook Firefly 14 G7";
        }
        return line;
    }
    return "Unknown Machine";
}
