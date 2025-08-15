#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <vector>

struct Transform {
    sf::Vector2f position;
    float rotation{};
    sf::Vector2f scale;
};

namespace Animation {
    enum class Type {
        Move,
        Rotate,
        Scale,
    };

    namespace Easing {
        float linear(float t);
        float easeInQuad(float t);
        float easeOutQuad(float t);
        float easeInOutQuad(float t);
        float easeInCubic(float t);
        float easeOutCubic(float t);
        float easeInOutCubic(float t);
        float easeInElastic(float t);
    }


    class Base {
    public:
        float duration;
        std::function<float(float)> easingFunction;

        explicit Base(const std::function<float(float)>& easingFunction, const float& dur);
        virtual ~Base() = default;
        virtual void apply(sf::Sprite& sprite, const float& progress) = 0;
        virtual Type getType() const = 0;
    };


    class Move : public Base {
    public:
        sf::Vector2f startPos;
        sf::Vector2f delta;

        Move(const sf::Vector2f& startPos, const sf::Vector2f& delta,
            const std::function<float(float)>& easingFunction, const float& dur);
        void apply(sf::Sprite& sprite, const float& progress) override;
        Type getType() const override;
    };


    class Rotate : public Base {
    public:
        float angle;

        Rotate(const float& angle, const std::function<float(float)>& easingFunction, const float& dur);
        void apply(sf::Sprite& sprite, const float& progress) override;
        Type getType() const override;
    };


    class Scale : public Base {
    public:
        sf::Vector2f fromScale;
        sf::Vector2f toScale;

        Scale(const sf::Vector2f& fromScale, const sf::Vector2f& toScale, const std::function<float(float)>& easingFunction, const float& dur);
        void apply(sf::Sprite& sprite, const float& progress) override;
        Type getType() const override;
    };


    // template<typename... Args>
    // std::unique_ptr<Base> createAnimation(const Type type, Args&&... args) {
    //     switch (type) {
    //         case Type::Move:
    //             return std::make_unique<Move>(std::forward<Args>(args)...);
    //         case Type::Rotate:
    //             return std::make_unique<Rotate>(std::forward<Args>(args)...);
    //         case Type::Scale:
    //             return std::make_unique<Scale>(std::forward<Args>(args)...);
    //     }
    //     return nullptr;
    // }

    std::shared_ptr<Base> createMoveAnimation(sf::Vector2f start, sf::Vector2f delta,
                                            const std::function<float(float)>& easing, float duration);

    std::shared_ptr<Base> createRotateAnimation(float angle,
                                             const std::function<float(float)>& easing, float duration);

    std::shared_ptr<Base> createScaleAnimation(const sf::Vector2f& fromScale, const sf::Vector2f& toScale,
                                            const std::function<float(float)>& easing, float duration);

    struct AnimationData {
        float progress = 0.0f;
        bool isInfinity = false;
    };

    using AnimationList = std::vector<std::pair<std::shared_ptr<Base>, AnimationData>>;
}