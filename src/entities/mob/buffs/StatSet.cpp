#include "StatSet.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>

void StatSet::setBaseAttribute(StatType type, float value) {
    baseAttributes[type] = value;
    recalculateStats();
}

float StatSet::getBaseAttribute(StatType type) const {
    auto it = baseAttributes.find(type);
    if (it != baseAttributes.end()) {
        return it->second;
    }
    return -1.f;
}

std::optional<float*> StatSet::getCurrentValue(StatType type) {
    const auto it = currentValues.find(type);
    if (it != currentValues.end()) {
        return &it->second;
    }

    const auto baseIt = baseAttributes.find(type);
    if (baseIt != baseAttributes.end()) {
        return &baseIt->second;
    }

    return std::nullopt;
}

void StatSet::recalculateStats() {
    for (auto& [key, baseValue] : baseAttributes) {
        if (key != StatType::Health && key != StatType::Mana) {
            currentValues[key] = baseValue;
        }
    }

    for (auto mod_it  = activeModifiers.begin(); mod_it != activeModifiers.end(); ) {
        std::cout << static_cast<unsigned>(mod_it->targetStat) << std::endl;
        currentValues[mod_it->targetStat] += mod_it->value;
        if (mod_it->targetStat == StatType::Health || mod_it->targetStat == StatType::Mana) {
            mod_it = activeModifiers.erase(mod_it);
        }
        else {
            ++mod_it;
        }
    }
}

void StatSet::addModifier(const StatModifier& modifier) {
    activeModifiers.push_back(modifier);
    recalculateStats();
}


void StatSet::clearAllModifiers() {
    if (!activeModifiers.empty()) {
        activeModifiers.clear();
    }
}