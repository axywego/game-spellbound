#include "ResourceManager.hpp"

 std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& ResourceManager::getTextures() {
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    return textures;
}
const sf::Texture& ResourceManager::loadTexture(const std::string& filename) {
    auto& textures = getTextures();

    if (auto it = textures.find(filename); it != textures.end()) {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    const auto& ref = *texture;
    textures[filename] = std::move(texture);
    return ref;
}

void ResourceManager::clearAll() {
    getTextures().clear();
}