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

// void GameLevelScene::update(const float& dt) {
//
//     std::erase_if(enemies,
//               [](const std::unique_ptr<Enemy>& e) {
//                   return !e->getIsAlive();
//               });
//
//     player.lock()->update(dt);
//
//     camera.update(dt);
//
//     player.lock()->updateProjectiles(dt);
//
//     const auto& playerMeleeAreaAttack = player.lock()->getAttackArea();
//
//     for (size_t i = 0; i < enemies.size(); i++) {
//         const auto& enemy = enemies[i];
//
//         //Player has distance damage
//         for (auto& projectile : player.lock()->getProjectiles()) {
//             if (projectile->getCollisionRect().findIntersection(enemy->getCollisionRect())) {
//                 projectile->onHit(enemy.get());
//             }
//         }
//
//         //Player has melee damage
//         if(playerMeleeAreaAttack && playerMeleeAreaAttack->findIntersection(enemy->getCollisionRect())){
//             enemy->takeDamage(player.lock()->getDamage());
//         }
//
//         enemy->update(dt);
//
//     }
//
//     lastPlayerPos = player.lock()->getSprite().getPosition();
//
//     HUD::update(*player.lock(), camera.getCenter());
// }

void GameLevelScene::update(const float& dt) {

    const auto playerPtr = player.lock();

    std::erase_if(enemies,
              [](const std::unique_ptr<Enemy>& e) {
                  return !e->getIsAlive();
              });

    playerPtr->update(dt);

    camera.update(dt);

    playerPtr->updateProjectiles(dt);

    auto& playerProjectiles = playerPtr->getProjectiles();

    const auto& playerMeleeAreaAttack = playerPtr->getAttackArea();

    std::erase_if(playerProjectiles, [&](const auto& p) {
        return !isOnScreen(p->getCollisionRect(), window);
    });

    for (auto& enemy : enemies) {
        if (!isOnScreen(enemy->getSprite(), window)) continue;

        //Player has distance damage
        for (auto& projectile : playerProjectiles) {
            if (projectile->getCollisionRect().findIntersection(enemy->getCollisionRect())) {
                projectile->onHit(enemy.get());
            }
        }

        //Player has melee damage
        if(playerMeleeAreaAttack && playerMeleeAreaAttack->findIntersection(enemy->getCollisionRect())){
            enemy->takeDamage(player.lock()->getDamage());
        }

        enemy->update(dt);
    }
    lastPlayerPos = playerPtr->getSprite().getPosition();

    HUD::update(*playerPtr, camera.getCenter());
}

void GameLevelScene::render(sf::RenderTarget& renderTarget) {
    camera.applyTo(renderTarget);

    gameWorld.render(renderTarget);

    player.lock()->render(renderTarget);

    for(const auto& enemy : enemies) {
        if (!isOnScreen(enemy->getSprite(), renderTarget)) continue;
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