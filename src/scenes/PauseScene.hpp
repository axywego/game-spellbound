#pragma once

#include "Scene.hpp"
#include "../core/UI/Button.hpp"
#include "../core/ResourceManager.hpp"

#include <functional>

class PauseScene : public Scene {
private:
    sf::View view;
    
    //sf::Sprite backgroundImage;

    sf::RectangleShape overlay;
    UI::Button resumeButton;
    UI::Button menuButton;

    std::function<void()> onResumeClick;
    std::function<void()> onMenuClick;
public:
    PauseScene(sf::RenderWindow& window_, const std::function<void()> &resumeCallback, const std::function<void()> &menuCallback);

    void load() override ;

    void update(const float& dt) override ;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override ;
};