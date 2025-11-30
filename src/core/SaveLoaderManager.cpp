#include "SaveLoaderManager.hpp"

#include "Utils.hpp"
#include "../entities/enemy/EnemyFactory.hpp"
#include "../entities/player/PlayerFactory.hpp"
#include "../world/GameWorld.hpp"
#include "../world/Tilemap.hpp"
#include "../entities/mob/buffs/StatSet.hpp"

SaveLoaderManager& SaveLoaderManager::getInstance() {
	static SaveLoaderManager instance;
	return instance;
}

Tilemap SaveLoaderManager::getSavedTilemap() {
	auto json = getVariablesFromFile("../saves/game_world.json");

	std::vector<std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, bool>>> tiledShapeData = json["converted_tiled_shape"];
	std::vector<std::vector<sf::RectangleShape>> tiledShape;
	for (const auto& row : tiledShapeData) {
		std::vector<sf::RectangleShape> tiledRowShape;
		for (const auto& [pos, size, isWhite] : row) {
			sf::RectangleShape rect;
			rect.setPosition({pos.first, pos.second});
			rect.setSize({size.first, size.second});
			rect.setFillColor(isWhite ? sf::Color::White : sf::Color::Black);
			tiledRowShape.push_back(rect);
		}
		tiledShape.push_back(tiledRowShape);
	}
	Tilemap tilemap;
	tilemap.createFromTiledShape(tiledShape);
	return tilemap;
}

std::shared_ptr<GameWorld> SaveLoaderManager::getSavedGameWorld(const std::weak_ptr<Player>& player) {
	auto json = getVariablesFromFile("../saves/game_world.json");

	// Loading TiledShape

	auto tilemap = getSavedTilemap();

	auto gameWorld = std::make_shared<GameWorld>(tilemap);

	// Loading Enemies
	std::vector<std::pair<EnemyClass, std::pair<float, float>>> enemiesPosition = json["enemies"];
	for (const auto& [enemyClass, pos] : enemiesPosition) {
		std::unique_ptr<Enemy> enemy = EnemyFactory::create(enemyClass, gameWorld->getTilemap(), player);
		if (enemyClass == EnemyClass::Shaman) {
			enemy->setSpawnCallback([gameWorld](auto&& mob, auto&& pos) {
			   gameWorld->addEnemy(std::move(mob), std::move(pos));
		   });
		}
		gameWorld->addEnemy(std::move(enemy), {pos.first, pos.second});
	}

	//Loading BuffItems
	std::vector<
		std::tuple<
			std::string,
			std::vector<std::tuple<StatType, ModifierType, float>>,
			std::pair<float, float>
		>
	> modifiersItems = json["buffs_items"];
	for (const auto& [name, modifiers, pos] : modifiersItems) {
		std::vector<StatModifier> modifiersStats;
		for (const auto& [targetStat, modType, value] : modifiers) {
			StatModifier modifier = { targetStat, modType, value };
			modifiersStats.emplace_back(modifier);
		}
		auto buffItem = std::make_unique<BuffItem>(ResourceManager::getInstance().getTexture("buff_item"), name, modifiersStats);
		gameWorld->addBuffItem(std::move(buffItem), {pos.first, pos.second});
	}

	return gameWorld;
}

std::shared_ptr<Player> SaveLoaderManager::getSavedPlayer() {
	auto json = getVariablesFromFile("../saves/player.json");

	Tilemap tilemap = getSavedTilemap();
	std::shared_ptr<Player> player = PlayerFactory::create(json["type"], tilemap);

	std::unordered_map<StatType, std::vector<std::pair<ModifierType, float>>> modifiersMap = json["active_modifiers"];

	for (const auto& [targetStat, vector] : modifiersMap) {
		for (const auto& [modType, value] : vector) {
			player->getStats().addModifier(StatModifier{targetStat, modType, value});
		}
	}

	// std::vector<std::pair<StatType, std::vector<std::pair<ModifierType, float>>>> modifiers = json["active_modifiers"];
	//
	// for (const auto& [targetStat, vector] : modifiers) {
	// 	for (const auto& [modType, value] : vector) {
	// 		player->getStats().addModifier(StatModifier{targetStat, modType, value});
	// 	}
	// }

	std::pair<float, float> pos;
	pos.first = json["pos"]["x"];
	pos.second = json["pos"]["y"];
	player->setPosition({pos.first, pos.second});

	return player;
}
