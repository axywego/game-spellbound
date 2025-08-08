#pragma once

#include "Enemy.hpp"

class Rat : public Enemy {
private:

public:
    Rat(const Tilemap& map_, std::weak_ptr<Player> player_);

    void spawnProjectile() override;
    void spawnMobs() override;
};