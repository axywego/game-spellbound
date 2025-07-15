#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "World.hpp"
#include "Mob.hpp"

class Projectile {
protected:
    sf::Sprite sprite;
    sf::Vector2f direction;

    std::weak_ptr<World> world;

    float speed;
    float damage;
    bool isAlive;
    sf::FloatRect collisionRect;

    const float factor = 4.5f;

public:
    Projectile(const sf::Texture& texture_, const std::weak_ptr<World>& world_, const sf::Vector2f& startpos, const sf::Vector2f& dir, float damage_, float speed_):
     direction(dir), speed(speed_), damage(damage_), isAlive(true), sprite(texture_), world(world_) {
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

         std::cout << "arrow.move = " << direction.x * speed * dt << '\n';
    }

    bool checkCollisionWithWorld() {
        const auto collisions = world.lock()->getCollisionTiles();

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
        renderTarget.draw(sprite);
    } 

};

class Arrow : public Projectile {
public:
    Arrow(const sf::Texture& texture, const std::weak_ptr<World>& world, const sf::Vector2f& startPos, const sf::Vector2f& dir):
     Projectile(texture, world, startPos, dir, 1.5f, 800.f) {

        sf::Vector2f normalizedDir = dir;
        float length = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (length > 0) {
            normalizedDir /= length;
        }
        direction = normalizedDir; // Обновляем направление

        float angleRad = atan2(dir.y, dir.x);
        //sprite.setRotation(sf::radians(angleRad * 180.f / 3.1415f));
        sprite.setRotation(sf::radians(angleRad));
        sprite.setOrigin({0.f, sprite.getLocalBounds().size.y / 2.f});

    }
};

class Fireball : public Projectile {
private:
    float lifetime = 2.0f;
    float currentTime = 0.0f;

public:
    Fireball(const sf::Texture& texture, const std::weak_ptr<World>& world, const sf::Vector2f& startPos, 
             const sf::Vector2f& dir)
        : Projectile(texture, world, startPos, dir, 2.f, 600.f) { }

    void update(float dt) override {
        Projectile::update(dt);
        currentTime += dt;
        if (currentTime >= lifetime) {
            isAlive = false;
        }
        
        // Можно добавить эффект "пульсации"
        float scale = factor + 0.1f * sin(currentTime * 10);
        sprite.setScale({scale, scale});
    }
};

#endif