#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../entities/player/Player.hpp"
#include "StringFormat.hpp"
#include "ResourceManager.hpp"
#include "TransformAnimation.hpp"

class HUD {
private:
    static sf::Vector2i posLeft;
    static sf::Vector2i posCenter;
    static sf::Vector2i posRight;

    static sf::Texture textureHealthMana;
    static std::vector<sf::Sprite> spritesHP;
    static std::vector<sf::Sprite> spritesMana;

    static sf::Font font;
    static sf::Text textDamage;
    static sf::Text textSpeed;
    static sf::Texture textureIcons;
    static sf::Sprite spriteDamage;
    static sf::Sprite spriteSpeed;
public:
    static void update(const Player& player, const sf::Vector2f& cameraCenter);

    static void render(sf::RenderTarget& target);
};

class Button {
public:
    enum class State {
        Normal,
        Hovered,
        Pressed,
    };

private:

    sf::RenderWindow& window;
    sf::Texture texture;
    sf::Sprite sprite;

    State currentState = State::Normal;
    std::unordered_map<State, Animation::AnimationList> animations;
    std::unordered_map<State, float> animationProgressInSec;

    //bool isAnimationActive { false };

public:
    Button(const sf::Texture& texture_, sf::RenderWindow& window_);

    void setPosition(const sf::Vector2f& pos);

    sf::Vector2f getPosition() const;

    template<typename... Animations>
    void addAnimation(const State state, Animations&&... newAnimations) {
        if (!animations.contains(state)) {
            animations[state] = Animation::AnimationList{};
            animationProgressInSec[state] = 0.0f;
        }

        (animations[state].emplace_back(std::make_pair(
            std::forward<Animations>(newAnimations),
            Animation::AnimationData{0.f, getCurrentTransform(), false}
            )), ...);
    }

    bool isHovered() const;

    bool isClicked(const std::optional<sf::Event>& event) const;

    void setState(State state);

    State getState() const;

    Transform getCurrentTransform() const;

    void update(const float& dt);

    void render(sf::RenderTarget& renderTarget) const;
};