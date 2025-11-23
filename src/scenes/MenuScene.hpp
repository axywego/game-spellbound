#pragma once 

#include "Scene.hpp"
#include "../core/UI/Button.hpp"
#include "../core/UI/Image.hpp"
#include "../core/ResourceManager.hpp"


#include <functional>

class MenuScene: public Scene {
private:

    sf::View view;
    sf::Vector2f target;

    sf::CircleShape circle;
    float circleScale { 1.f };
    float transitionSpeed { 2.f };
    bool isTransition { true };

    UI::Image backgroundImage;

    UI::Button startButton;
    UI::Button continueButton;
    UI::Button settingsButton;
    UI::Button exitButton;

    std::function<void()> onStartClick;
    std::function<void()> onSettingsClick;
    std::function<void()> onExitClick;
    std::function<void()> onContinueClick;

public:
    MenuScene(sf::RenderWindow& window_,
        const std::string& name,
        const std::function<void()> &startCallback,
        const std::function<void()> &continueCallback,
        const std::function<void()> &settingsCallback,
        const std::function<void()> &exitCallback);

    void load() override ;

    void update(const float& dt) override;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override;
};