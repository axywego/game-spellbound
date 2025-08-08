#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class ResourceManager {
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::mutex textures_mutex;

    ResourceManager() = default;
    ~ResourceManager() = default;
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance();

    void loadTextures();

    sf::Texture& getTexture(const std::string& name);

    const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& getAllTextures() const;

};