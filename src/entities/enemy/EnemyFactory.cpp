#include "EnemyFactory.hpp"

std::unique_ptr<Enemy> EnemyFactory::create(EnemyClass type, const Tilemap& map, std::weak_ptr<Player> player) {
    switch(type) {
        case EnemyClass::Rat: return std::make_unique<Rat>(map, player);
        case EnemyClass::Shaman: return std::make_unique<Shaman>(map, player);
            // case EnemyClass::Goblin: return std::make_unique<Knight>(world);
            // case EnemyClass::Demon: return std::make_unique<Mage>(world);
            // case EnemyClass::Shaman: return std::make_unique<Mage>(world);
        default: return nullptr;
    }
}