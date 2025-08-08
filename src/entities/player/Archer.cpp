#include "Archer.hpp"

#include "../../core/ResourceManager.hpp"

Archer::Archer(const Tilemap& map): Player(ResourceManager::getInstance().getTexture("player_archer"), map) {
    speed = 450.f;
    maxHealth = 4.f;
    health = maxHealth;
    attackCooldownTime = 0.5f;
    typeDamage = TypeDamage::Ranged;
    damage = 1.5f;
}

void Archer::spawnProjectile()  {
    sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
    spawnPos.y -= 20.f;
    //spawnPos.x += currentSprite.getGlobalBounds().size.x / 2;
    //spawnPos.y += currentSprite.getGlobalBounds().size.y / 2;

    projectiles.push_back(
            std::make_unique<Arrow>(
                    arrowTexture,
                    map,
                    spawnPos,
                    lastDirection,
                    damage
            )
    );
}

void Archer::checkMelee() {}

void Archer::attack() {
    // later...
    startAttacking();
}