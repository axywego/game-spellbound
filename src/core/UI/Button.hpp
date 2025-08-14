#pragma once

#include "UIObject.hpp"

namespace UI {
    class Button : public UIObject {
    private:
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        Button(const sf::Texture& texture_, sf::RenderWindow& window_);

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        sf::Vector2f getPosition() const override;

        bool isHovered() const;

        bool isClicked(const std::optional<sf::Event>& event) const;

        void update(const float& dt) override;

        void render() const override;
    };
}