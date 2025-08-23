#pragma once

#include "Tilemap.hpp"
#include "../entities/enemy/Enemy.hpp"
#include <vector>

#include "../entities/mob/buffs/BuffItem.hpp"

class GameWorld {
private:

    Tilemap map;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<BuffItem>> buffItems;

public:
    GameWorld(const Tilemap& map_);

    GameWorld(Tilemap&& map_);

    GameWorld(const TiledShape& tiledShape);

    void addEnemy(std::unique_ptr<Enemy> e, const sf::Vector2f& pos);

    void addBuffItem(std::unique_ptr<BuffItem> buffItem, const sf::Vector2f& pos);

    std::vector<std::unique_ptr<Enemy>>& getEnemies();

    std::vector<std::unique_ptr<BuffItem>>& getBuffItems();

    Tilemap& getTilemap();

    void render(sf::RenderTarget& target) const;
};