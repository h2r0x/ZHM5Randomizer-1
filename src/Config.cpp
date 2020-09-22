#include "Config.h"

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Console.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "third_party/tomlpp/toml.hpp"

namespace hitman_randomizer {

void Config::Load() {
  auto logger = spdlog::get("basic_logger");
  std::ostringstream writer;

  if (is_loaded_) {
    logger->warn("Asking to reload config, ignoring.");
    return;
  }

  base_directory_ =
      std::filesystem::current_path().generic_string(); //..\\HITMAN2

  auto ini_path = base_directory_ + "\\Retail\\hitman_randomizer.toml";
  toml::table tbl;

  try {
    std::ifstream t(ini_path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    tbl = toml::parse(buffer.str());
    writer << "Success reading toml:" << tbl;
    logger->info(writer.str());
  } catch (const toml::parse_error &err) {
    writer << "Failed to load file: " << ini_path << ", err: " << err;
    logger->info(writer.str());
    ExitProcess(0);
  } catch (...) {
    writer << "Unknown error reading toml";
    logger->error(writer.str());
    ExitProcess(0);
  }

  world_inventory_randomizer_ =
      tbl["ZHM5Randomizer"]["worldInventoryRandomizer"].value_or("NONE");
  hero_inventory_randomizer_ = tbl["ZHM5Randomizer"]["heroInventoryRandomizer"].value_or("NONE");
  npc_inventory_randomizer_ = tbl["ZHM5Randomizer"]["npcInventoryRandomizer"].value_or("NONE");
  stash_inventory_randomizer_ =
      tbl["ZHM5Randomizer"]["stashInventoryRandomizer"].value_or("NONE");

  randomizeNPCGrenades = tbl["ZHM5Randomizer"]["randomizeNPCGrenades"].value_or(false);
  RNGSeed = tbl["ZHM5Randomizer"]["RNGSeed"].value_or(0);
  is_loaded_ = true;
}

} // namespace hitman_randomizer
