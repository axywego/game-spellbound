#pragma once

#include <SFML/Graphics.hpp>

#include "Text.hpp"
#include "../../entities/player/Player.hpp"
#include "../StringFormat.hpp"

namespace UI {
    class HUD {
    private:
        sf::RenderWindow* window{};

        sf::Vector2i posLeft;
        sf::Vector2i posCenter;
        sf::Vector2i posRight;

        sf::Texture textureHealthMana;
        std::vector<sf::Sprite> spritesHP;
        std::vector<sf::Sprite> spritesMana;

        Text textDamage;
        Text textSpeed;

        sf::Texture textureIcons = {ResourceManager::getInstance().getTexture("icons")};
        sf::Sprite spriteDamage {textureIcons};
        sf::Sprite spriteSpeed {textureIcons};

        HUD() = default;
        ~HUD() = default;
    public:
        explicit HUD(const ResourceManager&) = delete;
        HUD& operator=(const ResourceManager&) = delete;

        static HUD& getInstance();

        void init(sf::RenderWindow& window);

        void update(const Player& player, const sf::Vector2f& cameraCenter);

        void render();
    };
}

