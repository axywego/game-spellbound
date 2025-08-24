#pragma once

#include <SFML/Graphics.hpp>

class Scene {
protected:
    sf::RenderWindow& window;

    sf::Vector2f lastPlayerPos {0.f,0.f};

public:
    Scene(sf::RenderWindow& window_): window(window_) {}

    virtual void load() = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget& renderTarget) = 0;
    virtual void handleEvent(const std::optional<sf::Event>& event) = 0;
    virtual sf::Vector2f getCameraCenter() const = 0;

    virtual ~Scene() = default;
};