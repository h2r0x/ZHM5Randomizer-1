#include <algorithm>
#include "Randomizer.h"

#include <random>

#include "Config.h"
#include "Console.h"
#include "DefaultItemPool.h"
#include "Item.h"
#include "Offsets.h"
#include "RNG.h"
#include "Repository.h"

RandomisationStrategy::RandomisationStrategy()
    : repo(RandomDrawRepository::inst()) {}

void RandomisationStrategy::initialize(Scenario, const DefaultItemPool* const) {
}

const RepositoryID* WorldInventoryRandomisation::randomize(
    const RepositoryID* in_out_ID) {
  if (repo.contains(*in_out_ID) && item_queue.size()) {
    const RepositoryID* id = item_queue.front();
    item_queue.pop();
    Console::log("WorldInventoryRandomisation::randomize: %d: %s -> %s\n",
                 static_cast<int>(item_queue.size()),
                 repo.getItem(*in_out_ID)->string().c_str(),
                 repo.getItem(*id)->string().c_str());
    return id;
  } else {
    if (!item_queue.size())
      Console::log(
          "WorldInventoryRandomisation::randomize: skipped (queue exhausted) "
          "[%s]\n",
          in_out_ID->toString().c_str());
    else
      Console::log(
          "WorldInventoryRandomisation::randomize: skipped (not in repo) "
          "[%s]\n",
          in_out_ID->toString().c_str());
    return in_out_ID;
  }
}

void setupTools(std::vector<const RepositoryID*>& items) {}

void WorldInventoryRandomisation::initialize(
    Scenario scen, const DefaultItemPool* const default_pool) {
  std::vector<const RepositoryID*> new_item_pool;

  // Key and quest items
  default_pool->get(new_item_pool, &Item::isEssential);
  unsigned int essential_item_count = new_item_pool.size();

  // Tool items
  // TODO: factor this out of init
  auto crowbar = repo.getStablePointer(
      RepositoryID("01ed6d15-e26e-4362-b1a6-363684a7d0fd"));
  auto screwdriver = repo.getStablePointer(
      RepositoryID("12cb6b51-a6dd-4bf5-9653-0ab727820cac"));
  auto wrench = repo.getStablePointer(
      RepositoryID("6adddf7e-6879-4d51-a7e2-6a25ffdca6ae"));

  auto addOriginalNumberOfItems = [default_pool,
                                   &new_item_pool](const RepositoryID* id) {
    auto cnt = default_pool->getCount(*id);
    for (int i = 0; i < cnt; ++i) new_item_pool.push_back(id);
  };

  addOriginalNumberOfItems(crowbar);
  addOriginalNumberOfItems(screwdriver);
  addOriginalNumberOfItems(wrench);

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
  std::vector<const RepositoryID*> weapons;
  repo.getRandom(weapons, default_item_pool_weapon_count, &Item::isWeapon);

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);
  for (int i = 0; i < weapon_slots.size(); i++) {
    new_item_pool.insert(new_item_pool.begin() + weapon_slots[i], weapons[i]);
  }

  // fill queue
  for (const auto& id : new_item_pool) item_queue.push(id);

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

const RepositoryID* OopsAllExplosivesWorldInventoryRandomization::randomize(
    const RepositoryID* in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void OopsAllExplosivesWorldInventoryRandomization::initialize(
    Scenario scen, const DefaultItemPool* const default_pool) {
  std::vector<const RepositoryID*> new_item_pool;

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
                 [](Item it) { return it.string() == "Octane Booster"; });

  // Shuffle item pool
  std::shuffle(new_item_pool.begin(), new_item_pool.end(),
               *RNG::inst().getEngine());

  // Insert weapons
  std::vector<const RepositoryID*> weapons;
  repo.getRandom(weapons, default_item_pool_weapon_count, &Item::isExplosive);

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);
  for (int i = 0; i < weapon_slots.size(); i++) {
    new_item_pool.insert(new_item_pool.begin() + weapon_slots[i], weapons[i]);
  }

  // fill queue
  for (const auto& id : new_item_pool) item_queue.push(id);

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

