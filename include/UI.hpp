#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>

class Button {
private:
    sf::RenderWindow& window;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Button(const sf::Texture& texture_, sf::RenderWindow& window_): texture(texture_), sprite(texture), window(window_) { }

    void setPosition(sf::Vector2f&& pos){
        sprite.setPosition(pos);
    }

    bool isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(window);
        return sprite.getGlobalBounds().contains({static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)});
    }

    bool isClicked(const std::optional<sf::Event>& event){
        if (!event.has_value()) {
                return false;
        }

        // Безопасная проверка типа события
        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            return mouseEvent->button == sf::Mouse::Button::Left && isHovered();
        }

        return false;
    }

    void render(sf::RenderTarget& renderTarget) {
        renderTarget.draw(sprite);
    }
};

#endif