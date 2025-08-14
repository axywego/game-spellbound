#pragma once

#include "UIObject.hpp"
#include "../ResourceManager.hpp"


namespace UI {
    class CheckBox : public UIObject {
    private:
        sf::RectangleShape frame[4];
        sf::Texture texture {ResourceManager::getInstance().getTexture("check_mark")};
        sf::Sprite checkMark {texture};

        sf::FloatRect checkMarkRect;

        bool checked {false};

    public:
        CheckBox(sf::RenderWindow& window, const sf::Vector2f& pos, const bool& checked);

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        sf::Vector2f getPosition() const override;

        bool isHovered() const;

        bool isClicked(const std::optional<sf::Event> &event) const;

        void handleInput(const std::optional<sf::Event> &event);

        void update(const float& dt) override;

        void render() const override;

    };
}

