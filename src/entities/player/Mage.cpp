#include "Mage.hpp"

Mage::Mage(const Tilemap& map): Player("real img/1 Characters/3/3.png", map) {
    speed = 425.f;
    maxHealth = 3.f;
    health = maxHealth;
    //attackCooldownTime = 0.75f;
    attackCooldownTime = 0.4f;
    typeDamage = TypeDamage::Ranged;
    damage = 2.f;
    maxMana = 4.f;
    mana = maxMana;
    manaCost = 0.5f;

    timeToUpMana = 0.5f;
    manaToUpPerTime = 0.25f;
}

void Mage::spawnProjectile() {
    sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
    spawnPos.x += 20.f;
    spawnPos.y -= 20.f;

    projectiles.push_back(
            std::make_unique<Fireball>(
                    fireballTexture,
                    map,
                    spawnPos,
                    lastDirection,
                    damage
            )
    );
}

void Mage::checkMelee() {}

void Mage::attack() {
    startAttacking();
}