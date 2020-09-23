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
  spdlog::get("console")->info("CustomWorldStrategy::initialize");
  std::vector<RepositoryID*> item_pool;

  default_pool->getL(item_pool, [this](Item it) {
    return config_->custom_world_rules_.ShouldPermit(it);
  });
  if (item_pool.size() == 0) {
    spdlog::get("console")->error(
        "CustomWorldStrategy::randomize: could not find any matching items. Game "
        "will probably crash.");
  }

  // Key and quest items
  std::vector<int> essential_items;
  default_pool->getPosition(essential_items, &Item::isEssential);

  const unsigned int default_item_pool_size = default_pool->size();

  std::vector<const RepositoryID*> new_item_pool;

  for (int i = 0; i < default_item_pool_size; i++) {
    if (std::find(essential_items.begin(), essential_items.end(), i) != essential_items.end()) {
      RepositoryID &original_item =
          RepositoryID("00000000-0000-0000-0000-000000000000");
      default_pool->getIdAt(original_item, i);
      new_item_pool.insert(
          new_item_pool.begin() + i,
          repo.getStablePointer(RepositoryID(original_item.toString())));
    } else {
      new_item_pool.insert(
          new_item_pool.begin() + i,
          *select_randomly(item_pool.begin(), item_pool.end()));
    }
  }

  for (const auto &id : new_item_pool) {
    item_queue.push(id);
  }
  spdlog::get("console")->info("CustomWorldStrategy::initialize complete with {} items.", item_pool.size());
}

void CustomNPCStrategy::initialize(Scenario scen,
                                   const DefaultItemPool *const default_pool) {
  spdlog::get("console")->info("CustomNPCStrategy::initialize");

  default_pool->getL(item_pool_, [this](Item it) {
    return config_->custom_npc_rules_.ShouldPermit(it);
  });
  if (item_pool_.size() == 0) {
    spdlog::get("console")->error(
        "CustomNPCStrategy::randomize: could not find any matching items. Game "
        "will probably crash.");
  }

  spdlog::get("console")->info("CustomNPCStrategy::initialize complete with {} items.", item_pool_.size());
}

const RepositoryID *
CustomNPCStrategy::randomize(const RepositoryID *in_out_ID) {
  spdlog::get("console")->info("CustomNPCStrategy::randomize");
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
  auto result = *select_randomly(item_pool_.begin(), item_pool_.end());
  spdlog::get("console")->info("CustomNPCStrategy::randomize complete.");
  return result;
}