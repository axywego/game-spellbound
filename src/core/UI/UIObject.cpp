#include "UIObject.hpp"

namespace UI {
    UIObject::UIObject(sf::RenderWindow& window_):
    window(window_), transform(Transform {{0.f, 0.f}, 0.f, {1.f, 1.f}}) { }
}