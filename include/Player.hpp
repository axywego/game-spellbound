#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Mob.hpp"
#include <cmath>
#include "Inventory.hpp"
#include "Projectile.hpp"

enum class PlayerClass {
    Archer, Knight, Mage
};

class Player : public Mob {
protected:
    Inventory inventory;

    bool hasSpawnedProjectile = false;

    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Texture arrowTexture;
    sf::Texture fireballTexture;

    sf::FloatRect attackArea;
    
public:
    Player(const std::string& texturePath, Tilemap& map_): Mob(sf::Texture(texturePath), map_, sf::FloatRect {{40.f, 44.f}, {64.f, 52.f}}),
     arrowTexture("real img/1 Characters/Other/Arrow.png"), fireballTexture("real img/1 Characters/Other/Fireball.png") { }

    void update(const float& dt) override {
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

        // collision.update();
        // attacking(dt);
        // if(!isAttacking) move(dt);
        animController.update(dt);
        updateAnimation();
    }

    const std::vector<std::unique_ptr<Projectile>>& getProjectiles() const {
        return projectiles; 
    }

    virtual void spawnProjectile() = 0;
    virtual void checkMelee() = 0;

    void attacking(const float& dt) override {
        if (isAttacking) {
            attackTimer -= dt;

            if(typeDamage == TypeDamage::Melee && animController.hasPenultFrame() && !hasDealtDamage){
                hasDealtDamage = true;
                checkMelee();
            }
            else if(typeDamage == TypeDamage::Ranged && animController.hasPenultFrame() && !hasSpawnedProjectile) {
                hasSpawnedProjectile = true;
                if(mana - manaCost >= 0.f){
                    mana -= manaCost;
                    timerDelayMana = delayBeforeUpMana;
                    spawnProjectile();
                }
            }

            if (attackTimer <= 0.0f) {
                isAttacking = false;
                hasSpawnedProjectile = false;
                hasDealtDamage = false;
                hasMeleeAttacked = false;
                attackCooldown = attackCooldownTime;
                currentState = State::Idle;
            }
        }
        
        if (attackCooldown > 0.0f) {
            attackCooldown -= dt;
        }
    }

    // move player
    void move(const float& dt) override {
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
            dir.x *= std::sqrt(2) / 2;
            dir.y *= std::sqrt(2) / 2;
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

    void updateProjectiles(float dt) {
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

    std::optional<sf::FloatRect> getAttackArea() {
        if(!hasMeleeAttacked) {
            hasMeleeAttacked = true;
            return attackArea;
        }
        else return std::nullopt;
    }

    void render(sf::RenderTarget& target) override {
        target.draw(currentSprite);

        for(auto& projectile : projectiles) {
            projectile->render(target);
        }

        if(attackArea.position.x != 0){
            sf::RectangleShape shape;
            shape.setPosition(attackArea.position);
            shape.setSize(attackArea.size);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
            target.draw(shape);
        }
        //collision.render(target);
    }

};


class Archer : public Player {  
public:
    Archer(Tilemap& map): Player("real img/1 Characters/1/1.png", map) {
        speed = 450.f;
        maxHealth = 4.f;
        health = maxHealth;
        attackCooldownTime = 0.5f;
        typeDamage = TypeDamage::Ranged;
        damage = 1.5f;
    }

    void spawnProjectile() override {
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

    void checkMelee() override {}

    void attack() override {
        // later...
        startAttacking();
    }
};

class Knight : public Player {
private:
    void calculateAttackArea() {
        attackArea = currentSprite.getGlobalBounds();

        if (lastDirection.x > 0) {
            attackArea.position.x += 100.f;
            attackArea.position.y += 20.f;
            attackArea.size.x = 50.f;
            attackArea.size.y = 70.f;
        } 
        else if (lastDirection.x < 0) {
            attackArea.position.y += 20.f;
            attackArea.size.x = 50.f;
            attackArea.size.y = 70.f;
            
        }
        else if (lastDirection.y > 0) {
            attackArea.position.x += 40.f;
            attackArea.position.y += 90.f;
            attackArea.size.x = 60.f;
            attackArea.size.y = 40.f;
        }
        else {
            attackArea.position.x += 40.f;
            attackArea.size.x = 60.f;
            attackArea.size.y = 40.f;
        }
    }
public:
    Knight(Tilemap& map): Player("real img/1 Characters/2/2.png", map) {
        speed = 400.f;
        maxHealth = 5.f;
        health = maxHealth;
        attackCooldownTime = 0.25f;
        typeDamage = TypeDamage::Melee;
        damage = 2.5f;
    }

    void spawnProjectile() override {}

    void checkMelee() override {
        calculateAttackArea();
        std::cout << "da\n";
        //gameWorld.handleAttackEnemy(attackArea, *this);
    }

    void attack() override {
        // later...
        startAttacking();
    }
};

class Mage : public Player {
public:
    Mage(Tilemap& map): Player("real img/1 Characters/3/3.png", map) {
        speed = 425.f;
        maxHealth = 3.f;
        health = maxHealth;
        //attackCooldownTime = 0.75f;
        attackCooldownTime = 0.4f;
        typeDamage = TypeDamage::Ranged;
        damage = 2.f;
        maxMana = 4.f;
        mana = maxMana;
        manaCost = 0.5f;

        timeToUpMana = 0.5f;
        manaToUpPerTime = 0.25f;
    }

    void spawnProjectile() override {
        sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
        spawnPos.x += 20.f;
        spawnPos.y -= 20.f;
        
        projectiles.push_back(
            std::make_unique<Fireball>(
                fireballTexture,
                map,
                spawnPos,
                lastDirection,
                damage
            )
        );
    }

    void checkMelee() override {}

    void attack() override {
        // later...
        startAttacking();
    }
};

class PlayerFactory {
public:
    static std::unique_ptr<Player> create(PlayerClass type, Tilemap& map) {
        switch(type) {
            case PlayerClass::Archer: return std::make_unique<Archer>(map);
            case PlayerClass::Knight: return std::make_unique<Knight>(map);
            case PlayerClass::Mage: return std::make_unique<Mage>(map);
            default: return nullptr;
        }
    }
};

#endif