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
        sf::Texture texture;
        sf::Sprite sprite;

        TypeAnimation currentAnimType = TypeAnimation::None;

        std::unordered_map<TypeAnimation, Animation::AnimationList> animations;
        std::unordered_map<TypeAnimation, float> animationProgressInSec;

    public:

        UIObject(const sf::Texture& texture_, sf::RenderWindow& window_);

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

        Transform getCurrentTransform() const;

        void setPosition(const sf::Vector2f& pos);

        sf::Vector2f getPosition() const;

        virtual void update(const float& dt) = 0;

        void render() const;
    };

}


