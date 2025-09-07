#pragma once

#include <SFML/Graphics.hpp>
#include "../mob/Mob.hpp"
#include "../mob/Projectile.hpp"
#include <cmath>

#include <iostream>
#include <format>

enum class PlayerClass {
    Archer, Knight, Mage
};

class Player : public Mob {
protected:

    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Texture arrowTexture {ResourceManager::getInstance().getTexture("player_arrow")};
    sf::Texture fireballTexture {ResourceManager::getInstance().getTexture("player_fireball")};

    sf::FloatRect attackArea;

    bool meleeAttacked = false;
    
public:
    Player(const sf::Texture& texture_, const Tilemap& map_);
    void update(const float& dt) override;

    std::vector<std::unique_ptr<Projectile>>& getProjectiles();

    virtual void spawnProjectile();
    virtual void checkMelee();

    void attack() override ;

    void attacking(const float& dt) override;

    // move player
    void move(const float& dt) override;

    std::optional<sf::FloatRect> getAttackArea();

    void render(sf::RenderTarget& target) override;

};