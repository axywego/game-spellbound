#include "Shaman.hpp"

Shaman::Shaman(const Tilemap& map_, const std::weak_ptr<Player> &player_):
Enemy(ResourceManager::getInstance().getTexture("enemy_shaman"), map_,
sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}), player_) {
    stats.setBaseAttribute(StatType::Speed, 230.f);
    stats.setBaseAttribute(StatType::MaxHealth, 2.f);
    stats.setBaseAttribute(StatType::Health, 2.f);
    rangeRadius = 900.f;
    attackRange = 500.f;
    stats.setBaseAttribute(StatType::Damage, 0.f);
    attackCooldownTime = 15.f;
    timeToStayAfterAttack = 1.f;
    typeDamage = TypeDamage::Summon;
    raycastView.setRange(rangeRadius);
}

void Shaman::spawnProjectile() {}

void Shaman::spawnMobs() {
    if (!spawnCallback) return;

    auto rat = std::make_unique<Rat>(map, player);
    auto spawnPosition = getSprite().getPosition() + sf::Vector2f(50.f, 0.f);

    spawnCallback(std::move(rat), spawnPosition);
}