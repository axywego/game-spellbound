#include "CharacterSelectScene.hpp"

#include "SceneManager.hpp"

CharacterSelectScene::CharacterSelectScene(sf::RenderWindow& window, const std::string& name, const std::function<void()>& backToMenu, const std::function<void(PlayerClass)>& startGame):
Scene(window, name),
background(window, ResourceManager::getInstance().getTexture("background_menu")),
selectButton(ResourceManager::getInstance().getTexture("select_button"), window),
backButton(ResourceManager::getInstance().getTexture("back_button"), window),
prevButton(ResourceManager::getInstance().getTexture("prev_button"), window),
nextButton(ResourceManager::getInstance().getTexture("next_button"), window),
mageSprite(ResourceManager::getInstance().getTexture("player_mage")),
archerSprite(ResourceManager::getInstance().getTexture("player_archer")),
knightSprite(ResourceManager::getInstance().getTexture("player_knight")),
backToMenuCallback(backToMenu),
startGameCallback(startGame)
{
    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    mageSprite.setTextureRect({{0,0}, {32, 32}});
    mageSprite.setScale({7.f, 7.f});
    mageSprite.setPosition({820.f, 450.f});

    knightSprite.setTextureRect({{0,0}, {32, 32}});
    knightSprite.setScale({7.f, 7.f});
    knightSprite.setPosition({820.f, 450.f});

    archerSprite.setTextureRect({{0,0}, {32, 32}});
    archerSprite.setScale({7.f, 7.f});
    archerSprite.setPosition({820.f, 450.f});

    playerClasses.emplace_back(PlayerClass::Mage, mageSprite);
    playerClasses.emplace_back(PlayerClass::Archer, archerSprite);
    playerClasses.emplace_back(PlayerClass::Knight, knightSprite);

    auto animationScalePrevNextButton = Animation::createScaleAnimation(
        {0.4f, 0.4f}, {0.5f, 0.5f},
        std::function{Animation::Easing::easeInOutCubic}, 0.2f
    );

    auto animationScaleSelectButton = Animation::createScaleAnimation(
        {0.4f, 0.4f}, {0.5f, 0.5f},
        std::function{Animation::Easing::easeInOutCubic}, 0.2f
    );

    auto animationScaleBackButton = Animation::createScaleAnimation(
        {0.4f, 0.4f}, {0.5f, 0.5f},
        std::function{Animation::Easing::easeInOutCubic}, 0.2f
    );

    prevButton.setTransform({
        {700.f,500.f},
        0.f,
        {0.4f, 0.4f}
    });

    nextButton.setTransform({
        {1100.f,500.f},
        0.f,
        {0.4f, 0.4f}
    });

    selectButton.setTransform({
        {800.f,800.f},
        0.f,
        {0.4f, 0.4f}
    });

    backButton.setTransform({
        {50.f,50.f},
        0.f,
        {0.4f, 0.4f}
    });

    prevButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationScalePrevNextButton, false)
    );

    nextButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationScalePrevNextButton, false)
    );

    selectButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationScaleSelectButton, false)
    );

    backButton.addAnimation(
        UI::Button::TypeAnimation::Hovered,
        std::make_pair(animationScaleBackButton, false)
    );

}

void CharacterSelectScene::load() {

}

void CharacterSelectScene::update(const float &dt) {
    prevButton.update(dt);
    nextButton.update(dt);
    selectButton.update(dt);
    backButton.update(dt);
}

void CharacterSelectScene::render(sf::RenderTarget &renderTarget) {
    background.render();
    window.draw(playerClasses[currentPlayer].second);
    backButton.render();
    prevButton.render();
    nextButton.render();
    selectButton.render();
}

void CharacterSelectScene::handleEvent(const std::optional<sf::Event> &event) {
    if (backButton.isClicked(event)) {
        backToMenuCallback();
    }
    if (selectButton.isClicked(event)) {
        startGameCallback(playerClasses[currentPlayer].first);
    }

    if (prevButton.isClicked(event)) {
        if (--currentPlayer < 0) {
            currentPlayer = playerClasses.size() - 1;
        }
    }
    if (nextButton.isClicked(event)) {
        if (++currentPlayer >= playerClasses.size()) {
            currentPlayer = 0;
        }
    }
}

sf::Vector2f CharacterSelectScene::getCameraCenter() const {
    return view.getCenter();
}
