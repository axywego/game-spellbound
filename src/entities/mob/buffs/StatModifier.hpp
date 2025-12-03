#pragma once

#include <unordered_map>

enum class StatType {
    Health,
    MaxHealth,
    Damage,
    Speed,
    Mana,
    MaxMana,
    ManaCost,
    Count
};

const static inline std::unordered_map<StatType, float> statLimits = {
    {StatType::Health, 15.f},
    {StatType::MaxHealth, 15.f},
    {StatType::Damage, 10.f},
    {StatType::Speed, 700.f},
    {StatType::Mana, 15.f},
    {StatType::MaxMana, 15.f},
};

enum class ModifierType {
    Flat, Percent
};

struct StatModifier {
    StatType targetStat;
    ModifierType modType;
    float value;

    StatModifier(StatType stat, ModifierType type, float val);
};