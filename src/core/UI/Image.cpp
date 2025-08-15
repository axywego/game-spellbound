#include "Image.hpp"

namespace UI {
    Image::Image(sf::RenderWindow &window_, const sf::Texture &texture_): UIObject(window_), image(texture_) { }

    void Image::setTransform(const Transform &t) {
        transform = t;
        setPosition(transform.position);
        setScale(transform.scale);
    }

    Transform Image::getCurrentTransform() const {
        return transform;
    }

    void Image::setPosition(const sf::Vector2f &pos) {
        image.setPosition(pos);
    }

    void Image::setScale(const sf::Vector2f &scale) {
        image.setScale(scale);
    }

    void Image::update(const float &dt) {

    }

    void Image::render() const {
        window->draw(image);
    }
}
