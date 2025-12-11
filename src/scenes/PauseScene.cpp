#include "PauseScene.hpp"

#include "../core/Utils.hpp"

PauseScene::PauseScene(sf::RenderWindow& window_, const std::string& name, const std::function<void()> &resumeCallback,
    const std::function<void()> &menuCallback):

Scene(window_, name),
resumeButton(ResourceManager::getInstance().getTexture("anim_start_btn_1"), window),
menuButton(ResourceManager::getInstance().getTexture("anim_exit_btn_1"), window),
onResumeClick(resumeCallback),
onMenuClick(menuCallback) {

    view.setSize({1920.f, 1080.f});
    view.setCenter({960.f, 540.f});

    overlay.setSize({1920.f, 1080.f});
    overlay.setFillColor(sf::Color(255, 255, 255, 150));

    resumeButton.setPosition({760.f, 400.f});
    menuButton.setPosition({760.f, 500.f});

    auto animationScaleStartButton = Animation::createScaleAnimation(
        {0.3f, 0.3f}, {0.34f, 0.34f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    sf::Texture textureAnimStartBtn1 = ResourceManager::getInstance().getTexture("anim_start_btn_1");
    sf::Texture textureAnimStartBtn2 = ResourceManager::getInstance().getTexture("anim_start_btn_2");
    sf::Texture textureAnimStartBtn3 = ResourceManager::getInstance().getTexture("anim_start_btn_3");
    auto animationStartBtn = Animation::createSlideShowAnimation(
        {textureAnimStartBtn1, textureAnimStartBtn2, textureAnimStartBtn3}, 0.75f
    );

    auto animationScaleExitButton = Animation::createScaleAnimation(
        {0.3f, 0.3f}, {0.34f, 0.34f},
        std::function{Animation::Easing::easeInOutQuad}, 0.2f
    );

    sf::Texture textureAnimExitBtn1 = ResourceManager::getInstance().getTexture("anim_exit_btn_1");
    sf::Texture textureAnimExitBtn2 = ResourceManager::getInstance().getTexture("anim_exit_btn_2");
    sf::Texture textureAnimExitBtn3 = ResourceManager::getInstance().getTexture("anim_exit_btn_3");
    auto animationExitBtn = Animation::createSlideShowAnimation(
        {textureAnimExitBtn1, textureAnimExitBtn2, textureAnimExitBtn3}, 0.75f
    );

    resumeButton.setTransform(Transform{
            {400.f, 400.f},
            0.f,
            {0.3f, 0.3f}
        }
    );
    resumeButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationStartBtn, true), std::make_pair(animationScaleStartButton, false)
    );

    menuButton.setTransform(Transform{
            sf::Vector2f{400.f, 600.f},
            0.f,
            {0.3f, 0.3f}
        }
    );
    menuButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationExitBtn, true), std::make_pair(animationScaleExitButton, false)
    );
}

void PauseScene::load()  {

}

void PauseScene::update(const float& dt)  {
    resumeButton.update(dt);
    menuButton.update(dt);
}

void PauseScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    renderTarget.draw(overlay);

    resumeButton.render();
    menuButton.render();
}

void PauseScene::handleEvent(const std::optional<sf::Event>& event)  {
    if(event){
        if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
            if(key->code == sf::Keyboard::Key::Escape)
                onResumeClick();
        }
    }

    if (resumeButton.isClicked(event)) {
        onResumeClick();
    }

    if (menuButton.isClicked(event)) {        
        onMenuClick();
    }
}
sf::Vector2f PauseScene::getCameraCenter() const {
    return view.getCenter();
}