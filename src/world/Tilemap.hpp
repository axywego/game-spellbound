#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../generators/WorldGenerator.hpp"
#include "../core/Utils.hpp"

enum class TileType {
    CornerTopLeft, CornerTopRight, CornerBottomLeft, CornerBottomRight,
    JointTopLeft, JointTopRight, JointBottomLeft, JointBottomRight,
    WallLeft, WallRight, WallTop, WallBottom,
    Floor,
    Void
};

TileType determineTileType(const TiledShape& map, int x, int y);

class Tile {
private:
    sf::Sprite sprite;
    bool hasCollision;
    float scale;
    int texX;
    int texY;
    TileType type;

public:
    Tile(const sf::Texture& texture, const sf::IntRect& rect, bool collision = false, float scale = 5.0f, int texX = 0, int texY = 0, TileType type = TileType::Void);

    void setPosition(const sf::Vector2f& pos);

    TileType getType() const;
    bool getHasCollision() const;
    sf::Sprite getSprite() const;
    sf::FloatRect getGlobalBounds() const;
    int getTexX() const;
    int getTexY() const;
};

class Tilemap {
private:
    std::shared_ptr<sf::Texture> tileset;
    std::vector<Tile> tiles;
    sf::Vector2u worldSize;
    unsigned tileSize = 16;
    float worldScale = 5.0f;

public:
    Tilemap() = default;

    std::vector<Tile> getCollisionTiles() const;

    void createFromTiledShape(const TiledShape& shape);

    const sf::Vector2u& getWorldSize() const;
    float getTileSize() const;
    float getMapScale() const;
    const sf::Texture& getTileset() const;
    std::vector<Tile> getCollisionTilesInRange(const sf::Vector2f& pos, const float& range) const;

    std::vector<Tile> getTiles() const;

    void render(sf::RenderTarget& target) const;

};