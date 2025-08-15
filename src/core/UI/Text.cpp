#include "Text.hpp"

namespace UI {
    Text::Text(): UIObject() { }

    Text::Text(sf::RenderWindow &window): UIObject(window) { }

    Text::Text(sf::RenderWindow &window, const sf::Font& font): UIObject(window), font(font), text(font) { }

    Text::Text(sf::RenderWindow &window, const sf::Font& font, const std::string& text):
    UIObject(window), font(font), text({font, text}) { }

    Text::Text(sf::RenderWindow &window, const std::string& text): UIObject(window), text({font, text}) { }

    Text::Text(sf::RenderWindow &window, const std::string& text, const sf::Color& color):
    UIObject(window), text({font, text}) {
        this->text.setFillColor(color);
    }

    Text::Text(const Text& other): UIObject(other) {
        this->text = other.text;
        this->font = other.font;
    }

    Text& Text::operator=(const Text& other) {
        if (this == &other) {
            return *this;
        }
        text = other.text;
        font = other.font;
        return *this;
    }

    void Text::setFont(const sf::Font &newFont) {
        font = newFont;
    }

    void Text::setText(const std::string &textStr) {
        text.setString(textStr);
    }

    void Text::setColor(const sf::Color &color) {
        text.setFillColor(color);
    }

    std::string Text::getText() const {
        return text.getString();
    }

    void Text::setTransform(const Transform &t) {
        transform = t;
        setPosition(transform.position);
        setScale(transform.scale);
    }

    Transform Text::getCurrentTransform() const {
        return transform;
    }

    void Text::setPosition(const sf::Vector2f &pos) {
        text.setPosition(pos);
    }

    void Text::setScale(const sf::Vector2f &scale) {
        text.setScale(scale);
    }

    bool Text::isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(*window);
        return text.getGlobalBounds().contains({static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)});
    }

    bool Text::isClicked(const std::optional<sf::Event>& event) const {
        if (!event.has_value()) {
            return false;
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            return mouseEvent->button == sf::Mouse::Button::Left && isHovered();
        }

        return false;
    }
    void Text::update(const float &dt) {

    }

    void Text::render() const {
        if (window)
            window->draw(text);
    }

    void Text::setOutlineColor(const sf::Color& color) {
        text.setOutlineColor(color);
    }

    void Text::setOutlineThickness(const float& x) {
        text.setOutlineThickness(x);
    }
}
