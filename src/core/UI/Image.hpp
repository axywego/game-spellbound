#pragma once

#include "UIObject.hpp"

namespace UI {
    class Image : public UIObject {
    private:
        sf::Sprite image;

    public:
        Image(sf::RenderWindow& window_, const sf::Texture& texture_);

        void setTransform(const Transform& t) override;

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        void setScale(const sf::Vector2f& scale) override;

        void update(const float& dt) override;

        void render() const override;
    };

}
