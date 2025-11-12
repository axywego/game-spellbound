#pragma once

#include "UIObject.hpp"
#include "../ResourceManager.hpp"

namespace UI {
    class Text final : public UIObject {
    private:
        sf::Font font{ResourceManager::getInstance().getFont("font_game")};
        sf::Text text{font};
    public:
        Text();
        explicit Text(sf::RenderWindow& window);
        Text(sf::RenderWindow& window, const sf::Font& font);
        Text(sf::RenderWindow& window, const sf::Font& font, const std::string& text);
        Text(sf::RenderWindow& window, const std::string& text);
        Text(sf::RenderWindow& window, const std::string& text, const sf::Color& color);

        Text(const Text& other);
        Text& operator=(const Text& other);

        void setFont(const sf::Font& newFont);
        void setText(const std::string& textStr);
        void setColor(const sf::Color& color);
        std::string getText() const;

        void setTransform(const Transform& t) override;

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        void setScale(const sf::Vector2f& scale) override;

        void setRotation(const float degrees) override;

        bool isHovered() const override;

        bool isClicked(const std::optional<sf::Event>& event) const override;

        void update(const float& dt) override;

        void render() const override;

        void setOutlineColor(const sf::Color& color);

        void setOutlineThickness(const float& x);
    };
}

