#pragma once

#include "StatSet.hpp"
#include "../../player/Player.hpp"
#include "../../enemy/Enemy.hpp"

class StatsBuilder {
private:

public:
	static StatSet getBaseStatSetByPlayerClass(const PlayerClass& playerClass);
	static StatSet getBaseStatSetByEnemyClass(const EnemyClass& enemyClass);
};