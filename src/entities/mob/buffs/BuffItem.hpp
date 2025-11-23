#pragma once

#include <string>
#include <vector>
#include "StatModifier.hpp"
#include <SFML/Graphics.hpp>
#include "../../../core/UI/HUD.hpp"

class Mob;

class BuffItem {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::string name;
    std::vector<StatModifier> onPickupEffects;

    bool isPickup {false};
public:
    BuffItem(const sf::Texture& texture, const std::string& itemName, const std::vector<StatModifier>& effects);

    const std::string& getName() const;

    std::vector<StatModifier> getOnPickupEffects() const;

    bool getIsPickup() const;

    sf::Sprite& getSprite();

    void onPickup(Mob* pickuper);

    void render(sf::RenderTarget& target) const ;
};