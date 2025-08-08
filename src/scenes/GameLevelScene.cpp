#include "GameLevelScene.hpp"

#include <utility>

GameLevelScene::GameLevelScene(GameWorld& world_, std::weak_ptr<Player> player_,
    sf::RenderWindow& window_, std::function<void()> pauseCallback):
Scene(window_), player(player_), gameWorld(world_), camera(&(player.lock()->getSprite()),
    world_.getTilemap()), requestPause(std::move(pauseCallback)), enemies(world_.getEnemies()) {}

void GameLevelScene::load() {

    auto generatedEnemies = EnemyGenerator::generateEnemies(gameWorld, player);

    for(auto& [enemy, pos] : generatedEnemies){
        gameWorld.addEnemy(std::move(enemy), pos);
    }

    player.lock()->setPosition({50 * 16 * 5 + 16 / 2, 50 * 16 * 5 + 16 / 2});

}

void GameLevelScene::update(const float& dt) {

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const std::unique_ptr<Enemy>& e) {
                           return !e->getIsAlive();
                       }),
        enemies.end()
    );

    player.lock()->update(dt);

    camera.update(dt);

    player.lock()->updateProjectiles(dt);

    for (size_t i = 0; i < enemies.size(); i++) {
        const auto& enemy = enemies[i];

        //Player has distance damage
        for (auto& projectile : player.lock()->getProjectiles()) {
            if (projectile->getCollisionRect().findIntersection(enemy->getCollisionRect())) {
                projectile->onHit(enemy.get());
            }
        }
        //Player has melee damage
        if(const auto area = player.lock()->getAttackArea()){
            if(area->findIntersection(enemy->getCollisionRect())){
                enemy->takeDamage(player.lock()->getDamage());
            }
        }
        enemy->update(dt);

    }

    // for(const auto& enemy : enemies) {
    //     if (!enemy) {
    //         std::cout << "pizda\n";
    //     }
    //
    //     //Player has distance damage
    //     for (auto& projectile : player.lock()->getProjectiles()) {
    //         if (projectile->getCollisionRect().findIntersection(enemy->getCollisionRect())) {
    //             projectile->onHit(enemy.get());
    //         }
    //     }
    //     //Player has melee damage
    //     if(const auto area = player.lock()->getAttackArea()){
    //         if(area->findIntersection(enemy->getCollisionRect())){
    //             enemy->takeDamage(player.lock()->getDamage());
    //         }
    //     }
    //     i++;
    //     enemy->update(dt);
    // }

    lastPlayerPos = player.lock()->getSprite().getPosition();

    HUD::update(*player.lock(), camera.getCenter());
}

void GameLevelScene::render(sf::RenderTarget& renderTarget) {
    camera.applyTo(renderTarget);

    gameWorld.render(renderTarget);

    player.lock()->render(renderTarget);

    for(const auto& enemy : enemies) {
        enemy->render(renderTarget);
    }

    HUD::render(renderTarget);
}

void GameLevelScene::handleEvent(const std::optional<sf::Event>& event) {
    if(event){
        if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
            if(key->code == sf::Keyboard::Key::Escape)
                requestPause();

            if(key->code == sf::Keyboard::Key::Space)
                player.lock()->attack();
        }
    }
}
sf::Vector2f GameLevelScene::getCameraCenter() const {
    return camera.getCenter();
}