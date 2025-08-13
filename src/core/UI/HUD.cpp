#include "HUD.hpp"

namespace UI {
    void HUD::update(const Player& player, const sf::Vector2f& cameraCenter) {
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
            bool isPositive;
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

            auto mana_sprites = static_cast<size_t>(player.getMaxMana() * 4);

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
        textDamage.setString(makeFormatedFloat(player.getDamage()));
        textDamage.setPosition({130.f + cameraDelta.x, offset + 180.f + cameraDelta.y});
        textDamage.setOutlineColor(sf::Color::Black);
        textDamage.setOutlineThickness(3.f);

        //update show speed
        spriteSpeed.setTextureRect({{50, 50},{10, 10}});
        spriteSpeed.setScale({7.f, 7.f});
        spriteSpeed.setPosition({50.f + cameraDelta.x, offset + 240.f + cameraDelta.y});
        textSpeed.setString(makeFormatedFloat(player.getCurrentSpeed()));
        textSpeed.setPosition({130.f + cameraDelta.x, offset + 250.f + cameraDelta.y});
        textSpeed.setOutlineColor(sf::Color::Black);
        textSpeed.setOutlineThickness(3.f);

    }

    void HUD::render(sf::RenderTarget& target){
        for(auto& sprite : spritesHP){
            target.draw(sprite);
        }

        for(auto& sprite : spritesMana){
            target.draw(sprite);
        }

        target.draw(spriteDamage);
        target.draw(spriteSpeed);
        target.draw(textDamage);
        target.draw(textSpeed);
    }


    sf::Texture HUD::textureHealthMana {ResourceManager::getInstance().getTexture("health")};
    std::vector<sf::Sprite> HUD::spritesHP;
    std::vector<sf::Sprite> HUD::spritesMana;
    sf::Vector2i HUD::posLeft {54, 18};
    sf::Vector2i HUD::posCenter {62, 18};
    sf::Vector2i HUD::posRight {98, 18};
    sf::Texture HUD::textureIcons {ResourceManager::getInstance().getTexture("icons")};
    sf::Sprite HUD::spriteDamage(textureIcons);
    sf::Sprite HUD::spriteSpeed(textureIcons);
    sf::Font HUD::font("../resources/fonts/Cafe24PROUP.ttf");
    sf::Text HUD::textDamage(font);
    sf::Text HUD::textSpeed(font);
}
