#include "Mage.hpp"

#include "../mob/buffs/StatsBuilder.hpp"

Mage::Mage(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_mage"), map) {
	playerClass = PlayerClass::Mage;

	stats = StatsBuilder::getBaseStatSetByPlayerClass(playerClass);

	attackCooldownTime = 0.4f;
	typeDamage = TypeDamage::Ranged;

    hasMana = true;

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
                    *stats.getCurrentValue(StatType::Damage).value()
            )
    );
}

void Mage::checkMelee() {}

void Mage::attack() {
    startAttacking();
}