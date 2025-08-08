#include "Projectile.hpp"


Projectile::Projectile(const sf::Texture& texture_, const Tilemap& map_, const sf::Vector2f& startpos, const sf::Vector2f& dir, float damage_, float speed_):
        direction(dir), speed(speed_), damage(damage_), isAlive(true), sprite(texture_), map(map_) {
    sprite.setPosition(startpos);
    sprite.setScale({factor, factor});
    collisionRect = sprite.getGlobalBounds();
}

void Projectile::update(float dt) {
    if (!isAlive) return;

    sprite.move(direction * speed * dt);
    collisionRect = sprite.getGlobalBounds();

    if (checkCollisionWithWorld()) {
        isAlive = false;
    }
}

bool Projectile::checkCollisionWithWorld() {
    const auto collisions = map.getCollisionTiles();

    return std::ranges::any_of(collisions, [this](const sf::Sprite& tile) {
        return tile.getGlobalBounds().findIntersection(collisionRect).has_value();
    });
}

void Projectile::onHit(Mob* target) {
    target->takeDamage(damage);
    isAlive = false;
}

bool Projectile::isActive() const { return isAlive; }
const sf::FloatRect& Projectile::getCollisionRect() const { return collisionRect; }
void Projectile::render(sf::RenderTarget& renderTarget) {
    if(isAlive) renderTarget.draw(sprite);
}


Arrow::Arrow(const sf::Texture& texture, const Tilemap& map, const sf::Vector2f& startPos, const sf::Vector2f& dir, float damage_):
        Projectile(texture, map, startPos, dir, damage_, 800.f) {

    sf::Vector2f normalizedDir = dir;
    float length = sqrtf(dir.x*dir.x + dir.y*dir.y);
    if (length > 0) {
        normalizedDir /= length;
    }
    direction = normalizedDir;

    float angleRad = atan2f(dir.y, dir.x);

    sprite.setRotation(sf::radians(angleRad));
    sprite.setOrigin({0.f, sprite.getLocalBounds().size.y / 2.f});

}

Fireball::Fireball(const sf::Texture& texture, const Tilemap& map, const sf::Vector2f& startPos,
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

void Fireball::update(float dt)  {
    Projectile::update(dt);
    currentTime += dt;

    if(currentTime >= lifetime - 1.f){
        factor = std::abs(currentTime - lifetime) / factor;
    }

    if (currentTime >= lifetime) {
        isAlive = false;
    }
    collisionRect = sprite.getGlobalBounds();

    // pulse
    float scale = factor + 1.25f * sinf(currentTime * 10);
    sprite.setScale({scale, scale});
}