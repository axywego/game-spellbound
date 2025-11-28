// #include "Tilemap.hpp"
//
// #include <iostream>
//
// #include "../core/ResourceManager.hpp"
//
// TileType determineTileType(const TiledShape& map, int x, int y) {
//     if (map[x][y].getFillColor() != sf::Color::White) {
//         return TileType::Void;
//     }
//
//     const bool top = x > 0 && map[x-1][y].getFillColor() == sf::Color::Black;
//     const bool bottom = x < map.size() && map[x+1][y].getFillColor() == sf::Color::Black;
//     const bool left = y > 0 && map[x][y-1].getFillColor() == sf::Color::Black;
//     const bool right = y < map[0].size() && map[x][y+1].getFillColor() == sf::Color::Black;
//
//     const bool emptyTopLeft = x > 0 && y > 0 && map[x-1][y-1].getFillColor() == sf::Color::Black;
//     const bool emptyTopRight = x > 0 && y < map[0].size() && map[x-1][y+1].getFillColor() == sf::Color::Black;
//     const bool emptyBottomLeft = x < map.size() && y > 0 && map[x+1][y-1].getFillColor() == sf::Color::Black;
//     const bool emptyBottomRight = x < map.size() && y < map[0].size() && map[x+1][y+1].getFillColor() == sf::Color::Black;
//
//     if (top && !bottom && left && !right) return TileType::CornerTopLeft;
//     if (!top && bottom && left && !right) return TileType::CornerBottomLeft;
//     if (top && !bottom && !left && right) return TileType::CornerTopRight;
//     if (!top && bottom && !left && right) return TileType::CornerBottomRight;
//     if (!top && !bottom && left && !right) return TileType::WallLeft;
//     if (top && !bottom && !left && !right) return TileType::WallTop;
//     if (!top && !bottom && !left && right) return TileType::WallRight;
//     if (!top && bottom && !left && !right) return TileType::WallBottom;
//     if (!top && !bottom && !left && !right) {
//         if(emptyBottomLeft) return TileType::JointBottomLeft;
//         if(emptyBottomRight) return TileType::JointBottomRight;
//         if(emptyTopLeft) return TileType::JointTopLeft;
//         if(emptyTopRight) return TileType::JointTopRight;
//         return TileType::Floor;
//     }
//
//     return TileType::Void;
// }
//
// void Tile::setPosition(const sf::Vector2f& pos) {
//     sprite.setPosition({pos.x * scale * 16.f, pos.y * scale * 16.f});
// }
//
// TileType Tile::getType() const { return type; }
// bool Tile::getHasCollision() const { return hasCollision; }
// sf::Sprite Tile::getSprite() const { return sprite; }
// sf::FloatRect Tile::getGlobalBounds() const { return sprite.getGlobalBounds(); }
// int Tile::getTexX() const { return texX; }
// int Tile::getTexY() const { return texY; }
//
// Tile::Tile(const sf::Texture &texture, const sf::IntRect &rect, bool collision, float scale, int texX, int texY,
//            TileType type): sprite(texture, rect),
//                            hasCollision(collision),
//                            scale(scale),
//                            texX(texX),
//                            texY(texY),
//                            type(type) {
//     sprite.setScale({scale, scale});
// }
//
// Tilemap & Tilemap::operator=(const Tilemap &other) {
//     if (this != &other) {
//         tileset = other.tileset;
//         tiles = other.tiles;
//         collisionTiles = other.collisionTiles;
//         worldSize = other.worldSize;
//         spatialGrid = other.spatialGrid;
//         spatialCollisionGrid = other.spatialCollisionGrid;
//     }
//     return *this;
// }
// std::vector<Tile> Tilemap::getCollisionTiles() const {
//     return collisionTiles;
// }
//
// void Tilemap::createFromTiledShape(const TiledShape& shape) {
//     tiles.clear();
//
//     tileset = std::make_shared<sf::Texture>(ResourceManager::getInstance().getTexture("dungeon"));
//
//     worldSize.y = shape.size();
//     worldSize.x = shape[0].size();
//
//     //tiles.resize(worldSize.y * worldSize.x);
//     for (size_t x = 0; x < worldSize.x; ++x) {
//         for (size_t y = 0; y < worldSize.y; ++y) {
//             TileType type = determineTileType(shape, y, x);
//
//             int texX = 0, texY = 0;
//             bool collision = false;
//
//             switch (type) {
//                 case TileType::Floor:
//                     texX = generate8Bytes(2, 4); texY = generate8Bytes(2, 4);
//                     collision = false;
//                     break;
//                 case TileType::CornerTopLeft:
//                     texX = 1; texY = 1;
//                     collision = true;
//                     break;
//                 case TileType::CornerTopRight:
//                     texX = 5; texY = 1;
//                     collision = true;
//                     break;
//                 case TileType::CornerBottomLeft:
//                     texX = 1; texY = 5;
//                     collision = true;
//                     break;
//                 case TileType::CornerBottomRight:
//                     texX = 5; texY = 5;
//                     collision = true;
//                     break;
//                 case TileType::JointTopLeft:
//                     texX = 5; texY = 5;
//                     collision = true;
//                     break;
//                 case TileType::JointTopRight:
//                     texX = 1; texY = 5;
//                     collision = true;
//                     break;
//                 case TileType::JointBottomLeft:
//                     texX = 5; texY = 1;
//                     collision = true;
//                     break;
//                 case TileType::JointBottomRight:
//                     texX = 1; texY = 1;
//                     collision = true;
//                     break;
//                 case TileType::WallLeft:
//                     texX = 1; texY = generate8Bytes(2, 4);
//                     collision = true;
//                     break;
//                 case TileType::WallRight:
//                     texX = 5; texY = generate8Bytes(2, 4);
//                     collision = true;
//                     break;
//                 case TileType::WallTop:
//                     texX = generate8Bytes(2, 4); texY = 1;
//                     collision = true;
//                     break;
//                 case TileType::WallBottom:
//                     texX = generate8Bytes(2, 4); texY = 5;
//                     collision = true;
//                     break;
//                 default:
//                     texX = 0; texY = 0;
//                     collision = false;
//                     break;
//             }
//
//             // auto tile = Tile(*tileset,
//             //         sf::IntRect({
//             //             static_cast<int>(texX * tileSize), static_cast<int>(texY * tileSize)
//             //             },
//             //         {
//             //             static_cast<int>(tileSize), static_cast<int>(tileSize)
//             //             }
//             //         ),
//             //         collision,
//             //         worldScale,
//             //         texX,
//             //         texY,
//             //         type);
//             //
//             // tile.setPosition({static_cast<float>(x), static_cast<float>(y)});
//             //
//             // tiles.push_back(tile);
//             // if (collision) {
//             //     collisionTiles.push_back(tile);
//             // }
//             tiles.emplace_back(*tileset,
//                 sf::IntRect({static_cast<int>(texX * tileSize), static_cast<int>(texY * tileSize)},
//                             {static_cast<int>(tileSize), static_cast<int>(tileSize)}),
//                 collision,
//                 worldScale,
//                 texX,
//                 texY,
//                 type);
//
//             Tile& tileRef = tiles.back();
//             tileRef.setPosition({static_cast<float>(x), static_cast<float>(y)});
//
//             if (collision) {
//                 collisionTiles.push_back(tileRef);
//             }
//         }
//     }
//     std::cout << "sheesh" << std::endl;
//     buildSpatialGrid();
//     buildSpatialCollisionGrid();
//     //std::cout << "sheesh" << std::endl;
// }
//
// void Tilemap::buildSpatialGrid() {
//     const int worldWidthPixels = worldSize.x * cellSize;
//     const int worldHeightPixels = worldSize.y * cellSize;
//
//     const int gridWidth = (worldWidthPixels + cellSize - 1) / cellSize;
//     const int gridHeight = (worldHeightPixels + cellSize - 1) / cellSize;
//
//     spatialGrid.clear();
//     spatialGrid.resize(gridWidth, std::vector<std::vector<const Tile*>>(gridHeight));
//
//     for (size_t i = 0; i < tiles.size(); ++i) {
//         const int gridX = tiles[i].getGlobalBounds().position.x / cellSize;
//         const int gridY = tiles[i].getGlobalBounds().position.y / cellSize;
//
//         if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
//             spatialGrid[gridX][gridY].emplace_back(&tiles[i]);
//         }
//     }
// }
//
// void Tilemap::buildSpatialCollisionGrid() {
//     const int worldWidthPixels = worldSize.x * cellSize;
//     const int worldHeightPixels = worldSize.y * cellSize;
//
//     const int gridWidth = (worldWidthPixels + cellSize - 1) / cellSize;
//     const int gridHeight = (worldHeightPixels + cellSize - 1) / cellSize;
//
//     spatialCollisionGrid.clear();
//     spatialCollisionGrid.resize(gridWidth, std::vector<std::vector<const Tile*>>(gridHeight));
//
//     for (size_t i = 0; i < collisionTiles.size(); ++i) {
//         const int gridX = collisionTiles[i].getGlobalBounds().position.x / cellSize;
//         const int gridY = collisionTiles[i].getGlobalBounds().position.y / cellSize;
//
//         if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
//             spatialCollisionGrid[gridX][gridY].emplace_back(&collisionTiles[i]);
//         }
//     }
// }
//
// const sf::Vector2u& Tilemap::getWorldSize() const { return worldSize; }
// float Tilemap::getTileSize() const { return tileSize; }
// float Tilemap::getMapScale() const { return worldScale; }
// const sf::Texture& Tilemap::getTileset() const { return *tileset; }
//
// std::vector<Tile> Tilemap::getCollisionTilesInRange(const sf::Vector2f &pos, const float &range) const {
//     std::vector<Tile> ret;
//     // for (const auto& tile : collisionTiles) {
//     //     const auto& posTile = tile.getGlobalBounds().position;
//     //     if (const float lengthSquare = powf(posTile.x - pos.x, 2.f) + powf(posTile.y - pos.y, 2.f);
//     //       lengthSquare <= range * range) {
//     //         ret.push_back(tile);
//     //     }
//     // }
//
//     const sf::FloatRect bounds {{pos.x - range, pos.y - range}, {range * 2, range * 2}};
//
//     const int minGridX = std::max(0, static_cast<int>(bounds.position.x / cellSize));
//     const int maxGridX = std::min(static_cast<int>(spatialCollisionGrid.size() - 1),
//                            static_cast<int>((bounds.position.x + bounds.size.x) / cellSize));
//
//     const int minGridY = std::max(0, static_cast<int>(bounds.position.y / cellSize));
//     const int maxGridY = std::min(static_cast<int>(spatialCollisionGrid[0].size() - 1),
//                            static_cast<int>((bounds.position.y + bounds.size.y) / cellSize));
//
//     if (minGridX > maxGridX || minGridY > maxGridY) {
//         return ret;
//     }
//
//     for (int gy = minGridY; gy <= maxGridY; ++gy) {
//         for (int gx = minGridX; gx <= maxGridX; ++gx) {
//             for (const Tile* tile : spatialCollisionGrid[gx][gy]) {
//                 ret.push_back(*tile);
//             }
//         }
//     }
//
//     return ret;
// }
//
// std::vector<Tile> Tilemap::getTiles() const {
//     return tiles;
// }
//
// void Tilemap::render(sf::RenderTarget& target) const {
//     const sf::View& view = target.getView();
//     const sf::Vector2f viewCenter = view.getCenter();
//     const sf::Vector2f viewSize = view.getSize();
//
//     const sf::FloatRect viewBounds({{viewCenter.x - viewSize.x/2, viewCenter.y - viewSize.y/2},
//         {viewSize.x, viewSize.y}});
//
//     const int minGridX = std::max(0, static_cast<int>(viewBounds.position.x / cellSize));
//     const int maxGridX = std::min(static_cast<int>(spatialGrid.size() - 1),
//                            static_cast<int>((viewBounds.position.x + viewBounds.size.x) / cellSize));
//
//     const int minGridY = std::max(0, static_cast<int>(viewBounds.position.y / cellSize));
//     const int maxGridY = std::min(static_cast<int>(spatialGrid[0].size() - 1),
//                            static_cast<int>((viewBounds.position.y + viewBounds.size.y) / cellSize));
//
//     for (int gy = minGridY; gy <= maxGridY; ++gy) {
//         for (int gx = minGridX; gx <= maxGridX; ++gx) {
//             for (const Tile* tile : spatialGrid[gx][gy]) {
//                 if (isOnScreen(tile->getSprite(), target)) {
//                     target.draw(tile->getSprite());
//                 }
//             }
//         }
//     }
// }

