#include "Knight.hpp"

Knight::Knight(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_knight"), map) {
    stats.setBaseAttribute(StatType::Speed, 400.f);
    stats.setBaseAttribute(StatType::MaxHealth, 5.f);
    stats.setBaseAttribute(StatType::Health, 5.f);
    attackCooldownTime = 0.25f;
    typeDamage = TypeDamage::Melee;
    stats.setBaseAttribute(StatType::Damage, 2.5f);
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
}

void Knight::attack() {
    startAttacking();
}