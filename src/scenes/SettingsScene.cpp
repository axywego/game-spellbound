#include "SettingsScene.hpp"

SettingsScene::SettingsScene(sf::RenderWindow& window_, const std::function<void()> &exitCallback):
Scene(window_), backgroundImage(textureBackground),
buttonBack(ResourceManager::getInstance().getTexture("exit_button"), window),
onBackClick(exitCallback), slider(window, {500.f, 500.f}, 300.f, 0.2f) {

    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);

    buttonBack.setPosition({50.f, 50.f});

}

void SettingsScene::load()  {

}

void SettingsScene::update(const float& dt)  {
    if(isTransition) {
        circleScale -= transitionSpeed * dt;

        if(circleScale <= 0.f){
            circleScale = 0.f;
            isTransition = false;
        }
        circle.setScale( {circleScale, circleScale} );
    }
    else {
        buttonBack.update(dt);
    }
}

void SettingsScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    renderTarget.draw(backgroundImage);

    buttonBack.render();
    slider.render();

    if(isTransition) renderTarget.draw(circle);
}

void SettingsScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(buttonBack.isClicked(event)) {
        onBackClick();
    }

    slider.handleInput(event);

    // if(event){
    //     if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
    //         if(key->code == sf::Keyboard::Key::Escape)
    //             onExitClick();
    //         if(key->code == sf::Keyboard::Key::Enter){
    //             onStartClick();
    //         }
    //     }
    // }
}

sf::Vector2f SettingsScene::getCameraCenter() const {
    return view.getCenter();
}