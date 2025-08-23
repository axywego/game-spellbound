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

void GameWorld::addBuffItem(std::unique_ptr<BuffItem> buffItem, const sf::Vector2f &pos) {
    buffItem->getSprite().setPosition(pos);
    buffItems.push_back(std::move(buffItem));
}

std::vector<std::unique_ptr<Enemy>>& GameWorld::getEnemies() {
    return enemies;
}

std::vector<std::unique_ptr<BuffItem>> & GameWorld::getBuffItems() {
    return buffItems;
}

Tilemap& GameWorld::getTilemap() {
    return map;
}

void GameWorld::render(sf::RenderTarget& target) const {
    map.render(target);
    for (const auto& buffItem : buffItems) {
        buffItem->render(target);
    }
}
