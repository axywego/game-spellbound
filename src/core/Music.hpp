#pragma once

#include <SFML/Audio.hpp>

class Music {
private:
    sf::Music music;
    bool isPlaying {false};

public:
    Music(const std::string& musicName);
    void play();
    void stop();
    void pause();
    void resume();
    void setVolume(float vol);
};
