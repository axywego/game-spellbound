#pragma once

#include "Rat.hpp"
#include "Shaman.hpp"

class EnemyFactory {
public:
    static std::unique_ptr<Enemy> create(EnemyClass type, const Tilemap& map, std::weak_ptr<Player> player);
};