#include "Tilemap.hpp"

#include <iostream>

#include "../core/ResourceManager.hpp"

TileType determineTileType(const TiledShape& map, int x, int y) {
    if (map[x][y].getFillColor() != sf::Color::White) {
        return TileType::Void;
    }

    const bool top = x > 0 && map[x-1][y].getFillColor() == sf::Color::Black;
    const bool bottom = x < map.size() - 1 && map[x+1][y].getFillColor() == sf::Color::Black;
    const bool left = y > 0 && map[x][y-1].getFillColor() == sf::Color::Black;
    const bool right = y < map[0].size() - 1 && map[x][y+1].getFillColor() == sf::Color::Black;

    const bool emptyTopLeft = x > 0 && y > 0 && map[x-1][y-1].getFillColor() == sf::Color::Black;
    const bool emptyTopRight = x > 0 && y < map[0].size() - 1 && map[x-1][y+1].getFillColor() == sf::Color::Black;
    const bool emptyBottomLeft = x < map.size() - 1 && y > 0 && map[x+1][y-1].getFillColor() == sf::Color::Black;
    const bool emptyBottomRight = x < map.size() - 1 && y < map[0].size() - 1 && map[x+1][y+1].getFillColor() == sf::Color::Black;

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
        if(emptyBottomRight) return TileType::JointBottomRight;
        if(emptyTopLeft) return TileType::JointTopLeft;
        if(emptyTopRight) return TileType::JointTopRight;
        return TileType::Floor;
    }

    return TileType::Void;
}

