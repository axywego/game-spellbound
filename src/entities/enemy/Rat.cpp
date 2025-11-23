#include "Rat.hpp"

Rat::Rat(const Tilemap& map_, const std::weak_ptr<Player> &player_):
Enemy(ResourceManager::getInstance().getTexture("enemy_rat"), map_,
sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}),
player_) {

    enemyClass = EnemyClass::Rat;

    stats.setBaseAttribute(StatType::Speed, 500.f);
    stats.setBaseAttribute(StatType::MaxHealth, 4.f);
    stats.setBaseAttribute(StatType::Health, 4.f);
    stats.setBaseAttribute(StatType::Damage, 0.5f);

    rangeRadius = 700.f;
    attackRange = 60.f;

    attackCooldownTime = 0.8f;
    timeToStayAfterAttack = 3.f;
    typeDamage = TypeDamage::Melee;
    raycastView.setRange(rangeRadius);
}

void Rat::spawnProjectile() {}
void Rat::spawnMobs() {}