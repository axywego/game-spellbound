#include "Mage.hpp"

Mage::Mage(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_mage"), map) {
    stats.setBaseAttribute(StatType::Speed, 425.f);
    stats.setBaseAttribute(StatType::MaxHealth, 3.f);
    stats.setBaseAttribute(StatType::Health, 3.f);
    attackCooldownTime = 0.4f;
    typeDamage = TypeDamage::Ranged;
    stats.setBaseAttribute(StatType::Damage, 2.f);
    stats.setBaseAttribute(StatType::MaxMana, 4.f);
    stats.setBaseAttribute(StatType::Mana, 4.f);
    stats.setBaseAttribute(StatType::ManaCost, 0.5f);

    timeToUpMana = 0.5f;
    manaToUpPerTime = 0.25f;
}

void Mage::spawnProjectile() {
    sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
    spawnPos.x += 20.f;
    spawnPos.y -= 20.f;

    projectiles.push_back(
            std::make_unique<Fireball>(
                    fireballTexture,
                    map,
                    spawnPos,
                    lastDirection,
                    stats.getCurrentValue(StatType::Damage)
            )
    );
}

void Mage::checkMelee() {}

void Mage::attack() {
    startAttacking();
}