#include "UIObject.hpp"

namespace UI {

    UIObject::UIObject(const sf::Texture& texture_, sf::RenderWindow& window_):
        texture(texture_), sprite(texture), window(window_) {}

    Transform UIObject::getCurrentTransform() const {
        return {
            sprite.getPosition(),
            sprite.getRotation().asDegrees(),
            sprite.getScale(),
        };
    }

    void UIObject::setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
    }

    sf::Vector2f UIObject::getPosition() const {
        return sprite.getPosition();
    }

    void UIObject::render() const {
        window.draw(sprite);
    }

}