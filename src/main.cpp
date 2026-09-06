#include <iostream>
#include <string>
#include <vector>

#include "ascii.hpp"
#include "battery.hpp"
#include "config.hpp"
#include "cpu.hpp"
#include "display.hpp"
#include "distro.hpp"
#include "formatter.hpp"
#include "gpu.hpp"
#include "kernel.hpp"
#include "machine.hpp"
#include "network.hpp"
#include "pkgcount.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "shell.hpp"
#include "storage.hpp"
#include "temperature.hpp"
#include "terminal.hpp"
#include "uptime.hpp"
#include "userhost.hpp"
#include "wm.hpp"

auto main(int argc, char* argv[]) -> int {
  auto show_ascii = kDefaultShowAscii;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      std::cout << "Usage:\n"
                << "  -h, --help           Show this help message\n"
                << "  -n, --no-ascii       Disable ASCII art\n";
      return 0;
    }
    if (arg == "-n" || arg == "--no-ascii") {
      show_ascii = false;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return 1;
    }
  }

  std::vector<std::string> ascii_art;
  if (show_ascii) {
    ascii_art = std::string(kAsciiArtChoice).empty()
                    ? GetAsciiArt()
                    : GetAsciiArt(kAsciiArtChoice);
  }

  auto gray = kCol8;
  auto reset = kColR;

  // clang-format off
  std::vector<std::string> info = {
    gray + "╭───┬" + "─────────────────────────────┬───┬─────────────────────────────" + "╮" + reset,

    gray + "│ " + kCol3 + PadC(kIconUser, 1) + gray + " │ " + kCol2 + PadC(GetUsername() + "@" + GetHostname(), 27) + reset +
    gray + " │ " + kCol2 + PadC(kIconDistro, 1) + gray + " │ " + reset + PadC(GetDistroName(), 27) + gray + " │" + reset,

    gray + "│ " + kCol4 + PadC(kIconMachine, 1) + gray + " │ " + reset + PadC(GetMachineModel(), 27) +
    gray + " │ " + kCol6 + PadC(kIconTerm, 1) + gray + " │ " + reset + PadC(GetTerminalEmulatorName(), 27) + gray + " │" + reset,

    gray + "│ " + kCol3 + PadC(kIconCPU, 1) + gray + " │ " + reset + PadC(GetCpuModel(), 27) +
    gray + " │ " + kCol5 + PadC(kIconShell, 1) + gray + " │ " + reset + PadC(GetShellName(), 27) + gray + " │" + reset,

    gray + "│ " + kCol2 + PadC(kIconGPU, 1) + gray + " │ " + reset + PadC(GetGpuInfo(), 27) +
    gray + " │ " + kCol4 + PadC(kIconWM, 1) + gray + " │ " + reset + PadC(GetWindowManagerName(), 27) + gray + " │" + reset,

    gray + "│ " + kCol6 + PadC(kIconDisplay, 1) + gray + " │ " + reset + PadC(GetDisplayInfo(), 27) +
    gray + " ├───┴──────── Storage ────────────┤" + reset,

    gray + "│ " + kCol5 + PadC(kIconRAM, 1) + gray + " │ " + reset + PadC(GetRamUsage(), 27) +
    gray + " │ " + reset + PadC("/: " + GetFreeStorage("/") + "  D: " + GetFreeStorage("/mnt/d") + "  C: " + GetFreeStorage("/mnt/c"), 31) + gray + " │" + reset,

    gray + "│ " + kCol1 + PadC(kIconKernel, 1) + gray + " │ " + reset + PadC(GetKernelName(), 27) +
    gray + " ├──────────── Thermal ────────────┤" + reset,

    gray + "│ " + kCol3 + PadC(kIconUptime, 1) + gray + " │ " + reset + PadC(GetUptimeAsString(), 27) +
    gray + " │ " + reset + PadC(kCol3 + kIconTempCPU + reset + " " + GetCpuTemp() + "    " + kCol4 + kIconTempNVMe + reset + "  " + GetNvmeTemp() + "    " + kCol5 + kIconTempPCH + reset + "  " + GetPchTemp(), 2) + gray + " │" + reset,

    gray + "│ " + kCol8 + PadC(kIconPkgs, 1) + gray + " │ " + reset + PadC(GetPacmanPackageCount() + " (pacman)", 27) +
    gray + " ├──────────── Battery ────────────┤" + reset,

    gray + "│ " + kCol6 + PadC(kIconIP, 1) + gray + " │ " + reset + PadC(GetLocalIP(), 27) +
    gray + " │ " + kCol1 + " " + kIconBatHealth + reset + " " + Pad(GetBatteryHealth(), 20) + "  " + kCol3 + kIconBatCycles + reset + " " + Pad(GetBatteryCycleCount(), 2) + gray + "  │" + reset,

    gray + "╰───┴" + "─────────────────────────────┴─────────────────────────────────" + "╯" + reset
  };
  // clang-format on

  if (show_ascii) {
    PrintSideBySide(ascii_art, info);
  } else {
    for (const auto& line : info) {
      std::cout << line << "\n";
    }
  }

  return 0;
}
