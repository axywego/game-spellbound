#include "Player.hpp"

Player::Player(const sf::Texture& texture_, const Tilemap& map_):
Mob(texture_, map_, sf::FloatRect {{40.f, 44.f},
{64.f, 52.f}}) { }

void Player::update(const float& dt) {
    if (isDying) {
        currentState = State::Dying;
        if(animController.hasLastFrame()){
            isAlive = false;
        }
    }

    if (isTakingDamage) {
        currentState = State::Hurt;
        damageTimer -= dt;
        if (damageTimer <= 0.0f) {
            isTakingDamage = false;
            currentState = State::Idle;
        }
    }

    if(timerDelayMana > 0.f){
        timerDelayMana -= dt;
    }

    if(mana < maxMana && timerDelayMana <= 0.f){
        if(timerMana >= timeToUpMana){
            mana += manaToUpPerTime;
            timerMana = 0.f;
        }
    }

    if(!isDying && !isTakingDamage) {
        collision.update();
        attacking(dt);
        if(!isAttacking) move(dt);
    }

    if(!isAttacking) timerMana += dt;

    animController.update(dt);
    updateAnimation();
}

std::vector<std::unique_ptr<Projectile>>& Player::getProjectiles() {
    return projectiles;
}


void Player::attacking(const float& dt) {
    if (isAttacking) {
        attackTimer -= dt;

        if(typeDamage == TypeDamage::Melee && animController.isSameAnimation("attack") && animController.hasPenultFrame() && !hasAttacked){
            checkMelee();
            hasAttacked = true;
        }

        else if(typeDamage == TypeDamage::Ranged && animController.isSameAnimation("attack") && animController.hasPenultFrame() && !hasAttacked) {
            hasAttacked = true;
            if(mana - manaCost >= 0.f){
                mana -= manaCost;
                timerDelayMana = delayBeforeUpMana;
                spawnProjectile();
            }
        }

        if (attackTimer <= 0.0f) {
            isAttacking = false;

            hasAttacked = false;
            meleeAttacked = false;

            attackCooldown = attackCooldownTime;
            currentState = State::Idle;
        }
    }

    if (attackCooldown > 0.0f) {
        attackCooldown -= dt;
    }
}

// move player
void Player::move(const float& dt) {
    sf::Vector2f dir{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
        dir.y--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        dir.y++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        dir.x--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        dir.x++;
    }

    const auto incorrectDir = collision.checkCollisions(map);

    if(dir.x != 0 && dir.y != 0){
        dir.x *= sqrtf(2) / 2;
        dir.y *= sqrtf(2) / 2;
    }

    if(!(dir.x == 0 && dir.y == 0)){
        lastDirection = dir;
    }

    if (incorrectDir.x != 0.f || incorrectDir.y != 0.f) {
        if (incorrectDir.x != 0.f && std::signbit(incorrectDir.x) == std::signbit(dir.x)) {
            dir.x = 0.f;
        }
        if (incorrectDir.y != 0.f && std::signbit(incorrectDir.y) == std::signbit(dir.y)) {
            dir.y = 0.f;
        }
    }

    if(dir.x == 0 && dir.y == 0){
        currentState = State::Idle;
    }
    else {
        currentState = State::Moving;
        currentSprite.move(dir * speed * dt);
    }
}

void Player::updateProjectiles(float dt) {
    for(auto it = projectiles.begin(); it != projectiles.end(); ){
        (*it)->update(dt);

        if(!(*it)->isActive()){
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

std::optional<sf::FloatRect> Player::getAttackArea() {
    if(!meleeAttacked && typeDamage == TypeDamage::Melee) {
        meleeAttacked = true;
        return attackArea;
    }
    return std::nullopt;
}

void Player::render(sf::RenderTarget& target) {
    target.draw(currentSprite);

    for(const auto& projectile : projectiles) {
        projectile->render(target);
    }

    sf::RectangleShape shape;
    shape.setPosition(attackArea.position);
    shape.setSize(attackArea.size);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.f);
    target.draw(shape);
    //collision.render(target);
}

void Player::attack() {

}

void Player::checkMelee() {

}

void Player::spawnProjectile() {

}
