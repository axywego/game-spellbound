#include "Enemy.hpp"

Enemy::Enemy(const sf::Texture& texture, const Tilemap& map_, const sf::FloatRect& collisionRect_, std::weak_ptr<Player> player_):
        Mob(texture, map_, collisionRect_), player(player_) {
    hpBackgroundShape.setFillColor(sf::Color::Black);
    hpRedShape.setFillColor(sf::Color::Red);

    hpBackgroundShape.setSize({70.f, 10.f});
    hpRedShape.setSize({65.f, 8.f});
}

void Enemy::setSpawnCallback(const std::function<void(std::unique_ptr<Enemy>, sf::Vector2f)> &callBack) {
    spawnCallback = callBack;
}

void Enemy::attacking(const float& dt) {
    if (isAttacking) {
        attackTimer -= dt;

        // attackTimer говорит о том, когда будет атака( на каком моменте анимации )
        if(typeDamage == TypeDamage::Melee && !hasAttacked && attackTimer <= .1f){
            meleeAttack();
            hasAttacked = true;
        }

        if(typeDamage == TypeDamage::Ranged && !hasAttacked && attackTimer <= .1f){
            spawnProjectile();
            hasAttacked = true;
        }

        if(typeDamage == TypeDamage::Summon && !hasAttacked && attackTimer <= .1f) {
            spawnMobs();
            hasAttacked = true;
        }

        if (attackTimer <= 0.f) {
            isAttacking = false;
            hasAttacked = false;
            attackCooldown = attackCooldownTime;
            timerStayAfterAttack = timeToStayAfterAttack;
        }
    }
    else {
        if (attackCooldown > 0.0f) {
            attackCooldown -= dt;
        }
        if(timerStayAfterAttack > 0.f){
            timerStayAfterAttack -= dt;
        }
    }
}

void Enemy::meleeAttack() const {
    if(getDistanceToPlayer() <= attackRange) player.lock()->takeDamage(damage);
}

float Enemy::getDistanceToPlayer() const {
    sf::Vector2f playerPos = player.lock()->getSprite().getPosition();
    sf::Vector2f enemyPos = currentSprite.getPosition();
    return sqrtf(powf(playerPos.x - enemyPos.x, 2) +
                      powf(playerPos.y - enemyPos.y, 2));
}

void Enemy::attack() {

}

void Enemy::move(const float& dt) {
    sf::Vector2f direction =  player.lock()->getSprite().getPosition() - currentSprite.getPosition();
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.f) {
        direction /= length;
        lastDirection = direction;
        currentSprite.move(direction * speed * dt);
    }
}

void Enemy::update(const float& dt) {

    animController.update(dt);
    updateAnimation();

    if (isDying) {
        if(animController.hasLastFrame()){
            isAlive = false;
        }
    }

    if (isTakingDamage) {
        isShowingHp = true;
        currentState = State::Hurt;
        damageTimer -= dt;
        if (damageTimer <= 0.0f) {
            isTakingDamage = false;
        }
    }

    if(!isTakingDamage && !isAttacking && currentState != State::Dying) {
        if(getDistanceToPlayer() < rangeRadius){
            if(timerStayAfterAttack > 0.f) {
                currentState = State::Idle;
            }
            else if(getDistanceToPlayer() > attackRange){
                currentState = State::Moving;
                move(dt);
            }
            else if(attackCooldown <= 0.f) {
                currentState = State::Attack;
                startAttacking();
            }
            else {
                currentState = State::Idle;
            }
        }
        else {
            currentState = State::Idle;
        }
    }

    collision.update();
    attacking(dt);

    updateShowingHP(dt);
}

void Enemy::updateShowingHP(const float& dt) {
    if(isShowingHp){
        timerToShowHp += dt;
    }

    if(timerToShowHp > fadeOutTime){
        isShowingHp = false;
        timerFadingOut = timeToFadeOut;
        isFading = true;
        timerToShowHp = 0.f;
    }

    if(timerFadingOut > 0.f){
        hpBackgroundShape.setFillColor(sf::Color(0, 0, 0, (timerFadingOut * 255) / timeToFadeOut));
        hpRedShape.setFillColor(sf::Color(255, 0, 0, (timerFadingOut * 255) / timeToFadeOut));

        timerFadingOut -= dt;
    }
    else if(timerFadingOut <= 0.f) {
        isFading = false;
        hpBackgroundShape.setFillColor(sf::Color::Black);
        hpRedShape.setFillColor(sf::Color::Red);
    }


    auto spriteRect = currentSprite.getGlobalBounds();

    hpBackgroundShape.setPosition({spriteRect.position.x + spriteRect.size.x / 2 - hpBackgroundShape.getSize().x / 2, spriteRect.position.y});

    sf::FloatRect shapeRect = {hpBackgroundShape.getPosition(), hpBackgroundShape.getSize()};
    hpRedShape.setPosition({shapeRect.position.x + 2.5f, shapeRect.position.y + 1.f});

    hpRedShape.setSize({health * 65.f / maxHealth, hpRedShape.getSize().y});
}

void Enemy::render(sf::RenderTarget& target) {
    target.draw(currentSprite);
    collision.render(target);
    if(isAlive && (isFading || isShowingHp)){
        target.draw(hpBackgroundShape);
        target.draw(hpRedShape);
    }
}
