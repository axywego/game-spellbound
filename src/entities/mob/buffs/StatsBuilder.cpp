#include "StatsBuilder.hpp"

StatSet StatsBuilder::getBaseStatSetByPlayerClass(const PlayerClass& playerClass) {
	StatSet stats;
	switch (playerClass) {
		case PlayerClass::Archer:
			stats.initBaseAttributes({
				{StatType::Speed, 450.f},
				{StatType::MaxHealth, 4.f},
				{StatType::Health, 4.f},
				{StatType::Damage, 1.5f},
			});
			return stats;
		case PlayerClass::Knight:
			stats.initBaseAttributes({
				{StatType::Speed, 400.f},
				{StatType::MaxHealth, 5.f},
				{StatType::Health, 5.f},
				{StatType::Damage, 2.5f},
			});
			return stats;
		case PlayerClass::Mage:
			stats.initBaseAttributes({
				{StatType::Speed, 425.f},
				{StatType::MaxHealth, 3.f},
				{StatType::Health, 3.f},
				{StatType::Damage, 2.f},
				{StatType::MaxMana, 4.f},
				{StatType::Mana, 4.f},
				{StatType::ManaCost, 0.5f},
			});
			return stats;
		default:
			break;
	}
	return stats;
}

StatSet StatsBuilder::getBaseStatSetByEnemyClass(const EnemyClass &enemyClass) {
	StatSet stats;
	switch (enemyClass) {
		case EnemyClass::Goblin:
			stats.initBaseAttributes({
				{StatType::Speed, 600.f},
				{StatType::MaxHealth, 2.f},
				{StatType::Health, 2.f},
				{StatType::Damage, 0.5f},
			});
			return stats;
		case EnemyClass::Ogre:
			stats.initBaseAttributes({
				{StatType::Speed, 300.f},
				{StatType::MaxHealth, 6.f},
				{StatType::Health, 6.f},
				{StatType::Damage, 1.f},
			});
			return stats;
		case EnemyClass::Rat:
			stats.initBaseAttributes({
				{StatType::Speed, 500.f},
				{StatType::MaxHealth, 4.f},
				{StatType::Health, 4.f},
				{StatType::Damage, 0.5f},
			});
			return stats;
		case EnemyClass::Shaman:
			stats.initBaseAttributes({
				{StatType::Speed, 230.f},
				{StatType::MaxHealth, 2.f},
				{StatType::Health, 2.f},
				{StatType::Damage, 0.f},
			});
			return stats;
		default:
			break;
	}
	return stats;
}