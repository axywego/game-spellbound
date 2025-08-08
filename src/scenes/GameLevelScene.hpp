#pragma once 

#include "Scene.hpp"
#include "../core/Camera.hpp"
#include "../entities/player/Player.hpp"
#include "../world/GameWorld.hpp"
#include "../entities/enemy/Enemy.hpp"
#include "../generators/EnemyGenerator.hpp"
#include "../core/UI.hpp"

#include <algorithm>
#include <functional>

class GameLevelScene : public Scene {
private:
    std::weak_ptr<Player> player;
    Camera camera;

    std::function<void()> requestPause;

    GameWorld& gameWorld;

    std::vector<std::unique_ptr<Enemy>>& enemies;

public:
    GameLevelScene(GameWorld& world_, std::weak_ptr<Player> player_, sf::RenderWindow& window_, std::function<void()> pauseCallback);
    void load() override;

    void update(const float& dt) override;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override;
};