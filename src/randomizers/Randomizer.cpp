#include "Randomizer.h"

#include <algorithm>
#include <random>

#include "ZHM5Randomizer/src/Config.h"
#include "ZHM5Randomizer/src/Console.h"
#include "ZHM5Randomizer/src/DefaultItemPool.h"
#include "ZHM5Randomizer/src/Item.h"
#include "ZHM5Randomizer/src/Offsets.h"
#include "ZHM5Randomizer/src/RNG.h"
#include "ZHM5Randomizer/src/Repository.h"

RandomisationStrategy::RandomisationStrategy(
    std::shared_ptr<hitman_randomizer::Config> config)
    : repo(RandomDrawRepository::inst()), config_(config) {}

void RandomisationStrategy::initialize(Scenario, const DefaultItemPool *const) {
}

const RepositoryID *
WorldInventoryRandomisation::randomize(const RepositoryID *in_out_ID) {
  if (repo.contains(*in_out_ID) && item_queue.size()) {
    const RepositoryID *id = item_queue.front();
    item_queue.pop();
    // spdlog::get("console")->info(
    //     "WorldInventoryRandomisation::randomize: {}: {} -> {}",
    //     static_cast<int>(item_queue.size()), repo.getItem(*in_out_ID)->string(),
    //     repo.getItem(*id)->string());
    return id;
  } else {
    if (!item_queue.size()) {
      spdlog::get("console")->info("WorldInventoryRandomisation::randomize: skipped (queue exhausted) [{}]", in_out_ID->toString());
    } else {
      spdlog::get("console")->info("WorldInventoryRandomisation::randomize: skipped (not in repo) [{}]", in_out_ID->toString());
    }
    return in_out_ID;
  }
}

void setupTools(std::vector<const RepositoryID *> &items) {}

void WorldInventoryRandomisation::initialize(
    Scenario scen, const DefaultItemPool *const default_pool) {
  std::vector<const RepositoryID *> new_item_pool;

  // Key and quest items
  default_pool->get(new_item_pool, &Item::isEssential);
  unsigned int essential_item_count = new_item_pool.size();

  // Fill remaining slots with random items
  size_t default_item_pool_weapon_count =
      default_pool->getCount(&Item::isWeapon);
  int default_item_pool_size = default_pool->size();
  unsigned int random_item_count = default_item_pool_size -
                                   new_item_pool.size() -
                                   default_item_pool_weapon_count;

  repo.getRandom(new_item_pool, random_item_count,
                 &Item::isNotEssentialAndNotWeapon);

  // Shuffle item pool
  std::shuffle(new_item_pool.begin(), new_item_pool.end(),
               *RNG::inst().getEngine());

  // Insert weapons
  std::vector<const RepositoryID *> weapons;
  repo.getRandom(weapons, default_item_pool_weapon_count, &Item::isWeapon);

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);
  for (int i = 0; i < weapon_slots.size(); i++) {
    new_item_pool.insert(new_item_pool.begin() + weapon_slots[i], weapons[i]);
  }

  // fill queue
  for (const auto &id : new_item_pool)
    item_queue.push(id);

  // // TODO: Move this print code
  // Console::log("ItemPool report:\n");
  // Console::log("total size: %d(%d)\n", default_item_pool_size,
  //              static_cast<int>(new_item_pool.size()));
  // Console::log("\tessentials: %d\n", essential_item_count);
  // Console::log("\tweapons: %d\n",
  //              static_cast<int>(default_item_pool_weapon_count));
  // Console::log("\trandom: %d\n", random_item_count);
  // Console::log("\n");
}

