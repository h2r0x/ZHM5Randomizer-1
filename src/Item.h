#pragma once

#include <string>

#include "..\thirdparty\json.hpp"

using json = nlohmann::json;

enum class ICON {
  MELEE,
  KEY,
  EXPLOSIVE,
  QUESTITEM,
  TOOL,
  SNIPERRIFLE,
  ASSAULTRIFLE,
  REMOTE,
  SHOTGUN,
  SUITCASE,
  PISTOL,
  INVALID_CATEGORY_ICON,
  DISTRACTION,
  POISON,
  CONTAINER,
  SMG,
};

enum class CHEAT_GROUP {
  NONE,
  DEVICES,
  SNIPERS,
  ASSAULTRIFLES,
  PISTOLS,
  SHOTGUNS,
  EXOTICS,
  SMGS,
};

enum class THROW_TYPE {
  NONE,
  PACIFY_LIGHT,
  PACIFY_HEAVY,
  DEADLY_LIGHT,
  DEADLY_HEAVY,
};

enum class SILENCE_RATING {
  NONE,
  NOT_SILENCED,
  SILENCED,
  SUPER_SILENCED,
};

class Item {
  ICON icon;
  CHEAT_GROUP cheat_group;
  THROW_TYPE throw_type;
  SILENCE_RATING silence_rating;

  std::string common_name;
  std::string title_;
  std::string name_LOC_;
  bool isCoin_;

public:
  Item();
  Item(const json &config);

  bool isEssential() const;
  bool isNotEssential() const;
  bool isKey() const;
  bool isQuestItem() const;
  bool isWeapon() const;
  bool isPistol() const;
  bool isSmg() const;
  bool isAssaultRifle() const;
  bool isShotgun() const;
  bool isSniper() const;
  bool isMelee() const;
  bool isExplosive() const;
  bool isTool() const;
  bool isSuitcase() const;
  bool isDistraction() const;
  bool isNotEssentialAndNotWeapon() const;
  bool isPoison() const;
  bool isGoodTreasureLocation() const;
  bool isCoin() const;
  bool isDecorativeMeleeItem() const {
    return (!isWeapon()) && name_LOC().rfind("ui_prop_melee", 0) == 0;
  }

  const std::string IconString() const {
    std::string icon_name;
    for (const auto &i : icon_map) {
      if (i.second == getType()) {
        return i.first;
        break;
      }
    }
    return "THIS_SHOULD_NEVER_HAPPEN";
  }

  const std::string& title() const {
    return title_;
  }
  const std::string &string() const;
  const std::string &name_LOC() const { return name_LOC_; }
  const ICON &getType() const;
  const THROW_TYPE &getThrowType() const;
  const SILENCE_RATING &getSilenceRating() const;

  void print() const;

  std::unordered_map<std::string, ICON> icon_map{
      {"melee", ICON::MELEE},
      {"key", ICON::KEY},
      {"explosives", ICON::EXPLOSIVE},
      {"questitem", ICON::QUESTITEM},
      {"tool", ICON::TOOL},
      {"sniperrifle", ICON::SNIPERRIFLE},
      {"assaultrifle", ICON::ASSAULTRIFLE},
      {"remote", ICON::REMOTE},
      {"QuestItem", ICON::QUESTITEM},
      {"shotgun", ICON::SHOTGUN},
      {"suitcase", ICON::SUITCASE},
      {"pistol", ICON::PISTOL},
      {"INVALID_CATEGORY_ICON", ICON::INVALID_CATEGORY_ICON},
      {"distraction", ICON::DISTRACTION},
      {"poison", ICON::POISON},
      {"Container", ICON::CONTAINER},
      {"smg", ICON::SMG},
  };
};
