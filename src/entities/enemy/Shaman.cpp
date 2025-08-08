#include "Shaman.hpp"

Shaman::Shaman(const Tilemap& map_, std::weak_ptr<Player> player_):
Enemy(sf::Texture("real img/3 Dungeon Enemies/4/4.png"), map_,
sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}), player_) {
    speed = 230.f;
    maxHealth = 2.f;
    health = maxHealth;
    rangeRadius = 900.f;
    attackRange = 500.f;
    damage = 0.0f;
    attackCooldownTime = 15.f;
    timeToStayAfterAttack = 1.f;
    typeDamage = TypeDamage::Summon;
}

void Shaman::spawnProjectile() {}

void Shaman::spawnMobs() {
    if (!spawnCallback) return;

    auto rat = std::make_unique<Rat>(map, player);
    auto spawnPosition = getSprite().getPosition() + sf::Vector2f(50.f, 0.f);

    spawnCallback(std::move(rat), spawnPosition);
}