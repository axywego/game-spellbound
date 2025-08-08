#pragma once

#include "Scene.hpp"
#include "../core/UI.hpp"

#include <functional>

class PauseScene : public Scene {
private:
    sf::View view;
    
    //sf::Sprite backgroundImage;

    sf::RectangleShape overlay;
    Button resumeButton;
    Button menuButton;

    std::function<void()> onResumeClick;
    std::function<void()> onMenuClick;
public:
    PauseScene(sf::RenderWindow& window_, std::function<void()> resumeCallback, std::function<void()> menuCallback);

    void load() override ;

    void update(const float& dt) override ;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override ;
};