#pragma once

#include <SFML/Graphics.hpp>
#include "../../world/Tilemap.hpp"

class MobCollision {
private:
    sf::VertexArray lineHorizontalUp;
    sf::VertexArray lineHorizontalDown;

    sf::VertexArray lineVerticalLeft;
    sf::VertexArray lineVerticalRight;

    sf::Sprite& target;

    sf::FloatRect collisionRect;

    const float offset = 5.f;
public:
    MobCollision(sf::Sprite& target_, const sf::FloatRect collisionRect_);

    void update() ;

    sf::Vector2f checkCollisions(const Tilemap& map) const;
    
    void render(sf::RenderTarget& renderTarget);

    sf::FloatRect getCollisionRect() const;
};