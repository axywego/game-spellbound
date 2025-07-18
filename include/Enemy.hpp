#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include "Mob.hpp"
#include "Player.hpp"

enum class EnemyClass {
    Rat, Goblin, Demon, Shaman
};

class Enemy: public Mob {
private:
    Player& player;
    bool isMoving;
protected:
    float rangeRadius;
    float attackRange;
    float damage;
public:
    Enemy(const sf::Texture& texture, const std::shared_ptr<World>& world_, const sf::FloatRect& collisionRect_, Player& player_): 
     Mob(texture, world_, collisionRect_), player(player_) {}
    void startAttacking() override {
        if (!isAttacking && attackCooldown <= 0.0f) {
            isAttacking = true;
            attackTimer = attackAnimationTime;
            attackCooldown = attackCooldownTime;
            currentState = State::Attack;
        }
    }

    void attacking(const float& dt) override {
        if (isAttacking) {
            attackTimer -= dt;
            if (attackTimer <= 0.0f) {
                isAttacking = false;
                tryAttack();
            }
        }
    }

    void tryAttack() {
        if (getDistanceToPlayer() <= attackRange) {
            player.takeDamage(damage);
        }
    }

    float getDistanceToPlayer() const {
        sf::Vector2f playerPos = player.getSprite().getPosition();
        sf::Vector2f enemyPos = currentSprite.getPosition();
        return std::sqrtf(std::powf(playerPos.x - enemyPos.x, 2) + 
                         std::powf(playerPos.y - enemyPos.y, 2));
    }

    void playMovementAnimation() override {
        if (currentState == State::Moving) {
            if (std::abs(lastDirection.y) > std::abs(lastDirection.x)) {
                if (lastDirection.y < 0) animController.play("walk_up");
                else animController.play("walk_down");
            } else {
                if (lastDirection.x < 0) animController.play("walk_left");
                else animController.play("walk_right");
            }
        }
        else if(currentState == State::Idle) {
            if (lastDirection.y < 0) animController.play("idle_up");
            else if (lastDirection.y > 0) animController.play("idle_down");
            else if (lastDirection.x < 0) animController.play("idle_left");
            else animController.play("idle_right");
        }
    }

    // void updateAnimation() override {
    //     if(isMoving) {
    //         if (std::abs(lastDirection.y) > std::abs(lastDirection.x)) {
    //             if (lastDirection.y < 0) animController.play("walk_up");
    //             else animController.play("walk_down");
    //         } else {
    //             if (lastDirection.x < 0) animController.play("walk_left");
    //             else animController.play("walk_right");
    //         }
    //     }
    //     else {
    //         if (lastDirection.y < 0) animController.play("idle_up");
    //         else if (lastDirection.y > 0) animController.play("idle_down");
    //         else if (lastDirection.x < 0) animController.play("idle_left");
    //         else animController.play("idle_right");
    //     }
    // }

    void attack() override {

    }

    void move(const float& dt) override {
        float distanceToPlayer = getDistanceToPlayer();
        if(distanceToPlayer < rangeRadius){
            if (distanceToPlayer > attackRange) {
                sf::Vector2f direction = player.getSprite().getPosition() - currentSprite.getPosition();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length > 0) {
                    direction /= length;
                    lastDirection = direction;
                    currentSprite.move(direction * speed * dt);
                    currentState = State::Moving;
                }
            } else {
                currentState = State::Attack;
                startAttacking();
            }
        }
        else {
            currentState = State::Idle;
        }
        // const auto [xp, yp] = player.getSprite().getGlobalBounds().position;
        // const auto [xe, ye] = currentSprite.getGlobalBounds().position;

        // float l = std::sqrtf(std::powf(xp - xe, 2.f) + std::powf(yp - ye, 2.f));

        // if(l < rangeRadius) {
        //     isMoving = true;
        //     lastDirection = {(xp-xe)/l, (yp-ye)/l};
        //     currentSprite.move(lastDirection * speed * dt);
        // }
        // else {
        //     isMoving = false;
        // }
    }

    void update(const float& dt) override {
        
        if (isDying) {
            currentState = State::Dying;
            if(animController.hasLastFrame()){
                isAlive = false;
            }
        }

        if(!isDying) {
                    
            if (isTakingDamage) {
                currentState = State::Hurt;
                damageTimer -= dt;
                if (damageTimer <= 0.0f) {
                    isTakingDamage = false;
                }
            }
            collision.update();
            attacking(dt);
            if(!isAttacking) move(dt);
        }
        animController.update(dt);
        updateAnimation();
    }

    void render(sf::RenderTarget& target) override {
        target.draw(currentSprite);
        collision.render(target);
    }
};

class Rat : public Enemy {
private:

public:
    Rat(const std::shared_ptr<World>& world_, Player& player_): Enemy(sf::Texture("real img/3 Dungeon Enemies/1/1.png"), world_, sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}), player_) {
        speed = 250.f;
        health = 2.f;
        rangeRadius = 500.f;
        attackRange = 100.f;
        initAnimations();
    }
};

class EnemyFactory {
public:
    static std::unique_ptr<Enemy> create(EnemyClass type, const std::shared_ptr<World>& world, Player& player) {
        switch(type) {
            case EnemyClass::Rat: return std::make_unique<Rat>(world, player);
            // case EnemyClass::Goblin: return std::make_unique<Knight>(world);
            // case EnemyClass::Demon: return std::make_unique<Mage>(world);
            // case EnemyClass::Shaman: return std::make_unique<Mage>(world);
            default: return nullptr;
        }
    }
};

#endif