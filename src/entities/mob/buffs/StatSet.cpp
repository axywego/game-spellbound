#include "StatSet.hpp"
#include <algorithm>

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
        currentValues[key] = baseValue;
    }
    for (const auto& mod : activeModifiers) {
        currentValues[mod.targetStat] += mod.value;
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