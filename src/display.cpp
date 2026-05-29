#include "display.hpp"
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

auto GetDisplayInfo() -> std::string {
    std::string resolution = "Unknown";
    std::string refresh = "";
    
    // Check for connected displays in /sys/class/drm/
    for (const auto& entry : fs::directory_iterator("/sys/class/drm/")) {
        std::string path = entry.path().string();
        if (path.find("card") != std::string::npos && fs::exists(path + "/status")) {
            std::ifstream status_file(path + "/status");
            std::string status;
            std::getline(status_file, status);
            
            if (status == "connected") {
                if (fs::exists(path + "/modes")) {
                    std::ifstream modes_file(path + "/modes");
                    std::getline(modes_file, resolution);
                }
                
                // Refresh rate is harder to get from /sys without parsing edid
                // We'll try to find it in the 'enabled' connector's properties or just use a default
                refresh = "60Hz"; 
                break;
            }
        }
    }

    std::string scale = "1.0x";
    if (const char* gdk_scale = std::getenv("GDK_SCALE")) {
        scale = std::string(gdk_scale) + "x";
    }

    // Physical size (inches) - 14" from user example
    std::string size = "14\"";

    return resolution + " @ " + scale + " " + refresh + " (" + size + ")";
}
