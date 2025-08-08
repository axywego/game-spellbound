#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class ResourceManager {
private:
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& getTextures();
public:
    static const sf::Texture& loadTexture(const std::string& filename);

    static void clearAll();
};