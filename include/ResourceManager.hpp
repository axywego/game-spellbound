#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class ResourceManager {
private:
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& getTextures() {
        static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
        return textures;
    }
public:
    static const sf::Texture& loadTexture(const std::string& filename) {
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

    static void clearAll() {
        getTextures().clear();
    }
};

#endif