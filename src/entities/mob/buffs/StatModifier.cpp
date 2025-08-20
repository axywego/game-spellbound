#include "StatModifier.hpp"

StatModifier::StatModifier(StatType stat, ModifierType type, float val,
const std::string& src, float dur = -1.0f) : targetStat(stat), modType(type), value(val),
          sourceId(src), duration(dur), timeRemaining(dur) {}

bool StatModifier::update(float dt) {
    if (duration > 0) {
        timeRemaining -= dt;
        return timeRemaining <= 0;
    }
    return false;
}