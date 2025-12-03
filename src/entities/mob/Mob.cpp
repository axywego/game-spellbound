#include "Mob.hpp"

Mob::Mob(const sf::Texture& texture_, const Tilemap& map_, const sf::FloatRect collisionRect): texture(texture_), map(map_), currentSprite(texture), collision(currentSprite, collisionRect), animController(currentSprite) {
    initAnimations();
    playIdleAnimation();
}

void Mob::setTilemap(const Tilemap &map_) {
    map = map_;
}

void Mob::setPosition(const sf::Vector2f& pos){
    currentSprite.setPosition(pos);
}

sf::Sprite& Mob::getSprite(){
    return currentSprite;
}

sf::FloatRect Mob::getCollisionRect() const {
    return collision.getCollisionRect();
}

void Mob::initAnimations() {
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
    animController.addAnimation("hurt_right", hurtLeft, 0.1f, false, true);

    animController.addAnimation("death_down", deathDown, 0.1f);
    animController.addAnimation("death_up", deathUp, 0.1f);
    animController.addAnimation("death_left", deathLeft, 0.1f);
    animController.addAnimation("death_right", deathLeft, 0.1f, false, true);

    animController.addAnimation("attack_down", attackDown, 0.1f);
    animController.addAnimation("attack_up", attackUp, 0.1f);
    animController.addAnimation("attack_left", attackLeft, 0.1f);
    animController.addAnimation("attack_right", attackLeft, 0.1f, true, true);
}

bool Mob::isAttackInProgress() const {
    return isAttacking;
}

void Mob::startAttacking() {
    if (!isAttacking && attackCooldown <= 0.0f) {
        isAttacking = true;
        attackTimer = attackAnimationTime;
        currentState = State::Attack;
    }
}

void Mob::takeDamage(float damage) {
    if (isDying) return;

    auto& health = *stats.getCurrentValue(StatType::Health).value();

    health -= damage;
    if (health <= 0.f) {
        health = 0.f;
        startDying();
        return;
    }

    isTakingDamage = true;
    damageTimer = damageAnimationTime;

}

void Mob::startDying() {
    currentState = State::Dying;
    isDying = true;
}

void Mob::updateAnimation() {
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

void Mob::playDeathAnimation() {
    playAnimation("death");
}

void Mob::playHurtAnimation() {
    playAnimation("hurt");
}

void Mob::playAttackAnimation() {
    playAnimation("attack");
}

void Mob::playMovementAnimation() {
    playAnimation("walk");
}

void Mob::playIdleAnimation() {
    playAnimation("idle");
}

void Mob::playAnimation(const std::string& name) {
    if (std::abs(lastDirection.x) > std::abs(lastDirection.y)){
        if(std::signbit(lastDirection.x)) animController.play(name + "_left");
        else animController.play(name + "_right");
    }
    else {
        if(std::signbit(lastDirection.y)) animController.play(name + "_up");
        else animController.play(name + "_down");
    }
}

float Mob::getDamage() const {
    return *stats.getCurrentValue(StatType::Damage).value();
}

bool Mob::getIsAlive() const {
    return isAlive;
}

bool Mob::getIsDying() const {
    return isDying;
}

float Mob::getMaxHealth() const {
    return *stats.getCurrentValue(StatType::MaxHealth).value();
}

float Mob::getCurrentHealth() const {
    return *stats.getCurrentValue(StatType::Health).value();
}

Mob::TypeDamage Mob::getTypeDamage() const {
    return typeDamage;
}

float Mob::getCurrentSpeed() const {
    return *stats.getCurrentValue(StatType::Speed).value();
}

/**
    @brief возвращает -1.f, если у моба нет маны
*/
float Mob::getMaxMana() const {
    if (hasMana) return *stats.getCurrentValue(StatType::MaxMana).value();
    return -1.f;
}

/**
    @brief возвращает -1.f, если у моба нет маны
*/
float Mob::getCurrentMana() const {
    if (hasMana) return *stats.getCurrentValue(StatType::Mana).value();
    return -1.f;
}

StatSet & Mob::getStats() {
    return stats;
}

const StatSet & Mob::getStats() const {
    return stats;
}
