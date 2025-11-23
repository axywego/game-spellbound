#include "BuffItem.hpp"
#include "../Mob.hpp"

BuffItem::BuffItem(const sf::Texture& texture, const std::string& itemName, const std::vector<StatModifier>& effects)
    : texture(texture), sprite(texture), name(itemName), onPickupEffects(effects) {
    sprite.setOrigin(sprite.getGlobalBounds().size / 2.f);
    sprite.setScale({5.f, 5.f});
}

const std::string& BuffItem::getName() const {
    return name;
}

std::vector<StatModifier> BuffItem::getOnPickupEffects() const {
    return onPickupEffects;
}

bool BuffItem::getIsPickup() const {
    return isPickup;
}

sf::Sprite & BuffItem::getSprite() {
    return sprite;
}

void BuffItem::onPickup(Mob* pickuper) {
    if (pickuper && !isPickup) {
        for (auto& effect : onPickupEffects) {
            pickuper->getStats().addModifier(effect);
        }
        UI::HUD::getInstance().addTitle("You picked up the book and studied: " + name, 4.f);
        isPickup = true;
    }
}

void BuffItem::render(sf::RenderTarget &target) const {
    target.draw(sprite);
}
