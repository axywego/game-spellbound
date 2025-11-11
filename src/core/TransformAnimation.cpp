#include "TransformAnimation.hpp"

#include <iostream>

namespace Animation {
    namespace Easing {
        float empty(float t = 0.f) { return 0.f; }

        float linear(float t) { return t; }

        float easeInQuad(float t) { return t * t; }
        float easeOutQuad(float t) { return 1 - (1 - t) * (1 - t); }
        float easeInOutQuad(float t) {
            return t < 0.5 ? 2 * t * t : 1 - powf(-2 * t + 2, 2) / 2;
        }

        float easeInCubic(float t) { return t * t * t; }
        float easeOutCubic(float t) { return 1 - powf(1 - t, 3); }
        float easeInOutCubic(float t) {
            return t < 0.5 ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;
        }

        float easeInElastic(float t) {
            constexpr float c4 = (2 * M_PI) / 3;
            return t == 0 ? 0 : t == 1 ? 1 : -powf(2, 10 * t - 10) * sin((t * 10 - 10.75) * c4);
        }
    }

    Base::Base(const std::function<float(float)>& easingFunction, const float& dur):
        easingFunction(easingFunction), duration(dur) {}


    Move::Move(const sf::Vector2f& startPos, const sf::Vector2f& delta,
        const std::function<float(float)>& easingFunction, const float& dur):
        Base(easingFunction, dur), startPos(startPos), delta(delta) {}

    void Move::apply(sf::Sprite& sprite, const float& progress) {
        sf::Vector2f newPos = startPos + delta * easingFunction(progress);
        sprite.setPosition(newPos);
    }

    Type Move::getType() const { return Type::Move; }


    Rotate::Rotate(const float& angle, const std::function<float(float)>& easingFunction, const float& dur)
        : Base(easingFunction, dur), angle(angle) {}

    void Rotate::apply(sf::Sprite& sprite, const float& progress) {
        const float newAngle = angle * easingFunction(progress);
        sprite.setRotation(sf::degrees(newAngle));
    }

    Type Rotate::getType() const { return Type::Rotate; }


    Scale::Scale(const sf::Vector2f& fromScale, const sf::Vector2f& toScale, const std::function<float(float)>& easingFunction, const float& dur)
        : Base(easingFunction, dur), fromScale(fromScale), toScale(toScale) {}

    void Scale::apply(sf::Sprite& sprite, const float& progress) {
        const auto newScale = fromScale + (toScale - fromScale) * easingFunction(progress);
        sprite.setScale(newScale);
    }

    Type Scale::getType() const { return Type::Scale; }


    SlideShow::SlideShow(const std::vector<sf::Texture>& textures, const float& duration)
        : Base(Easing::empty, duration), textures(textures) {}

    void SlideShow::apply(sf::Sprite& sprite, const float& progress) {
        const float index_progress = progress * textures.size();
        auto index = static_cast<unsigned>(index_progress);

        if (index >= static_cast<int>(textures.size())) {
            index = static_cast<int>(textures.size()) - 1;
        }

        const auto pos = sprite.getPosition();
        sprite.setTexture(textures[index], true);
        sprite.setPosition(pos);
    }

    Type SlideShow::getType() const {
        return Type::SlideShow;
    }


    std::shared_ptr<Base> createMoveAnimation(sf::Vector2f start, sf::Vector2f delta,
                                            const std::function<float(float)>& easing, float duration) {
        return std::make_shared<Move>(start, delta, easing, duration);
    }

    std::shared_ptr<Base> createRotateAnimation(float angle,
                                             const std::function<float(float)>& easing, float duration) {
        return std::make_shared<Rotate>(angle, easing, duration);
    }

    std::shared_ptr<Base> createScaleAnimation(const sf::Vector2f& fromScale, const sf::Vector2f& toScale,
                                            const std::function<float(float)>& easing, float duration) {
        return std::make_shared<Scale>(fromScale, toScale, easing, duration);
    }

    std::shared_ptr<Base> createSlideShowAnimation(const std::vector<sf::Texture>& textures, const float& duration) {
        return std::make_shared<SlideShow>(textures, duration);
    }

}
