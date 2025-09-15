#include "SoundManager.hpp"

SoundManager & SoundManager::getInstance() {
	static SoundManager instance;
	instance.loadSounds();
	return instance;
}

void SoundManager::loadSounds() {
	std::lock_guard lock(mutex);

	const std::string basePath = "../resources/sounds/";

	auto loadSound = [&](const std::string& key, const std::string& path) {
		auto soundBuffer = std::make_unique<sf::SoundBuffer>();
		if (!soundBuffer->loadFromFile(basePath + path)) {
			throw std::runtime_error("Failed to load sound: " + path);
		}

		auto sound = std::make_unique<sf::Sound>(*soundBuffer);

		sounds[key] = std::move(sound);
		soundBuffers[key] = std::move(soundBuffer);
	};

	try {
		loadSound("enemy_hit", "enemy_hit.wav");
		//loadSound("death", "death.mp3");
		loadSound("knight_attack", "knight_attack.wav");
		loadSound("pickup_buff", "pickup_buff.wav");
		loadSound("player_hit", "player_hit.mp3");
	} catch (const std::exception& e) {
		sounds.clear();
		throw e;
	}
}

sf::Sound& SoundManager::getSound(const std::string &name) {
	std::lock_guard lock(mutex);

	const auto it = sounds.find(name);
	if (it == sounds.end() || !it->second) {
		throw std::runtime_error("Sound not found: " + name);
	}
	return *it->second;
}

const std::unordered_map<std::string, std::unique_ptr<sf::Sound>> & SoundManager::getAllSounds() const {
	return sounds;
}
