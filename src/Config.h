#pragma once
#include <memory>
#include <string>

namespace hitman_randomizer {
class Config {
public:
  Config() {}

  void Load();

  bool show_debug_console() {
      return showDebugConsole;
  }

  bool randomize_npc_grenades() {
      return randomizeNPCGrenades;
  }

private:
  std::string base_directory_;
  std::string worldInventoryRandomizer;
  std::string heroInventoryRandomizer;
  std::string npcInventoryRandomizer;
  std::string stashInventoryRandomizer;
  bool randomizeNPCGrenades;
  bool showDebugConsole;
  bool enableDebugLogging;
  bool logToFile;
  int RNGSeed;
  std::string randomizationScenario;
};
} // namespace hitman_randomizer
