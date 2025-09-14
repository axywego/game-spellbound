#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class MusicManager {
private:
    static inline std::unordered_map<std::string, std::string> musicRegistry;
    static inline std::string basePath = "../resources/music/";

public:
    static void setBasePath(const std::string& path);
    static void registerMusic(const std::string& key, const std::string& path);
    static std::unique_ptr<sf::Music> createMusicInstance(const std::string& key);
    static bool isMusicRegistered(const std::string& key);
};