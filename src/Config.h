#pragma once

#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <string>

#include "ZHM5Randomizer/src/Item.h"
#include "spdlog/spdlog.h"

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
    std::copy(allowed_words_.begin(), allowed_words_.end(),
              std::ostream_iterator<std::string>(fmt, " "));
    fmt << "] ";
    fmt << "+Categories[";
    std::copy(allowed_categories_.begin(), allowed_categories_.end(),
              std::ostream_iterator<std::string>(fmt, " "));
    fmt << "] ";
    fmt << "-Words[";
    std::copy(ignored_words_.begin(), ignored_words_.end(),
              std::ostream_iterator<std::string>(fmt, " "));
    fmt << "] ";
    fmt << "-Categories[";
    std::copy(allowed_categories_.begin(), allowed_categories_.end(),
              std::ostream_iterator<std::string>(fmt, " "));
    fmt << "]";

    return fmt.str();
  }

  const bool ShouldPermit(const Item &it) const {
    if (it.title() == "") {
      return false;
    }
    
    if (allowed_categories_.size() == 0 && ignored_categories_.size() == 0 &&
        allowed_words_.size() == 0 && allowed_categories_.size() == 0) {
      return true;
    }

    bool matches_allowed_word = false;
    bool matches_allowed_category = false;
    bool matches_ignored_word = false;
    bool matches_ignored_category = false;

    if (allowed_categories_.empty()) {
      matches_allowed_category = true;
    }
    for (auto &category : allowed_categories_) {
      if (it.IconString() == category) {
        matches_allowed_category = true;
      }
    }
    for (auto &category : ignored_categories_) {
      if (it.IconString() == category) {
        matches_ignored_category = true;
      }
    }
    if (allowed_words_.empty()) {
      matches_allowed_word = true;
    }
    for (auto &word : allowed_words_) {
      if (it.title().find(word) == std::string::npos) {
        matches_allowed_word = true;
      }
    }
    for (auto &word : ignored_words_) {
      if (it.title().find(word) != std::string::npos) {
        matches_ignored_word = true;
      }
    }

    if ((matches_ignored_word || matches_ignored_category) &&
        (matches_allowed_word || matches_ignored_category)) {
      spdlog::get("console")->error(
          "The item {} is both allowed and ignored. This is probably bad.",
          it.title());
    }

    if (matches_ignored_category || matches_ignored_word) {
      return false;
    }

    if ((allowed_words_.empty() || matches_allowed_word) &&
        (allowed_categories_.empty() || matches_allowed_category)) {
      return true;
    }

    return false;
  }
};

class Config {
public:
  Config() : custom_world_rules_(), custom_npc_rules_() {}
  void Load();

  bool show_debug_console() { return showDebugConsole; }

  bool randomize_npc_grenades() { return randomizeNPCGrenades; }

  const std::string &world_inventory_randomizer() const {
    return world_inventory_randomizer_;
  };
  const std::string &hero_inventory_randomizer() const {
    return hero_inventory_randomizer_;
  };
  const std::string &npc_inventory_randomizer() const {
    return npc_inventory_randomizer_;
  };
  const std::string &stash_inventory_randomizer() const {
    return stash_inventory_randomizer_;
  };
  const int rng_seed() const { return RNGSeed; }

  CustomRules custom_world_rules_;
  CustomRules custom_npc_rules_;

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
};

} // namespace hitman_randomizer
