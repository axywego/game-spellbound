#include "GameLevelScene.hpp"

#include <map>
#include <utility>
#include "../entities/mob/buffs/BuffItem.hpp"

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

    const auto playerPtr = player.lock();

    std::ranges::for_each(enemies,
        [&](const std::unique_ptr<Enemy>& e) {
            if (!e->getIsAlive()) {
                std::vector<StatModifier> modifiers;
                modifiers.push_back({StatType::MaxHealth, ModifierType::Flat, 10.f});
                gameWorld.addBuffItem(std::make_unique<BuffItem>(
                    ResourceManager::getInstance().getTexture("buff_item"), "Health Improvement", modifiers),
                    e->getSprite().getPosition());
            }
        }
    );

    std::erase_if(enemies,
      [](const std::unique_ptr<Enemy>& e) {
          return !e->getIsAlive();
        }
    );

    std::erase_if(gameWorld.getBuffItems(),
        [&](const std::unique_ptr<BuffItem>& b) {
            return b->getIsPickup();
        }
    );

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
            enemy->takeDamage(playerPtr->getDamage());
        }

        enemy->update(dt);
    }

    std::ranges::for_each(gameWorld.getBuffItems(),
        [&](const std::unique_ptr<BuffItem>& b) {
            if (b->getSprite().getGlobalBounds().findIntersection(playerPtr->getCollisionRect())) {
                b->onPickup(playerPtr.get());
            }
        }
    );

    lastPlayerPos = playerPtr->getSprite().getPosition();

    UI::HUD::getInstance().update(dt, *playerPtr, camera.getCenter());
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