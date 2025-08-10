#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../../world/Tilemap.hpp"
#include "Mob.hpp"
#include <ranges>
#include <algorithm>

class Projectile {
protected:
    sf::Sprite sprite;
    sf::Vector2f direction;

    const Tilemap& map;

    float speed;
    float damage;
    bool isAlive;
    sf::FloatRect collisionRect;

    float factor = 4.5f;

public:
    Projectile(const sf::Texture& texture_, const Tilemap& map_, const sf::Vector2f& startpos, const sf::Vector2f& dir, float damage_, float speed_);

    virtual void update(float dt);

    bool checkCollisionWithWorld();

    virtual void onHit(Mob* target);

    bool isActive() const;

    void destroy();

    sf::FloatRect getCollisionRect() const;
    void render(sf::RenderTarget& renderTarget);
};

class Arrow : public Projectile {
public:
    Arrow(const sf::Texture& texture, const Tilemap& map, const sf::Vector2f& startPos, const sf::Vector2f& dir, float damage_);
};

class Fireball : public Projectile {
private:
    float lifetime = 2.0f;
    float currentTime = 0.0f;
public:
    Fireball(const sf::Texture& texture, const Tilemap& map, const sf::Vector2f& startPos, const sf::Vector2f& dir, float damage_);

    void update(float dt) override;
};