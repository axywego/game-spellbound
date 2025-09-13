//
// Created by goasd on 13.09.2025.
//

#include "Music.hpp"

#include <iostream>

Music::Music(const std::string& musicName) {
	if (!music.openFromFile("../resources/music/" + musicName)) {
		std::cerr << "Failed to load music: " << musicName << std::endl;
	}
}

void Music::play() {
	music.play();
}

void Music::stop() {
	music.stop();
}

void Music::pause() {
	if (isPlaying) {
		music.pause();
		isPlaying = false;
	}
}

void Music::resume() {
	if (!isPlaying) {
		music.play();
		isPlaying = true;
	}
}

void Music::setVolume(float vol) {
	music.setVolume(vol);
}
