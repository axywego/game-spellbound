//
// Created by goasd on 14.08.2025.
//

#include "CheckBox.hpp"

namespace UI {
    CheckBox::CheckBox(sf::RenderWindow &window, const sf::Vector2f& pos, const bool& checked): UIObject(window), checked(checked) {
        frame[0].setSize({50.f, 5.f});
        frame[1].setSize({5.f, 50.f});
        frame[2].setSize({50.f, 5.f});
        frame[3].setSize({5.f, 50.f});

        frame[0].setFillColor(sf::Color::Black);
        frame[1].setFillColor(sf::Color::Black);
        frame[2].setFillColor(sf::Color::Black);
        frame[3].setFillColor(sf::Color::Black);

        checkMark.setTextureRect({{20, 20},{10, 10}});
        checkMark.setOrigin(checkMark.getGlobalBounds().size / 2.f);
        checkMark.setScale({5.f, 5.f});
        checkMark.setColor(sf::Color::Black);

        CheckBox::setPosition(pos);
    }

    Transform CheckBox::getCurrentTransform() const {
        return {};
    }

    void CheckBox::setPosition(const sf::Vector2f &pos) {
        //top
        frame[0].setPosition(pos);
        //right
        frame[1].setPosition({pos.x + 45.f, pos.y});
        //bottom
        frame[2].setPosition({pos.x, pos.y + 45.f});
        //left
        frame[3].setPosition(pos);

        checkMark.setPosition(pos + sf::Vector2f{25.f, 25.f});

        checkMarkRect.position = frame[0].getPosition();
        checkMarkRect.size = {50.f, 50.f};
    }

    sf::Vector2f CheckBox::getPosition() const {
        return frame[0].getPosition();
    }

    bool CheckBox::isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(window);
        return checkMarkRect.contains({static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)});
    }

    bool CheckBox::isClicked(const std::optional<sf::Event>& event) const {
        if (!event.has_value()) {
            return false;
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            return mouseEvent->button == sf::Mouse::Button::Left && isHovered();
        }

        return false;
    }

    void CheckBox::handleInput(const std::optional<sf::Event> &event) {
        if (isClicked(event)) {
            checked = !checked;
        }
    }

    void CheckBox::update(const float &dt) {
        const bool isPlus = isHovered();
        for (auto& currentAnim = animations[currentAnimType]; auto& [anim, data] : currentAnim) {
            if (isPlus) {
                if(data.progress >= 1.f)
                    continue;
            }
            else {
                if(data.progress <= 0.f)
                    continue;
            }
            data.progress = animationProgressInSec[currentAnimType] / anim->duration;
            isPlus ? animationProgressInSec[currentAnimType] += dt : animationProgressInSec[currentAnimType] -= dt;

            anim->apply(checkMark, data.progress);
        }
    }

    void CheckBox::render() const {
        for (const auto& el : frame) {
            window.draw(el);
        }
        if (checked) {
            window.draw(checkMark);
        }
    }
}

