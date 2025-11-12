#pragma once

#include "UIObject.hpp"

namespace UI {
    class Image : public UIObject {
    protected:
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        Image(sf::RenderWindow& window_, const sf::Texture& texture_);

        virtual void setTransform(const Transform& t) override;

        virtual Transform getCurrentTransform() const override;

        virtual void setPosition(const sf::Vector2f& pos) override;

        virtual void setScale(const sf::Vector2f& scale) override;

        bool isHovered() const override;

        bool isClicked(const std::optional<sf::Event>& event) const override;

        virtual void update(const float& dt) override;

        virtual void render() const override;
    };

}
