#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class AnimationController {
private:
    struct Animation {
        std::vector<sf::IntRect> frames;
        float frameDuration;
        bool loop;
        bool flipX;
    };

    const float factor = 4.5f; // для увеличения персонажа ибо я уже хз сука не понимаю
    
    sf::Sprite& sprite;
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimation;
    float currentTime;
    size_t currentFrame;
    bool isInitialized = false;
    bool isFlippedX;

    void updateSpriteTextureRect();

public:

    explicit AnimationController(sf::Sprite& sprite_): sprite(sprite_), currentTime(0.f), currentFrame(0), isFlippedX(false) {}

    void addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames, float frameDuration, bool loop = true, bool flipX = false);

    void play(const std::string& name);

    void update(const float& dt);

    bool hasPenultFrame();

    bool hasLastFrame();
};