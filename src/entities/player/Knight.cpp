#include "Knight.hpp"

Knight::Knight(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_knight"), map) {
    speed = 400.f;
    maxHealth = 5.f;
    health = maxHealth;
    attackCooldownTime = 0.25f;
    typeDamage = TypeDamage::Melee;
    damage = 2.5f;
}

void Knight::calculateAttackArea() {
    attackArea = currentSprite.getGlobalBounds();

    if (lastDirection.x > 0) {
        attackArea.position.x += 100.f;
        attackArea.position.y += 20.f;
        attackArea.size.x = 50.f;
        attackArea.size.y = 70.f;
    }
    else if (lastDirection.x < 0) {
        attackArea.position.y += 20.f;
        attackArea.size.x = 50.f;
        attackArea.size.y = 70.f;

    }
    else if (lastDirection.y > 0) {
        attackArea.position.x += 40.f;
        attackArea.position.y += 90.f;
        attackArea.size.x = 60.f;
        attackArea.size.y = 40.f;
    }
    else {
        attackArea.position.x += 40.f;
        attackArea.size.x = 60.f;
        attackArea.size.y = 40.f;
    }
}

void Knight::spawnProjectile() {}

void Knight::checkMelee() {
    calculateAttackArea();
    //gameWorld.handleAttackEnemy(attackArea, *this);
}

void Knight::attack() {
    // later...
    startAttacking();
}