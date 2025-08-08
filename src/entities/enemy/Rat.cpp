#include "Rat.hpp"

Rat::Rat(const Tilemap& map_, std::weak_ptr<Player> player_):
Enemy(sf::Texture("real img/3 Dungeon Enemies/1/1.png"), map_,
sf::FloatRect({{32.f, 32.f},{60.f, 60.f}}),
player_) {
    speed = 500.f;
    maxHealth = 4.f;
    health = maxHealth;
    rangeRadius = 700.f;
    attackRange = 60.f;
    damage = 0.5f;
    attackCooldownTime = 0.8f;
    timeToStayAfterAttack = 3.f;
    typeDamage = TypeDamage::Melee;
}

void Rat::spawnProjectile() {}
void Rat::spawnMobs() {}