#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include "Player.hpp"
#include "WorldGenerator.hpp"

class Player;

using json = nlohmann::json;

enum class TileType {
    CornerTopLeft, CornerTopRight, CornerBottomLeft, CornerBottomRight,
    JointTopLeft, JointTopRight, JointBottomLeft, JointBottomRight,
    WallLeft, WallRight, WallTop, WallBottom,
    Floor,
    Void
};

TileType determineTileType(const TiledShape& map, int x, int y) {
    if (map[x][y].getFillColor() != sf::Color::White) {
        return TileType::Void;
    }

    bool top = x > 0 && map[x-1][y].getFillColor() == sf::Color::Black;
    bool bottom = x < map.size() && map[x+1][y].getFillColor() == sf::Color::Black;
    bool left = y > 0 && map[x][y-1].getFillColor() == sf::Color::Black;
    bool right = y < map[0].size() && map[x][y+1].getFillColor() == sf::Color::Black;

    bool emptyTopLeft = x > 0 && y > 0 && map[x-1][y-1].getFillColor() == sf::Color::Black;
    bool emptyTopRight = x > 0 && y < map[0].size() && map[x-1][y+1].getFillColor() == sf::Color::Black;
    bool emptyBottomLeft = x < map.size() && y > 0 && map[x+1][y-1].getFillColor() == sf::Color::Black;
    bool emptyBottomRight = x < map.size() && y < map[0].size() && map[x+1][y+1].getFillColor() == sf::Color::Black;

    if (top && !bottom && left && !right) return TileType::CornerTopLeft;
    if (!top && bottom && left && !right) return TileType::CornerBottomLeft;
    if (top && !bottom && !left && right) return TileType::CornerTopRight;
    if (!top && bottom && !left && right) return TileType::CornerBottomRight;
    if (!top && !bottom && left && !right) return TileType::WallLeft;
    if (top && !bottom && !left && !right) return TileType::WallTop;
    if (!top && !bottom && !left && right) return TileType::WallRight;
    if (!top && bottom && !left && !right) return TileType::WallBottom;
    if (!top && !bottom && !left && !right) {
        if(emptyBottomLeft) return TileType::JointBottomLeft;
        else if(emptyBottomRight) return TileType::JointBottomRight;
        else if(emptyTopLeft) return TileType::JointTopLeft;
        else if(emptyTopRight) return TileType::JointTopRight;
        else return TileType::Floor;
    }
    
    return TileType::Void;
}

class Tile {
private:
    sf::Sprite sprite;
    bool hasCollision;
    float scale;
    int texX;
    int texY;
    TileType type;

public:
    Tile(const sf::Texture& texture, const sf::IntRect& rect, 
             bool collision = false, float scale = 5.0f, int texX = 0, int texY = 0, TileType type = TileType::Void)
        : sprite(texture, rect), 
          hasCollision(collision),
          scale(scale),
          texX(texX),
          texY(texY),
          type(type) {
        sprite.setScale({scale, scale});
    }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition({pos.x * scale * 16.f, pos.y * scale * 16.f});
    }

    TileType getType() const { return type; }
    bool getHasCollision() const { return hasCollision; }
    sf::Sprite getSprite() const { return sprite; }
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    int getTexX() const { return texX; }
    int getTexY() const { return texY; }
};

class Tilemap {
private:
    std::shared_ptr<sf::Texture> tileset;
    std::vector<std::vector<Tile>> tiles;
    sf::Vector2u worldSize;
    unsigned tileSize = 16;
    float worldScale = 5.0f;

public:
    Tilemap() = default;

    Tilemap(const std::string& filename) {
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

        if (!data.contains("metadata") || !data.contains("layers") || 
            !data["layers"].contains("ground")) {
            throw std::runtime_error("Invalid map file structure");
        }

        try {
            tileSize = data["metadata"]["tile_size"];
            int tsColumns = data["metadata"]["columns"];
            int tsRows = data["metadata"]["rows"];

            tileset = std::make_shared<sf::Texture>();
            if (!tileset->loadFromFile(std::string(data["metadata"]["tileset"]))) {
                throw std::runtime_error("Failed to load tileset: " + std::string(data["metadata"]["tileset"]));
            }

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
                        texY,
                        TileType::Void
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

    void createFromTiledShape(const TiledShape& shape) {
        tiles.clear();

        tileset = std::make_shared<sf::Texture>("real img/2 Dungeon Tileset/1 Tiles/Tileset.png");
        
        worldSize.y = shape.size();
        worldSize.x = shape[0].size();
        
        tiles.resize(worldSize.y);
        for (size_t x = 0; x < worldSize.x; ++x) {
            tiles[x].reserve(worldSize.y);
            for (size_t y = 0; y < worldSize.y; ++y) {
                TileType type = determineTileType(shape, y, x);
                
                int texX = 0, texY = 0;
                bool collision = false;
                
                switch (type) {
                    case TileType::Floor:
                        texX = generate8Bytes(2, 4); texY = generate8Bytes(2, 4);
                        collision = false;
                        break;
                    case TileType::CornerTopLeft:
                        texX = 1; texY = 1;
                        collision = true;
                        break;
                    case TileType::CornerTopRight:
                        texX = 5; texY = 1;
                        collision = true;
                        break;
                    case TileType::CornerBottomLeft:
                        texX = 1; texY = 5;
                        collision = true;
                        break;
                    case TileType::CornerBottomRight:
                        texX = 5; texY = 5;
                        collision = true;
                        break;
                    case TileType::JointTopLeft:
                        texX = 5; texY = 5;
                        collision = true;
                        break;
                    case TileType::JointTopRight:
                        texX = 1; texY = 5;
                        collision = true;
                        break;
                    case TileType::JointBottomLeft:
                        texX = 5; texY = 1;
                        collision = true;
                        break;
                    case TileType::JointBottomRight:
                        texX = 1; texY = 1;
                        collision = true;
                        break;
                    case TileType::WallLeft:
                        texX = 1; texY = generate8Bytes(2, 4);
                        collision = true;
                        break;
                    case TileType::WallRight:
                        texX = 5; texY = generate8Bytes(2, 4);
                        collision = true;
                        break;
                    case TileType::WallTop:
                        texX = generate8Bytes(2, 4); texY = 1;
                        collision = true;
                        break;
                    case TileType::WallBottom:
                        texX = generate8Bytes(2, 4); texY = 5;
                        collision = true;
                        break;
                    default:
                        texX = 0; texY = 0;
                        collision = false;
                        break;
                }
                
                tiles[y].emplace_back(
                    *tileset,
                    sf::IntRect({texX * tileSize, texY * tileSize}, {tileSize, tileSize}),
                    collision,
                    worldScale,
                    texX,
                    texY,
                    type
                );

                tiles[y].back().setPosition({x, y});
            }
        }
    }

    const sf::Vector2u& getWorldSize() const { return worldSize; }
    float getTileSize() const { return tileSize; }
    float getMapScale() const { return worldScale; }
    const sf::Texture& getTileset() const { return *tileset; }

    std::vector<std::vector<Tile>> getTiles() const {
        return tiles;
    }

    void render(sf::RenderTarget& target) {
        for (const auto& row : tiles) {
            for (const auto& tile : row) {
                target.draw(tile.getSprite());
            }
        }
    }

};

#endif