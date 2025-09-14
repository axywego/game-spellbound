#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class ResourceManager {
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;

    std::mutex textures_mutex;
    std::mutex fonts_mutex;

    ResourceManager() = default;
    ~ResourceManager() = default;
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance();

    void loadTextures();
    void loadFonts();

    sf::Texture& getTexture(const std::string& name);
    sf::Font& getFont(const std::string& name);

    const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& getAllTextures() const;
    const std::unordered_map<std::string, std::unique_ptr<sf::Font>>& getAllFonts() const;
};
