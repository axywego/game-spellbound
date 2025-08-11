#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../world/Tilemap.hpp"
#include "Utils.hpp"
#include <thread>
#include <mutex>

class Raycast {
private:
    float range{0.f};
    sf::VertexArray line;
    sf::Sprite& targetSprite;
    sf::Sprite& spriteToFind;
    const Tilemap& tilemap;
public:
    Raycast(sf::Sprite& targetSprite_, const Tilemap& tilemap_, sf::Sprite& spriteToFind_);

    void setRange(const float& range_);

    void update();

    bool isInView(const sf::FloatRect& rect) const ;

    void render(sf::RenderTarget& renderTarget) const;
};