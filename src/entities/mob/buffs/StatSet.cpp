#include "StatSet.hpp"
#include <algorithm>
#include <iostream>

void StatSet::setBaseAttribute(StatType type, float value) {
    baseAttributes[type] = value;
    isDirty = true;
}

float StatSet::getBaseAttribute(StatType type) const {
    auto it = baseAttributes.find(type);
    if (it != baseAttributes.end()) {
        return it->second;
    }
    return 0.0f;
}

std::optional<float*> StatSet::getCurrentValue(StatType type) {
    if (isDirty) {
        recalculateStats();
    }

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

void StatSet::modifyResource(StatType type, float amount) {
    float oldValue = *getCurrentValue(type).value();
    float newValue = oldValue + amount;

    if (type == StatType::Health) {
        const float maxHealth = *getCurrentValue(StatType::MaxHealth).value();
        newValue = std::clamp(newValue, 0.0f, maxHealth);
    } else if (type == StatType::Mana) {
        const float maxMana = *getCurrentValue(StatType::MaxMana).value();
        newValue = std::clamp(newValue, 0.0f, maxMana);
    }

    currentValues[type] = newValue;
    notifyChanges();
}

void StatSet::setResource(StatType type, float value) {
    currentValues[type] = value;
    notifyChanges();
}

void StatSet::recalculateStats() {
    if (!isDirty) return;

    const float oldHealth = currentValues[StatType::Health];
    const float oldMana = currentValues[StatType::Mana];
    //const float oldSpeed = currentValues[StatType::Speed];

    currentValues = baseAttributes;

    for (const auto& mod : activeModifiers) {
        if (mod.modType == ModifierType::Flat) {
            currentValues[mod.targetStat] += mod.value;
        } else {
            currentValues[mod.targetStat] *= (1.0f + mod.value / 100.0f);
        }
    }

    if (oldHealth > 0) currentValues[StatType::Health] = oldHealth;
    if (oldMana > 0) currentValues[StatType::Mana] = oldMana;
    //if (oldSpeed > 0) currentValues[StatType::Speed] = oldSpeed;

    if (const float maxHealth = currentValues[StatType::MaxHealth]; currentValues[StatType::Health] > maxHealth) {
        currentValues[StatType::Health] = maxHealth;
    }

    if (float maxMana = currentValues[StatType::MaxMana]; currentValues[StatType::Mana] > maxMana) {
        currentValues[StatType::Mana] = maxMana;
    }

    isDirty = false;
    notifyChanges();
}

void StatSet::notifyChanges() {
    for (const auto& callback : onChangeCallbacks) {
        for (const auto& [stat, value] : currentValues) {
            float oldValue = 0.0f;
            callback(stat, oldValue, value);
        }
    }
}

void StatSet::addModifier(const StatModifier& modifier) {
    activeModifiers.push_back(modifier);
    isDirty = true;
}


void StatSet::clearAllModifiers() {
    if (!activeModifiers.empty()) {
        activeModifiers.clear();
        isDirty = true;
    }
}

void StatSet::addChangeCallback(const std::function<void(StatType, float, float)>& callback) {
    onChangeCallbacks.push_back(callback);
}