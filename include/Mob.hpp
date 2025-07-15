#ifndef MOB_HPP
#define MOB_HPP

#include <SFML/Graphics.hpp>
#include "World.hpp"
#include <vector>

class MobCollision {
private:
    sf::VertexArray lineHorizontalUp;
    sf::VertexArray lineHorizontalDown;

    sf::VertexArray lineVerticalLeft;
    sf::VertexArray lineVerticalRight;

    sf::Sprite& target;

    sf::FloatRect collisionRect;

    const float offset = 5.f;
public:
    MobCollision(sf::Sprite& target_, const sf::FloatRect collisionRect_):
     lineHorizontalUp(sf::PrimitiveType::Lines, 2), lineHorizontalDown(sf::PrimitiveType::Lines, 2),
     lineVerticalLeft(sf::PrimitiveType::Lines, 2), lineVerticalRight(sf::PrimitiveType::Lines, 2),
     target(target_), collisionRect(collisionRect_) { 

        lineHorizontalDown[0].color = sf::Color::White;
        lineHorizontalDown[1].color = sf::Color::White;

        lineHorizontalUp[0].color = sf::Color::White;
        lineHorizontalUp[1].color = sf::Color::White;

        lineVerticalLeft[0].color = sf::Color::White;
        lineVerticalLeft[1].color = sf::Color::White;

        lineVerticalRight[0].color = sf::Color::White;
        lineVerticalRight[1].color = sf::Color::White;
    }

    void update() {
        auto rectSprite = target.getGlobalBounds();
        rectSprite.position += collisionRect.position;
        rectSprite.size = collisionRect.size;

        // lineHorizontalUp[0].position = {collisionRect.position.x, collisionRect.position.y + offset};
        // lineHorizontalUp[1].position = {collisionRect.position.x + collisionRect.size.x, collisionRect.position.y + offset};

        // lineHorizontalDown[0].position = {collisionRect.position.x, collisionRect.position.y + collisionRect.size.y - offset};
        // lineHorizontalDown[1].position = {collisionRect.position.x + collisionRect.size.x, collisionRect.position.y + collisionRect.size.y - offset};
        
        // lineVerticalLeft[0].position = {collisionRect.position.x + offset, collisionRect.position.y};
        // lineVerticalLeft[1].position = {collisionRect.position.x + offset, collisionRect.position.y + collisionRect.size.y};

        // lineVerticalRight[0].position = {collisionRect.position.x + collisionRect.size.x - offset, collisionRect.position.y};
        // lineVerticalRight[1].position = {collisionRect.position.x + collisionRect.size.x - offset, collisionRect.position.y + collisionRect.size.y};


        lineHorizontalUp[0].position = {rectSprite.position.x, rectSprite.position.y + offset};
        lineHorizontalUp[1].position = {rectSprite.position.x + rectSprite.size.x, rectSprite.position.y + offset};

        lineHorizontalDown[0].position = {rectSprite.position.x, rectSprite.position.y + rectSprite.size.y - offset};
        lineHorizontalDown[1].position = {rectSprite.position.x + rectSprite.size.x, rectSprite.position.y + rectSprite.size.y - offset};
        
        lineVerticalLeft[0].position = {rectSprite.position.x + offset, rectSprite.position.y};
        lineVerticalLeft[1].position = {rectSprite.position.x + offset, rectSprite.position.y + rectSprite.size.y};

        lineVerticalRight[0].position = {rectSprite.position.x + rectSprite.size.x - offset, rectSprite.position.y};
        lineVerticalRight[1].position = {rectSprite.position.x + rectSprite.size.x - offset, rectSprite.position.y + rectSprite.size.y};

    }

    sf::Vector2f checkCollisions(const World& world) const {
        const auto collisions = world.getCollisionTiles();

        sf::Vector2f incorrectDir {0.f, 0.f};

        for(const auto& spr : collisions) {
            if (spr.getGlobalBounds().contains(lineHorizontalUp[0].position) || spr.getGlobalBounds().contains(lineHorizontalDown[0].position)){
                incorrectDir.x = -1.f;
            }
            else if (spr.getGlobalBounds().contains(lineHorizontalUp[1].position) || spr.getGlobalBounds().contains(lineHorizontalDown[1].position)){
                incorrectDir.x = 1.f;
            }
            if (spr.getGlobalBounds().contains(lineVerticalLeft[0].position) || spr.getGlobalBounds().contains(lineVerticalRight[0].position)){
                incorrectDir.y = -1.f;
            }
            else if (spr.getGlobalBounds().contains(lineVerticalLeft[1].position) || spr.getGlobalBounds().contains(lineVerticalRight[1].position)){
                incorrectDir.y = 1.f;
            }
        }
        return incorrectDir;
    }
    
    void render(sf::RenderTarget& renderTarget) {
        renderTarget.draw(lineHorizontalDown);
        renderTarget.draw(lineHorizontalUp);

        renderTarget.draw(lineVerticalLeft);
        renderTarget.draw(lineVerticalRight);
    }
};

class Mob {
protected:
    sf::Texture texture;
    sf::Sprite currentSprite;

    float speed;
    sf::Vector2f lastDirection;
    int health;

    bool isAttacking = false;
    float attackTimer = 0.0f; // Таймер текущей атаки
    const float attackAnimationTime = 0.4f; // Длительность анимации атаки (const)
    float attackCooldown = 0.0f; // Таймер перезарядки
    float attackCooldownTime; // Время между атаками (const)

    MobCollision collision;

    std::weak_ptr<World> world;

public:
    Mob(const sf::Texture& texture_, const std::shared_ptr<World>& world_, const sf::FloatRect collisionRect): texture(texture_), world(world_), currentSprite(texture), collision(currentSprite, collisionRect) {}
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Sprite& getSprite() = 0;
    virtual void initAnimations() = 0;

    virtual bool isAttackInProgress() const = 0;
    virtual void startAttacking() = 0;
    virtual void attacking(const float& dt) = 0;

    virtual void setWorld(const std::shared_ptr<World>& world_) = 0;

    virtual void updateAnimation() = 0;
    virtual void attack() = 0;

    void takeDamage(float damage) {
        health -= damage;
    }

    virtual void move(const float& dt) = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
    virtual ~Mob() = default;
};

#endif