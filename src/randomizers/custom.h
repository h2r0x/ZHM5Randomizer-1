#pragma once

#include <queue>
#include <random>
#include <type_traits>
#include <unordered_map>

#include "ZHM5Randomizer/src/Config.h"
#include "ZHM5Randomizer/src/Repository.h"
#include "ZHM5Randomizer/src/Scenario.h"
#include "ZHM5Randomizer/src/randomizers/Randomizer.h"


class CustomWorldStrategy : public WorldInventoryRandomisation {
public:
  CustomWorldStrategy(std::shared_ptr<hitman_randomizer::Config> config)
      : WorldInventoryRandomisation(config) {}
  const RepositoryID *randomize(const RepositoryID *in_out_ID) override final;
  void initialize(Scenario scen,
                  const DefaultItemPool *const default_pool) override final;
};

class CustomNPCStrategy : public RandomisationStrategy {
public:
  CustomNPCStrategy(std::shared_ptr<hitman_randomizer::Config> config)
      : RandomisationStrategy(config) {}
  const RepositoryID *randomize(const RepositoryID *in_out_ID) override final;
};
