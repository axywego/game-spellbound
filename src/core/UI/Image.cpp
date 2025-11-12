#include "Image.hpp"

#include <ranges>
#include <iostream>

namespace UI {
    Image::Image(sf::RenderWindow &window_, const sf::Texture &texture_):
    UIObject(window_),
    texture(texture_),
    sprite(texture_) {
        std::cout << "eblan" << '\n';
    }

    void Image::setTransform(const Transform &t) {
        transform = t;
        setPosition(transform.position);
        setScale(transform.scale);
    }

    Transform Image::getCurrentTransform() const {
        return transform;
    }

    void Image::setPosition(const sf::Vector2f &pos) {
        sprite.setPosition(pos);
    }

    void Image::setScale(const sf::Vector2f &scale) {
        sprite.setScale(scale);
    }

    bool Image::isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(*window);
        return sprite.getGlobalBounds().contains({static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)});
    }

    bool Image::isClicked(const std::optional<sf::Event>& event) const {
        if (!event.has_value()) {
            return false;
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            return mouseEvent->button == sf::Mouse::Button::Left && isHovered();
        }

        return false;
    }

    void Image::update(const float &dt) {
        if (currentAnimType == TypeAnimation::Hovered) {
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
        else if (currentAnimType == TypeAnimation::SlideShow) {
            for (auto&& [el_anim, el_timer] :
              std::views::zip(animations[currentAnimType], animationProgressInSec[currentAnimType])){
                auto& [anim, data] = el_anim;

                el_timer += dt;
                if (el_timer >= anim->duration) {
                    if (data.isInfinity) {
                        el_timer = 0.f;
                    }
                    else {
                        el_timer = anim->duration;
                    }
                }

                data.progress = el_timer / anim->duration;

                anim->apply(sprite, data.progress);
            }
        }
    }

    void Image::render() const {
        window->draw(sprite);
    }
}