const RepositoryID* TreasureHuntWorldInventoryRandomization::randomize(
    const RepositoryID* in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

void TreasureHuntWorldInventoryRandomization::initialize(
    Scenario scen, const DefaultItemPool* const default_pool) {
  std::vector<const RepositoryID*> new_item_pool;
  
  auto gold_idol = RepositoryID("4b0def3b-7378-494d-b885-92c334f2f8cb");
  std::vector<int> gold_idol_possible_slots;
  std::vector<int> gold_idol_final_slots;
  default_pool->getPosition(gold_idol_possible_slots,
                            &Item::isGoodTreasureLocation);
  std::vector<int> used_idxes;
  int idx;
  for (int i = 0; i < 10; i++) {
    do {
      idx = rand() % gold_idol_possible_slots.size();
    } while (std::find(used_idxes.begin(), used_idxes.end(), idx) != used_idxes.end());
    gold_idol_final_slots.push_back(gold_idol_possible_slots[idx]);
    used_idxes.push_back(idx);
  }

  std::vector<int> decorative_item_slots;
  default_pool->getPosition(decorative_item_slots,
                            &Item::isDecorativeMeleeItem);

  const std::vector<RepositoryID> tools({
      RepositoryID("01ed6d15-e26e-4362-b1a6-363684a7d0fd"),
      RepositoryID("12cb6b51-a6dd-4bf5-9653-0ab727820cac"),
      RepositoryID("6adddf7e-6879-4d51-a7e2-6a25ffdca6ae")
  });

  for (int i = 0; i < default_pool->size(); i++) {
    auto& item = RepositoryID("00000000-0000-0000-0000-000000000000");
    if (std::find(gold_idol_final_slots.begin(), gold_idol_final_slots.end(),
                  i) != gold_idol_final_slots.end()) {
      Console::log("adding gold idol: %d\n", i);
      item = gold_idol;
    } else if (std::find(decorative_item_slots.begin(),
                         decorative_item_slots.end(),
                         i) != decorative_item_slots.end()) {
      int j = rand() % tools.size();
      item = tools[j];
      Console::log("adding tool: %d\n", i);
    } else {
      default_pool->getIdAt(item, i);
      Console::log("adding %s: %d\n", repo.getItem(item)->string().c_str(), i);
    }
    item_queue.push(repo.getStablePointer(item));
  }
}

const RepositoryID* NoItemsWorldInventoryRandomization::randomize(
    const RepositoryID* in_out_ID) {
  return WorldInventoryRandomisation::randomize(in_out_ID);
}

/** Replaces all non-essential, non-quest items with coins, and all placed
 * weapons with a Shaskha. */
void NoItemsWorldInventoryRandomization::initialize(
    Scenario scen, const DefaultItemPool* const default_pool) {
  std::vector<int> essential_items;
  default_pool->getPosition(essential_items, &Item::isEssential);

  std::vector<int> weapon_slots;
  default_pool->getPosition(weapon_slots, &Item::isWeapon);

  std::vector<const RepositoryID*> new_item_pool;
  auto rifle = repo.getStablePointer(
      RepositoryID("6e4afb04-417e-4cfc-aaa2-43f3ecca9037"));
  auto coin = repo.getStablePointer(
      RepositoryID("dda002e9-02b1-4208-82a5-cf059f3c79cf"));

  for (int i = 0; i < default_pool->size(); i++) {
    if (std::find(weapon_slots.begin(), weapon_slots.end(), i) !=
        weapon_slots.end()) {
      new_item_pool.insert(new_item_pool.begin() + i, rifle);
    } else if (std::find(essential_items.begin(), essential_items.end(), i) !=
               essential_items.end()) {
      RepositoryID& original_item =
          RepositoryID("00000000-0000-0000-0000-000000000000");
      default_pool->getIdAt(original_item, i);
      new_item_pool.insert(
          new_item_pool.begin() + i,
          repo.getStablePointer(RepositoryID(original_item.toString())));
    } else {
      new_item_pool.insert(new_item_pool.begin() + i, coin);
    }
  }

  for (const auto& id : new_item_pool) item_queue.push(id);
}

// TODO: factor this fn
const RepositoryID* NPCItemRandomisation::randomize(
    const RepositoryID* in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  // Special case for flash grenades: ~10% banana chance
  if ((*in_out_ID == RepositoryID("042fae7b-fe9e-4a83-ac7b-5c914a71b2ca") &&
       Config::randomizeNPCGrenades && (rand() % 10 == 0)))
    return repo.getStablePointer(
        RepositoryID("903d273c-c750-441d-916a-31557fea3382"));  // Banana

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not a weapon) [%s]\n",
        repo.getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto sameType = [&in_item](const Item& item) {
    return in_item->getType() == item.getType();
  };

  auto randomized_item = repo.getRandom(sameType);
  Console::log("NPCItemRandomisation::randomize: %s -> %s\n",
               repo.getItem(*in_out_ID)->string().c_str(),
               repo.getItem(*randomized_item)->string().c_str());

  return randomized_item;
}

