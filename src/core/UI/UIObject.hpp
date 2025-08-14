#pragma once

#include <SFML/Graphics.hpp>
#include "../TransformAnimation.hpp"

namespace UI {

    class UIObject {
    public:

        enum class TypeAnimation {
            None, Hovered, SlideShow
        };

    protected:

        sf::RenderWindow& window;

        TypeAnimation currentAnimType = TypeAnimation::None;

        std::unordered_map<TypeAnimation, Animation::AnimationList> animations;
        std::unordered_map<TypeAnimation, float> animationProgressInSec;

    public:

        UIObject(sf::RenderWindow& window_);

        virtual ~UIObject() = default;

        template<typename... Animations>
        void addAnimation(const TypeAnimation state, Animations&&... newAnimations) {
            currentAnimType = state;

            if (!animations.contains(state)) {
                animations[state] = Animation::AnimationList{};
                animationProgressInSec[state] = 0.0f;
            }

            (animations[state].emplace_back(std::make_pair(
                std::forward<Animations>(newAnimations),
                Animation::AnimationData{0.f, getCurrentTransform(), false}
                )), ...);
        }

        virtual Transform getCurrentTransform() const = 0;

        virtual void setPosition(const sf::Vector2f& pos) = 0;

        virtual sf::Vector2f getPosition() const = 0;

        virtual void update(const float& dt) = 0;

        virtual void render() const = 0;
    };

}


