#include "MenuScene.hpp"

MenuScene::MenuScene(sf::RenderWindow& window_, const std::function<void()> &startCallback,
    const std::function<void()> &exitCallback):
Scene(window_), backgroundImage(textureBackground),
startButton(ResourceManager::getInstance().getTexture("start_button"), window),
settingsButton(ResourceManager::getInstance().getTexture("settings_button"), window),
exitButton(ResourceManager::getInstance().getTexture("exit_button"), window),
onStartClick(startCallback),
onExitClick(exitCallback) {

    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);

    auto anim1 = Animation::createScaleAnimation(
        1.1f, std::function<float(float)>{Animation::Easing::easeInOutQuad}, 0.2f
    );
    auto anim2 = Animation::createScaleAnimation(
        1.1f, std::function<float(float)>{Animation::Easing::easeInOutQuad}, 0.2f
    );
    auto anim3 = Animation::createScaleAnimation(
        1.1f, std::function<float(float)>{Animation::Easing::easeInOutQuad}, 0.2f
    );

    startButton.setPosition({50.f, 500.f});
    startButton.addAnimation(Button::State::Hovered, std::move(anim1));

    settingsButton.setPosition({50.f, 600.f});
    settingsButton.addAnimation(Button::State::Hovered, std::move(anim2));

    exitButton.setPosition({50.f, 700.f});
    exitButton.addAnimation(Button::State::Hovered, std::move(anim3));
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
    else {
        startButton.update(dt);
        settingsButton.update(dt);
        exitButton.update(dt);
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