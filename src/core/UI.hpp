#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../entities/player/Player.hpp"
#include "StringFormat.hpp"
#include "ResourceManager.hpp"

class HUD {
private:
    static sf::Vector2i posLeft;
    static sf::Vector2i posCenter;
    static sf::Vector2i posRight;

    static sf::Texture textureHealthMana;
    static std::vector<sf::Sprite> spritesHP;
    static std::vector<sf::Sprite> spritesMana;

    static sf::Font font;
    static sf::Text textDamage;
    static sf::Text textSpeed;
    static sf::Texture textureIcons;
    static sf::Sprite spriteDamage;
    static sf::Sprite spriteSpeed;
public:
    static void update(const Player& player, const sf::Vector2f& cameraCenter);

    static void render(sf::RenderTarget& target);
};

class Button {
private:
    sf::RenderWindow& window;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Button(const sf::Texture& texture_, sf::RenderWindow& window_);

    void setPosition(sf::Vector2f&& pos);

    bool isHovered() const;

    bool isClicked(const std::optional<sf::Event>& event);

    void render(sf::RenderTarget& renderTarget);
};