const RepositoryID *
ActionWorldRandomization::randomize(const RepositoryID *in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void ActionWorldRandomization::initialize(
    Scenario scen, const DefaultItemPool *const default_pool) {
  int default_item_pool_size = default_pool->size();

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);

  std::vector<int> essential_items;
  default_pool->getPosition(essential_items, &Item::isEssential);

  for (int i = 0; i < default_item_pool_size; i++) {
    auto found = std::find(weapon_slots.begin(), weapon_slots.end(), i);
    if (found != weapon_slots.end()) {
      item_queue.push(repo.getStablePointer(*repo.getRandom(&Item::isWeapon)));
    } else if (std::find(essential_items.begin(), essential_items.end(), i) !=
               essential_items.end()) {
      RepositoryID &original_item =
          RepositoryID("00000000-0000-0000-0000-000000000000");
      default_pool->getIdAt(original_item, i);
      item_queue.push(
          repo.getStablePointer(RepositoryID(original_item.toString())));
    } else {
      int j = rand() % 100;
      if (j < 10) {
        item_queue.push(repo.getStablePointer(*repo.getRandom(&Item::isCoin)));
      } else if (j >= 10 && j < 50) {
        item_queue.push(
            repo.getStablePointer(*repo.getRandom(&Item::isExplosive)));
      } else if (j >= 50 && j < 90) {
        item_queue.push(
            repo.getStablePointer(*repo.getRandom(&Item::isWeapon)));
      } else {
        RepositoryID &original_item =
            RepositoryID("00000000-0000-0000-0000-000000000000");
        default_pool->getIdAt(original_item, i);
        item_queue.push(
            repo.getStablePointer(RepositoryID(original_item.toString())));
      }
    }
  }
}

const RepositoryID *OopsAllExplosivesWorldInventoryRandomization::randomize(
    const RepositoryID *in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void OopsAllExplosivesWorldInventoryRandomization::initialize(
    Scenario scen, const DefaultItemPool *const default_pool) {
  std::vector<const RepositoryID *> new_item_pool;

  // Key and quest items
  default_pool->get(new_item_pool, &Item::isEssential);
  unsigned int essential_item_count = new_item_pool.size();

  size_t default_item_pool_weapon_count =
      default_pool->getCount(&Item::isWeapon);
  int default_item_pool_size = default_pool->size();
  unsigned int random_item_count = default_item_pool_size -
                                   new_item_pool.size() -
                                   default_item_pool_weapon_count;

  repo.getRandom(new_item_pool, random_item_count,
                 [](Item it) { return it.isExplosive(); });

  // Shuffle item pool
  std::shuffle(new_item_pool.begin(), new_item_pool.end(),
               *RNG::inst().getEngine());

  // Insert weapons
  std::vector<const RepositoryID *> weapons;
  repo.getRandom(weapons, default_item_pool_weapon_count, &Item::isExplosive);

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);
  for (int i = 0; i < weapon_slots.size(); i++) {
    new_item_pool.insert(new_item_pool.begin() + weapon_slots[i], weapons[i]);
  }

  // fill queue
  for (const auto &id : new_item_pool)
    item_queue.push(id);

  // TODO: Move this print code
  Console::log("ItemPool report:\n");
  Console::log("total size: %d(%d)\n", default_item_pool_size,
               static_cast<int>(new_item_pool.size()));
  Console::log("\tessentials: %d\n", essential_item_count);
  Console::log("\tweapons: %d\n",
               static_cast<int>(default_item_pool_weapon_count));
  Console::log("\trandom: %d\n", random_item_count);
  Console::log("\n");
}

