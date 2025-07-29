#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/SceneManager.hpp"

#include "include/WorldGenerator.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "чмо", sf::Style::Default, sf::State::Fullscreen);

    //window.setVerticalSyncEnabled(true); // мб потом убрать хз

    sf::Clock clock;

    SceneManager manager(window);

    while(window.isOpen()) {

        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

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