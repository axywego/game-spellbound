#include "Button.hpp"

namespace UI {

    Button::Button(const sf::Texture& texture_, sf::RenderWindow& window_):
    UIObject(window_), texture(texture_), sprite(texture_) {
        sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
    }

    Transform Button::getCurrentTransform() const {
        return {
            sprite.getPosition(),
            sprite.getRotation().asDegrees(),
            sprite.getScale()
        };
    }

    void Button::setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos + sprite.getGlobalBounds().size / 2.f);
    }

    sf::Vector2f Button::getPosition() const {
        return sprite.getPosition() - sprite.getGlobalBounds().size / 2.f;
    }

    bool Button::isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(window);
        return sprite.getGlobalBounds().contains({static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)});
    }

    bool Button::isClicked(const std::optional<sf::Event>& event) const {
        if (!event.has_value()) {
            return false;
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            return mouseEvent->button == sf::Mouse::Button::Left && isHovered();
        }

        return false;
    }

    void Button::update(const float& dt) {
        const bool isPlus = isHovered();
        for (auto& currentAnim = animations[currentAnimType]; auto& [anim, data] : currentAnim) {
            if (isPlus) {
                if(data.progress >= 1.f)
                    continue;
            }
            else {
                if(data.progress <= 0.f)
                    continue;
            }
            data.progress = animationProgressInSec[currentAnimType] / anim->duration;
            isPlus ? animationProgressInSec[currentAnimType] += dt : animationProgressInSec[currentAnimType] -= dt;

            anim->apply(sprite, data.progress);
        }
    }

    void Button::render() const {
        window.draw(sprite);
    }
}
