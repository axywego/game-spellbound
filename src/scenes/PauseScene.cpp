#include "PauseScene.hpp"

PauseScene::PauseScene(sf::RenderWindow& window_, const std::function<void()> &resumeCallback,
    const std::function<void()> &menuCallback):

Scene(window_),
resumeButton(ResourceManager::getInstance().getTexture("start_button"), window),
menuButton(ResourceManager::getInstance().getTexture("exit_button"), window),
onResumeClick(resumeCallback),
onMenuClick(menuCallback) {

    view.setSize({1920.f, 1080.f});
    view.setCenter({960.f, 540.f});

    overlay.setSize({1920.f, 1080.f});
    overlay.setFillColor(sf::Color(255, 255, 255, 150));

    resumeButton.setPosition({760.f, 400.f});
    menuButton.setPosition({760.f, 500.f});

}

void PauseScene::load()  {

}

void PauseScene::update(const float& dt)  {

}

void PauseScene::render(sf::RenderTarget& renderTarget)  {
    renderTarget.setView(view);

    renderTarget.draw(overlay);

    resumeButton.render(renderTarget);
    menuButton.render(renderTarget);
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