#include "Button.hpp"

#include <ranges>

namespace UI {

    Button::Button(const sf::Texture& texture_, sf::RenderWindow& window_):
    UIObject(window_), texture(texture_), sprite(texture_) {
        sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
    }

    void Button::setTransform(const Transform &t) {
        transform = t;
        setPosition(t.position);
        setScale(t.scale);
    }

    Transform Button::getCurrentTransform() const {
        return transform;
    }

    void Button::setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos + sprite.getGlobalBounds().size / 2.f);
    }

    void Button::setScale(const sf::Vector2f &scale) {
        sprite.setScale(scale);
    }

    bool Button::isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(*window);
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

        for (auto&& [el_anim, el_timer] :
          std::views::zip(animations[currentAnimType], animationProgressInSec[currentAnimType])){
            auto& [anim, data] = el_anim;
            if (isPlus) {
                if(data.progress >= 1.f)
                    if (data.isInfinity) data.progress = 0.f;
                    else continue;
            }
            else {
                if(data.progress <= 0.f)
                    if (data.isInfinity) data.progress = 1.f;
                    else continue;
            }
            data.progress = el_timer / anim->duration;
            isPlus ? el_timer += dt : el_timer -= dt;

            anim->apply(sprite, data.progress);
        }
    }

    void Button::render() const {
        window->draw(sprite);
    }
}
