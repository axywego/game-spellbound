#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.hpp"

enum class EnemyClass {
    Rat, Goblin, Demon, Shaman
};

class Enemy: public Mob {
private:
    Player& player;
    bool isMoving;

    bool isFading = false;
    bool isShowingHp = false;
    float timerToShowHp{0.f};
    float fadeOutTime{3.f};
    float timeToFadeOut{0.5f};
    float timerFadingOut{0.f};

    sf::RectangleShape hpRedShape;
    sf::RectangleShape hpBackgroundShape;

protected:
    float rangeRadius;
    float attackRange;
public:
    Enemy(const sf::Texture& texture, Tilemap& map_, const sf::FloatRect& collisionRect_, Player& player_): 
     Mob(texture, map_, collisionRect_), player(player_) {
        hpBackgroundShape.setFillColor(sf::Color::Black);
        hpRedShape.setFillColor(sf::Color::Red);
        
        hpBackgroundShape.setSize({70.f, 10.f});
        hpRedShape.setSize({65.f, 8.f});
    }

    // void startAttacking() override {
    //     if (!isAttacking && attackCooldown <= 0.0f) {
    //         isAttacking = true;
    //         attackTimer = attackAnimationTime;
    //     }
    // }

    void attacking(const float& dt) override {
        if (isAttacking) {
            attackTimer -= dt;

            // attackTimer говорит о том, когда будет атака( на каком моменте анимации )
            if(!hasDealtDamage && attackTimer <= .1f){
                tryAttack();
                hasDealtDamage = true;
            }

            if (attackTimer <= 0.f) {
                isAttacking = false;
                hasDealtDamage = false;
                attackCooldown = attackCooldownTime;
            }
        }
        else {
            if (attackCooldown > 0.0f) {
                attackCooldown -= dt;
            }
        }
    }

    void tryAttack() {
        if(getDistanceToPlayer() <= attackRange) player.takeDamage(damage);
    }

    float getDistanceToPlayer() const {
        sf::Vector2f playerPos = player.getSprite().getPosition();
        sf::Vector2f enemyPos = currentSprite.getPosition();
        return std::sqrtf(std::powf(playerPos.x - enemyPos.x, 2) + 
                         std::powf(playerPos.y - enemyPos.y, 2));
    }

    // void playMovementAnimation() override {
    //     if (currentState == State::Moving) {
    //         if (std::abs(lastDirection.y) > std::abs(lastDirection.x)) {
    //             if (lastDirection.y < 0) animController.play("walk_up");
    //             else animController.play("walk_down");
    //         } else {
    //             if (lastDirection.x < 0) animController.play("walk_left");
    //             else animController.play("walk_right");
    //         }
    //     }
    //     else if(currentState == State::Idle) {
    //         if (std::abs(lastDirection.x) > std::abs(lastDirection.y)){
    //             if(std::signbit(lastDirection.x)) animController.play("idle_left");
    //             else animController.play("idle_right");
    //         }
    //         else {
    //             if(std::signbit(lastDirection.y)) animController.play("idle_up");
    //             else animController.play("idle_down");
    //         }
    //     }
    // }

    void attack() override {

    }

    void move(const float& dt) override {
        sf::Vector2f direction =  player.getSprite().getPosition() - currentSprite.getPosition();
        float length = std::sqrtf(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.f) {
            direction /= length;
            lastDirection = direction;
            currentSprite.move(direction * speed * dt);
        }
    }

    void update(const float& dt) override {

        animController.update(dt);
        updateAnimation();

        if (isDying) {
            if(animController.hasLastFrame()){                
                isAlive = false;
                //std::cout << isAlive << '\n';
            }
        }

        if (isTakingDamage) {
            isShowingHp = true;
            currentState = State::Hurt;
            damageTimer -= dt;
            if (damageTimer <= 0.0f) {
                isTakingDamage = false;
            }
        }

        if(!isTakingDamage && !isAttacking && currentState != State::Dying) {
            if(getDistanceToPlayer() < rangeRadius){
                if(getDistanceToPlayer() > attackRange && attackCooldown <= 0.f){
                    currentState = State::Moving;
                    move(dt);
                }
                else if(attackCooldown <= 0.f) {
                    currentState = State::Attack;
                    startAttacking();
                }
                else {
                    currentState = State::Idle;
                }
            }
            else {
                currentState = State::Idle;
            }
        }

        collision.update();
        attacking(dt);


        if(isShowingHp){
            timerToShowHp += dt;
        }

        if(timerToShowHp > 3.f){
            isShowingHp = false;
            timerFadingOut = timeToFadeOut;
            isFading = true;
            timerToShowHp = 0.f;
        }

        if(timerFadingOut > 0.f){
            hpBackgroundShape.setFillColor(sf::Color(0, 0, 0, (timerFadingOut * 255) / timeToFadeOut));
            hpRedShape.setFillColor(sf::Color(255, 0, 0, (timerFadingOut * 255) / timeToFadeOut));

            timerFadingOut -= dt;
        }
        else if(timerFadingOut <= 0.f) {
            isFading = false;
            hpBackgroundShape.setFillColor(sf::Color::Black);
            hpRedShape.setFillColor(sf::Color::Red);
        }
        

        auto spriteRect = currentSprite.getGlobalBounds();

        hpBackgroundShape.setPosition({spriteRect.position.x + spriteRect.size.x / 2 - hpBackgroundShape.getSize().x / 2, spriteRect.position.y});

        sf::FloatRect shapeRect = {hpBackgroundShape.getPosition(), hpBackgroundShape.getSize()};
        hpRedShape.setPosition({shapeRect.position.x + 2.5f, shapeRect.position.y + 1.f});

        hpRedShape.setSize({health * 65.f / maxHealth, hpRedShape.getSize().y});
    }

    void render(sf::RenderTarget& target) override {
        target.draw(currentSprite);
        collision.render(target);
        if(isAlive && (isFading || isShowingHp)){
            target.draw(hpBackgroundShape);
            target.draw(hpRedShape);
        }
    }
};

class Rat : public Enemy {
private:

public:
    Rat(Tilemap& map_, Player& player_): Enemy(sf::Texture("real img/3 Dungeon Enemies/1/1.png"), map_, sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}), player_) {
        speed = 500.f;
        maxHealth = 4.f;
        health = maxHealth;
        rangeRadius = 700.f;
        attackRange = 60.f;
        damage = 0.5f;
        attackCooldownTime = 0.8f;
        typeDamage = TypeDamage::Melee;
    }
};

class Goblin : public Enemy {
private:

public:
};

class EnemyFactory {
public:
    static std::unique_ptr<Enemy> create(EnemyClass type, Tilemap& map, Player& player) {
        switch(type) {
            case EnemyClass::Rat: return std::make_unique<Rat>(map, player);
            // case EnemyClass::Goblin: return std::make_unique<Knight>(world);
            // case EnemyClass::Demon: return std::make_unique<Mage>(world);
            // case EnemyClass::Shaman: return std::make_unique<Mage>(world);
            default: return nullptr;
        }
    }
};

#endif