const RepositoryID *TreasureHuntWorldInventoryRandomization::randomize(
    const RepositoryID *in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void TreasureHuntWorldInventoryRandomization::initialize(
    Scenario scen, const DefaultItemPool *const default_pool) {
  std::vector<const RepositoryID *> new_item_pool;

  auto gold_idol = RepositoryID("4b0def3b-7378-494d-b885-92c334f2f8cb");
  auto bust = RepositoryID("a6bcac8b-9772-424e-b2c4-3bdb4da0e349");
  std::vector<int> gold_idol_possible_slots;
  std::vector<int> gold_idol_final_slots;
  default_pool->getPosition(gold_idol_possible_slots,
                            &Item::isGoodTreasureLocation);
  std::vector<int> used_idxes;
  int idx;
  for (int i = 0; i < 10; i++) {
    do {
      idx = rand() % gold_idol_possible_slots.size();
    } while (std::find(used_idxes.begin(), used_idxes.end(), idx) !=
             used_idxes.end());
    gold_idol_final_slots.push_back(gold_idol_possible_slots[idx]);
    used_idxes.push_back(idx);
  }

  for (int i = 0; i < default_pool->size(); i++) {
    auto &item = RepositoryID("00000000-0000-0000-0000-000000000000");
    if (std::find(gold_idol_final_slots.begin(), gold_idol_final_slots.end(),
                  i) != gold_idol_final_slots.end()) {
      item = gold_idol;
    } else {
      default_pool->getIdAt(item, i);
      if (item == gold_idol) {
        item = bust;
      }
    }
    item_queue.push(repo.getStablePointer(item));
  }
}

const RepositoryID *
NoItemsWorldInventoryRandomization::randomize(const RepositoryID *in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

/** Replaces all non-essential, non-quest items with coins, and all placed
 * weapons with a Hackl 9S. */
void NoItemsWorldInventoryRandomization::initialize(
    Scenario scen, const DefaultItemPool *const default_pool) {
  std::vector<int> essential_items;
  default_pool->getPosition(essential_items, &Item::isEssential);

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);

  std::vector<const RepositoryID *> new_item_pool;
  auto pistol = repo.getStablePointer(
      RepositoryID("1e11fbea-cd51-48bf-8316-a050772d6135"));
  auto coin = repo.getStablePointer(
      RepositoryID("dda002e9-02b1-4208-82a5-cf059f3c79cf"));

  for (int i = 0; i < default_pool->size(); i++) {
    if (std::find(weapon_slots.begin(), weapon_slots.end(), i) !=
        weapon_slots.end()) {
      new_item_pool.insert(new_item_pool.begin() + i, pistol);
    } else if (std::find(essential_items.begin(), essential_items.end(), i) !=
               essential_items.end()) {
      RepositoryID &original_item =
          RepositoryID("00000000-0000-0000-0000-000000000000");
      default_pool->getIdAt(original_item, i);
      new_item_pool.insert(
          new_item_pool.begin() + i,
          repo.getStablePointer(RepositoryID(original_item.toString())));
    } else {
      new_item_pool.insert(new_item_pool.begin() + i, coin);
    }
  }

  for (const auto &id : new_item_pool)
    item_queue.push(id);
}

const RepositoryID *
UnlimitedNPCItemRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not a weapon) [%s]\n",
        repo.getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto randomized_item = repo.getRandom(&Item::isWeapon);
  return randomized_item;
}

// TODO: factor this fn
const RepositoryID *
NPCItemRandomisation::randomize(const RepositoryID *in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not a weapon) [%s]\n",
        repo.getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType();
  };

  auto randomized_item = repo.getRandom(sameType);
  Console::log("NPCItemRandomisation::randomize: %s -> %s\n",
               repo.getItem(*in_out_ID)->string().c_str(),
               repo.getItem(*randomized_item)->string().c_str());

  return randomized_item;
}

