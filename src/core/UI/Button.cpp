#include "Button.hpp"

#include <ranges>
#include <iostream>

#include "../ResourceManager.hpp"

namespace UI {
    Button::Button(sf::RenderWindow& window_):
    Image(window_, ResourceManager::getInstance().getTexture("default_texture")) {
        sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
        transform.position = sf::Vector2f{0.f, 0.f};
        transform.scale = sf::Vector2f{1.f, 1.f};
        transform.rotation = 0.f;
        transform.size = sf::Vector2f{1.f, 1.f};
    }

    Button::Button(const sf::Texture& texture_, sf::RenderWindow& window_):
    Image(window_, texture_) {
        sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
        transform.position = sprite.getPosition();
        transform.scale = sf::Vector2f{1.f, 1.f};
        transform.rotation = 0.f;
        transform.size = sprite.getGlobalBounds().size;
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

    void Button::render() const {
        window->draw(sprite);
    }
}
