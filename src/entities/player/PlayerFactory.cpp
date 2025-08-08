#include "PlayerFactory.hpp"

std::shared_ptr<Player> PlayerFactory::create(PlayerClass type, const Tilemap& map) {
    switch(type) {
        case PlayerClass::Archer: return std::make_shared<Archer>(map);
        case PlayerClass::Knight: return std::make_shared<Knight>(map);
        case PlayerClass::Mage: return std::make_shared<Mage>(map);
        default: return nullptr;
    }
}