void Tile::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition({pos.x * scale * 16.f, pos.y * scale * 16.f});
}

TileType Tile::getType() const { return type; }
bool Tile::getHasCollision() const { return hasCollision; }
sf::Sprite Tile::getSprite() const { return sprite; }
sf::FloatRect Tile::getGlobalBounds() const { return sprite.getGlobalBounds(); }
int Tile::getTexX() const { return texX; }
int Tile::getTexY() const { return texY; }

Tile::Tile(const sf::Texture &texture, const sf::IntRect &rect, bool collision, float scale, int texX, int texY,
           TileType type): sprite(texture, rect),
                           hasCollision(collision),
                           scale(scale),
                           texX(texX),
                           texY(texY),
                           type(type) {
    sprite.setScale({scale, scale});
}

Tilemap & Tilemap::operator=(const Tilemap &other) {
    if (this != &other) {
        tileset = other.tileset;
        tiles = other.tiles;
        collisionTiles = other.collisionTiles;
        worldSize = other.worldSize;
        spatialGrid = other.spatialGrid;
        spatialCollisionGrid = other.spatialCollisionGrid;
    }
    return *this;
}
std::vector<Tile*> Tilemap::getCollisionTiles() const {
    return collisionTiles;
}

void Tilemap::createFromTiledShape(const TiledShape& shape) {
    tiledShape = shape;

    tiles.clear();

    tileset = std::make_shared<sf::Texture>(ResourceManager::getInstance().getTexture("dungeon"));

    worldSize.y = shape.size();
    worldSize.x = shape[0].size();

    //tiles.resize(worldSize.y * worldSize.x);
    for (size_t x = 0; x < worldSize.x; ++x) {
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

            Tile* tile = new Tile(*tileset,
                sf::IntRect({static_cast<int>(texX * tileSize), static_cast<int>(texY * tileSize)},
                            {static_cast<int>(tileSize), static_cast<int>(tileSize)}),
                collision,
                worldScale,
                texX,
                texY,
                type);

            tiles.emplace_back(tile);

            Tile* tileBack = tiles.back();
            tileBack->setPosition({static_cast<float>(x), static_cast<float>(y)});

            if (collision) {
                collisionTiles.push_back(tileBack);
            }
        }
    }
    // std::cout << "sheesh" << std::endl;
    buildSpatialGrid();
    buildSpatialCollisionGrid();
}

