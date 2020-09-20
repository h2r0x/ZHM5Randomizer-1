#include "Config.h"

#include <filesystem>

#include "third_party/tomlpp/toml.hpp"

namespace hitman_randomizer {

constexpr auto iniStringBufSize = 32;
constexpr const char *iniMainCategory = "ZHM5Randomizer";

void Config::Load() {
  base_directory_ =
      std::filesystem::current_path().generic_string(); //..\\HITMAN2

  auto ini_path = base_directory_ + "\\Retail\\hitman_randomizer.toml";
  const auto data = toml::parse(ini_path);

  // LOAD_INI_STRING_ENTRY(worldInventoryRandomizer, iniMainCategory,
  // "DEFAULT"); LOAD_INI_STRING_ENTRY(heroInventoryRandomizer, iniMainCategory,
  // "DEFAULT"); LOAD_INI_STRING_ENTRY(npcInventoryRandomizer, iniMainCategory,
  // "DEFAULT"); LOAD_INI_STRING_ENTRY(stashInventoryRandomizer,
  // iniMainCategory, "DEFAULT"); LOAD_INI_ENTRY(randomizeNPCGrenades,
  // iniMainCategory, 0); LOAD_INI_ENTRY(RNGSeed, iniMainCategory, 0);

  // LOAD_INI_ENTRY(showDebugConsole, "Debug", 0);
  // LOAD_INI_ENTRY(enableDebugLogging, "Debug", 0);
  // LOAD_INI_ENTRY(logToFile, "Debug", 0);
}

} // namespace hitman_randomizer
