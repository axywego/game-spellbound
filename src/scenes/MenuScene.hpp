#pragma once 

#include "Scene.hpp"
#include "../core/UI.hpp"

#include <functional>

class MenuScene: public Scene {
private:

    sf::View view;
    sf::Vector2f target;

    sf::CircleShape circle;
    float circleScale { 1.f };
    float transitionSpeed { 2.f };
    bool isTransition { true };

    sf::Texture textureBackground;
    sf::Sprite backgroundImage;

    Button startButton;
    Button settingsButton;
    Button exitButton;

    std::function<void()> onStartClick;
    std::function<void()> onExitClick;

public:
    MenuScene(sf::RenderWindow& window_, std::function<void()> startCallback, std::function<void()> exitCallback);

    void load() override ;

    void update(const float& dt) override;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override;
};