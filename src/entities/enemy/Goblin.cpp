#include "Goblin.hpp"

Goblin::Goblin(const Tilemap& map_, const std::weak_ptr<Player> &player_):
Enemy(ResourceManager::getInstance().getTexture("enemy_goblin"), map_,
sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}),
player_) {

    enemyClass = EnemyClass::Goblin;

    stats.setBaseAttribute(StatType::Speed, 600.f);
    stats.setBaseAttribute(StatType::MaxHealth, 2.f);
    stats.setBaseAttribute(StatType::Health, 2.f);
    stats.setBaseAttribute(StatType::Damage, 0.5f);

    rangeRadius = 500.f;
    attackRange = 60.f;

    attackCooldownTime = 0.8f;
    timeToStayAfterAttack = 2.f;
    typeDamage = TypeDamage::Melee;
    raycastView.setRange(rangeRadius);
}

void Goblin::spawnProjectile() {}
void Goblin::spawnMobs() {}