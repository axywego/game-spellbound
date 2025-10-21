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

        std::function<void(const bool&)> onClick;

        bool checked {false};

    public:
        CheckBox(sf::RenderWindow& window, const bool& checked);

        void setTransform(const Transform& t) override;

        Transform getCurrentTransform() const override;

        void setPosition(const sf::Vector2f& pos) override;

        void setScale(const sf::Vector2f &scale) override;

        void setFunc(const std::function<void(const bool&)>& func);

        void setValue(bool value);

        bool getValue() const;

        bool isHovered() const;

        bool isClicked(const std::optional<sf::Event> &event) const;

        void handleInput(const std::optional<sf::Event> &event);

        void update(const float& dt) override;

        void render() const override;

    };
}

