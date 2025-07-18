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
    
public:
    Player(const std::string& texturePath, const std::shared_ptr<World>& world_): Mob(sf::Texture(texturePath), world_, sf::FloatRect {{40.f, 44.f}, {64.f, 52.f}}),
     arrowTexture("real img/1 Characters/Other/Arrow.png"), fireballTexture("real img/1 Characters/Other/Fireball.png") { }

    void update(const float& dt) override {
        collision.update();
        animController.update(dt);
        attacking(dt);
        if(!isAttacking) move(dt);
        updateAnimation();
        //updateProjectiles(dt);
    }

    const std::vector<std::unique_ptr<Projectile>>& getProjectiles() const {
        return projectiles; 
    }

    void startAttacking() override {
        if (!isAttacking && attackCooldown <= 0.0f) {
            isAttacking = true; 
            attackTimer = attackAnimationTime; // Инициализируем таймер
            attackCooldown = attackCooldownTime;
            currentState = State::Attack;
        }
    }

    virtual void spawnProjectile() = 0;

    void attacking(const float& dt) override {
        if (isAttacking) {
            attackTimer -= dt;

            if(animController.hasPenultFrame() && !hasSpawnedProjectile){
                spawnProjectile();
                hasSpawnedProjectile = true;
            }
            if (attackTimer <= 0.0f) {
                isAttacking = false;
                currentState = State::Idle;
                hasSpawnedProjectile = false;
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
        
        const auto incorrectDir = collision.checkCollisions(*world.lock());

        if(dir.x != 0 && dir.y != 0){
            dir.x *= std::sqrt(2) / 2;
            dir.y *= std::sqrt(2) / 2;
        }

        if (incorrectDir.x != 0.f || incorrectDir.y != 0.f) {
            if (incorrectDir.x != 0.f && std::signbit(incorrectDir.x) == std::signbit(dir.x)) {
                dir.x = 0.f;
            }
            if (incorrectDir.y != 0.f && std::signbit(incorrectDir.y) == std::signbit(dir.y)) {
                dir.y = 0.f;
            }
        }

        currentState = State::Moving;

        currentSprite.move(dir * speed * dt);
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

    void playMovementAnimation() override {
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
        
        if (dir.x != 0 || dir.y != 0) {
            lastDirection = dir;
            
            if (std::abs(dir.y) > std::abs(dir.x)) {
                if (dir.y < 0) animController.play("walk_up");
                else animController.play("walk_down");
            } else {
                if (dir.x < 0) animController.play("walk_left");
                else animController.play("walk_right");
            }
        } else {
            if (lastDirection.y < 0) animController.play("idle_up");
            else if (lastDirection.y > 0) animController.play("idle_down");
            else if (lastDirection.x < 0) animController.play("idle_left");
            else animController.play("idle_right");
        }
    }

    // void updateAnimation() override {
    //     if (isAttacking) {
    //         if (lastDirection.y < 0) animController.play("attack_up");
    //         else if (lastDirection.y > 0) animController.play("attack_down");
    //         else if (lastDirection.x < 0) animController.play("attack_left");
    //         else animController.play("attack_right");
    //         return;
    //     }

    //     sf::Vector2f dir{0.f, 0.f};

    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dir.y--;
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dir.y++;
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir.x--;
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir.x++;

    //     if (dir.x != 0 || dir.y != 0) {
    //         lastDirection = dir;
            
    //         if (std::abs(dir.y) > std::abs(dir.x)) {
    //             if (dir.y < 0) animController.play("walk_up");
    //             else animController.play("walk_down");
    //         } else {
    //             if (dir.x < 0) animController.play("walk_left");
    //             else animController.play("walk_right");
    //         }
    //     } else {
            
    //         if (lastDirection.y < 0) animController.play("idle_up");
    //         else if (lastDirection.y > 0) animController.play("idle_down");
    //         else if (lastDirection.x < 0) animController.play("idle_left");
    //         else animController.play("idle_right");
    //     }
    // }

    void render(sf::RenderTarget& target) override {
        target.draw(currentSprite);

        for(auto& projectile : projectiles) {
            projectile->render(target);
        }
        //collision.render(target);
    }

};


class Archer : public Player {  
public:
    Archer(const std::shared_ptr<World>& world): Player("real img/1 Characters/1/1.png", world) {
        speed = 450.f;
        health = 4;
        attackCooldownTime = 0.5f;
        initAnimations();
    }

    void spawnProjectile() override {
        sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
        spawnPos.y -= 20.f;
        //spawnPos.x += currentSprite.getGlobalBounds().size.x / 2;
        //spawnPos.y += currentSprite.getGlobalBounds().size.y / 2;
        
        projectiles.push_back(
            std::make_unique<Arrow>(
                arrowTexture, 
                world,
                spawnPos, 
                lastDirection
            )
        );
    }

    void attack() override {
        // later...
        startAttacking();
    }
};

class Knight : public Player {
public:
    Knight(const std::shared_ptr<World>& world): Player("real img/1 Characters/2/2.png", world) {
        speed = 400.f;
        health = 5;
        attackCooldownTime = 0.25f;
        initAnimations();
    }

    void spawnProjectile() override {}

    void attack() override {
        // later...
        startAttacking();
    }
};

class Mage : public Player {
public:
    Mage(const std::shared_ptr<World>& world): Player("real img/1 Characters/3/3.png", world) {
        speed = 425.f;
        health = 3;
        attackCooldownTime = 0.75f;
        initAnimations();
    }

    void spawnProjectile() override {
        sf::Vector2f spawnPos = currentSprite.getPosition() + lastDirection * 50.f;
        spawnPos.y -= 20.f;
        //spawnPos.x += currentSprite.getGlobalBounds().size.x / 2;
        //spawnPos.y += currentSprite.getGlobalBounds().size.y / 2;
        
        projectiles.push_back(
            std::make_unique<Fireball>(
                fireballTexture, 
                world,
                spawnPos, 
                lastDirection
            )
        );
    }

    void attack() override {
        // later...
        startAttacking();
    }
};

class PlayerFactory {
public:
    static std::unique_ptr<Player> create(PlayerClass type, const std::shared_ptr<World>& world) {
        switch(type) {
            case PlayerClass::Archer: return std::make_unique<Archer>(world);
            case PlayerClass::Knight: return std::make_unique<Knight>(world);
            case PlayerClass::Mage: return std::make_unique<Mage>(world);
            default: return nullptr;
        }
    }
};

#endif