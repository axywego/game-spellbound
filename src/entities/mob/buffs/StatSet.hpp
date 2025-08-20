#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "StatModifier.hpp"

class StatSet {
private:
    std::unordered_map<StatType, float> baseAttributes;

    std::unordered_map<StatType, float> currentValues;

    std::vector<StatModifier> activeModifiers;

    bool isDirty = true;

    std::vector<std::function<void(StatType, float, float)>> onChangeCallbacks;

    void recalculateStats();
    void notifyChanges();

public:
    void setBaseAttribute(StatType type, float value);
    float getBaseAttribute(StatType type) const;

    float& getCurrentValue(StatType type);

    void modifyResource(StatType type, float amount);
    void setResource(StatType type, float value);

    void addModifier(const StatModifier& modifier);
    void removeModifiersBySource(const std::string& sourceId);
    void clearAllModifiers();

    void update(float deltaTime);

    void addChangeCallback(const std::function<void(StatType, float, float)>& callback);
};