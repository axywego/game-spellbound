#pragma once

#include "Archer.hpp"
#include "Knight.hpp"
#include "Mage.hpp"

class PlayerFactory {
public:
    static std::shared_ptr<Player> create(PlayerClass type, const Tilemap& map);
};