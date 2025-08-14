#include "SettingsScene.hpp"

SettingsScene::SettingsScene(sf::RenderWindow& window_, const std::function<void()>& backCallback):
Scene(window_), backgroundImage(textureBackground),
slider(window, {500.f, 500.f}, 300.f, 0.2f),
checkbox(window, {600.f, 900.f}, false),
buttonBack(ResourceManager::getInstance().getTexture("exit_button"), window),
onBackClick(backCallback) {

    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);

    buttonBack.setPosition({50.f, 50.f});

    auto animCheckBox = Animation::createScaleAnimation(
        6.f, std::function(Animation::Easing::easeOutCubic), 0.1f
    );
    checkbox.addAnimation(UI::UIObject::TypeAnimation::Hovered, std::move(animCheckBox));

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
        checkbox.update(dt);
        buttonBack.update(dt);
    }
}

void SettingsScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    renderTarget.draw(backgroundImage);

    buttonBack.render();
    slider.render();
    checkbox.render();

    if(isTransition) renderTarget.draw(circle);
}

void SettingsScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(buttonBack.isClicked(event)) {
        onBackClick();
    }

    checkbox.handleInput(event);

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