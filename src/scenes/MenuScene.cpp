#include "MenuScene.hpp"

MenuScene::MenuScene(sf::RenderWindow& window_, const std::string& name, const std::function<void()> &startCallback,
    const std::function<void()> &settingsCallback, const std::function<void()> &exitCallback):

Scene(window_, name),
backgroundImage(window_, ResourceManager::getInstance().getTexture("anim_background_menu_1")),
startButton(ResourceManager::getInstance().getTexture("anim_start_btn_1"), window),
settingsButton(ResourceManager::getInstance().getTexture("settings_button"), window),
exitButton(ResourceManager::getInstance().getTexture("exit_button"), window),
onStartClick(startCallback),
onSettingsClick(settingsCallback),
onExitClick(exitCallback) {

    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);


    constexpr auto scaleStartButton = sf::Vector2f{0.3f, 0.3f};
    constexpr auto scaleSettingsButton = sf::Vector2f{1.f, 1.f};
    constexpr auto scaleExitButton = sf::Vector2f{1.f, 1.f};

    auto anim1 = Animation::createScaleAnimation(
        scaleStartButton, {scaleStartButton.x + 0.1f, scaleStartButton.y + 0.1f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );
    auto anim2 = Animation::createScaleAnimation(
        scaleSettingsButton, {scaleSettingsButton.x + 0.1f, scaleSettingsButton.y + 0.1f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );
    auto anim3 = Animation::createScaleAnimation(
        scaleExitButton, {scaleExitButton.x + 0.1f, scaleExitButton.y + 0.1f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    sf::Texture texture1 = ResourceManager::getInstance().getTexture("anim_start_btn_1");
    sf::Texture texture2 = ResourceManager::getInstance().getTexture("anim_start_btn_2");
    sf::Texture texture3 = ResourceManager::getInstance().getTexture("anim_start_btn_3");
    auto animation = Animation::createSlideShowAnimation(
        {texture1, texture2, texture3}, 0.75f
    );

    sf::Texture texture4 = ResourceManager::getInstance().getTexture("anim_background_menu_1");
    sf::Texture texture5 = ResourceManager::getInstance().getTexture("anim_background_menu_2");
    sf::Texture texture6 = ResourceManager::getInstance().getTexture("anim_background_menu_3");
    sf::Texture texture7 = ResourceManager::getInstance().getTexture("anim_background_menu_4");

    auto animation_background = Animation::createSlideShowAnimation(
        {texture4, texture5, texture6, texture7}, 1.f
    );

    backgroundImage.setTransform(Transform{{0.f, 0.f}, 0.f, {1.f, 1.f}});
    backgroundImage.addAnimation(UI::Image::TypeAnimation::SlideShow, std::make_pair(animation_background, true));

    // startButton.setTransform(Transform{{50.f, 500.f}, 0.f, {startButton.getCurrentTransform().scale}});
    // startButton.addAnimation(UI::Button::TypeAnimation::Hovered, std::make_pair(anim1, false));
    startButton.setTransform(Transform{{50.f, 500.f}, 0.f, {0.3f, 0.3f}});
    startButton.addAnimation(UI::Button::TypeAnimation::Hovered, std::make_pair(animation, true));

    settingsButton.setTransform(Transform{{50.f, 600.f}, 0.f, {settingsButton.getCurrentTransform().scale}});
    settingsButton.addAnimation(UI::Button::TypeAnimation::Hovered, std::make_pair(anim2, false));

    exitButton.setTransform(Transform{sf::Vector2f{50.f, 700.f}, 0.f, exitButton.getCurrentTransform().scale});
    exitButton.addAnimation(UI::Button::TypeAnimation::Hovered, std::make_pair(anim3, false));
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
        backgroundImage.update(dt);

        startButton.update(dt);
        settingsButton.update(dt);
        exitButton.update(dt);
    }
}

void MenuScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    backgroundImage.render();

    startButton.render();
    settingsButton.render();
    exitButton.render();

    if(isTransition) renderTarget.draw(circle);
}

void MenuScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(startButton.isClicked(event)) {
        onStartClick();
    }

    if (settingsButton.isClicked(event)) {
        onSettingsClick();
    }

    if(exitButton.isClicked(event)) {
        onExitClick();
    }

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

sf::Vector2f MenuScene::getCameraCenter() const {
    return view.getCenter();
}