#pragma once

#include <string>

enum class StatType {
    Health,
    MaxHealth,
    Damage,
    Mana,
    MaxMana,
    ManaCost,
    Speed,
};

enum class ModifierType {
    Flat, Percent
};

struct StatModifier {
    StatType targetStat;
    ModifierType modType;
    float value;
    std::string sourceId;
    float duration;
    float timeRemaining;

    StatModifier(StatType stat, ModifierType type, float val,
                 const std::string& src, float dur);
    bool update(float deltaTime);
};