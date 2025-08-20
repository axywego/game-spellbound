#pragma once

#include <string>
#include <vector>
#include "StatModifier.hpp"

class Mob;

class BuffItem {
private:
    std::string name;
    std::vector<StatModifier> onPickupEffects;

public:
    BuffItem(const std::string& itemName, const std::vector<StatModifier>& effects);

    const std::string& getName() const;

    void onPickup(Mob* pickuper);
};