#include <SFML/Graphics.hpp>
#include <iostream>
#include "src/core/ResourceManager.hpp"
#include "src/core/SaveSystem.hpp"
#include "src/core/SettingsManager.hpp"
#include "src/scenes/SceneManager.hpp"
#include "src/core/UI/HUD.hpp"

// class FPSCounter {
// private:
//     sf::Clock clock;
//     unsigned int frameCount = 0;
//     float updateTime = 0.5f;
//     float timeSinceLastUpdate = 0.f;
//     float currentFPS = 0.f;
//
// public:
//     FPSCounter() = default;
//
//     void update(float dt) {
//         frameCount++;
//         timeSinceLastUpdate += dt;
//
//         if (timeSinceLastUpdate >= updateTime) {
//             currentFPS = frameCount / timeSinceLastUpdate;
//             frameCount = 0;
//             timeSinceLastUpdate = 0.f;
//
//             std::ostringstream ss;
//             ss << "FPS: " << static_cast<int>(currentFPS);
//             std::cout << ss.str() << std::endl;
//         }
//     }
// };

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "aga", sf::State::Fullscreen);

    window.setMouseCursorVisible(true);

    SaveSystem::getInstance().init();
    window.setVerticalSyncEnabled(SettingsManager::getInstance().getIsVerticalSync());

    MusicManager::setBasePath("../resources/music/");
    MusicManager::registerMusic("main", "1.wav");

    UI::HUD::getInstance().init(window);

    sf::Clock clock;

    SceneManager manager(window);

    //FPSCounter fpsCounter;

    while(window.isOpen()) {

        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        //fpsCounter.update(dt);

        while (const std::optional event = window.pollEvent()) {
            manager.handleEvent(event);
        }

        manager.update(dt);

        window.clear();

        manager.render(window);

        window.display();

    }

    return 0;
}