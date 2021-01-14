#ifndef __ZHM5_RANDOMIZER_SRC_CONFIG_H__
#define __ZHM5_RANDOMIZER_SRC_CONFIG_H__

#include <algorithm>
#include <cctype>
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
  std::set<std::string> ignored_words_;

  const std::string ToString() const {
    std::stringstream fmt;
    fmt << "+Words[";
    std::copy(allowed_words_.begin(), allowed_words_.end(),
              std::ostream_iterator<std::string>(fmt, " "));
    fmt << "] ";
    fmt << "-Words[";
    std::copy(ignored_words_.begin(), ignored_words_.end(),
              std::ostream_iterator<std::string>(fmt, " "));
    fmt << "] ";

    return fmt.str();
  }

  const bool ShouldPermit(const Item &it) const {
    if (it.title() == "") {
      return false;
    }

    if (it.getType() == ICON::QUESTITEM || it.getType() == ICON::KEY ||
        it.getType() == ICON::REMOTE) {
      return false;
    }

    if (allowed_words_.size() == 0 && ignored_words_.size() == 0) {
      return true;
    }

    bool matches_allowed_word = false;
    bool matches_ignored_word = false;

    for (auto &word : allowed_words_) {
      if (it.IconString() == word) {
        matches_allowed_word = true;
      } else if (this->findStringIC(it.title(), word)) {
        matches_allowed_word = true;
      } else if (this->findStringIC(it.string(), word)) {
        matches_allowed_word = true;
      }
    }
    for (auto &word : ignored_words_) {
      if (it.IconString() == word) {
        matches_ignored_word = true;
      } else if (this->findStringIC(it.title(), word)) {
        matches_ignored_word = true;
      } else if (this->findStringIC(it.string(), word)) {
        matches_ignored_word = true;
      }
    }

    if (matches_ignored_word && matches_allowed_word) {
      spdlog::get("console")->warn(
          "The item {} is both allowed and ignored. This is probably bad.",
          it.title());
    }

    if (matches_ignored_word) {
      return false;
    }

    if (allowed_words_.empty() || matches_allowed_word) {
      return true;
    }

    return false;
  }

  const bool findStringIC(const std::string &strHaystack,
                    const std::string &strNeedle) const {
    auto it =
        std::search(strHaystack.begin(), strHaystack.end(), strNeedle.begin(),
                    strNeedle.end(), [](char ch1, char ch2) {
                      return std::toupper(ch1) == std::toupper(ch2);
                    });
    return (it != strHaystack.end());
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

#endif // __ZHM5_RANDOMIZER_SRC_CONFIG_H__