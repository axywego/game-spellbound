#ifndef MOB_HPP
#define MOB_HPP

#include <SFML/Graphics.hpp>
#include "Tilemap.hpp"
#include "AnimationController.hpp"
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

    sf::Vector2f checkCollisions(const Tilemap& map) const {
        const auto collisions = map.getCollisionTiles();

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

    sf::FloatRect getCollisionRect() const {
        auto rectSprite = target.getGlobalBounds();
        rectSprite.position += collisionRect.position;
        rectSprite.size = collisionRect.size;
        return rectSprite;
    }
};

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

    float delayBeforeUpMana{2.5f};
    float timerDelayMana{0.f};
    float timerMana{0.f};
    float timeToUpMana{0.5f};
    float manaToUpPerTime{0.25f};
    
    bool hasDealtDamage = false;
    bool hasMeleeAttacked = false;

    MobCollision collision;
    AnimationController animController;

    Tilemap& map;

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
        Ranged, Melee
    };

protected:
    TypeDamage typeDamage;

public:


    Mob(const sf::Texture& texture_, Tilemap& map_, const sf::FloatRect collisionRect): texture(texture_), map(map_), currentSprite(texture), collision(currentSprite, collisionRect), animController(currentSprite) {
        initAnimations();
        playIdleAnimation();
    }

    void setPosition(const sf::Vector2f& pos){
        currentSprite.setPosition(pos);
    }

    sf::Sprite& getSprite(){
        return currentSprite;
    }

    sf::FloatRect getCollisionRect() const {
        return collision.getCollisionRect();
    }

    void initAnimations() {
        std::vector<sf::IntRect> idleDown {
            {{0, 0}, {32, 32}},
            {{32, 0}, {32, 32}},
            {{64, 0}, {32, 32}},
            {{96, 0}, {32, 32}},
        };

        std::vector<sf::IntRect> idleUp {
            {{0, 32}, {32, 32}},
            {{32, 32}, {32, 32}},
            {{64, 32}, {32, 32}},
            {{96, 32}, {32, 32}},
        };

        std::vector<sf::IntRect> idleLeft {
            {{0, 64}, {32, 32}},
            {{32, 64}, {32, 32}},
            {{64, 64}, {32, 32}},
            {{96, 64}, {32, 32}},
        };

        std::vector<sf::IntRect> walkDown {
            {{0, 192}, {32, 32}},
            {{32, 192}, {32, 32}},
            {{64, 192}, {32, 32}},
            {{96, 192}, {32, 32}},
            {{128, 192}, {32, 32}},
            {{160, 192}, {32, 32}},
        };

        std::vector<sf::IntRect> walkUp {
            {{0, 224}, {32, 32}},
            {{32, 224}, {32, 32}},
            {{64, 224}, {32, 32}},
            {{96, 224}, {32, 32}},
            {{128, 224}, {32, 32}},
            {{160, 224}, {32, 32}},
        };

        std::vector<sf::IntRect> walkLeft {
            {{0, 256}, {32, 32}},
            {{32, 256}, {32, 32}},
            {{64, 256}, {32, 32}},
            {{96, 256}, {32, 32}},
            {{128, 256}, {32, 32}},
            {{160, 256}, {32, 32}},
        };

        std::vector<sf::IntRect> hurtDown {
            {{0, 96},{32,32}},
            {{32, 96},{32,32}},
        };

        std::vector<sf::IntRect> hurtUp {
            {{0, 128},{32,32}},
            {{32, 128},{32,32}},
        };

        std::vector<sf::IntRect> hurtLeft {
            {{0, 160},{32,32}},
            {{0, 160},{32,32}},
        };

        std::vector<sf::IntRect> deathDown {
            {{0, 288}, {32, 32}},
            {{32, 288}, {32, 32}},
            {{64, 288}, {32, 32}},
            {{96, 288}, {32, 32}},
            {{128, 288}, {32, 32}},
            {{160, 288}, {32, 32}},
            {{192, 288}, {32, 32}},
            {{224, 288}, {32, 32}},
        };

        std::vector<sf::IntRect> deathUp {
            {{0, 320}, {32, 32}},
            {{32, 320}, {32, 32}},
            {{64, 320}, {32, 32}},
            {{96, 320}, {32, 32}},
            {{128, 320}, {32, 32}},
            {{160, 320}, {32, 32}},
            {{192, 320}, {32, 32}},
            {{224, 320}, {32, 32}},
        };

        std::vector<sf::IntRect> deathLeft {
            {{0, 352}, {32, 32}},
            {{32, 352}, {32, 32}},
            {{64, 352}, {32, 32}},
            {{96, 352}, {32, 32}},
            {{128, 352}, {32, 32}},
            {{160, 352}, {32, 32}},
            {{192, 352}, {32, 32}},
            {{224, 352}, {32, 32}},
        };

        std::vector<sf::IntRect> attackDown {
            {{0, 384}, {32, 32}},
            {{32, 384}, {32, 32}},
            {{64, 384}, {32, 32}},
            {{96, 384}, {32, 32}},
        };

        std::vector<sf::IntRect> attackUp {
            {{0, 416}, {32, 32}},
            {{32, 416}, {32, 32}},
            {{64, 416}, {32, 32}},
            {{96, 416}, {32, 32}},
        };

        std::vector<sf::IntRect> attackLeft {
            {{0, 448}, {32, 32}},
            {{32, 448}, {32, 32}},
            {{64, 448}, {32, 32}},
            {{96, 448}, {32, 32}},
        };

        animController.addAnimation("idle_down", idleDown, 0.1f);
        animController.addAnimation("idle_up", idleUp, 0.1f);
        animController.addAnimation("idle_left", idleLeft, 0.1f);
        animController.addAnimation("idle_right", idleLeft, 0.1f, true, true);

        animController.addAnimation("walk_down", walkDown, 0.1f);
        animController.addAnimation("walk_up", walkUp, 0.1f);
        animController.addAnimation("walk_left", walkLeft, 0.1f);
        animController.addAnimation("walk_right", walkLeft, 0.1f, true, true);

        animController.addAnimation("hurt_down", hurtDown, 0.1f);
        animController.addAnimation("hurt_up", hurtUp, 0.1f);
        animController.addAnimation("hurt_left", hurtLeft, 0.1f);
        animController.addAnimation("hurt_right", hurtLeft, 0.1f, true, true);

        animController.addAnimation("death_down", deathDown, 0.1f);
        animController.addAnimation("death_up", deathUp, 0.1f);
        animController.addAnimation("death_left", deathLeft, 0.1f);
        animController.addAnimation("death_right", deathLeft, 0.1f, false, true);
        
        animController.addAnimation("attack_down", attackDown, 0.1f);
        animController.addAnimation("attack_up", attackUp, 0.1f);
        animController.addAnimation("attack_left", attackLeft, 0.1f);
        animController.addAnimation("attack_right", attackLeft, 0.1f, true, true);
    }

    bool isAttackInProgress() const {
        return isAttacking;
    }

    void startAttacking() {
        if (!isAttacking && attackCooldown <= 0.0f) {
            isAttacking = true;
            attackTimer = attackAnimationTime;
            currentState = State::Attack;
        }
    }
    virtual void attacking(const float& dt) = 0;

    void setWorld(Tilemap& map_) {
        map = map_;
    }

    Tilemap& getWorld() const {
        return map;
    }

    virtual void attack() = 0;

    void takeDamage(float damage) {
        if (isDying) return;
        
        health -= damage;
        if (health <= 0.f) {
            health = 0.f;
            startDying();
            return;
        }

        isTakingDamage = true;
        damageTimer = damageAnimationTime;
        
    }

    void startDying() {
        currentState = State::Dying;
        isDying = true;
        // Можно добавить дополнительные действия при начале смерти
        // Например, отключение коллизий или воспроизведение звука
    }

    void updateAnimation() {
        switch (currentState) {
            case State::Dying:
                playDeathAnimation();
                break;
            case State::Hurt:
                playHurtAnimation();
                break;
            case State::Attack:
                playAttackAnimation();
                break;
            case State::Moving:
                playMovementAnimation();
                break;
            case State::Idle:
                playIdleAnimation();
                break;
        }
    }

    void playDeathAnimation() {
        playAnimation("death");
    }

    void playHurtAnimation() {
        playAnimation("hurt");
    }

    void playAttackAnimation() {
        playAnimation("attack");
    }

    void playMovementAnimation() {
        playAnimation("walk");
    }

    void playIdleAnimation() {
        playAnimation("idle");
    }

    void playAnimation(const std::string& name) {
        if (std::abs(lastDirection.x) > std::abs(lastDirection.y)){
            if(std::signbit(lastDirection.x)) animController.play(name + "_left");
            else animController.play(name + "_right");
        }
        else {
            if(std::signbit(lastDirection.y)) animController.play(name + "_up");
            else animController.play(name + "_down");
        }
    }

    float getDamage() const {
        return damage;
    }

    bool getIsAlive() const {
        return isAlive;
    }

    float getMaxHealth() const {
        return maxHealth;
    }

    float getCurrentHealth() const {
        return health;
    }

    TypeDamage getTypeDamage() const {
        return typeDamage;
    }

    float getCurrentSpeed() const {
        return speed;
    }

    float getMaxMana() const {
        return maxMana;
    }

    float getCurrentMana() const {
        return mana;
    }

    virtual void move(const float& dt) = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
    virtual ~Mob() = default;
};

#endif