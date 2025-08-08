#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "../../core/AnimationController.hpp"
#include "MobCollision.hpp"

class Mob {
protected:
    sf::Texture texture;
    sf::Sprite currentSprite;

    float speed;
    sf::Vector2f lastDirection{0.f, 1.f};
    float maxHealth;
    float health;
    float damage;
    float mana{0.f};
    float maxMana{0.f};
    float manaCost{0.f};

    bool isAttacking = false;
    float attackTimer = 0.0f;
    const float attackAnimationTime = 0.4f;
    float attackCooldown = 0.0f;
    float attackCooldownTime;
    float timeToStayAfterAttack;
    float timerStayAfterAttack = 0.f;

    float delayBeforeUpMana{2.5f};
    float timerDelayMana{0.f};
    float timerMana{0.f};
    float timeToUpMana{0.5f};
    float manaToUpPerTime{0.25f};

    bool hasAttacked{false};

    MobCollision collision;
    AnimationController animController;

    const Tilemap& map;

    bool isAlive = true;
    bool isTakingDamage = false;
    float damageTimer = 0.0f;
    const float damageAnimationTime = 0.2f;
    bool isDying = false;

    enum class State {
        Idle, Moving, Attack, Hurt, Dying
    };

    State currentState = State::Idle;

public:
    enum class TypeDamage {
        Ranged, Melee, Summon
    };

protected:
    TypeDamage typeDamage;

public:

    Mob(const sf::Texture& texture_, const Tilemap& map_, const sf::FloatRect collisionRect);

    void setPosition(const sf::Vector2f& pos);

    sf::Sprite& getSprite();

    sf::FloatRect getCollisionRect() const;

    void initAnimations();

    bool isAttackInProgress() const ;

    void startAttacking() ;
    virtual void attacking(const float& dt) = 0;

    virtual void attack() = 0;

    void takeDamage(float damage);

    void startDying();

    void updateAnimation() ;

    void playDeathAnimation();

    void playHurtAnimation() ;

    void playAttackAnimation();

    void playMovementAnimation();

    void playIdleAnimation();

    void playAnimation(const std::string& name);

    float getDamage() const;

    bool getIsAlive() const;

    float getMaxHealth() const;

    float getCurrentHealth() const;

    TypeDamage getTypeDamage() const;

    float getCurrentSpeed() const;

    float getMaxMana() const;

    float getCurrentMana() const;

    virtual void move(const float& dt) = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
    virtual ~Mob() = default;
};