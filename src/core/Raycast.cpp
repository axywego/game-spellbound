#include "Raycast.hpp"

#include <iostream>

Raycast::Raycast(sf::Sprite& targetSprite_, const Tilemap& tilemap_, sf::Sprite& spriteToFind_):
targetSprite(targetSprite_), tilemap(tilemap_), spriteToFind(spriteToFind_) {
    line.resize(2);
    line.setPrimitiveType(sf::PrimitiveType::Lines);
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
}

void Raycast::setRange(const float &range_) {
    range = range_;
}

void Raycast::update() {
    const auto pos = targetSprite.getPosition();
    const auto posSpriteToFind = spriteToFind.getPosition();
    const float squareDistance = powf(posSpriteToFind.x - pos.x, 2) + powf(posSpriteToFind.y - pos.y, 2);
    if (squareDistance > range * range) return;

    line[0].position = pos;
    line[1].position = posSpriteToFind;

    const auto collisionTiles = tilemap.getCollisionTilesInRange(pos, range);

    for (const auto& tile : collisionTiles) {
        const auto intersection = findIntersection(tile.getGlobalBounds(), line[0].position, line[1].position);
        if (intersection) {
            line[1].position = intersection.value();
            break;
        }
    }
}

bool Raycast::isInView(const sf::FloatRect& rect) const {
    return findIntersection(rect, line[0].position, line[1].position).has_value();
}

void Raycast::render(sf::RenderTarget& renderTarget) const {
    renderTarget.draw(line);
}
