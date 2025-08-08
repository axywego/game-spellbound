#include <SFML/Graphics.hpp>
#include <iostream>
#include "src/scenes/SceneManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "ndnn", sf::State::Fullscreen);

    window.setMouseCursorVisible(true);
    window.setVerticalSyncEnabled(true);

    sf::Clock clock;

    SceneManager manager(window);

    while(window.isOpen()) {

        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        while (const std::optional event = window.pollEvent()) {
            manager.handleEvent(event);
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        manager.update(dt);

        window.clear();

        manager.render(window);

        window.display();

    }

    return 0;
}