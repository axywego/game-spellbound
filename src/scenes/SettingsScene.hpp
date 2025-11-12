#pragma once

#include "Scene.hpp"
#include "../core/UI/Button.hpp"
#include "../core/UI/Slider.hpp"
#include "../core/ResourceManager.hpp"
#include <functional>

#include "../core/UI/CheckBox.hpp"

class SettingsScene: public Scene {
private:

    sf::View view;
    sf::Vector2f target;

    sf::CircleShape circle;
    float circleScale { 1.f };
    float transitionSpeed { 2.f };
    bool isTransition { true };

    sf::Texture textureBackground {ResourceManager::getInstance().getTexture("menu_background")};
    sf::Sprite backgroundImage;

    UI::Slider sliderMusic;
    UI::Slider sliderSound;
    UI::CheckBox checkbox;

    UI::Button backButton;

    std::function<void()> onBackClick;
    std::function<void(float)> setMusicVolumeCallback;

public:
    SettingsScene(sf::RenderWindow& window_, const std::string& name, const std::function<void()> &backCallback, const std::function<void(float)> &setMusicVolumeCallback);

    void load() override ;

    void update(const float& dt) override;

    void render(sf::RenderTarget& renderTarget) override;

    void handleEvent(const std::optional<sf::Event>& event) override;

    sf::Vector2f getCameraCenter() const override;
};
