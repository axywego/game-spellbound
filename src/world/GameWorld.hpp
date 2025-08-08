#pragma once

#include "Tilemap.hpp"
#include "../entities/enemy/Enemy.hpp"
#include <vector>

class GameWorld {
private:

    Tilemap map;
    std::vector<std::unique_ptr<Enemy>> enemies;

public:
    GameWorld(const Tilemap& map_);

    GameWorld(Tilemap&& map_);

    GameWorld(const TiledShape& tiledShape);

    void addEnemy(std::unique_ptr<Enemy> e, const sf::Vector2f& pos);

    std::vector<std::unique_ptr<Enemy>>& getEnemies();

    Tilemap& getTilemap();

    void render(sf::RenderTarget& target);
};