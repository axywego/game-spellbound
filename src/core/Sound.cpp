#include "Sound.hpp"

Sound::Sound(const sf::SoundBuffer &buffer): sound(buffer) {}

void Sound::play() {
	sound.play();
}

void Sound::stop() {
	sound.stop();
}

void Sound::pause() {
	sound.pause();
}

void Sound::setVolume(float volume) {
	sound.setVolume(volume);
}
