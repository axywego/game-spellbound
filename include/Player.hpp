#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Mob.hpp"
#include <cmath>
#include "Inventory.hpp"
#include "AnimationController.hpp"
#include "Projectile.hpp"

enum class PlayerClass {
    Archer, Knight, Mage
};

class Player : public Mob {
protected:
    Inventory inventory;
    AnimationController animController;

    bool hasSpawnedProjectile = false;

    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Texture arrowTexture;
    sf::Texture fireballTexture;
    
public:
    Player(const std::string& texturePath, const std::shared_ptr<World>& world_): Mob(sf::Texture(texturePath), world_, sf::FloatRect {{40.f, 44.f}, {64.f, 52.f}}),
     animController(currentSprite), arrowTexture("real img/1 Characters/Other/Arrow.png"), fireballTexture("real img/1 Characters/Other/Fireball.png") { }

    void initAnimations() override {

        std::vector<sf::IntRect> idleDown {
            {{0, 0}, {32, 32}},
            {{32, 0}, {32, 32}},
            {{64, 0}, {32, 32}},
            {{96, 0}, {32, 32}},
        };

        std::vector<sf::IntRect> idleUp {
            {{0, 32}, {32, 32}},
            {{32, 32}, {32, 32}},
            {{64, 32}, {32, 32}},
            {{96, 32}, {32, 32}},
        };

        std::vector<sf::IntRect> idleLeft {
            {{0, 64}, {32, 32}},
            {{32, 64}, {32, 32}},
            {{64, 64}, {32, 32}},
            {{96, 64}, {32, 32}},
        };

        std::vector<sf::IntRect> walkDown {
            {{0, 192}, {32, 32}},
            {{32, 192}, {32, 32}},
            {{64, 192}, {32, 32}},
            {{96, 192}, {32, 32}},
            {{128, 192}, {32, 32}},
            {{160, 192}, {32, 32}},
        };

        std::vector<sf::IntRect> walkUp {
            {{0, 224}, {32, 32}},
            {{32, 224}, {32, 32}},
            {{64, 224}, {32, 32}},
            {{96, 224}, {32, 32}},
            {{128, 224}, {32, 32}},
            {{160, 224}, {32, 32}},
        };

        std::vector<sf::IntRect> walkLeft {
            {{0, 256}, {32, 32}},
            {{32, 256}, {32, 32}},
            {{64, 256}, {32, 32}},
            {{96, 256}, {32, 32}},
            {{128, 256}, {32, 32}},
            {{160, 256}, {32, 32}},
        };

        std::vector<sf::IntRect> hurtDown {
            {{0, 96},{32,32}},
            {{32, 96},{32,32}},
        };

        std::vector<sf::IntRect> hurtUp {
            {{0, 128},{32,32}},
            {{32, 128},{32,32}},
        };

        std::vector<sf::IntRect> hurtLeft {
            {{0, 160},{32,32}},
            {{0, 160},{32,32}},
        };

        std::vector<sf::IntRect> deathDown {
            {{0, 288}, {32, 32}},
            {{32, 288}, {32, 32}},
            {{64, 288}, {32, 32}},
            {{96, 288}, {32, 32}},
            {{128, 288}, {32, 32}},
            {{160, 288}, {32, 32}},
            {{192, 288}, {32, 32}},
            {{224, 288}, {32, 32}},
        };

        std::vector<sf::IntRect> deathUp {
            {{0, 320}, {32, 32}},
            {{32, 320}, {32, 32}},
            {{64, 320}, {32, 32}},
            {{96, 320}, {32, 32}},
            {{128, 320}, {32, 32}},
            {{160, 320}, {32, 32}},
            {{192, 320}, {32, 32}},
            {{224, 320}, {32, 32}},
        };

        std::vector<sf::IntRect> deathLeft {
            {{0, 352}, {32, 32}},
            {{32, 352}, {32, 32}},
            {{64, 352}, {32, 32}},
            {{96, 352}, {32, 32}},
            {{128, 352}, {32, 32}},
            {{160, 352}, {32, 32}},
            {{192, 352}, {32, 32}},
            {{224, 352}, {32, 32}},
        };

        std::vector<sf::IntRect> attackDown {
            {{0, 384}, {32, 32}},
            {{32, 384}, {32, 32}},
            {{64, 384}, {32, 32}},
            {{96, 384}, {32, 32}},
        };

        std::vector<sf::IntRect> attackUp {
            {{0, 416}, {32, 32}},
            {{32, 416}, {32, 32}},
            {{64, 416}, {32, 32}},
            {{96, 416}, {32, 32}},
        };

        std::vector<sf::IntRect> attackLeft {
            {{0, 448}, {32, 32}},
            {{32, 448}, {32, 32}},
            {{64, 448}, {32, 32}},
            {{96, 448}, {32, 32}},
        };

        animController.addAnimation("idle_down", idleDown, 0.1f);
        animController.addAnimation("idle_up", idleUp, 0.1f);
        animController.addAnimation("idle_left", idleLeft, 0.1f);
        animController.addAnimation("idle_right", idleLeft, 0.1f, true, true);

        animController.addAnimation("walk_down", walkDown, 0.1f);
        animController.addAnimation("walk_up", walkUp, 0.1f);
        animController.addAnimation("walk_left", walkLeft, 0.1f);
        animController.addAnimation("walk_right", walkLeft, 0.1f, true, true);

        animController.addAnimation("hurt_down", hurtDown, 0.1f);
        animController.addAnimation("hurt_up", hurtUp, 0.1f);
        animController.addAnimation("hurt_left", hurtLeft, 0.1f);
        animController.addAnimation("hurt_right", hurtLeft, 0.1f, true, true);

        animController.addAnimation("death_down", deathDown, 0.1f);
        animController.addAnimation("death_up", deathUp, 0.1f);
        animController.addAnimation("death_left", deathLeft, 0.1f);
        animController.addAnimation("death_right", deathLeft, 0.1f, true, true);
        
        animController.addAnimation("attack_down", attackDown, 0.1f);
        animController.addAnimation("attack_up", attackUp, 0.1f);
        animController.addAnimation("attack_left", attackLeft, 0.1f);
        animController.addAnimation("attack_right", attackLeft, 0.1f, true, true);
    }

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

