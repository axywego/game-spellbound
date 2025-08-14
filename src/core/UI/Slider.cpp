#include "Slider.hpp"

namespace UI {

    Slider::Slider(sf::RenderWindow& window_, const float& length_, const float& value_):
    UIObject(window_), value(value_), length(length_) {
        leftRectangle.setFillColor(sf::Color::Red);
        rightRectangle.setFillColor(sf::Color::Green);

        leftRectangle.setSize({length * value, 20.f});
        rightRectangle.setSize({length * (1.f - value), 20.f});

        circle.setRadius(20.f);
        circle.setOrigin(circle.getGlobalBounds().size / 2.f);
        circle.setFillColor(sf::Color::Black);
    }

    void Slider::setTransform(const Transform& t) {
        transform = t;
        setPosition(transform.position);
        setScale(transform.scale);
    }

    Transform Slider::getCurrentTransform() const {
        return transform;
    }

    void Slider::setPosition(const sf::Vector2f& pos) {
        leftRectangle.setPosition(pos);
        rightRectangle.setPosition(leftRectangle.getPosition() + sf::Vector2f(length * value, 0.f));
        circle.setPosition(rightRectangle.getPosition() + sf::Vector2f(0.f, 10.f));
    }

    void Slider::setScale(const sf::Vector2f &scale) {

    }

    void Slider::handleInput(const std::optional<sf::Event> &event) {
        if (event.has_value()) {
            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (const auto mousePos = mousePress->position;
                    circle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {

                    inChange = true;
                }
            }
            else if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                inChange = false;
            }
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                if (inChange) {
                    const auto mousePos = mouseMoved->position;
                    const float newX = std::max(leftRectangle.getPosition().x,
                                 std::min(static_cast<float>(mousePos.x), leftRectangle.getPosition().x + length));
                    setValue((newX - leftRectangle.getPosition().x) / length);
                }
            }
        }
    }

    void Slider::setValue(const float &newValue) {
        value = newValue;

        leftRectangle.setSize({length * value, 20.f});
        rightRectangle.setSize({length * (1.f - value), 20.f});
        rightRectangle.setPosition(leftRectangle.getPosition() + sf::Vector2f(length * value, 0.f));

        circle.setPosition(rightRectangle.getPosition() + sf::Vector2f(0.f, 10.f));
    }

    float Slider::getValue() const {
        return value;
    }

    void Slider::update(const float& dt) {

    }

    void Slider::render() const {
        window.draw(leftRectangle);
        window.draw(rightRectangle);
        window.draw(circle);
    }

}