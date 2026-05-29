#pragma once

#include <string>

auto GetTemperature() -> std::string;
auto GetCpuTemp() -> std::string;
auto GetNvmeTemp() -> std::string;
auto GetPchTemp() -> std::string;
