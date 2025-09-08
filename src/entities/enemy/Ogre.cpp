#include "Ogre.hpp"

Ogre::Ogre(const Tilemap& map_, const std::weak_ptr<Player> &player_):
Enemy(ResourceManager::getInstance().getTexture("enemy_ogre"), map_,
sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}),
player_) {
    stats.setBaseAttribute(StatType::Speed, 300.f);
    stats.setBaseAttribute(StatType::MaxHealth, 6.f);
    stats.setBaseAttribute(StatType::Health, 6.f);
    rangeRadius = 800.f;
    attackRange = 70.f;
    stats.setBaseAttribute(StatType::Damage, 1.f);
    attackCooldownTime = 0.8f;
    timeToStayAfterAttack = 4.f;
    typeDamage = TypeDamage::Melee;
    raycastView.setRange(rangeRadius);
}

void Ogre::spawnProjectile() {}
void Ogre::spawnMobs() {}