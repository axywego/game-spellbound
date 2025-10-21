#include "SettingsScene.hpp"

#include "../core/SaveSystem.hpp"
#include "../core/SettingsManager.hpp"

SettingsScene::SettingsScene(sf::RenderWindow& window_, const std::string& name, const std::function<void()>& backCallback, const std::function<void(float)> &setMusicVolumeCallback):
Scene(window_, name), backgroundImage(textureBackground),
sliderMusic(window, 300.f, 0.2f),
checkbox(window, false),
buttonBack(ResourceManager::getInstance().getTexture("exit_button"), window),
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

    sliderMusic.setValue(1.f);


    buttonBack.setPosition({50.f, 50.f});

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
    checkbox.setValue(SettingsManager::getInstance().getIsVerticalSync());
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
        if (sliderMusic.isInChange()) {
            setMusicVolumeCallback(sliderMusic.getValue() * 100.f);
        }
        checkbox.update(dt);
        buttonBack.update(dt);
    }
}

void SettingsScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    renderTarget.draw(backgroundImage);

    buttonBack.render();
    sliderMusic.render();
    checkbox.render();

    if(isTransition) renderTarget.draw(circle);
}

void SettingsScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(buttonBack.isClicked(event)) {
        SaveSystem::getInstance().saveSettings();
        onBackClick();
    }

    checkbox.handleInput(event);
    SettingsManager::getInstance().setIsVerticalSync(checkbox.getValue());

    sliderMusic.handleInput(event);
    SettingsManager::getInstance().setMusicValue(sliderMusic.getValue());
}

sf::Vector2f SettingsScene::getCameraCenter() const {
    return view.getCenter();
}