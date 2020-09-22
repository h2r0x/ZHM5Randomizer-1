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

const RepositoryID* CustomWorldStrategy::randomize(
    const RepositoryID* in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void CustomWorldStrategy::initialize(
    Scenario scen, const DefaultItemPool* const default_pool) {
  std::vector<const RepositoryID*> new_item_pool;

  // Key and quest items
  default_pool->get(new_item_pool, &Item::isEssential);

  int default_item_pool_size = default_pool->size();
  unsigned int random_item_count = default_item_pool_size -
                                   new_item_pool.size();

  repo.getRandom(new_item_pool, random_item_count, [this](Item it) {
      return config_->custom_world_rules_.ShouldPermit(it);
  });

  // Shuffle item pool
  std::shuffle(new_item_pool.begin(), new_item_pool.end(),
               *RNG::inst().getEngine());

//   std::vector<int> weapon_slots;
//   default_pool->getPosition(weapon_slots, &Item::isWeapon);
//   for (int i = 0; i < weapon_slots.size(); i++) {
//     new_item_pool.insert(new_item_pool.begin() + weapon_slots[i], weapons[i]);
//   }

  // fill queue
  for (const auto& id : new_item_pool) {
      item_queue.push(id);
  }
}

const RepositoryID* CustomNPCStrategy::randomize(
    const RepositoryID* in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "CustomNPCStrategy::randomize: skipped (not in repo) [%s]\n",
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

  auto randomized_item = repo.getRandom([this](Item it) {
      return config_->custom_npc_rules_.ShouldPermit(it);
  });
  return randomized_item;  
}