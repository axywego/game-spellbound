#pragma once

#include <SFML/Audio.hpp>

class Sound {
	sf::Sound sound;
public:
	Sound(const sf::SoundBuffer& buffer);
	void play();
	void stop();
	void pause();
	void setVolume(float volume);

};