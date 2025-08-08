#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <format>
#include <unordered_set>
#include <queue>
#include <array>
#include <cmath>
#include "../core/Generator.hpp"

#include <fstream>

extern const float factor;
extern const unsigned tileSize;
extern sf::Vector2i limitsTiles;

using TiledShape = std::vector<std::vector<sf::RectangleShape>>;

std::vector<std::pair<size_t, size_t>> checkNeighbors(const TiledShape& room, const std::pair<size_t, size_t>& pair);

struct MyHash {
    size_t operator()(const std::pair<size_t, size_t>& p) const;
};

struct EqualTuples {
    bool operator()(const std::pair<size_t, size_t>& t1, const std::pair<size_t, size_t>& t2) const;
};

bool isConnected(const TiledShape& room);

TiledShape createRectangle(const sf::Vector2i& pos, const sf::Vector2i& size);

TiledShape createRectangle(const sf::Vector2i& start, const sf::Vector2i& end, int thickness);

TiledShape generateRectangle();

class Room {
private:
    TiledShape part;
public:
    Room();

    void init();

    void addRectangle(const TiledShape& rect);

    sf::Vector2i getSize() const;

    sf::Vector2i getPosition() const ;

    void setPosition(const sf::Vector2i& newPos);

    sf::Vector2i getCenterPosition() const;

    void setCenterPosition(const sf::Vector2i newPos);

    TiledShape& getPart();
};

class WorldGenerator {
private:
    static TiledShape map;
    static const sf::Vector2i mapSize;
    static const float radius;

public:
    static void init();

    static TiledShape generateDungeon();

    static void addRoom(Room& room);

    static void addCorridor(const sf::Vector2i& startTile, const sf::Vector2i& endTile, int thickness);

    static void render(sf::RenderTarget& target);

    //get radius in pixels
    static float getRadius();

    static sf::Vector2f getCenter();
};