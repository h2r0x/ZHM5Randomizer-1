#include "ZHM5Randomizer/src/randomizers/custom.h"

#include <algorithm>

#include <random>

#include "ZHM5Randomizer/src/Config.h"
#include "ZHM5Randomizer/src/Console.h"
#include "ZHM5Randomizer/src/DefaultItemPool.h"
#include "ZHM5Randomizer/src/Item.h"
#include "ZHM5Randomizer/src/Offsets.h"
#include "ZHM5Randomizer/src/RNG.h"
#include "ZHM5Randomizer/src/Repository.h"
#include "spdlog/spdlog.h"

const RepositoryID *
CustomWorldStrategy::randomize(const RepositoryID *in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void CustomWorldStrategy::initialize(
    Scenario scen, const DefaultItemPool *const default_pool) {
  std::vector<const RepositoryID *> test_pool;
  repo.getRandom(test_pool, 1, [this](Item it) {
    return config_->custom_npc_rules_.ShouldPermit(it);
  });
  if (test_pool.size() == 0) {
    spdlog::get("console")->error(
        "CustomWorldStrategy::initialize: could not find any matching items. "
        "Game will probably crash.");
  }

  // Key and quest items
  std::vector<const RepositoryID *> new_item_pool;
  default_pool->get(new_item_pool, &Item::isEssential);

  int default_item_pool_size = default_pool->size();
  unsigned int random_item_count =
      default_item_pool_size - new_item_pool.size();

  repo.getRandom(new_item_pool, random_item_count, [this](Item it) {
    return config_->custom_world_rules_.ShouldPermit(it);
  });

  // Shuffle item pool
  std::shuffle(new_item_pool.begin(), new_item_pool.end(),
               *RNG::inst().getEngine());

  //   std::vector<int> weapon_slots;
  //   default_pool->getPosition(weapon_slots, &Item::isWeapon);
  //   for (int i = 0; i < weapon_slots.size(); i++) {
  //     new_item_pool.insert(new_item_pool.begin() + weapon_slots[i],
  //     weapons[i]);
  //   }

  // fill queue
  for (const auto &id : new_item_pool) {
    item_queue.push(id);
  }
}

void CustomNPCStrategy::initialize(Scenario scen,
                                   const DefaultItemPool* const default_pool) {
  default_pool->getL(item_pool_, [this](Item it) {
    return config_->custom_npc_rules_.ShouldPermit(it);
  });
  if (item_pool_.size() == 0) {
    spdlog::get("console")->error(
        "CustomNPCStrategy::randomize: could not find any matching items. Game "
        "will probably crash.");
  }

  return;
}

const RepositoryID *
CustomNPCStrategy::randomize(const RepositoryID *in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    spdlog::get("console")->info(
        "CustomNPCStrategy::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  if (in_item->isEssential()) {
    spdlog::get("console")->info(
        "CustomNPCStrategy::randomize: skipped (essential) [{}]",
        repo.getItem(*in_out_ID)->string());
    return in_out_ID;
  }

  return *select_randomly(item_pool_.begin(), item_pool_.end());
}