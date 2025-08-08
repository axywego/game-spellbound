#pragma once

#include "Player.hpp"

class Archer : public Player {  
public:
    Archer(const Tilemap& map);

    void spawnProjectile() override;

    void checkMelee() override;

    void attack() override;
};