//
// Created by goasd on 14.08.2025.
//

#include "CheckBox.hpp"

#include <ranges>

namespace UI {
    CheckBox::CheckBox(sf::RenderWindow &window, const bool& checked): UIObject(window), checked(checked) {
        frame[0].setSize({10.f, 1.f});
        frame[1].setSize({1.f, 10.f});
        frame[2].setSize({10.f, 1.f});
        frame[3].setSize({1.f, 10.f});

        frame[0].setFillColor(sf::Color::Green);
        frame[1].setFillColor(sf::Color::Red);
        frame[2].setFillColor(sf::Color::Black);
        frame[3].setFillColor(sf::Color::Blue);

        checkMark.setTextureRect({{20, 20},{10, 10}});
        checkMark.setOrigin(checkMark.getGlobalBounds().size / 2.f);
        checkMark.setColor(sf::Color::Black);
    }

    void CheckBox::setTransform(const Transform &t) {
        transform = t;
        setScale(transform.scale);
        setPosition(transform.position);
        checkMarkRect.position = frame[0].getPosition();
        checkMarkRect.size = {10.f * transform.scale.x, 10.f * transform.scale.y};
    }

    Transform CheckBox::getCurrentTransform() const {
        return transform;
    }

    void CheckBox::setPosition(const sf::Vector2f &pos) {
        frame[0].setPosition(pos);

        const float offsetX = (frame[0].getSize().x - frame[0].getSize().y) * transform.scale.x;

        frame[1].setPosition({pos.x + offsetX, pos.y});

        const float offsetY = (frame[1].getSize().y - frame[1].getSize().x) * transform.scale.y;

        frame[2].setPosition({pos.x, pos.y + offsetY});
        frame[3].setPosition(pos);

        const sf::Vector2f centerOffset = {
            frame[0].getSize().x / 2.f * transform.scale.x,
            frame[1].getSize().y / 2.f * transform.scale.y
        };
        checkMark.setPosition(pos + centerOffset);
    }

    void CheckBox::setScale(const sf::Vector2f& scale) {
        frame[0].setScale(scale);
        frame[1].setScale(scale);
        frame[2].setScale(scale);
        frame[3].setScale(scale);

        const float offsetX = (frame[0].getSize().x - frame[0].getSize().y) * transform.scale.x;

        frame[1].setPosition({frame[0].getPosition().x + offsetX, frame[0].getPosition().y});

        const float offsetY = (frame[1].getSize().y - frame[1].getSize().x) * transform.scale.y;

        frame[2].setPosition({frame[0].getPosition().x, frame[0].getPosition().y + offsetY});

        checkMark.setScale(scale);
        checkMark.setPosition({frame[0].getSize().x * scale.x / 2.f, frame[1].getSize().y * scale.y / 2.f});
    }

    void CheckBox::setFunc(const std::function<void(const bool&)> &func) {
        onClick = func;
    }

    void CheckBox::setValue(bool value) {
        checked = value;
    }

    bool CheckBox::getValue() const {
        return checked;
    }

    bool CheckBox::isHovered() const {
        const auto mousePos = sf::Mouse::getPosition(*window);
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
            onClick(checked);
        }
    }

    void CheckBox::update(const float& dt) {
        const bool isPlus = isHovered();

        for (auto&& [el_anim, el_timer] :
          std::views::zip(animations[currentAnimType], animationProgressInSec[currentAnimType])){
            auto& [anim, data] = el_anim;
            if (isPlus) {
                if(data.progress >= 1.f)
                    if (data.isInfinity) data.progress = 0.f;
                    else continue;
            }
            else {
                if(data.progress <= 0.f)
                    if (data.isInfinity) data.progress = 1.f;
                    else continue;
            }
            data.progress = el_timer / anim->duration;
            isPlus ? el_timer += dt : el_timer -= dt;

            anim->apply(checkMark, data.progress);
          }
    }

    void CheckBox::render() const {
        // for (const auto& el : frame) {
        //     window.draw(el);
        // }

        window->draw(frame[0]);
        window->draw(frame[1]);
        window->draw(frame[2]);
        window->draw(frame[3]);
        if (checked) {
            window->draw(checkMark);
        }
    }
}

