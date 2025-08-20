#include "Archer.hpp"

#include "../../core/ResourceManager.hpp"

Archer::Archer(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_archer"), map) {
    stats.setBaseAttribute(StatType::Speed, 450.f);
    stats.setBaseAttribute(StatType::MaxHealth, 4.f);
    stats.setBaseAttribute(StatType::Health, 4.f);
    attackCooldownTime = 0.5f;
    typeDamage = TypeDamage::Ranged;
    stats.setBaseAttribute(StatType::Damage, 1.5f);
}

void Archer::spawnProjectile()  {
    sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
    spawnPos.y -= 20.f;

    projectiles.push_back(
            std::make_unique<Arrow>(
                    arrowTexture,
                    map,
                    spawnPos,
                    lastDirection,
                    stats.getCurrentValue(StatType::Damage)
            )
    );
}

void Archer::checkMelee() {}

void Archer::attack() {
    startAttacking();
}