const RepositoryID* HeroInventoryRandomisation::randomize(
    const RepositoryID* in_out_ID) {
  Console::log("HeroInventoryRandomisation::randomize entered with %s\n",
               in_out_ID->toString().c_str());
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "HeroInventoryRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  auto sameType = [&in_item](const Item& item) {
    return in_item->getType() == item.getType();
  };

  auto randomized_item = repo.getRandom(sameType);
  Console::log("HeroInventoryRandomisation::randomize: %s -> %s\n",
               repo.getItem(*in_out_ID)->string().c_str(),
               repo.getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

const RepositoryID* StashInventoryRandomisation::randomize(
    const RepositoryID* in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "StashInventoryRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  auto sameType = [&in_item](const Item& item) {
    return in_item->getType() == item.getType();
  };

  auto randomized_item = repo.getRandom(sameType);
  Console::log("StashInventoryRandomisation::randomize: %s -> %s\n",
               repo.getItem(*in_out_ID)->string().c_str(),
               repo.getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

Randomizer::Randomizer(RandomisationStrategy* strategy_) {
  strategy = std::unique_ptr<RandomisationStrategy>(strategy_);
}

const RepositoryID* Randomizer::randomize(const RepositoryID* id) {
  // printf("%s\n", id->toString().c_str());
  if (enabled)
    return strategy->randomize(id);
  else
    return id;
}

void Randomizer::initialize(Scenario scen,
                            const DefaultItemPool* const default_pool) {
  enabled = true;
  strategy->initialize(scen, default_pool);
}

void Randomizer::disable() { enabled = false; }

const RepositoryID* IdentityRandomisation::randomize(
    const RepositoryID* in_out_ID) {
  return in_out_ID;
}

const RepositoryID* UnrestrictedNPCRandomization::randomize(
    const RepositoryID* in_out_ID) {
  if (!repo.contains(*in_out_ID)) {
    Console::log(
        "NPCItemRandomisation::randomize: skipped (not in repo) [%s]\n",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo.getItem(*in_out_ID);

  // flash grenades -> frag grenades
  if ((*in_out_ID == RepositoryID("042fae7b-fe9e-4a83-ac7b-5c914a71b2ca")) &&
      Config::randomizeNPCGrenades)
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

  int i = rand() % 100;
  if (i >= 0 && i < 40) {
    return rand() % 2 == 0 ? shotgun1 : shotgun2;
  } else if (i >= 40 && i < 80) {
    return rand() % 2 == 0 ? rifle1 : rifle2;
  } else {
    return sniper;
  }
}

const RepositoryID* SleepyNPCRandomization::randomize(
    const RepositoryID* in_out_ID) {
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

  // int i = rand() % 10;
  // if (i == 0) {
  //	return
  //repo.getStablePointer(RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
  //}
  // Cure coin
   return repo.getStablePointer(RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
  // Octane booster
  // return repo.getStablePointer(
  //     RepositoryID("c82fefa7-febe-46c8-90ec-c945fbef0cb4"));
}

const RepositoryID* ChainReactionNPCRandomization::randomize(
    const RepositoryID* in_out_ID) {
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
  if (i >= 0 && i < 3) {
    // Cure coin
    return repo.getStablePointer(
        RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
  } else if (i >= 3 && i < 6) {
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
