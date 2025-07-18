#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Tilemap.hpp"
#include "Mob.hpp"

class Projectile {
protected:
    sf::Sprite sprite;
    sf::Vector2f direction;

    Tilemap& map;

    float speed;
    float damage;
    bool isAlive;
    sf::FloatRect collisionRect;

    float factor = 4.5f;

public:
    Projectile(const sf::Texture& texture_, Tilemap& map_, const sf::Vector2f& startpos, const sf::Vector2f& dir, float damage_, float speed_):
     direction(dir), speed(speed_), damage(damage_), isAlive(true), sprite(texture_), map(map_) {
        sprite.setPosition(startpos);
        sprite.setScale({factor, factor});
        collisionRect = sprite.getGlobalBounds();
    }

    virtual void update(float dt) {
        if (!isAlive) return;
        
        sprite.move(direction * speed * dt);
        collisionRect = sprite.getGlobalBounds();
        
        if (checkCollisionWithWorld()) {
            isAlive = false;
        }
    }

    bool checkCollisionWithWorld() {
        const auto collisions = map.getCollisionTiles();

        for(const auto& tile : collisions){
            if(tile.getGlobalBounds().findIntersection(collisionRect)){
                return true;
            }
        }
        return false;
    }

    virtual void onHit(Mob& target) {
        target.takeDamage(damage);
        isAlive = false;
    }

    bool isActive() const { return isAlive; }
    const sf::FloatRect& getCollisionRect() const { return collisionRect; }
    void render(sf::RenderTarget& renderTarget) {
        if(isAlive) renderTarget.draw(sprite);
    } 

};

class Arrow : public Projectile {
public:
    Arrow(const sf::Texture& texture, Tilemap& map, const sf::Vector2f& startPos, const sf::Vector2f& dir, float damage_):
     Projectile(texture, map, startPos, dir, damage_, 800.f) {

        sf::Vector2f normalizedDir = dir;
        float length = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (length > 0) {
            normalizedDir /= length;
        }
        direction = normalizedDir;

        float angleRad = atan2(dir.y, dir.x);

        sprite.setRotation(sf::radians(angleRad));
        sprite.setOrigin({0.f, sprite.getLocalBounds().size.y / 2.f});

    }
};

class Fireball : public Projectile {
private:
    float lifetime = 2.0f;
    float currentTime = 0.0f;

public:
    Fireball(const sf::Texture& texture, Tilemap& map, const sf::Vector2f& startPos, 
             const sf::Vector2f& dir, float damage_)
        : Projectile(texture, map, startPos, dir, damage_, 600.f) {

        sf::Vector2f normalizedDir = dir;
        float length = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (length > 0) {
            normalizedDir /= length;
        }
        direction = normalizedDir;

        sprite.setOrigin({sprite.getGlobalBounds().size.x / 2.f / factor, sprite.getGlobalBounds().size.y / 2.f / factor});
    }

    void update(float dt) override {
        Projectile::update(dt);
        currentTime += dt;

        if(currentTime >= lifetime - 1.f){
            factor = std::abs(currentTime - lifetime) / factor;
        }

        if (currentTime >= lifetime) {
            isAlive = false;
        }
        collisionRect = sprite.getGlobalBounds();

        //sprite.setOrigin({sprite.getGlobalBounds().size.x / 2.f, sprite.getGlobalBounds().size.y / 2.f});
    
        // pulse 
        float scale = factor + 1.25f * sin(currentTime * 10);
        sprite.setScale({scale, scale});
    }
};

#endif