#include "GameWorld.hpp"

GameWorld::GameWorld(const Tilemap& map_): map(map_){
    //enemies[0]->setPosition({500.f, 500.f});
}

GameWorld::GameWorld(Tilemap&& map_): map(map_) {}

GameWorld::GameWorld(const TiledShape& tiledShape) {
    map.createFromTiledShape(tiledShape);
}

void GameWorld::addEnemy(std::unique_ptr<Enemy> e, const sf::Vector2f& pos){
    e->setPosition(pos);
    enemies.push_back(std::move(e));
}

std::vector<std::unique_ptr<Enemy>>& GameWorld::getEnemies() {
    return enemies;
}

Tilemap& GameWorld::getTilemap() {
    return map;
}

void GameWorld::render(sf::RenderTarget& target) const {
    map.render(target);
}
