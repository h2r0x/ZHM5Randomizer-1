#pragma once

#include <memory>
#include <string>
#include <set>
#include <sstream>
#include <ostream>

#include "ZHM5Randomizer/src/Item.h"

namespace hitman_randomizer {

class CustomRules {
  public:
  // private:
    std::set<std::string> allowed_words_;
    std::set<std::string> allowed_categories_;
    std::set<std::string> ignored_words_;
    std::set<std::string> ignored_categories_;

    const std::string ToString() const {
      std::stringstream fmt;
      fmt << "+Words[";
      std::copy(allowed_words_.begin(), allowed_words_.end(), std::ostream_iterator<std::string>(fmt, " "));
      fmt << "]\n";
      fmt << "+Categories[";
      std::copy(allowed_categories_.begin(), allowed_categories_.end(), std::ostream_iterator<std::string>(fmt, " "));
      fmt << "]\n";
      fmt << "-Words[";
      std::copy(ignored_words_.begin(), ignored_words_.end(), std::ostream_iterator<std::string>(fmt, " "));
      fmt << "]\n";
      fmt << "-Categories[";
      std::copy(allowed_categories_.begin(), allowed_categories_.end(), std::ostream_iterator<std::string>(fmt, " "));
      fmt << "]\n";

      return fmt.str();
    }
};

class Config {
public:
  Config() : custom_world_rules_(), custom_npc_rules_() {}
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
  bool is_loaded_ = false;
  CustomRules custom_world_rules_;
  CustomRules custom_npc_rules_;
};

} // namespace hitman_randomizer
