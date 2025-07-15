#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <stdexcept>

using json = nlohmann::json;

class WorldTile {
private:
    sf::Sprite sprite;
    bool hasCollision;
    float scale;
    int texX;
    int texY;

public:
    WorldTile(const sf::Texture& texture, const sf::IntRect& rect, 
             bool collision = false, float scale = 5.0f, int texX = 0, int texY = 0)
        : sprite(texture, rect), 
          hasCollision(collision),
          scale(scale),
          texX(texX),
          texY(texY) {
        sprite.setScale({scale, scale});
    }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition({pos.x * scale * 32.f, pos.y * scale * 32.f});
    }

    bool getHasCollision() const { return hasCollision; }
    sf::Sprite getSprite() const { return sprite; }
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    int getTexX() const { return texX; }
    int getTexY() const { return texY; }
};

class World {
private:
    std::shared_ptr<sf::Texture> tileset;
    std::vector<std::vector<WorldTile>> tiles;
    sf::Vector2u worldSize;
    float tileSize = 32.0f;
    float worldScale = 5.0f;

public:
    World() = default;

    World(const std::string& filename) {
        loadFromFile(filename);
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file("maps/" + filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open map file: " + filename);
        }

        json data;
        try {
            file >> data;
        } catch (const json::parse_error& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }

        // Проверка структуры JSON
        if (!data.contains("metadata") || !data.contains("layers") || 
            !data["layers"].contains("ground")) {
            throw std::runtime_error("Invalid map file structure");
        }

        try {
            // Загрузка метаданных
            tileSize = data["metadata"]["tile_size"];
            int tsColumns = data["metadata"]["columns"];
            int tsRows = data["metadata"]["rows"];

            // Загрузка текстуры
            tileset = std::make_shared<sf::Texture>();
            if (!tileset->loadFromFile(std::string(data["metadata"]["tileset"]))) {
                throw std::runtime_error("Failed to load tileset: " + std::string(data["metadata"]["tileset"]));
            }

            // Создание карты
            worldSize.y = data["layers"]["ground"].size();
            if (worldSize.y == 0) {
                throw std::runtime_error("Empty map layer");
            }
            worldSize.x = data["layers"]["ground"][0].size();

            tiles.resize(worldSize.y);
            for (size_t y = 0; y < worldSize.y; ++y) {
                if (data["layers"]["ground"][y].size() != worldSize.x) {
                    throw std::runtime_error("Inconsistent row size in map data");
                }

                tiles[y].reserve(worldSize.x);
                for (size_t x = 0; x < worldSize.x; ++x) {
                    const auto& tile = data["layers"]["ground"][y][x];
                    int texX = tile["tex_x"].get<int>();
                    int texY = tile["tex_y"].get<int>();
                    bool collision = tile["collision"].get<bool>();

                    tiles[y].emplace_back(
                        *tileset,
                        sf::IntRect({texX * tileSize, texY * tileSize}, {tileSize, tileSize}),
                        collision,
                        worldScale,
                        texX,
                        texY
                    );
                    tiles[y].back().setPosition(sf::Vector2f(x, y));
                }
            }
        } catch (const json::exception& e) {
            throw std::runtime_error("JSON error: " + std::string(e.what()));
        }

        worldSize = {tiles[0].size(), tiles.size()};

        return true;
    }

    void render(sf::RenderTarget& target) const {
        for (const auto& row : tiles) {
            for (const auto& tile : row) {
                target.draw(tile.getSprite());
            }
        }
    }

    std::vector<sf::Sprite> getCollisionTiles() const {
        std::vector<sf::Sprite> collisionTiles;
        for(const auto& row : tiles){
            for(const auto& tile : row) {
                if(tile.getHasCollision())
                    collisionTiles.push_back(tile.getSprite());
            }
        }
        return collisionTiles;
    }

    // Метод для сохранения в формате, совместимом с MapEditor
    void saveToFile(const std::string& filename) const {
        json j;
        j["metadata"] = {
            {"tileset", "img/tilemap.png"},
            {"tile_size", tileSize},
            {"columns", tileset->getSize().x / tileSize},
            {"rows", tileset->getSize().y / tileSize}
        };

        for (const auto& row : tiles) {
            json rowData;
            for (const auto& tile : row) {
                rowData.push_back({
                    {"tex_x", tile.getTexX()},
                    {"tex_y", tile.getTexY()},
                    {"collision", tile.getHasCollision()}
                });
            }
            j["layers"]["ground"].push_back(rowData);
        }

        std::ofstream file(filename);
        if (file.is_open()) {
            file << j.dump(4);
        } else {
            throw std::runtime_error("Failed to save file: " + filename);
        }
    }

    const sf::Vector2u& getWorldSize() const { return worldSize; }
    float getTileSize() const { return tileSize; }
    float getWorldScale() const { return worldScale; }
    const sf::Texture& getTileset() const { return *tileset; }
};

#endif