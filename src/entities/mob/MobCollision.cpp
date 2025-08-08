#include "MobCollision.hpp"

MobCollision::MobCollision(sf::Sprite& target_, const sf::FloatRect collisionRect_):
        lineHorizontalUp(sf::PrimitiveType::Lines, 2), lineHorizontalDown(sf::PrimitiveType::Lines, 2),
        lineVerticalLeft(sf::PrimitiveType::Lines, 2), lineVerticalRight(sf::PrimitiveType::Lines, 2),
        target(target_), collisionRect(collisionRect_) {

    lineHorizontalDown[0].color = sf::Color::White;
    lineHorizontalDown[1].color = sf::Color::White;

    lineHorizontalUp[0].color = sf::Color::White;
    lineHorizontalUp[1].color = sf::Color::White;

    lineVerticalLeft[0].color = sf::Color::White;
    lineVerticalLeft[1].color = sf::Color::White;

    lineVerticalRight[0].color = sf::Color::White;
    lineVerticalRight[1].color = sf::Color::White;
}

void MobCollision::update() {
    auto rectSprite = target.getGlobalBounds();
    rectSprite.position += collisionRect.position;
    rectSprite.size = collisionRect.size;

    lineHorizontalUp[0].position = {rectSprite.position.x, rectSprite.position.y + offset};
    lineHorizontalUp[1].position = {rectSprite.position.x + rectSprite.size.x, rectSprite.position.y + offset};

    lineHorizontalDown[0].position = {rectSprite.position.x, rectSprite.position.y + rectSprite.size.y - offset};
    lineHorizontalDown[1].position = {rectSprite.position.x + rectSprite.size.x, rectSprite.position.y + rectSprite.size.y - offset};

    lineVerticalLeft[0].position = {rectSprite.position.x + offset, rectSprite.position.y};
    lineVerticalLeft[1].position = {rectSprite.position.x + offset, rectSprite.position.y + rectSprite.size.y};

    lineVerticalRight[0].position = {rectSprite.position.x + rectSprite.size.x - offset, rectSprite.position.y};
    lineVerticalRight[1].position = {rectSprite.position.x + rectSprite.size.x - offset, rectSprite.position.y + rectSprite.size.y};

}

sf::Vector2f MobCollision::checkCollisions(const Tilemap& map) const {
    const auto collisions = map.getCollisionTiles();

    sf::Vector2f incorrectDir {0.f, 0.f};

    for(const auto& spr : collisions) {
        if (spr.getGlobalBounds().contains(lineHorizontalUp[0].position) || spr.getGlobalBounds().contains(lineHorizontalDown[0].position)){
            incorrectDir.x = -1.f;
        }
        else if (spr.getGlobalBounds().contains(lineHorizontalUp[1].position) || spr.getGlobalBounds().contains(lineHorizontalDown[1].position)){
            incorrectDir.x = 1.f;
        }
        if (spr.getGlobalBounds().contains(lineVerticalLeft[0].position) || spr.getGlobalBounds().contains(lineVerticalRight[0].position)){
            incorrectDir.y = -1.f;
        }
        else if (spr.getGlobalBounds().contains(lineVerticalLeft[1].position) || spr.getGlobalBounds().contains(lineVerticalRight[1].position)){
            incorrectDir.y = 1.f;
        }
    }
    return incorrectDir;
}

void MobCollision::render(sf::RenderTarget& renderTarget) {
    renderTarget.draw(lineHorizontalDown);
    renderTarget.draw(lineHorizontalUp);

    renderTarget.draw(lineVerticalLeft);
    renderTarget.draw(lineVerticalRight);
}

sf::FloatRect MobCollision::getCollisionRect() const {
    auto rectSprite = target.getGlobalBounds();
    rectSprite.position += collisionRect.position;
    rectSprite.size = collisionRect.size;
    return rectSprite;
}