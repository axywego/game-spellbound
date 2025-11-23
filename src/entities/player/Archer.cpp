#include "Archer.hpp"

#include "../../core/ResourceManager.hpp"
#include "../mob/buffs/StatsBuilder.hpp"

Archer::Archer(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_archer"), map) {
	playerClass = PlayerClass::Archer;

	stats = StatsBuilder::getBaseStatSetByPlayerClass(playerClass);

	attackCooldownTime = 0.5f;
	typeDamage = TypeDamage::Ranged;
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
                    *stats.getCurrentValue(StatType::Damage).value()
            )
    );
}

void Archer::checkMelee() {}

void Archer::attack() {
    startAttacking();
}