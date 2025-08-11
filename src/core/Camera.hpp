#pragma once

#include <SFML/Graphics.hpp>
#include "../world/Tilemap.hpp"

class Camera {
private:
    sf::View camera;
    sf::FloatRect rect;
    sf::Sprite* target;
    const Tilemap& map;

    float smoothSpeed = 0.05f; // i like 0.05
public:
    Camera(sf::Sprite* sprite, const Tilemap& map_);

    void setTarget(sf::Sprite* sprite);

    sf::Vector2f getCenter() const;

    void update(const float& dt);

    void applyTo(sf::RenderTarget& renderTarget) const;
};