#include "StatsBuilder.hpp"

StatSet StatsBuilder::getBaseStatSetByPlayerClass(const PlayerClass& playerClass) {
	StatSet stats;
	switch (playerClass) {
		case PlayerClass::Archer:
			stats.setBaseAttribute(StatType::Speed, 450.f);
			stats.setBaseAttribute(StatType::MaxHealth, 4.f);
			stats.setBaseAttribute(StatType::Health, 4.f);
			stats.setBaseAttribute(StatType::Damage, 1.5f);
		return stats;
		case PlayerClass::Knight:
			stats.setBaseAttribute(StatType::Speed, 400.f);
			stats.setBaseAttribute(StatType::MaxHealth, 5.f);
			stats.setBaseAttribute(StatType::Health, 5.f);
			stats.setBaseAttribute(StatType::Damage, 2.5f);
		case PlayerClass::Mage:
			stats.setBaseAttribute(StatType::Speed, 425.f);
			stats.setBaseAttribute(StatType::MaxHealth, 3.f);
			stats.setBaseAttribute(StatType::Health, 3.f);
			stats.setBaseAttribute(StatType::Damage, 2.f);
			stats.setBaseAttribute(StatType::MaxMana, 4.f);
			stats.setBaseAttribute(StatType::Mana, 4.f);
			stats.setBaseAttribute(StatType::ManaCost, 0.5f);
		default:
			//std::cout << "chego blyat?\n";
			break;
	}
	return stats;
}

StatSet StatsBuilder::getBaseStatSetByEnemyClass(const EnemyClass &enemyClass) {
	StatSet stats;
	switch (enemyClass) {
		case EnemyClass::Goblin:
			stats.setBaseAttribute(StatType::Speed, 600.f);
			stats.setBaseAttribute(StatType::MaxHealth, 2.f);
			stats.setBaseAttribute(StatType::Health, 2.f);
			stats.setBaseAttribute(StatType::Damage, 0.5f);
			return stats;
		case EnemyClass::Ogre:
			stats.setBaseAttribute(StatType::Speed, 300.f);
			stats.setBaseAttribute(StatType::MaxHealth, 6.f);
			stats.setBaseAttribute(StatType::Health, 6.f);
			stats.setBaseAttribute(StatType::Damage, 1.f);
			return stats;
		case EnemyClass::Rat:
			stats.setBaseAttribute(StatType::Speed, 500.f);
			stats.setBaseAttribute(StatType::MaxHealth, 4.f);
			stats.setBaseAttribute(StatType::Health, 4.f);
			stats.setBaseAttribute(StatType::Damage, 0.5f);
			return stats;
		case EnemyClass::Shaman:
			stats.setBaseAttribute(StatType::Speed, 230.f);
			stats.setBaseAttribute(StatType::MaxHealth, 2.f);
			stats.setBaseAttribute(StatType::Health, 2.f);
			stats.setBaseAttribute(StatType::Damage, 0.f);
			return stats;
		default:
			//std::cout << "chego blyat?\n";
			break;
	}
	return stats;
}