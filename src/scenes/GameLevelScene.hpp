#pragma once 

#include "Scene.hpp"
#include "../core/Camera.hpp"
#include "../entities/player/Player.hpp"
#include "../world/GameWorld.hpp"
#include "../entities/enemy/Enemy.hpp"
#include "../generators/EnemyGenerator.hpp"
#include "../core/UI/HUD.hpp"
#include "../core/Audio/SoundManager.hpp"

#include <functional>

class GameLevelScene : public Scene {
private:
    std::weak_ptr<Player> player;
    Camera camera;

    std::function<void()> requestPause;
    std::function<void()> requestNextScene;

    GameWorld& gameWorld;

    std::vector<std::unique_ptr<Enemy>>& enemies;

public:
    GameLevelScene(sf::RenderWindow& window_, const std::string& name, GameWorld& world_, std::weak_ptr<Player> player_, std::function<void()> pauseCallback, std::function<void()> nextDungeonCallback);
    void load() override;

    void updateEnemies(const std::shared_ptr<Player>& playerPtr, const float& dt) const;

    void updateBuffs(const std::shared_ptr<Player>& playerPtr, const float& dt) const;

    void updateProjectiles(const std::shared_ptr<Player>& playerPtr, const float& dt) const;

    void update(const float& dt) override;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override;
};