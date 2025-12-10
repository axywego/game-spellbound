#include "SettingsScene.hpp"

#include "../core/SaveSystem.hpp"
#include "../core/SettingsManager.hpp"

SettingsScene::SettingsScene(sf::RenderWindow& window_, const std::string& name, const std::function<void()>& backCallback, const std::function<void(float)> &setMusicVolumeCallback):
Scene(window_, name),
backgroundImage(window, ResourceManager::getInstance().getTexture("background_menu")),
sliderMusic(window, 300.f, 0.2f),
sliderSound(window, 300.f, 0.2f),
checkbox(window, false),
sliderMusicText(window, "Music Volume"),
sliderSoundText(window, "Sound Volume"),
checkboxText(window, "Vertical Sync"),
backButton(ResourceManager::getInstance().getTexture("back_button"), window),
onBackClick(backCallback),
setMusicVolumeCallback(setMusicVolumeCallback) {

    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    circle.setRadius(1200.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
    circle.setPosition(target);

    sliderMusic.setTransform(Transform{
        {100.f, 400.f},
        0.f,
        sliderMusic.getCurrentTransform().scale
    });

    sliderSound.setTransform(Transform{
        {500.f, 400.f},
        0.f,
        sliderMusic.getCurrentTransform().scale
    });

    //sliderMusic.setValue(1.f);

    auto animationScaleExitButton = Animation::createScaleAnimation(
        {0.4f, 0.4f}, {0.44f, 0.44f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    backButton.setTransform(Transform{
            sf::Vector2f{50.f, 50.f},
            0.f,
            {0.4f, 0.4f}
        }
    );

    backButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationScaleExitButton, false)
    );

    checkbox.setTransform(Transform{
        {100.f, 600.f},
        0.f,
        {5.f, 5.f}
    });

    const auto scaleCheckBox = checkbox.getCurrentTransform().scale;

    auto animCheckBox = Animation::createScaleAnimation(
        scaleCheckBox, {scaleCheckBox.x + 1.f, scaleCheckBox.y + 1.f}, std::function(Animation::Easing::easeOutCubic), 0.1f);
    checkbox.addAnimation(UI::UIObject::TypeAnimation::Hovered, std::make_pair(animCheckBox, false));

    checkbox.setFunc([this](const bool& checked) {
        window.setVerticalSyncEnabled(checked);
    });

    sliderMusic.setValue(SettingsManager::getInstance().getMusicValue());
    sliderSound.setValue(SettingsManager::getInstance().getSoundValue());
    checkbox.setValue(SettingsManager::getInstance().getIsVerticalSync());

    sliderMusicText.setColor(sf::Color::White);
    sliderMusicText.setPosition({80.f, 300.f});
    sliderMusicText.setOutlineThickness(3.f);
    sliderMusicText.setOutlineColor(sf::Color::Black);
    sliderMusicText.setScale({1.5f, 1.5f});

    sliderSoundText.setColor(sf::Color::White);
    sliderSoundText.setPosition({480.f, 300.f});
    sliderSoundText.setOutlineThickness(3.f);
    sliderSoundText.setOutlineColor(sf::Color::Black);
    sliderSoundText.setScale({1.5f, 1.5f});

    checkboxText.setColor(sf::Color::White);
    checkboxText.setPosition({80.f, 500.f});
    checkboxText.setOutlineThickness(3.f);
    checkboxText.setOutlineColor(sf::Color::Black);
    checkboxText.setScale({1.5f, 1.5f});
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
        backButton.update(dt);
        if (sliderMusic.isInChange()) {
            setMusicVolumeCallback(sliderMusic.getValue() * 100.f);
        }
        checkbox.update(dt);
        backButton.update(dt);
    }
}

void SettingsScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    backgroundImage.render();

    backButton.render();
    sliderMusic.render();
    sliderSound.render();
    checkbox.render();

    sliderMusicText.render();
    sliderSoundText.render();
    checkboxText.render();

    if(isTransition) renderTarget.draw(circle);
}

void SettingsScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(backButton.isClicked(event)) {
        SaveSystem::getInstance().saveSettings();
        onBackClick();
    }

    checkbox.handleInput(event);
    SettingsManager::getInstance().setIsVerticalSync(checkbox.getValue());

    sliderMusic.handleInput(event);
    SettingsManager::getInstance().setMusicValue(sliderMusic.getValue());

    sliderSound.handleInput(event);
    SettingsManager::getInstance().setSoundValue(sliderSound.getValue());
}

sf::Vector2f SettingsScene::getCameraCenter() const {
    return view.getCenter();
}