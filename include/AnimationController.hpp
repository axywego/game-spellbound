#ifndef ANIMATIONCONTROLLER_HPP
#define ANIMATIONCONTROLLER_HPP

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

    

    void updateSpriteTextureRect() {
        if(currentAnimation.empty()) return;
        
        auto& anim = animations[currentAnimation];
        sprite.setTextureRect(anim.frames[currentFrame]);
        sprite.setScale({(isFlippedX ? -1.f : 1.f) * factor, 1.f * factor});
        if(currentAnimation.contains("hurt") || currentAnimation.contains("death")) sprite.setColor(sf::Color::Red);
        else sprite.setColor(sf::Color::White);

    }

public:

    AnimationController(sf::Sprite& sprite_): sprite(sprite_), currentTime(0.f), currentFrame(0), isFlippedX(false) {

    }

    void addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames, float frameDuration, bool loop = true, bool flipX = false) {
        animations.insert({name, {frames, frameDuration, loop, flipX}});
    }

    void play(const std::string& name) {
        if(currentAnimation != name || animations[name].flipX != isFlippedX) {
            currentAnimation = name;
            currentTime = 0.f;
            currentFrame = 0;
            isFlippedX = animations[name].flipX;
            
            if(!animations[name].frames.empty()) {
                sprite.setTextureRect(animations[name].frames[0]);
                
                sprite.setOrigin({sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2});
                
                sprite.setScale({(isFlippedX ? -1.f : 1.f) * factor, 1.f * factor});

                if(currentAnimation.contains("hurt") || currentAnimation.contains("death")) sprite.setColor(sf::Color::Red);
            }
        }
    }

    void update(const float& dt) {
        if(currentAnimation.empty()) return;
        
        auto& anim = animations[currentAnimation];
        currentTime += dt;

        if(currentTime >= anim.frameDuration) {
            currentTime = 0.f;
            currentFrame++;

            if(currentFrame >= anim.frames.size()) {
                if(anim.loop) currentFrame = 0;
                else currentFrame = anim.frames.size() - 1;
            }

            updateSpriteTextureRect();
        }
    }

    bool hasPenultFrame() {
        if(animations[currentAnimation].frames.size() - 2 == currentFrame){
            return true;
        }
        return false;
    }

    bool hasLastFrame() {
        if(animations[currentAnimation].frames.size() - 1 == currentFrame){
            return true;
        }
        return false;
    }

};

#endif