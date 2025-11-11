#include "Button.hpp"

#include <ranges>
#include <iostream>

#include "../ResourceManager.hpp"

namespace UI {
    Button::Button(sf::RenderWindow& window_):
    UIObject(window_), texture(ResourceManager::getInstance().getTexture("default_texture")),
    sprite(texture) {
        sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
        transform.position = sf::Vector2f{0.f, 0.f};
        transform.scale = sf::Vector2f{1.f, 1.f};
        transform.rotation = 0.f;
        transform.size = sf::Vector2f{1.f, 1.f};
    }

    Button::Button(const sf::Texture& texture_, sf::RenderWindow& window_):
    UIObject(window_), texture(texture_), sprite(texture_) {
        sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
        transform.position = sprite.getPosition();
        transform.scale = sf::Vector2f{1.f, 1.f};
        transform.rotation = 0.f;
        transform.size = sprite.getGlobalBounds().size;
    }


    void Button::setTransform(const Transform &t) {
        transform = t;
        setPosition(t.position);
        setScale(t.scale);
    }

    Transform Button::getCurrentTransform() const {
        return transform;
    }

    /**
      @param pos позиция крайней левой верхней точки
     */
    void Button::setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos + sprite.getGlobalBounds().size / 2.f);
    }

    void Button::setScale(const sf::Vector2f &scale) {
        const auto pos = sprite.getGlobalBounds().position;
        sprite.setScale(scale);
        this->setPosition(pos);
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
        const bool isHoveredBool = isHovered();

        for (auto&& [el_anim, el_timer] :
          std::views::zip(animations[currentAnimType], animationProgressInSec[currentAnimType])){
            auto& [anim, data] = el_anim;

            if (isHoveredBool) {
                el_timer += dt;
                if (el_timer >= anim->duration) {
                    if (data.isInfinity) {
                        el_timer = 0.f;
                    }
                    else {
                        el_timer = anim->duration;
                    }
                }
            }
            else {
                el_timer -= dt;
                if (el_timer < 0.f) {
                    el_timer = 0.f;
                }
            }

            data.progress = el_timer / anim->duration;

            anim->apply(sprite, data.progress);
        }
    }

    void Button::render() const {
        window->draw(sprite);
    }
}
