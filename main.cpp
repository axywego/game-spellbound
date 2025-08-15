#include <SFML/Graphics.hpp>
#include <iostream>
#include "src/core/ResourceManager.hpp"
#include "src/scenes/SceneManager.hpp"
#include "src/core/UI/HUD.hpp"

class FPSCounter {
private:
    sf::Text text;
    sf::Font font;
    sf::Clock clock;
    unsigned int frameCount = 0;
    float updateTime = 0.5f; // Обновлять FPS каждые 0.5 секунды
    float timeSinceLastUpdate = 0.f;
    float currentFPS = 0.f;

public:
    FPSCounter(): font("../resources/fonts/Cafe24PROUP.ttf"), text(font) {
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition({10,10});
        text.setStyle(sf::Text::Bold);
    }

    void update(float dt) {
        frameCount++;
        timeSinceLastUpdate += dt;

        if (timeSinceLastUpdate >= updateTime) {
            currentFPS = frameCount / timeSinceLastUpdate;
            frameCount = 0;
            timeSinceLastUpdate = 0.f;

            std::ostringstream ss;
            ss << "FPS: " << static_cast<int>(currentFPS);
            std::cout << ss.str() << std::endl;
            text.setString(ss.str());
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(text);
    }
};


int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "ndnn", sf::State::Fullscreen);

    window.setMouseCursorVisible(true);

    UI::HUD::getInstance().init(window);

    sf::Clock clock;

    SceneManager manager(window);

    FPSCounter fpsCounter;

    while(window.isOpen()) {

        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        fpsCounter.update(dt);

        //manager.getPlayer().lock()->getSprite().getPosition();

        while (const std::optional event = window.pollEvent()) {
            manager.handleEvent(event);
        }

        manager.update(dt);

        window.clear();

        manager.render(window);

        fpsCounter.draw(window);

        window.display();

    }

    return 0;
}