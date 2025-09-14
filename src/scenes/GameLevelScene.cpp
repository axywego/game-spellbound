#include "GameLevelScene.hpp"

#include <utility>
#include "../generators/BuffsGenerator.hpp"

GameLevelScene::GameLevelScene( sf::RenderWindow& window_, const std::string& name, GameWorld& world_, std::weak_ptr<Player> player_, std::function<void()> pauseCallback, std::function<void()> nextDungeonCallback):
Scene(window_, name),
player(player_),
gameWorld(world_),
camera(&(player.lock()->getSprite()),
world_.getTilemap()),
requestPause(std::move(pauseCallback)),
enemies(world_.getEnemies()),
requestNextScene(std::move(nextDungeonCallback))
{
    auto generatedEnemies = EnemyGenerator::generateEnemies(gameWorld, player);

    for(auto& [enemy, pos] : generatedEnemies){
        gameWorld.addEnemy(std::move(enemy), pos);
    }
}

void GameLevelScene::load() {
    if (lastPlayerPos.x != 0.f && lastPlayerPos.y != 0.f)
        player.lock()->setPosition(lastPlayerPos);
}

void GameLevelScene::updateEnemies(const std::shared_ptr<Player>& playerPtr, const float &dt) const {

    const auto& playerMeleeAreaAttack = playerPtr->getAttackArea();
    auto& playerProjectiles = playerPtr->getProjectiles();

    for (size_t i = 0; i < enemies.size(); i++) {
        auto& enemy = enemies[i];
        if (enemy->getIsAlive() && isOnScreen(enemy->getSprite(), window)) {
            //Player has distance damage
            for (auto& projectile : playerProjectiles) {
                if (projectile->isActive() &&
                    projectile->getCollisionRect().findIntersection(enemy->getCollisionRect())
                    ) {
                    projectile->onHit(enemy.get());

                    SoundManager::getInstance().getSound("enemy_hit").play();
                }
            }

            if(playerMeleeAreaAttack &&
                playerMeleeAreaAttack->findIntersection(enemy->getCollisionRect())
                ){
                enemy->takeDamage(playerPtr->getDamage());

                SoundManager::getInstance().getSound("knight_attack").play();
                SoundManager::getInstance().getSound("enemy_hit").play();
            }

            enemy->update(dt);
        }
    }
    std::erase_if(enemies, [&](const auto& enemy) {
        if (!enemy->getIsAlive()) {
            if (auto opt = BuffsGenerator::create(*playerPtr)) {
                gameWorld.addBuffItem(std::move(opt.value()), enemy->getSprite().getPosition());
            }
            return true;
        }
        return false;
    });
}

void GameLevelScene::updateBuffs(const std::shared_ptr<Player>& playerPtr, const float &dt) const {

    auto& buffs = gameWorld.getBuffItems();
    for (auto it = buffs.begin(); it != buffs.end(); ) {
        if ((*it)->getSprite().getGlobalBounds().findIntersection(playerPtr->getCollisionRect())) {
            (*it)->onPickup(playerPtr.get());

            SoundManager::getInstance().getSound("pickup_buff").play();
        }

        if ((*it)->getIsPickup()) {
            it = buffs.erase(it);
        }
        else {
            ++it;
        }
    }

}

void GameLevelScene::updateProjectiles(const std::shared_ptr<Player>& playerPtr, const float &dt) const {

    auto& playerProjectiles = playerPtr->getProjectiles();
    for(auto it = playerProjectiles.begin(); it != playerProjectiles.end(); ){
        (*it)->update(dt);

        if(!(*it)->isActive() || !isOnScreen((*it)->getCollisionRect(), window)){
            it = playerProjectiles.erase(it);
        }
        else {
            ++it;
        }
    }

}

void GameLevelScene::update(const float& dt) {

    if (const auto playerPtr = player.lock()) {
        playerPtr->update(dt);

        camera.update(dt);

        updateProjectiles(playerPtr, dt);
        updateEnemies(playerPtr, dt);
        updateBuffs(playerPtr, dt);

        lastPlayerPos = playerPtr->getSprite().getPosition();

        UI::HUD::getInstance().update(dt, *playerPtr, camera.getCenter(), enemies.size());
    }

}

void GameLevelScene::render(sf::RenderTarget& renderTarget) {
    camera.applyTo(renderTarget);

    gameWorld.render(renderTarget);

    player.lock()->render(renderTarget);

    for(const auto& enemy : enemies) {
        if (!isOnScreen(enemy->getSprite(), renderTarget)) continue;
        enemy->render(renderTarget);
    }

    UI::HUD::getInstance().render();
}

void GameLevelScene::handleEvent(const std::optional<sf::Event>& event) {
    if(event){
        if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
            switch (key->code) {
                using Key = sf::Keyboard::Key;
                case Key::Escape: {
                    lastPlayerPos = player.lock()->getSprite().getPosition();
                    requestPause();
                    break;
                }
                case Key::Space: {
                    player.lock()->attack();
                    break;
                }
                case Key::Enter: {
                    if (enemies.empty()) {
                        requestNextScene();
                    }
                    break;
                }
                default: break;
            }
        }
    }
}

sf::Vector2f GameLevelScene::getCameraCenter() const {
    return camera.getCenter();
}