    void setWorld(const std::shared_ptr<World>& world_) override {
        world = world_;
    }

    bool isAttackInProgress() const override {
        return isAttacking;
    }

    void startAttacking() override {
        if (!isAttacking && attackCooldown <= 0.0f) {
            isAttacking = true; 
            attackTimer = attackAnimationTime; // Инициализируем таймер
            attackCooldown = attackCooldownTime;
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
                hasSpawnedProjectile = false;
                
                // Здесь можно вызвать метод, который наносит удар,
                // когда анимация дошла до нужного кадра
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

        currentSprite.move(dir * speed * dt);
        std::cout << "player.move = " << dir.x * speed * dt << '\n';
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

    void updateAnimation() override {
        if (isAttacking) {
            if (lastDirection.y < 0) animController.play("attack_up");
            else if (lastDirection.y > 0) animController.play("attack_down");
            else if (lastDirection.x < 0) animController.play("attack_left");
            else animController.play("attack_right");
            return;
        }

        sf::Vector2f dir{0.f, 0.f};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dir.y--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dir.y++;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir.x--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir.x++;

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

    void render(sf::RenderTarget& target) override {
        target.draw(currentSprite);

        for(auto& projectile : projectiles) {
            projectile->render(target);
        }
        //collision.render(target);
    }

    sf::Sprite& getSprite() override {
        return currentSprite;
    }

    void setPosition(const sf::Vector2f& pos) override {
        currentSprite.setPosition(pos);
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