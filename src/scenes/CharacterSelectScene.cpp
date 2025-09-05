#include "CharacterSelectScene.hpp"

#include "SceneManager.hpp"

CharacterSelectScene::CharacterSelectScene(sf::RenderWindow& window, const std::function<void()>& backToMenu, const std::function<void(PlayerClass)>& startGame): Scene(window),
selectButton(ResourceManager::getInstance().getTexture("select_button"), window),
backButton(ResourceManager::getInstance().getTexture("back_button"), window),
prevButton(ResourceManager::getInstance().getTexture("prev_button"), window),
nextButton(ResourceManager::getInstance().getTexture("next_button"), window),
mageSprite(ResourceManager::getInstance().getTexture("player_mage")),
archerSprite(ResourceManager::getInstance().getTexture("player_archer")),
knightSprite(ResourceManager::getInstance().getTexture("player_knight")),
backtoMenuCallback(backToMenu),
startGameCallback(startGame)
{
    target = { 960.f, 540.f };
    view.setSize({1920.f, 1080.f});
    view.setCenter(target);

    mageSprite.setTextureRect({{0,0}, {32, 32}});
    mageSprite.setScale({7.f, 7.f});
    mageSprite.setPosition({900.f, 400.f});

    knightSprite.setTextureRect({{0,0}, {32, 32}});
    knightSprite.setScale({7.f, 7.f});
    knightSprite.setPosition({900.f, 400.f});

    archerSprite.setTextureRect({{0,0}, {32, 32}});
    archerSprite.setScale({7.f, 7.f});
    archerSprite.setPosition({900.f, 400.f});

    playerClasses.push_back({PlayerClass::Mage, mageSprite});
    playerClasses.push_back({PlayerClass::Archer, archerSprite});
    playerClasses.push_back({PlayerClass::Knight, knightSprite});

    prevButton.setPosition({50.f, 400.f});
    nextButton.setPosition({1800.f, 400.f});
    selectButton.setPosition({700.f, 800.f});
    backButton.setPosition({50.f, 50.f});
}

void CharacterSelectScene::load() {

}

void CharacterSelectScene::update(const float &dt) {

}

void CharacterSelectScene::render(sf::RenderTarget &renderTarget) {
    window.draw(background);
    window.draw(playerClasses[currentPlayer].second);
    backButton.render();
    prevButton.render();
    nextButton.render();
    selectButton.render();
}

void CharacterSelectScene::handleEvent(const std::optional<sf::Event> &event) {
    if (backButton.isClicked(event)) {
        backtoMenuCallback();
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
