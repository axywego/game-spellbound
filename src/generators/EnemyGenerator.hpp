#pragma once

#include "../world/GameWorld.hpp"
#include "../entities/enemy/EnemyFactory.hpp"
#include "../entities/player/Player.hpp"

class EnemyGenerator {
private:

public:
    static std::vector<std::pair<std::unique_ptr<Enemy>, sf::Vector2f>> generateEnemies
            (GameWorld& map, std::weak_ptr<Player> player);
};
