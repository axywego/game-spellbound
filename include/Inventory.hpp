#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <filesystem>

class ItemInventory {
private: 
    std::string name;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    ItemInventory() = delete;
    ItemInventory(const std::string& name_): name(name_), texture(), sprite(texture) {
        // set sprites
        std::filesystem::path imgdir = "img";
        if(!texture.loadFromFile(imgdir / "tilemap.png")){
            throw std::runtime_error("cannot open texture file!");
        };
        sprite.setTexture(texture);
    }

    std::string getName() const {
        return name;
    }

    sf::Sprite& getSprite(){
        return sprite;
    }

    void render(sf::RenderTarget& target) const {
        target.draw(sprite);
    }
};

class Inventory {
private:
    std::list<ItemInventory> items;

public:
    void render(const sf::Vector2f& mousePos, sf::RenderTarget& target){
        sf::Font font;
        sf::Text text(font);

        //here must be logic of render frame inventory
        //

        float x_offset = 50.f;
        for(auto& it : items){
            std::string name = it.getName();
            // logic of drawing items(editing sprite position and scale(optionally))
            //
            decltype(auto) sprite = it.getSprite();

            it.render(target);

            if(sprite.getGlobalBounds().contains(mousePos)){
                text.setString(name);
                sf::Vector2f posSprite = sprite.getPosition();
                text.setPosition({posSprite.x, posSprite.y + 50.f});
                target.draw(text);
            }
            x_offset += 100.f;
        }
    }

};


#endif