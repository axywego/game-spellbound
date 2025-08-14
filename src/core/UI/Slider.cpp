#include "Slider.hpp"

namespace UI {

    Slider::Slider(sf::RenderWindow& window_, const sf::Vector2f& pos, const float& length_, float value_):
    UIObject(window_), value(value_), length(length_) {
        leftRectangle.setFillColor(sf::Color::Red);
        rightRectangle.setFillColor(sf::Color::Green);

        leftRectangle.setSize({length * value, 20.f});
        rightRectangle.setSize({length * (1.f - value), 20.f});

        leftRectangle.setPosition(pos);
        rightRectangle.setPosition(leftRectangle.getPosition() + sf::Vector2f(length * value, 0.f));

        circle.setRadius(20.f);
        circle.setOrigin(circle.getGlobalBounds().size / 2.f);
        circle.setPosition(rightRectangle.getPosition() + sf::Vector2f(0.f, 10.f));
        circle.setFillColor(sf::Color::Black);
    }

    Transform Slider::getCurrentTransform() const {
        return {};
    }

    void Slider::setPosition(const sf::Vector2f& pos) {
        leftRectangle.move(pos - leftRectangle.getPosition());
        rightRectangle.move(pos - rightRectangle.getPosition());
        circle.move(pos - circle.getPosition());
    }

    sf::Vector2f Slider::getPosition() const {
        return leftRectangle.getPosition();
    }

    void Slider::handleInput(const std::optional<sf::Event> &event) {
        if (event.has_value()) {
            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                auto mousePos = mousePress->position;
                if (circle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
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