void Tilemap::buildSpatialGrid() {
    const int worldWidthPixels = worldSize.x * cellSize;
    const int worldHeightPixels = worldSize.y * cellSize;

    const int gridWidth = (worldWidthPixels + cellSize - 1) / cellSize;
    const int gridHeight = (worldHeightPixels + cellSize - 1) / cellSize;

    spatialGrid.clear();
    spatialGrid.resize(gridWidth, std::vector<Tile*>(gridHeight));

    for (size_t i = 0; i < tiles.size(); ++i) {
        const int gridX = tiles[i]->getGlobalBounds().position.x / cellSize;
        const int gridY = tiles[i]->getGlobalBounds().position.y / cellSize;

        if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
            spatialGrid[gridX][gridY] = tiles[i];
        }
    }
    // std::cout << "#########\nSPATIAL GRID CREATED\n#########" << std::endl;
}

void Tilemap::buildSpatialCollisionGrid() {
    const int worldWidthPixels = worldSize.x * cellSize;
    const int worldHeightPixels = worldSize.y * cellSize;

    const int gridWidth = (worldWidthPixels + cellSize - 1) / cellSize;
    const int gridHeight = (worldHeightPixels + cellSize - 1) / cellSize;

    spatialCollisionGrid.clear();
    spatialCollisionGrid.resize(gridWidth, std::vector<Tile*>(gridHeight));

    for (size_t i = 0; i < collisionTiles.size(); ++i) {
        const int gridX = collisionTiles[i]->getGlobalBounds().position.x / cellSize;
        const int gridY = collisionTiles[i]->getGlobalBounds().position.y / cellSize;

        if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
            spatialCollisionGrid[gridX][gridY] = collisionTiles[i];
        }
    }
}

