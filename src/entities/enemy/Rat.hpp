#pragma once

#include "Enemy.hpp"
#include "../../core/ResourceManager.hpp"

class Rat : public Enemy {
private:

public:
    Rat(const Tilemap& map_, const std::weak_ptr<Player> &player_);

    void spawnProjectile() override;
    void spawnMobs() override;
};