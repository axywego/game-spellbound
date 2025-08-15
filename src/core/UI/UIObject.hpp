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

        Transform transform;

        sf::RenderWindow* window;

        TypeAnimation currentAnimType = TypeAnimation::None;

        std::unordered_map<TypeAnimation, Animation::AnimationList> animations;
        std::unordered_map<TypeAnimation, std::vector<float>> animationProgressInSec;

    public:

        UIObject();

        explicit UIObject(sf::RenderWindow& window_);

        virtual ~UIObject() = default;

        template<typename... Animations>
        void addAnimation(const TypeAnimation state, Animations&&... newAnimations) {
            currentAnimType = state;

            if (!animations.contains(state)) {
                animations[state] = Animation::AnimationList{};
                animationProgressInSec[state] = {};
            }

            (animations[state].emplace_back(
                std::forward<Animations>(newAnimations).first,
                Animation::AnimationData{
                    0.f,
                    std::forward<Animations>(newAnimations).second
                }
            ), ...);

            animationProgressInSec[state].insert(
                animationProgressInSec[state].end(),
                sizeof...(newAnimations),
                0.0f
            );
        }

        virtual void setTransform(const Transform& t) = 0;

        virtual Transform getCurrentTransform() const = 0;

        virtual void setPosition(const sf::Vector2f& pos) = 0;

        virtual void setScale(const sf::Vector2f& scale) = 0;

        virtual bool isHovered() const = 0;

        virtual bool isClicked(const std::optional<sf::Event>& event) const = 0;

        virtual void update(const float& dt) = 0;

        virtual void render() const = 0;

        void setWindow(sf::RenderWindow& window_);
    };

}


