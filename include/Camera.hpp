#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include "Tilemap.hpp"

class Camera {
private:
    sf::View camera;
    sf::FloatRect rect;
    sf::Sprite* target;
    const Tilemap& map;

    float smoothSpeed = 0.05f; // i like 0.05
public:
    Camera(sf::Sprite* sprite, const Tilemap& map_): target(sprite), map(map_) {
        camera.setSize({1920, 1080});
        auto bounds = target->getGlobalBounds();
        sf::Vector2f pos = {bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2};
        camera.setCenter(pos);
        rect.position = {pos.x - 300.f, pos.y - 150.f};
        rect.size = {600.f, 300.f};
    }

    void setTarget(sf::Sprite* sprite) {
        target = sprite;
    }

    void update(const float& dt) {
        auto bounds = target->getGlobalBounds();
        sf::Vector2f targetPosCenter = {bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2};

        rect.position = {targetPosCenter.x - 300.f, targetPosCenter.y - 150.f};
        //rect.size = {600.f, 300.f};

        sf::Vector2f oldCameraPos = camera.getCenter();
        sf::Vector2f newPos = oldCameraPos + (targetPosCenter - oldCameraPos) * smoothSpeed * 60.f * dt;
        
        newPos.x = std::clamp(newPos.x, rect.position.x, rect.position.x + rect.size.x);
        newPos.y = std::clamp(newPos.y, rect.position.y, rect.position.y + rect.size.y);

        sf::Vector2f worldSize = {static_cast<float>(map.getWorldSize().x), static_cast<float>(map.getWorldSize().y)};
        worldSize *= map.getTileSize() * map.getMapScale();

        if(worldSize.x <= 1920.f / 2 && worldSize.y <= 1080.f / 2) newPos = {1920.f / 2, 1080.f / 2};
        else {

            newPos.x = std::clamp(newPos.x, 1920.f / 2, worldSize.x - 1920.f / 2);
            newPos.y = std::clamp(newPos.y, 1080.f / 2, worldSize.y - 1080.f / 2);

        }
        camera.setCenter(newPos);
    }

    void applyTo(sf::RenderTarget& target){
        target.setView(camera);
    }
    // ~Camera() {
    //     delete target;
    // }
};

#endif