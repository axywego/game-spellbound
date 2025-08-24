#pragma once

#include "../entities/mob/buffs/BuffItem.hpp"
#include "../world/GameWorld.hpp"
#include "../core/Generator.hpp"

class BuffsGenerator {
private:
public:
    static std::optional<std::unique_ptr<BuffItem>> create(const Player &player);
};