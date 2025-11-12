#pragma once

#include "UIObject.hpp"

namespace UI {
    class Image : public UIObject {
    protected:
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        Image(sf::RenderWindow& window_, const sf::Texture& texture_);

        void setTransform(const Transform& t) override;

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        void setScale(const sf::Vector2f& scale) override;

        void setRotation(const float degrees) override;

        bool isHovered() const override;

        bool isClicked(const std::optional<sf::Event>& event) const override;

        void update(const float& dt) override;

        void render() const override;
    };

}
