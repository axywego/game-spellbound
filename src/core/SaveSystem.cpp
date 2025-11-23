#include "SaveSystem.hpp"

#include "SettingsManager.hpp"
#include "Utils.hpp"
#include "../entities/player/Player.hpp"
#include "../entities/mob/buffs/StatSet.hpp"
#include "../world/GameWorld.hpp"

SaveSystem& SaveSystem::getInstance() {
	static SaveSystem saveSystem;
	return saveSystem;
}

void SaveSystem::init() {
	SettingsManager::getInstance().init();
}

void SaveSystem::savePlayer(std::weak_ptr<Player> player) {
	nlohmann::json json;

	auto playerPtr = player.lock();

	json["type"] = playerPtr->getPlayerClass();

	auto stats = playerPtr->getStats();
	const auto modifiers = playerPtr->getStats().getActiveModifiers();
	std::unordered_map<StatType, std::vector<std::pair<ModifierType, float>>> modifiersMap;

	for (const auto& modifier : modifiers) {
		modifiersMap[modifier.targetStat].emplace_back(modifier.modType, modifier.value);
	}

	float difference = stats.getCurrentValue(StatType::Health).value() - stats.getCurrentValue(StatType::MaxHealth).value();
	if (difference < 0.f) {
		modifiersMap[StatType::Health].push_back({ModifierType::Flat, difference});
	}

	difference = stats.getCurrentValue(StatType::Mana).value() - stats.getCurrentValue(StatType::MaxMana).value();
	if (difference < 0.f) {
		modifiersMap[StatType::Mana].push_back({ModifierType::Flat, difference});
	}
	json["active_modifiers"] = modifiersMap;

	json["pos"] = { {"x", playerPtr->getSprite().getPosition().x}, {"y", playerPtr->getSprite().getPosition().y} };

	std::fstream file("../saves/player.json", std::ios::out | std::ios::trunc);
	file << json;
	file.close();
}

void SaveSystem::saveGameWorld(GameWorld& gameWorld) {
	nlohmann::json json;
	json["size"] = { {"x", gameWorld.getTilemap().getWorldSize().x }, {"y", gameWorld.getTilemap().getWorldSize().y} };
	std::vector<std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, bool>>> convertedTiledShape;

	for (const auto& row : gameWorld.getTilemap().getTiledShape()) {
		std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, bool>> convertedRow;
		for (const auto& tile : row) {
			const std::pair pos = {tile.getPosition().x, tile.getPosition().y};
			const std::pair size = {tile.getSize().x, tile.getSize().y};
			convertedRow.emplace_back(pos, size, tile.getFillColor() == sf::Color::White);
		}
		convertedTiledShape.emplace_back(convertedRow);
	}
	json["converted_tiled_shape"] = convertedTiledShape;

	std::vector<std::pair<EnemyClass, std::pair<float, float>>> enemiesPosition;
	for (const auto& enemy : gameWorld.getEnemies()) {
		enemiesPosition.push_back({enemy->getEnemyClass(), { enemy->getSprite().getPosition().x, enemy->getSprite().getPosition().y } });
	}
	json["enemies"] = enemiesPosition;

	const auto& buffItems = gameWorld.getBuffItems();
	std::vector<
		std::tuple<
			std::string,
			std::vector<std::tuple<StatType, ModifierType, float>>,
			std::pair<float, float>
		>
	> modifiersItems;
	for (const auto& item : buffItems) {
		const std::pair pos = { item->getSprite().getPosition().x, item->getSprite().getPosition().y };

		std::vector<std::tuple<StatType, ModifierType, float>> effects;
		for (const auto& modifier : item->getOnPickupEffects()) {
			effects.emplace_back(modifier.targetStat, modifier.modType, modifier.value);
		}
		modifiersItems.emplace_back(item->getName(), effects, pos);
	}
	json["buffs_items"] = modifiersItems;

	std::fstream file("../saves/game_world.json", std::ios::out | std::ios::trunc);
	file << json;
	file.close();
}

void SaveSystem::saveSettings() {
	nlohmann::json json;
	json["music_value"] = SettingsManager::getInstance().getMusicValue();
	json["sound_value"] = SettingsManager::getInstance().getSoundValue();
	json["is_vertical_sync"] = SettingsManager::getInstance().getIsVerticalSync();

	std::fstream file("../saves/settings.json", std::ios::out | std::ios::trunc);
	file << json;
	file.close();
}

