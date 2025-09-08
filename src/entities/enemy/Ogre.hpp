#pragma once

#include "Enemy.hpp"
#include "../../core/ResourceManager.hpp"

class Ogre : public Enemy {
private:

public:
    Ogre(const Tilemap& map_, const std::weak_ptr<Player> &player_);

    void spawnProjectile() override;
    void spawnMobs() override;
};