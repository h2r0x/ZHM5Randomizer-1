#include "Config.h"

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "ZHM5Randomizer/src/Console.h"
#include "ZHM5Randomizer/src/Item.h"
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
  hero_inventory_randomizer_ =
      tbl["ZHM5Randomizer"]["heroInventoryRandomizer"].value_or("NONE");
  npc_inventory_randomizer_ =
      tbl["ZHM5Randomizer"]["npcInventoryRandomizer"].value_or("NONE");
  stash_inventory_randomizer_ =
      tbl["ZHM5Randomizer"]["stashInventoryRandomizer"].value_or("NONE");

  randomizeNPCGrenades =
      tbl["ZHM5Randomizer"]["randomizeNPCGrenades"].value_or(false);
  RNGSeed = tbl["ZHM5Randomizer"]["RNGSeed"].value_or(0);
  is_loaded_ = true;

  auto custom_world = tbl["Custom"]["World"];
  auto custom_npc = tbl["Custom"]["NPC"];

  if (toml::array *arr = custom_world["allowed_words"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          if (el != "") {
            this->custom_world_rules_.allowed_words_.insert(*el);
          }
        }
      });
    }
  }
  if (toml::array *arr = custom_world["allowed_categories"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          this->custom_world_rules_.allowed_categories_.insert(
              Item::icon_map[el.get()]);
        }
      });
    }
  }
  if (toml::array *arr = custom_world["ignored_words"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          if (el != "") {
            this->custom_world_rules_.ignored_words_.insert(*el);
          }
        }
      });
    }
  }
  if (toml::array *arr = custom_world["ignored_categories"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          this->custom_world_rules_.ignored_categories_.insert(
              Item::icon_map[el.get()]);
        }
      });
    }
  }

  // I KNOW I'M SO FUCKING LAZY
  if (toml::array *arr = custom_npc["allowed_words"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          if (el != "") {
            this->custom_world_rules_.allowed_words_.insert(*el);
          }
        }
      });
    }
  }
  if (toml::array *arr = custom_npc["allowed_categories"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          this->custom_npc_rules_.allowed_categories_.insert(
              Item::icon_map[el.get()]);
        }
      });
    }
  }
  if (toml::array *arr = custom_npc["ignored_words"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          if (el != "") {
            this->custom_world_rules_.ignored_words_.insert(*el);
          }
        }
      });
    }
  }
  if (toml::array *arr = custom_npc["ignored_categories"].as_array()) {
    for (toml::node &elem : *arr) {
      elem.visit([this](auto &&el) noexcept {
        if constexpr (toml::is_string<decltype(el)>) {
          this->custom_npc_rules_.ignored_categories_.insert(
              Item::icon_map[el.get()]);
        }
      });
    }
  }
}

} // namespace hitman_randomizer
