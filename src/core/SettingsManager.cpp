#include "SettingsManager.hpp"

#include <iostream>

#include "Utils.hpp"

SettingsManager& SettingsManager::getInstance() {
	static SettingsManager instance;
	return instance;
}

void SettingsManager::init() {
	auto json = getVariablesFromFile("../saves/settings.json");
	musicValue = json["music_value"];
	soundValue = json["music_value"];
	isVerticalSync = json["is_vertical_sync"];
}

void SettingsManager::setMusicValue(float newMusicValue) {
	musicValue = newMusicValue;
}

void SettingsManager::setSoundValue(float newSoundValue) {
	soundValue = newSoundValue;
}

void SettingsManager::setIsVerticalSync(bool newIsVerticalSync) {
	isVerticalSync = newIsVerticalSync;
}

float SettingsManager::getMusicValue() const {
	return musicValue;
}

float SettingsManager::getSoundValue() const {
	return soundValue;
}

bool SettingsManager::getIsVerticalSync() const {
	return isVerticalSync;
}