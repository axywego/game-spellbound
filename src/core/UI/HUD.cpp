#include "HUD.hpp"

namespace UI {
    HUD & HUD::getInstance() {
        static HUD instance;
        return instance;
    }

    void HUD::init(sf::RenderWindow &win) {
        window = &win;
        textureHealthMana = ResourceManager::getInstance().getTexture("health");
        posLeft = {54, 18};
        posCenter = {62, 18};
        posRight = {98, 18};

        textDamage.setWindow(*window);
        textSpeed.setWindow(*window);
        textCompletedDungeon.setWindow(*window);

        textDamage.setFont(ResourceManager::getInstance().getFont("font_game"));
        textSpeed.setFont(ResourceManager::getInstance().getFont("font_game"));
        textCompletedDungeon.setFont(ResourceManager::getInstance().getFont("font_game"));

        textCompletedDungeon.setText("The dungeon is cleared! Press Enter to go to the next!");
        textCompletedDungeon.setScale({8.f, 8.f});
        textCompletedDungeon.setOutlineColor(sf::Color::Black);
        textCompletedDungeon.setOutlineThickness(3.f);
    }

    void HUD::addTitle(const std::string &title, const float& time) {
        Text text(*window, ResourceManager::getInstance().getFont("font_game"), title);
        text.setColor(sf::Color::White);

        currentTitle.first = std::make_unique<Text>(text);
        currentTitle.second = time + 1.f;

        timer = 0.f;
    }

    void HUD::update(const float& dt, const Player& player, const sf::Vector2f& cameraCenter, const size_t& numOfEnemies_) {
        numOfEnemies = numOfEnemies_;

        sf::Vector2f cameraDelta = {cameraCenter.x - 1920.f / 2, cameraCenter.y - 1080.f / 2};

        // update hp sprites
        auto hp_sprites = static_cast<size_t>(player.getMaxHealth() * 2);
        while(spritesHP.size() < hp_sprites) {
            spritesHP.push_back(sf::Sprite(textureHealthMana));
        }
        while(spritesHP.size() > hp_sprites) {
            spritesHP.pop_back();
        }

        float curr{0.f};
        size_t i = 0;

        while(curr < player.getMaxHealth()){
            sf::Sprite& spr = spritesHP[i];
            if(curr >= player.getCurrentHealth()){
                posLeft.y = 72;
                posCenter.y = 72;
                posRight.y = 72;
            }

            else {
                posLeft.y = 18;
                posCenter.y = 18;
                posRight.y = 18;
            }

            if(curr == 0.f){
                spr.setTextureRect({posLeft, {8, 16}});
            }
            else if (curr < player.getMaxHealth() - 0.5f) {
                spr.setTextureRect({posCenter, {8, 16}});
            }
            else {
                spr.setTextureRect({posRight, {8, 16}});
            }

            spr.setScale({5.f, 3.f});
            spr.setPosition({50.f + curr * 5.f * 8.f * 2.f + cameraDelta.x, 50.f + cameraDelta.y});
            curr += 0.5f;
            i++;
        }

        float offset = 0.f;
        posRight.x += 4;
        if(player.getMaxMana() > 0.f){

            offset = 30.f;

            const auto mana_sprites = static_cast<size_t>(player.getMaxMana() * 4);

            while(spritesMana.size() < mana_sprites) {
                spritesMana.push_back(sf::Sprite(textureHealthMana));
            }
            while(spritesMana.size() > mana_sprites) {
                spritesMana.pop_back();
            }

            curr = 0.f;
            i = 0;

            while(curr < player.getMaxMana()){
                sf::Sprite& spr = spritesMana[i];
                if(curr >= player.getCurrentMana()){
                    posLeft.y = 198;
                    posCenter.y = 198;
                    posRight.y = 198;
                }
                else {
                    posLeft.y = 144;
                    posCenter.y = 144;
                    posRight.y = 144;
                }

                if(curr == 0.f){
                    spr.setTextureRect({posLeft, {4, 16}});
                }
                else if (curr < player.getMaxMana() - 0.25f) {
                    spr.setTextureRect({posCenter, {4, 16}});
                }
                else {
                    spr.setTextureRect({posRight, {4, 16}});
                }

                spr.setScale({5.f, 3.f});
                spr.setPosition({50.f + curr * 5.f * 8.f * 2.f + cameraDelta.x, 120.f + cameraDelta.y});
                curr += 0.25f;
                i++;
            }

        }
        posRight.x -= 4;

        //update show damage
        if(player.getTypeDamage() == Mob::TypeDamage::Melee) {
            spriteDamage.setTextureRect({{60, 0},{10,10}});
        }
        else {
            spriteDamage.setTextureRect({{90, 50},{10,10}});
        }
        spriteDamage.setScale({7.f, 7.f});
        spriteDamage.setPosition({50.f + cameraDelta.x, offset + 170.f + cameraDelta.y});

        textDamage.setText(makeFormatedFloat(player.getDamage()));
        textDamage.setPosition({130.f + cameraDelta.x, offset + 180.f + cameraDelta.y});
        textDamage.setOutlineColor(sf::Color::Black);
        textDamage.setOutlineThickness(3.f);

        //update show speed
        spriteSpeed.setTextureRect({{50, 50},{10, 10}});
        spriteSpeed.setScale({7.f, 7.f});
        spriteSpeed.setPosition({50.f + cameraDelta.x, offset + 240.f + cameraDelta.y});
        textSpeed.setText(makeFormatedFloat(player.getCurrentSpeed()));
        textSpeed.setPosition({130.f + cameraDelta.x, offset + 250.f + cameraDelta.y});
        textSpeed.setOutlineColor(sf::Color::Black);
        textSpeed.setOutlineThickness(3.f);

        //update show isThereEnemies
        if (numOfEnemies == 0)
            textCompletedDungeon.setPosition({300.f + cameraDelta.x, offset + 400.f + cameraDelta.y});

        if (currentTitle.first) {
            float percent;
            if (timer <= 0.5f)
                percent = std::clamp(timer / 0.5f, 0.f, 1.f);
            else if (timer > currentTitle.second - 1.f + 0.5f) {
                percent = std::clamp(1.f - (timer - currentTitle.second + 0.5f) / 0.5f, 0.f, 1.f);
            }
            else {
                percent = 1.f;
            }
            currentTitle.first->setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(255 * percent)));
            currentTitle.first->setOutlineColor(sf::Color(0, 0, 0, static_cast<uint8_t>(255 * percent)));
            currentTitle.first->setPosition({300.f + cameraDelta.x, 240.f + cameraDelta.y});
            currentTitle.first->setOutlineThickness(3.0f);
        }

        if (timer < currentTitle.second) timer += dt;
    }

    void HUD::render() {
        if (!window) {
            std::cout << "wtf\n";
            return;
        }

        for(auto& sprite : spritesHP){
            window->draw(sprite);
        }

        for(auto& sprite : spritesMana){
            window->draw(sprite);
        }

        window->draw(spriteDamage);
        window->draw(spriteSpeed);
        textDamage.render();
        textSpeed.render();

        if (timer < currentTitle.second) {
            currentTitle.first->render();
        }

        if (numOfEnemies == 0)
            textCompletedDungeon.render();
    }
}
