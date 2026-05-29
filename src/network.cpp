#include "network.hpp"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <vector>

auto GetLocalIP() -> std::string {
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;
    void *tmpAddrPtr = nullptr;
    std::string ip_address = "N/A";

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            std::string interface_name = ifa->ifa_name;
            if (interface_name != "lo") {
                ip_address = std::string(addressBuffer);
                // Simple assumption for subnet mask, usually /24 for local networks
                // but we can try to get it from ifa_netmask
                if (ifa->ifa_netmask && ifa->ifa_netmask->sa_family == AF_INET) {
                    uint32_t mask = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr.s_addr;
                    int prefix = 0;
                    while (mask > 0) {
                        if (mask & 1) prefix++;
                        mask >>= 1;
                    }
                    ip_address += "/" + std::to_string(prefix);
                }
                break;
            }
        }
    }
    if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);
    return ip_address;
}
