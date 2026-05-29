#include "storage.hpp"

#include <sys/statvfs.h>
#include <iomanip>
#include <sstream>

auto GetFreeStorage(const std::string& path) -> std::string {
    struct statvfs buf;
    if (statvfs(path.c_str(), &buf) == 0) {
        unsigned long long free_bytes = buf.f_bavail * buf.f_frsize;
        double free_gb = static_cast<double>(free_bytes) / (1024.0 * 1024.0 * 1024.0);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << free_gb << " GB";
        return ss.str();
    }
    return "N/A";
}
