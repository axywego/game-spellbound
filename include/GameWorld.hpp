#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include "Tilemap.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include <vector>

class GameWorld {
private:

    Tilemap map;
    std::vector<std::unique_ptr<Enemy>> enemies;

public:
    GameWorld(const std::string& name) {
        map = Tilemap(name);
    }

    GameWorld(const Tilemap& map_): map(map_){
        //enemies[0]->setPosition({500.f, 500.f});
    }

    GameWorld(Tilemap&& map_): map(map_) {}

    GameWorld(const TiledShape& tiledShape) {
        map.createFromTiledShape(tiledShape);
    }

    void addEnemy(std::unique_ptr<Enemy> e, const sf::Vector2f& pos){
        e->setPosition(pos);
        enemies.push_back(std::move(e));
    }

    std::vector<std::unique_ptr<Enemy>>& getEnemies() {
        return enemies;
    }

    Tilemap& getTilemap() {
        return map;
    }

    void render(sf::RenderTarget& target) {
        map.render(target);
    }
};

#endif