#include "StatSet.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>

/**
 * @brief В параметры передается значения по примеру { {StatType::MaxHealth, 3.f }, { StaType::ManaConst, 0.5f}}
 */
void StatSet::initBaseAttributes(const std::vector<std::pair<StatType, float>>& initValues) {
    for (const auto& [type, value] : initValues) {
        baseAttributes[type] = value;
        currentValues[type] = value;
    }
}

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

    auto* ptr = new float(0.f);

    std::cout << std::format("Current HEALTH before recalculate = {}", *getCurrentValue(StatType::Health).value_or(ptr)) << '\n';

    for (auto& [key, baseValue] : baseAttributes) {
        if (key != StatType::Health && key != StatType::Mana) {
            currentValues[key] = baseValue;
        }
    }

    auto it = std::find_if(activeModifiers.begin(), activeModifiers.end(), [](const StatModifier& mod) {
        return mod.targetStat == StatType::Health;
    });
    if (it != activeModifiers.end()) {
        std::cout << "yes!!\n";
    }

    for (auto mod_it = activeModifiers.begin(); mod_it != activeModifiers.end(); ) {
        currentValues[mod_it->targetStat] += mod_it->value;
        if (mod_it->targetStat == StatType::Health || mod_it->targetStat == StatType::Mana) {
            mod_it = activeModifiers.erase(mod_it);
        }
        else {
            ++mod_it;
        }
    }

    std::cout << std::format("Current HEALTH after recalculate = {}", *getCurrentValue(StatType::Health).value_or(ptr)) << '\n';
}

void StatSet::addModifier(const StatModifier& modifier) {
    activeModifiers.push_back(modifier);
    recalculateStats();
}

std::vector<StatModifier> StatSet::getActiveModifiers() const {
    return activeModifiers;
}


void StatSet::clearAllModifiers() {
    if (!activeModifiers.empty()) {
        activeModifiers.clear();
    }
}
