#pragma once
#include "UIObject.hpp"

namespace UI {
    class Slider final : public UIObject {
    private:
        float value; //from 0 to 1
        float length;
        sf::CircleShape circle;
        sf::RectangleShape leftRectangle;
        sf::RectangleShape rightRectangle;

        bool inChange = false;

    public:
        Slider(sf::RenderWindow& window_, const float& length_, const float& value_);

        void setTransform(const Transform& t) override;

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        void setScale(const sf::Vector2f &scale) override;

        void handleInput(const std::optional<sf::Event>& event);

        void setValue(const float &newValue);

        float getValue() const;

        bool isHovered() const override;

        bool isClicked(const std::optional<sf::Event>& event) const override;

        void update(const float& dt) override;

        void render() const override;
    };

}