const RepositoryID *
HeroInventoryRandomisation::randomize(const RepositoryID *in_out_ID) {
  Console::log("HeroInventoryRandomisation::randomize entered with %s\n",
               in_out_ID->toString().c_str());
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "HeroInventoryRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType();
  };

  auto randomized_item = repo.getRandom(sameType);
  Console::log("HeroInventoryRandomisation::randomize: %s -> %s\n",
               repo.getItem(*in_out_ID)->string().c_str(),
               repo.getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

const RepositoryID *
StashInventoryRandomisation::randomize(const RepositoryID *in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "StashInventoryRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType();
  };

  auto randomized_item = repo.getRandom(sameType);
  Console::log("StashInventoryRandomisation::randomize: %s -> %s\n",
               repo.getItem(*in_out_ID)->string().c_str(),
               repo.getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

Randomizer::Randomizer(RandomisationStrategy *strategy_) {
  strategy = std::unique_ptr<RandomisationStrategy>(strategy_);
}

const RepositoryID *Randomizer::randomize(const RepositoryID *id) {
  // printf("%s\n", id->toString().c_str());
  if (enabled)
    return strategy->randomize(id);
  else
    return id;
}

void Randomizer::initialize(Scenario scen,
                            const DefaultItemPool *const default_pool) {
  enabled = true;
  strategy->initialize(scen, default_pool);
}

void Randomizer::disable() { enabled = false; }

const RepositoryID *
IdentityRandomisation::randomize(const RepositoryID *in_out_ID) {
  return in_out_ID;
}

const RepositoryID *
HardNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  // flash grenades -> frag grenades
  if ((*in_out_ID == RepositoryID("042fae7b-fe9e-4a83-ac7b-5c914a71b2ca")) &&
      config_->randomize_npc_grenades())
    return repo.getStablePointer(
        RepositoryID("3f9cf03f-b84f-4419-b831-4704cff9775c"));

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not a weapon) [%s]\n",
        repo.getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto shotgun = repo.getStablePointer(
      RepositoryID("901a3b51-51a0-4236-bdf2-23d20696b358"));
  auto rifle = repo.getStablePointer(
      RepositoryID("d8aa6eba-0cb7-4ed4-ab99-975f2793d731"));
  auto sniper = repo.getStablePointer(
      RepositoryID("43d15bea-d282-4a91-b625-8b7ba85c0ad5"));
  auto pistol = repo.getStablePointer(
      RepositoryID("304fd49f-0624-4691-8506-149a4b16808e"));
  auto smg = repo.getStablePointer(
      RepositoryID("e206ed81-0559-4289-9fec-e6a3e9d4ee7c"));

  if (in_item->isPistol()) {
    int i = rand() % 10;
    if (i == 0) {
      return pistol;
    } else {
      return smg;
    }
  }

  int i = rand() % 100;
  if (i >= 0 && i < 45) {
    return shotgun;
  } else if (i >= 45 && i < 90) {
    return rand() % 2 == 0 ? smg : rifle;
  } else {
    return sniper;
  }
}

const RepositoryID *
SleepyNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "SleepyNPCRandomization::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  if (!in_item->isWeapon()) {
    return in_out_ID;
  }

  return repo.getStablePointer(
      RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
}

const RepositoryID *
ChainReactionNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  auto shotgun1 = repo.getStablePointer(
      RepositoryID("0af419f5-e6d3-488d-b133-6ce0964b0770"));
  auto shotgun2 = repo.getStablePointer(
      RepositoryID("d5728a0f-fe8d-4e2d-9350-03cf4243c98e"));
  auto rifle1 = repo.getStablePointer(
      RepositoryID("6e4afb04-417e-4cfc-aaa2-43f3ecca9037"));
  auto rifle2 = repo.getStablePointer(
      RepositoryID("e206ed81-0559-4289-9fec-e6a3e9d4ee7c"));
  auto sniper = repo.getStablePointer(
      RepositoryID("370580fc-7fcf-47f8-b994-cebd279f69f9"));

  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "ChainReactionNPCRandomization::randomize: skipped (not in repo) "
        "[%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  if (!in_item->isWeapon()) {
    return in_out_ID;
  }

  // flash grenades -> octane booster
  if ((*in_out_ID == RepositoryID("042fae7b-fe9e-4a83-ac7b-5c914a71b2ca")))
    return repo.getStablePointer(
        RepositoryID("c82fefa7-febe-46c8-90ec-c945fbef0cb4"));

  int i = rand() % 10;
  if (i >= 0 && i < 4) {
    // Cure coin
    return repo.getStablePointer(
        RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
  } else if (i >= 4 && i < 8) {
    // Octane booster
    return repo.getStablePointer(
        RepositoryID("c82fefa7-febe-46c8-90ec-c945fbef0cb4"));
  } else {
    int i = rand() % 100;
    if (i >= 0 && i < 40) {
      return rand() % 2 == 0 ? shotgun1 : shotgun2;
    } else if (i >= 40 && i < 80) {
      return rand() % 2 == 0 ? rifle1 : rifle2;
    } else {
      return sniper;
    }
  }
}
