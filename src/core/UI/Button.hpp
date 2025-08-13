#pragma once

#include "UIObject.hpp"

namespace UI {
    class Button : public UIObject {
    private:

    public:
        Button(const sf::Texture& texture_, sf::RenderWindow& window_);

        bool isHovered() const;

        bool isClicked(const std::optional<sf::Event>& event) const;

        void update(const float& dt) override;
    };
}