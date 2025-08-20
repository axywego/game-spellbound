#include "BuffItem.hpp"
#include "../Mob.hpp"

BuffItem::BuffItem(const std::string& itemName, const std::vector<StatModifier>& effects)
    : name(itemName), onPickupEffects(effects) {}

const std::string& BuffItem::getName() const {
    return name;
}

void BuffItem::onPickup(Mob* pickuper) {
    if (pickuper) {
        for (auto& effect : onPickupEffects) {
            pickuper->getStats().addModifier(effect);
        }
    }
}