#pragma once 

#include "Player.hpp"

class Knight : public Player {
private:
    void calculateAttackArea();
public:
    Knight(const Tilemap& map);

    void spawnProjectile() override;

    void checkMelee() override;

    void attack() override;
};