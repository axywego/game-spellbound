#pragma once
#include "UIObject.hpp"

namespace UI {
    class Slider : public UIObject {
    private:
        float value; //from 0 to 1
        float length;
        sf::CircleShape circle;
        sf::RectangleShape leftRectangle;
        sf::RectangleShape rightRectangle;

        bool inChange = false;

    public:
        Slider(sf::RenderWindow& window_, const sf::Vector2f& pos, const float& length, float value_);

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        sf::Vector2f getPosition() const override;

        void handleInput(const std::optional<sf::Event>& event);

        void setValue(const float &newValue);

        float getValue() const;

        void update(const float& dt) override;

        void render() const override;
    };

}

