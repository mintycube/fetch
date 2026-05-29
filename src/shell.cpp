#include "shell.hpp"
#include <cstdlib>
#include <string>
#include <filesystem>

auto GetShellName() -> std::string {
    if (const char* shell_env = std::getenv("SHELL")) {
        std::filesystem::path shell_path(shell_env);
        return shell_path.filename().string();
    }
    return "Unknown Shell";
}
