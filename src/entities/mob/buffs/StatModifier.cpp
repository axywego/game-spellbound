#include "StatModifier.hpp"

StatModifier::StatModifier(StatType stat, ModifierType type, float val) : targetStat(stat), modType(type), value(val) {}