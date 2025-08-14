#include "ResourceManager.hpp"

#include <stdexcept>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    static bool initialized = false;
    if (!initialized) {
        instance.loadTextures();
        initialized = true;
    }
    return instance;
}

void ResourceManager::loadTextures() {
    std::lock_guard<std::mutex> lock(textures_mutex);

    const std::string basePath = "../resources/";

    auto loadTexture = [&](const std::string& key, const std::string& path) {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(basePath + path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        textures[key] = std::move(texture);
    };

    try {
        loadTexture("dungeon", basePath + "real img/2 Dungeon Tileset/1 Tiles/Tileset.png");
        loadTexture("health", basePath + "real img/4 GUI/4 Bars/BarsMap.png");
        loadTexture("icons", basePath + "real img/4 GUI/3 Icons/Iconset1.png");
        loadTexture("menu_background", basePath + "img/backgroundMenu.png");

        loadTexture("start_button", basePath + "img/startBtn.png");
        loadTexture("settings_button", basePath + "img/settingsBtn.png");
        loadTexture("exit_button", basePath + "img/exitBtn.png");

        loadTexture("enemy_rat", basePath + "real img/3 Dungeon Enemies/1/1.png");
        loadTexture("enemy_shaman", basePath + "real img/3 Dungeon Enemies/4/4.png");

        loadTexture("player_archer", basePath + "real img/1 Characters/1/1.png");
        loadTexture("player_knight", basePath + "real img/1 Characters/2/2.png");
        loadTexture("player_mage", basePath + "real img/1 Characters/3/3.png");

        loadTexture("player_arrow", basePath + "real img/1 Characters/Other/Arrow.png");
        loadTexture("player_fireball", basePath + "real img/1 Characters/Other/Fireball.png");

        loadTexture("check_mark", basePath + "real img/4 GUI/3 Icons/Iconset5.png");

    } catch (const std::exception& e) {
        textures.clear();
        throw e;
    }
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    std::lock_guard<std::mutex> lock(textures_mutex);

    const auto it = textures.find(name);
    if (it == textures.end() || !it->second) {
        throw std::runtime_error("Texture not found: " + name);
    }
    return *it->second;
}

const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>&
ResourceManager::getAllTextures() const {
    return textures;
}