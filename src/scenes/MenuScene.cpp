#include "MenuScene.hpp"

#include "../core/SaveLoaderManager.hpp"

MenuScene::MenuScene(sf::RenderWindow& window_,
    const std::string& name,
    const std::function<void()> &startCallback,
    const std::function<void()> &continueCallback,
    const std::function<void()> &settingsCallback,
    const std::function<void()> &exitCallback):
Scene(window_, name),
backgroundImage(window_, ResourceManager::getInstance().getTexture("anim_background_menu_1")),
startButton(ResourceManager::getInstance().getTexture("anim_start_btn_1"), window),
continueButton(ResourceManager::getInstance().getTexture("continue_btn"), window),
settingsButton(ResourceManager::getInstance().getTexture("anim_settings_btn_1"), window),
exitButton(ResourceManager::getInstance().getTexture("anim_exit_btn_1"), window),
onStartClick(startCallback),
onContinueClick(continueCallback),
onSettingsClick(settingsCallback),
onExitClick(exitCallback) {

    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);

    auto animationScaleStartButton = Animation::createScaleAnimation(
        {0.3f, 0.3f}, {0.34f, 0.34f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    auto animationScaleSettingsButton = Animation::createScaleAnimation(
        {0.3f, 0.3f}, {0.34f, 0.34f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    auto animationScaleExitButton = Animation::createScaleAnimation(
        {0.3f, 0.3f}, {0.34f, 0.34f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    sf::Texture textureAnimStartBtn1 = ResourceManager::getInstance().getTexture("anim_start_btn_1");
    sf::Texture textureAnimStartBtn2 = ResourceManager::getInstance().getTexture("anim_start_btn_2");
    sf::Texture textureAnimStartBtn3 = ResourceManager::getInstance().getTexture("anim_start_btn_3");
    auto animationStartBtn = Animation::createSlideShowAnimation(
        {textureAnimStartBtn1, textureAnimStartBtn2, textureAnimStartBtn3}, 0.75f
    );

    sf::Texture textureAnimSettingsBtn1 = ResourceManager::getInstance().getTexture("anim_settings_btn_1");
    sf::Texture textureAnimSettingsBtn2 = ResourceManager::getInstance().getTexture("anim_settings_btn_2");
    sf::Texture textureAnimSettingsBtn3 = ResourceManager::getInstance().getTexture("anim_settings_btn_3");
    auto animationSettingsBtn = Animation::createSlideShowAnimation(
        {textureAnimSettingsBtn1, textureAnimSettingsBtn2, textureAnimSettingsBtn3}, 0.75f
    );

    sf::Texture textureAnimExitBtn1 = ResourceManager::getInstance().getTexture("anim_exit_btn_1");
    sf::Texture textureAnimExitBtn2 = ResourceManager::getInstance().getTexture("anim_exit_btn_2");
    sf::Texture textureAnimExitBtn3 = ResourceManager::getInstance().getTexture("anim_exit_btn_3");
    auto animationExitBtn = Animation::createSlideShowAnimation(
        {textureAnimExitBtn1, textureAnimExitBtn2, textureAnimExitBtn3}, 0.75f
    );

    // sf::Texture textureBackMenu1 = ResourceManager::getInstance().getTexture("anim_background_menu_1");
    // sf::Texture textureBackMenu2 = ResourceManager::getInstance().getTexture("anim_background_menu_2");
    // sf::Texture textureBackMenu3 = ResourceManager::getInstance().getTexture("anim_background_menu_3");
    //
    // auto animationBackground = Animation::createSlideShowAnimation(
    //     {textureBackMenu1, textureBackMenu2}, 0.7f
    // );
    //
    // Setting backgroundImage
    // backgroundImage.setTransform(Transform{
    //         {0.f, 0.f},
    //         0.f,
    //         {1.f, 1.f}
    //     }
    // );
    // backgroundImage.addAnimation(
    //     UI::Image::TypeAnimation::SlideShow,
    //     std::make_pair(animationBackground, true)
    // );

    // Setting startButton
    startButton.setTransform(Transform{
            {700.f, 400.f},
            -8.f,
            {0.3f, 0.3f}
        }
    );
    startButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationStartBtn, true), std::make_pair(animationScaleStartButton, false)
    );

    // Setting continueButton
    continueButton.setTransform(Transform{
            {50.f, 50.f},
            0.f,
            {1.f, 1.3f}
        }
    );
    // continueButton.addAnimation(
    //     UI::Button::TypeAnimation::Hovered,
    //     std::make_pair(animationStartBtn, true), std::make_pair(animationScaleStartButton, false)
    // );

    // Setting settingsButton
    settingsButton.setTransform(Transform{
            {720.f, 550.f},
            -8.f,
            {0.3f, 0.3f}
        }
    );
    settingsButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationSettingsBtn, true), std::make_pair(animationScaleSettingsButton, false)
    );

    // Setting exitButton
    exitButton.setTransform(Transform{
            sf::Vector2f{740.f, 700.f},
            -8.f,
            {0.3f, 0.3f}
        }
    );
    exitButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationExitBtn, true), std::make_pair(animationScaleExitButton, false)
    );
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
        continueButton.update(dt);
        settingsButton.update(dt);
        exitButton.update(dt);
    }
}

void MenuScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    backgroundImage.render();

    startButton.render();
    continueButton.render();
    settingsButton.render();
    exitButton.render();

    if(isTransition) renderTarget.draw(circle);
}

void MenuScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(startButton.isClicked(event)) {
        onStartClick();
    }

    if (continueButton.isClicked(event)) {
        onContinueClick();
    }

    if (settingsButton.isClicked(event)) {
        onSettingsClick();
    }

    if(exitButton.isClicked(event)) {
        onExitClick();
    }
}

sf::Vector2f MenuScene::getCameraCenter() const {
    return view.getCenter();
}