TiledShape Tilemap::getTiledShape() const {
    return tiledShape;
}

const sf::Vector2u& Tilemap::getWorldSize() const { return worldSize; }
float Tilemap::getTileSize() const { return tileSize; }
float Tilemap::getMapScale() const { return worldScale; }
const sf::Texture& Tilemap::getTileset() const { return *tileset; }

std::vector<Tile*> Tilemap::getCollisionTilesInRange(const sf::Vector2f &pos, const float &range) const {
    std::vector<Tile*> ret;

    const sf::FloatRect bounds {{pos.x - range, pos.y - range}, {range * 2, range * 2}};

    const int minGridX = std::max(0, static_cast<int>(bounds.position.x / cellSize));
    const int maxGridX = std::min(static_cast<int>(spatialCollisionGrid.size() - 1),
                           static_cast<int>((bounds.position.x + bounds.size.x) / cellSize));

    const int minGridY = std::max(0, static_cast<int>(bounds.position.y / cellSize));
    const int maxGridY = std::min(static_cast<int>(spatialCollisionGrid[0].size() - 1),
                           static_cast<int>((bounds.position.y + bounds.size.y) / cellSize));

    if (minGridX > maxGridX || minGridY > maxGridY) {
        return ret;
    }

    for (int gy = minGridY; gy <= maxGridY; ++gy) {
        for (int gx = minGridX; gx <= maxGridX; ++gx) {
            if (spatialCollisionGrid[gx][gy]) {
                ret.push_back(spatialCollisionGrid[gx][gy]);
            }
        }
    }

    return ret;
}

std::vector<Tile*> Tilemap::getTiles() const {
    return tiles;
}

void Tilemap::render(sf::RenderTarget& target) const {
    const sf::View& view = target.getView();
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f viewSize = view.getSize();

    // std::cout << tiles.size() << std::endl;

    const sf::FloatRect viewBounds({{viewCenter.x - viewSize.x/2, viewCenter.y - viewSize.y/2},
        {viewSize.x, viewSize.y}});

    const int minGridX = std::max(0, static_cast<int>(viewBounds.position.x / cellSize));
    const int maxGridX = std::min(static_cast<int>(spatialGrid.size() - 1),
                           static_cast<int>((viewBounds.position.x + viewBounds.size.x) / cellSize));

    const int minGridY = std::max(0, static_cast<int>(viewBounds.position.y / cellSize));
    const int maxGridY = std::min(static_cast<int>(spatialGrid[0].size() - 1),
                           static_cast<int>((viewBounds.position.y + viewBounds.size.y) / cellSize));

    // if (minGridX == -1 || maxGridX == -1 || minGridY == -1 || maxGridY == -1) {
    //     return;
    // }

    for (int gy = minGridY; gy <= maxGridY; ++gy) {
        for (int gx = minGridX; gx <= maxGridX; ++gx) {
            if (isOnScreen(spatialGrid[gx][gy]->getSprite(), target)) {
                target.draw(spatialGrid[gx][gy]->getSprite());
            }
        }
    }
}

Tilemap::~Tilemap() {
    std::vector<std::vector<Tile*>> spatialGrid;
    std::vector<std::vector<Tile*>> spatialCollisionGrid;
    std::vector<Tile*> tiles;
    std::vector<Tile*> collisionTiles;

    std::unordered_set<Tile*> deletedTiles;

    auto deleteTiles = [&](const std::vector<Tile*>& vec) {
        for (Tile* tile : vec) {
            if (tile && !deletedTiles.contains(tile)) {
                delete tile;
                deletedTiles.insert(tile);
            }
        }
    };

    deleteTiles(tiles);
    deleteTiles(collisionTiles);
}
