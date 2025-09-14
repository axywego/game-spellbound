#pragma once

#include <SFML/Audio.hpp>

class SoundManager {
	std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
	std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;

	std::mutex mutex;

	SoundManager() = default;
	~SoundManager() = default;


public:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	static SoundManager& getInstance();

	void loadSounds();

	sf::Sound& getSound(const std::string& name);

	const std::unordered_map<std::string, std::unique_ptr<sf::Sound>>& getAllSounds() const;
};