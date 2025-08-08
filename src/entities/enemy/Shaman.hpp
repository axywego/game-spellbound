#pragma once

#include "Enemy.hpp"
#include "Rat.hpp"

class Shaman : public Enemy {
private:

public:
    Shaman(const Tilemap& map_, const std::weak_ptr<Player> &player_);

    void spawnProjectile() override;
    void spawnMobs() override;
};