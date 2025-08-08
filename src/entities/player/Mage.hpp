#pragma once

#include "Player.hpp"

class Mage : public Player {
public:
    Mage(const Tilemap& map);

    void spawnProjectile() override ;

    void checkMelee() override;

    void attack() override;
};