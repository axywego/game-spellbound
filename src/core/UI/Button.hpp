#pragma once

#include "UIObject.hpp"

namespace UI {
    class Button final : public UIObject {
    private:
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        Button(const sf::Texture& texture_, sf::RenderWindow& window_);

        void setTransform(const Transform& t) override;

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        void setScale(const sf::Vector2f &scale) override;

        bool isHovered() const override;

        bool isClicked(const std::optional<sf::Event>& event) const override;

        void update(const float& dt) override;

        void render() const override;
    };
}