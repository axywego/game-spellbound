#pragma once

#include "Scene.hpp"
#include "../core/UI/Button.hpp"
#include "../entities/player/PlayerFactory.hpp"
#include "../core/UI/Image.hpp"

class CharacterSelectScene final : public Scene {
private:

    sf::View view;
    sf::Vector2f target;

    UI::Image background;
    UI::Button selectButton;
    UI::Button backButton;
    UI::Button prevButton;
    UI::Button nextButton;

    sf::Sprite mageSprite;
    sf::Sprite archerSprite;
    sf::Sprite knightSprite;

    std::function<void()> backToMenuCallback;
    std::function<void(PlayerClass)> startGameCallback;

    PlayerClass player {PlayerClass::Mage};

    std::vector<std::pair<PlayerClass, sf::Sprite&>> playerClasses;
    int currentPlayer = 0;

public:
    CharacterSelectScene(sf::RenderWindow& window, const std::string& name, const std::function<void()>& backToMenu, const std::function<void(PlayerClass)>& startGame);
    void load() override;
    void update(const float& dt) override;
    void render(sf::RenderTarget& renderTarget) override;
    void handleEvent(const std::optional<sf::Event>& event) override;
    sf::Vector2f getCameraCenter() const override;
};