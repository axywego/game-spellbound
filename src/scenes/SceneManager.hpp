#pragma once

#include "PauseScene.hpp"
#include "MenuScene.hpp"
#include "GameLevelScene.hpp"
#include "../entities/player/PlayerFactory.hpp"

class SceneManager {
private:

    sf::RenderWindow& window;

    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::unordered_map<std::string, std::shared_ptr<GameWorld>> worlds;
    std::shared_ptr<Scene> currentScene, nextScene;

    std::shared_ptr<Player> player;

    sf::CircleShape transitionShape;
    bool isTransition = false;
    float transitionTime = 0.f;
    const float maxTransitionTime = 0.5f;

    bool isPaused = false;
    std::shared_ptr<Scene> pausedScene;

public:
    SceneManager(sf::RenderWindow& window_);

    void initScenes();

    std::weak_ptr<Player> getPlayer();

    Scene& getScene(const std::string& name);

    template<typename T, typename... Args>
    void addScene(const std::string& name, Args&&... args);

    void switchTo(const std::string& sceneName);

    void startTransition();

    void pause();

    void resume();

    void update(const float& dt);

    void render(sf::RenderTarget& renderTarget) const;

    void handleEvent(const std::optional<sf::Event>& event) const;
};