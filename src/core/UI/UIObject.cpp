#include "UIObject.hpp"

namespace UI {
    UIObject::UIObject(): transform(), window(nullptr) {
    }

    UIObject::UIObject(sf::RenderWindow& window_):
    transform(Transform {{0.f, 0.f}, 0.f, {1.f, 1.f}}), window(&window_) { }

    void UIObject::setWindow(sf::RenderWindow &window_) {
        window = &window_;
    }
}
