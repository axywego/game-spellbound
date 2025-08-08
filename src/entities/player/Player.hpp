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
    sf::Texture arrowTexture;
    sf::Texture fireballTexture;

    sf::FloatRect attackArea;
    
public:
    Player(const std::string& texturePath, const Tilemap& map_);
    void update(const float& dt) override;

    const std::vector<std::unique_ptr<Projectile>>& getProjectiles() const;

    virtual void spawnProjectile();
    virtual void checkMelee();

    void attack() override ;

    void attacking(const float& dt) override;

    // move player
    void move(const float& dt) override;

    void updateProjectiles(float dt);

    std::optional<sf::FloatRect> getAttackArea();

    void render(sf::RenderTarget& target) override;

};