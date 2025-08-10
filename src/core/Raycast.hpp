#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../world/Tilemap.hpp"
#include "Utils.hpp"
#include <thread>
#include <mutex>

class Raycast {
private:
    size_t numOfLines{30};
    std::vector<sf::VertexArray> lines;
    float range{0.f};
    sf::Sprite& targetSprite;
    const Tilemap& tilemap;
public:
    Raycast(sf::Sprite& targetSprite_, const Tilemap& tilemap_);

    void init();

    void setRange(const float& range_);

    void update();

    bool isInView(const sf::FloatRect& rect) const ;

    void render(sf::RenderTarget& renderTarget) const;
};