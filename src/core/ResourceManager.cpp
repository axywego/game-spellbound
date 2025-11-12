#include "ResourceManager.hpp"

#include <iostream>
#include <stdexcept>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    static bool initialized = false;
    if (!initialized) {
        instance.loadTextures();
        instance.loadFonts();
        initialized = true;
    }
    return instance;
}

void ResourceManager::loadTextures() {
    std::lock_guard lock(textures_mutex);

    const std::string basePath = "../resources/";

    auto loadTexture = [&](const std::string& key, const std::string& path) {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(basePath + path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        textures[key] = std::move(texture);
    };

    try {
        std::cout << "Loading textures..." << std::endl;

        loadTexture("dungeon", basePath + "real img/2 Dungeon Tileset/1 Tiles/Tileset.png");
        loadTexture("health", basePath + "real img/4 GUI/4 Bars/BarsMap.png");
        loadTexture("icons", basePath + "real img/4 GUI/3 Icons/Iconset1.png");
        loadTexture("menu_background", basePath + "img/backgroundMenu.png");

        loadTexture("start_button", basePath + "img/startBtn.png");
        loadTexture("settings_button", basePath + "img/settingsBtn.png");
        loadTexture("exit_button", basePath + "img/exitBtn.png");

        loadTexture("back_button", basePath + "img/backBtn.png");
        loadTexture("select_button", basePath + "img/selectBtn.png");
        loadTexture("prev_button", basePath + "img/prevBtn.png");
        loadTexture("next_button", basePath + "img/nextBtn.png");

        loadTexture("enemy_rat", basePath + "real img/3 Dungeon Enemies/1/1.png");
        loadTexture("enemy_goblin", basePath + "real img/3 Dungeon Enemies/2/2.png");
        loadTexture("enemy_ogre", basePath + "real img/3 Dungeon Enemies/3/3.png");
        loadTexture("enemy_shaman", basePath + "real img/3 Dungeon Enemies/4/4.png");

        loadTexture("player_archer", basePath + "real img/1 Characters/1/1.png");
        loadTexture("player_knight", basePath + "real img/1 Characters/2/2.png");
        loadTexture("player_mage", basePath + "real img/1 Characters/3/3.png");

        loadTexture("player_arrow", basePath + "real img/1 Characters/Other/Arrow.png");
        loadTexture("player_fireball", basePath + "real img/1 Characters/Other/Fireball.png");

        loadTexture("check_mark", basePath + "real img/4 GUI/3 Icons/Iconset5.png");

        loadTexture("buff_item", basePath + "real img/2 Dungeon Tileset/2 Objects/Bookshelf decor/24.png");

        loadTexture("default_texture", basePath + "img/default.png");

        loadTexture("anim_start_btn_1", basePath + "img/start_btn1.png");
        loadTexture("anim_start_btn_2", basePath + "img/start_btn2.png");
        loadTexture("anim_start_btn_3", basePath + "img/start_btn3.png");

        loadTexture("anim_background_menu_1", basePath + "img/background_menu1.png");
        loadTexture("anim_background_menu_2", basePath + "img/background_menu2.png");
        loadTexture("anim_background_menu_3", basePath + "img/background_menu3.png");
        loadTexture("anim_background_menu_4", basePath + "img/background_menu4.png");

        std::cout << "all textures was loaded!\n";

    } catch (const std::exception& e) {
        textures.clear();
        throw e;
    }
}

void ResourceManager::loadFonts() {
    std::lock_guard lock(fonts_mutex);

    const std::string basePath = "../resources/fonts/";

    auto loadFont = [&](const std::string& key, const std::string& path) {
        auto font = std::make_unique<sf::Font>();
        if (!font->openFromFile(basePath + path)) {
            throw std::runtime_error("Failed to load font: " + path);
        }
        fonts[key] = std::move(font);
    };

    try {
        loadFont("font_game", "Cafe24PROUP.ttf");
    } catch (const std::exception& e) {
        fonts.clear();
        throw e;
    }
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    std::lock_guard lock(textures_mutex);

    std::cout << name << std::endl;

    const auto it = textures.find(name);
    if (it == textures.end() || !it->second) {
        throw std::runtime_error("Texture not found: " + name);
    }
    return *it->second;
}

sf::Font& ResourceManager::getFont(const std::string& name) {
    std::lock_guard lock(fonts_mutex);

    const auto it = fonts.find(name);
    if (it == fonts.end() || !it->second) {
        throw std::runtime_error("Font not found: " + name);
    }
    return *it->second;
}


const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& ResourceManager::getAllTextures() const {
    return textures;
}

const std::unordered_map<std::string, std::unique_ptr<sf::Font>> & ResourceManager::getAllFonts() const {
    return fonts;
}


