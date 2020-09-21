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

  const std::string& world_inventory_randomizer() const {
    return world_inventory_randomizer_;
  };
  const std::string& hero_inventory_randomizer() const {
    return hero_inventory_randomizer_;
  };
  const std::string& npc_inventory_randomizer() const {
    return npc_inventory_randomizer_;
  };
  const std::string& stash_inventory_randomizer() const {
    return stash_inventory_randomizer_;
  };
  const int rng_seed() const {
    return RNGSeed;
  }

private:
  std::string base_directory_;
  std::string world_inventory_randomizer_;
  std::string hero_inventory_randomizer_;
  std::string npc_inventory_randomizer_;
  std::string stash_inventory_randomizer_;
  bool randomizeNPCGrenades;
  bool showDebugConsole = true;
  bool enableDebugLogging = true;
  bool logToFile;
  int RNGSeed;
};
} // namespace hitman_randomizer
