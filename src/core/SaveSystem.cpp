#include "SaveSystem.hpp"

#include "SettingsManager.hpp"
#include "Utils.hpp"

SaveSystem& SaveSystem::getInstance() {
	static SaveSystem saveSystem;
	return saveSystem;
}

void SaveSystem::init() {
	SettingsManager::getInstance().init();
}

void SaveSystem::savePlayer(const Player &player) {
}

void SaveSystem::saveGameWorldScene(const std::shared_ptr<Scene> &scene) {
}

void SaveSystem::saveSettings() {
	auto json = getVariablesFromFile("../saves/settings.json");
	json["music_value"] = SettingsManager::getInstance().getMusicValue();
	json["sound_value"] = SettingsManager::getInstance().getSoundValue();
	json["is_vertical_sync"] = SettingsManager::getInstance().getIsVerticalSync();

	std::fstream file("../saves/settings.json", std::ios::out | std::ios::trunc);
	file << json;
	file.close();
}

