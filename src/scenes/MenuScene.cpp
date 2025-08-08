#include "MenuScene.hpp"

MenuScene::MenuScene(sf::RenderWindow& window_, std::function<void()> startCallback, std::function<void()> exitCallback):
Scene(window_), textureBackground("img/backgroundMenu.png"), backgroundImage(textureBackground),
startButton(sf::Texture("img/startBtn.png"), window),
settingsButton(sf::Texture("img/settingsBtn.png"), window),
exitButton(sf::Texture("img/exitBtn.png"), window),
onStartClick(startCallback),
onExitClick(exitCallback) {
    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);

    startButton.setPosition({50.f, 500.f});
    settingsButton.setPosition({50.f, 600.f});
    exitButton.setPosition({50.f, 700.f});
}

void MenuScene::load()  {

}

void MenuScene::update(const float& dt)  {
    if(isTransition) {
        circleScale -= transitionSpeed * dt;

        if(circleScale <= 0.f){
            circleScale = 0.f;
            isTransition = false;
        }
        circle.setScale( {circleScale, circleScale} );

    }

}

void MenuScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    renderTarget.draw(backgroundImage);

    startButton.render(renderTarget);
    settingsButton.render(renderTarget);
    exitButton.render(renderTarget);

    if(isTransition) renderTarget.draw(circle);
}

void MenuScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(startButton.isClicked(event)) {
        onStartClick();
    }

    if(exitButton.isClicked(event)) {
        onExitClick();
    }

    if(event){
        if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
            if(key->code == sf::Keyboard::Key::Escape)
                onExitClick();
            if(key->code == sf::Keyboard::Key::Enter){
                onStartClick();
            }
        }
    }
}

sf::Vector2f MenuScene::getCameraCenter() const {
    return view.getCenter();
}