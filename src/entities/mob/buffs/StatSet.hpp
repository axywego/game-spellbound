#pragma once

#include <unordered_map>
#include <vector>
#include "StatModifier.hpp"
#include <optional>

class StatSet {
private:
    std::unordered_map<StatType, float> baseAttributes;

    std::unordered_map<StatType, float> currentValues;

    std::vector<StatModifier> activeModifiers;

    void recalculateStats();

public:
    void setBaseAttribute(StatType type, float value);
    float getBaseAttribute(StatType type) const;

    std::optional<float*> getCurrentValue(StatType type);

    void addModifier(const StatModifier& modifier);

    std::vector<StatModifier> getActiveModifiers() const;

    void clearAllModifiers();
};