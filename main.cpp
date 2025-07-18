#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/SceneManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "чмо", sf::Style::Default, sf::State::Fullscreen);

    //window.setVerticalSyncEnabled(true); // мб потом убрать хз
    
    // for show fps
    sf::Font font;
    if (!font.openFromFile("fonts/Boblox Fix.otf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    
    sf::Text fpsText(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition({10, 10});

    sf::Clock fpsClock;

    int frameCount = 0;
    float fpsUpdateTime = 0.f;
    float currentFps = 0.f;



    sf::Clock clock;

    SceneManager manager(window);

    while(window.isOpen()) {

        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();




        // calc fps
        // frameCount++;
        // fpsUpdateTime += dt;
        // if (fpsUpdateTime >= 0.25f) { // check fps every 0.25 sec
        //     currentFps = frameCount / fpsUpdateTime;
        //     frameCount = 0;
        //     fpsUpdateTime = 0.f;
            
        //     std::ostringstream ss;
        //     ss << "FPS: " << static_cast<int>(currentFps);
        //     fpsText.setString(ss.str());
        // }




        while (const std::optional event = window.pollEvent())
        {
            // if (event->is<sf::Event::Closed>())
            //     window.close();

            // if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
            //     if(key->code == sf::Keyboard::Key::Escape) {
            //         window.close();
            //     }
            // }
            
            manager.handleEvent(event);
            
        }

        manager.update(dt);

        window.clear();
        
        manager.render(window);

        window.draw(fpsText);

        window.display();

    }
    return 0;
}