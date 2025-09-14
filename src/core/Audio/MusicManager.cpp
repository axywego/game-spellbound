#include "MusicManager.hpp"
#include <iostream>

void MusicManager::setBasePath(const std::string& path) {
	basePath = path;
}

void MusicManager::registerMusic(const std::string& key, const std::string& path) {
	musicRegistry[key] = path;
}

bool MusicManager::isMusicRegistered(const std::string& key) {
	return musicRegistry.contains(key);
}

std::unique_ptr<sf::Music> MusicManager::createMusicInstance(const std::string& key) {
	if (!isMusicRegistered(key)) {
		std::cerr << "Error: Music '" << key << "' is not registered!" << std::endl;
		return nullptr;
	}

	auto music = std::make_unique<sf::Music>();

	if (const std::string fullPath = basePath + musicRegistry[key]; !music->openFromFile(fullPath)) {
		std::cerr << "Error: Failed to load music from: " << fullPath << std::endl;
		return nullptr;
	}

	return music;
}