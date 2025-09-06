#pragma once

#include <SFML/Graphics.hpp>

class Scene {
protected:
    std::string name;

    sf::RenderWindow& window;

    sf::Vector2f lastPlayerPos {0.f,0.f};

public:
    Scene(sf::RenderWindow& window_, const std::string& name): window(window_), name(name) {}

    virtual void load() = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget& renderTarget) = 0;
    virtual void handleEvent(const std::optional<sf::Event>& event) = 0;
    virtual sf::Vector2f getCameraCenter() const = 0;

    std::string getName() const { return name; }

    virtual ~Scene() = default;
};