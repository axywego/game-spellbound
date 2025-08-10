#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include "../mob/Mob.hpp"
#include "../player/Player.hpp"
#include "../../core/Raycast.hpp"
#include <functional>


enum class EnemyClass {
    Rat, Goblin, Demon, Shaman
};

class Enemy: public Mob {
private:
    bool isMoving{};

    bool isFading = false;
    bool isShowingHp = false;
    float timerToShowHp{0.f};
    float fadeOutTime{3.f};
    float timeToFadeOut{0.5f};
    float timerFadingOut{0.f};

    sf::RectangleShape hpRedShape;
    sf::RectangleShape hpBackgroundShape;

protected:

    Raycast raycastView;

    std::function<void(std::unique_ptr<Enemy>, sf::Vector2f)> spawnCallback;

    std::weak_ptr<Player> player;
    float rangeRadius{};
    float attackRange{};
public:
    Enemy(const sf::Texture& texture, const Tilemap& map_, const sf::FloatRect& collisionRect_, const std::weak_ptr<Player> &player_);

    void setSpawnCallback(const std::function<void(std::unique_ptr<Enemy>, sf::Vector2f)> &callBack);

    virtual void spawnProjectile() = 0;
    virtual void spawnMobs() = 0;

    void attacking(const float& dt) override;

    void meleeAttack() const;

    float getDistanceToPlayer() const;

    void attack() override ;

    void move(const float& dt) override ;

    void update(const float& dt) override ;

    void updateShowingHP(const float& dt);

    void render(sf::RenderTarget& target) override ;
};