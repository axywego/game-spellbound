#include "Enemy.hpp"

Enemy::Enemy(const sf::Texture& texture, const Tilemap& map_, const sf::FloatRect& collisionRect_,
    const std::weak_ptr<Player> &player_)
:
Mob(texture, map_, collisionRect_), player(player_), raycastView(currentSprite, map_, player_.lock()->getSprite()) {
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
    if(getDistanceToPlayer() <= attackRange) player.lock()->takeDamage(*stats.getCurrentValue(StatType::Damage).value());
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
        currentSprite.move(direction * *stats.getCurrentValue(StatType::Speed).value() * dt);
    }
}

// void Enemy::update(const float& dt) {
//
//     animController.update(dt);
//     updateAnimation();
//
//     if (isDying) {
//         if(animController.hasLastFrame()){
//             isAlive = false;
//         }
//     }
//
//     if (isTakingDamage) {
//         isShowingHp = true;
//         currentState = State::Hurt;
//         damageTimer -= dt;
//         if (damageTimer <= 0.0f) {
//             isTakingDamage = false;
//         }
//     }
//
//     attacking(dt);
//
//     if(!isTakingDamage && !isAttacking && currentState != State::Dying) {
//         const float distance = getDistanceToPlayer();
//
//         if(raycastView.isInView(player.lock()->getCollisionRect()) && distance > attackRange) {
//             currentState = State::Moving;
//             move(dt);
//         }
//         else if(attackCooldown <= 0.f && timerStayAfterAttack <= 0.f) {
//             currentState = State::Attack;
//             startAttacking();
//         }
//         else {
//             currentState = State::Idle;
//         }
//     }
//
//     raycastView.update();
//
//     collision.update();
//
//     updateShowingHP(dt);
// }

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

    if(!isTakingDamage && !isAttacking && !isDying) {
        bool isInView = raycastView.isInView(player.lock()->getSprite().getGlobalBounds());

        if (timerStayAfterAttack > 0.f || !isInView) {
            currentState = State::Idle;
        }
        else if(isInView && getDistanceToPlayer() > attackRange){
            currentState = State::Moving;
            move(dt);
        }
        else if(attackCooldown <= 0.f && timerStayAfterAttack <= 0.f) {
            currentState = State::Attack;
            startAttacking();
        }
        else {
            currentState = State::Idle;
        }
    }

    raycastView.update();

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

    hpRedShape.setSize({
        *stats.getCurrentValue(StatType::Health).value() * 65.f / *stats.getCurrentValue(StatType::MaxHealth).value(),
        hpRedShape.getSize().y
    });
}

void Enemy::render(sf::RenderTarget& target) {
    target.draw(currentSprite);
    collision.render(target);
    if(isAlive && (isFading || isShowingHp)){
        target.draw(hpBackgroundShape);
        target.draw(hpRedShape);
    }
    raycastView.render(target);
}
