#include "WorldGenerator.hpp"

const float factor = 5.f;
const unsigned tileSize = 16 * factor;
sf::Vector2i limitsTiles = {20, 20};

TiledShape WorldGenerator::map;
const sf::Vector2i WorldGenerator::mapSize{100, 100};
const float WorldGenerator::radius{mapSize.x * 0.34f};

std::vector<std::pair<size_t, size_t>> checkNeighbors(const TiledShape& room, const std::pair<size_t, size_t>& pair){
std::vector<std::pair<size_t, size_t>> neighbors;
if(pair.first > 0 && room[pair.first - 1][pair.second].getFillColor() == sf::Color::White){
    neighbors.push_back({pair.first - 1, pair.second});
}
if(pair.first < room.size() - 1 && room[pair.first + 1][pair.second].getFillColor() == sf::Color::White) {
    neighbors.push_back({pair.first + 1, pair.second});
}

if(pair.second > 0 && room[pair.first][pair.second - 1].getFillColor() == sf::Color::White){
    neighbors.push_back({pair.first, pair.second - 1});
}
if(pair.second < room[0].size() - 1 && room[pair.first][pair.second + 1].getFillColor() == sf::Color::White) {
    neighbors.push_back({pair.first, pair.second + 1});
}
return neighbors;
}

size_t MyHash::operator()(const std::pair<size_t, size_t>& p) const {
    return (std::hash<int>()(p.first)) ^ (std::hash<int>()(p.second) << 1);
}

bool EqualTuples::operator()(const std::pair<size_t, size_t>& t1, const std::pair<size_t, size_t>& t2) const {
    return t1.first == t2.first && t1.second == t2.second;
}

bool isConnected(const TiledShape& room) {
std::vector<std::vector<bool>> damn(room.size(), std::vector<bool>(room[0].size()));
std::vector<std::vector<bool>> checked(room.size(), std::vector<bool>(room[0].size()));
size_t countWhites = 0;

for(size_t i = 0; i < room.size(); i++){
    for(size_t j = 0; j < room[0].size(); j++){
        damn[i][j] = (room[i][j].getFillColor() == sf::Color::White);
        if (damn[i][j]) countWhites++;
    }
}

if (countWhites == 0) return false;

std::unordered_set<std::pair<size_t, size_t>, MyHash, EqualTuples> visited;
std::queue<std::pair<size_t, size_t>> needToVisit;

bool found = false;
for(size_t i = 0; i < room.size() && !found; i++){
    for(size_t j = 0; j < room[0].size() && !found; j++){
        if (damn[i][j]) {
            needToVisit.push({i, j});
            checked[i][j] = true;
            found = true;
        }
    }
}

while(!needToVisit.empty()){
    auto currTile = needToVisit.front();
    needToVisit.pop();
    visited.insert(currTile);

    auto neighbors = checkNeighbors(room, currTile);
    for(const auto& neighbor : neighbors){
        if(!checked[neighbor.first][neighbor.second]) {
            needToVisit.push(neighbor);
            checked[neighbor.first][neighbor.second] = true;
        }
    }
}

return visited.size() == countWhites;
}

TiledShape createRectangle(const sf::Vector2i& pos, const sf::Vector2i& size){
TiledShape shape(limitsTiles.y, std::vector<sf::RectangleShape>(limitsTiles.x));

for(int i = 0; i < limitsTiles.y; i++){
    for(int j = 0; j < limitsTiles.x; j++){
        auto& el = shape[i][j];
        el.setPosition({static_cast<float>(j * tileSize), static_cast<float>(i * tileSize)});
        el.setSize({static_cast<float>(tileSize), static_cast<float>(tileSize)});
        if(i == std::clamp(i, pos.y, pos.y + size.y) && j == std::clamp(j, pos.x, pos.x + size.x)){
            el.setFillColor(sf::Color::White);
        }
        else
            el.setFillColor(sf::Color::Black);
    }
}
return shape;
}

TiledShape createRectangle(const sf::Vector2i& start, const sf::Vector2i& end, int thickness) {
TiledShape shape(std::max(start.y, end.y), TiledShape::value_type(std::max(start.x, end.x)));

for (int i = 0; i < std::max(start.y, end.y); i++) {
    for (int j = 0; j < std::max(start.x, end.x); j++) {
        auto& el = shape[i][j];
        el.setPosition({static_cast<float>(j * tileSize), static_cast<float>(i * tileSize)});
        el.setSize({static_cast<float>(tileSize), static_cast<float>(tileSize)});
        el.setFillColor(sf::Color::Black);
    }
}

int dx = abs(end.x - start.x);
int dy = -abs(end.y - start.y);
int sx = start.x < end.x ? 1 : -1;
int sy = start.y < end.y ? 1 : -1;
int err = dx + dy;
int x = start.x;
int y = start.y;

while (true) {
    for (int i = -thickness/2; i <= thickness/2; i++) {
        for (int j = -thickness/2; j <= thickness/2; j++) {
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < abs(end.x - start.x) && ny >= 0 && ny < abs(end.y - start.y)) {
                shape[ny][nx].setFillColor(sf::Color::White);
            }
        }
    }

    if (x == end.x && y == end.y) break;
    int e2 = 2 * err;
    if (e2 >= dy) {
        err += dy;
        x += sx;
    }
    if (e2 <= dx) {
        err += dx;
        y += sy;
    }
}

return shape;
}

TiledShape generateRectangle(){

sf::Vector2i posRectangle {limitsTiles}, sizeRectangle {limitsTiles};

while(posRectangle.x + sizeRectangle.x >= limitsTiles.x &&
      posRectangle.y + sizeRectangle.y >= limitsTiles.y){
    posRectangle = {static_cast<int>(generate8Bytes(0, limitsTiles.x * 3 / 4 )), static_cast<int>(generate8Bytes(0, limitsTiles.y * 3 / 4))};
    sizeRectangle = {static_cast<int>(generate8Bytes(limitsTiles.x / 4, limitsTiles.x * 3 / 4)), static_cast<int>(generate8Bytes(limitsTiles.y / 4, limitsTiles.y * 3 / 4))};
}

return createRectangle(posRectangle, sizeRectangle);
}

Room::Room(): part(TiledShape(limitsTiles.y, std::vector<sf::RectangleShape>(limitsTiles.x))) {
    init();
}

void Room::init(){
    for(size_t i = 0; i < limitsTiles.y; i++){
        for(size_t j = 0; j < limitsTiles.x; j++){
            auto& el = part[i][j];
            el.setFillColor(sf::Color::Black);
            el.setPosition({static_cast<float>(i * tileSize), static_cast<float>(j * tileSize)});
            el.setSize({static_cast<float>(tileSize), static_cast<float>(tileSize)});
        }
    }
}

void Room::addRectangle(const TiledShape& rect) {
    for(size_t i = 0; i < limitsTiles.y; i++){
        for(size_t j = 0; j < limitsTiles.x; j++){
            if(part[i][j].getFillColor() == sf::Color::Black && rect[i][j].getFillColor() == sf::Color::White){
                part[i][j].setFillColor(sf::Color::White);
            }
        }
    }
}

sf::Vector2i Room::getSize() const {
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = 0, maxY = 0;
    bool found = false;

    for(const auto& row : part){
        for(const auto& el : row){
            if(el.getFillColor() == sf::Color::White) {
                int x = static_cast<int>(el.getPosition().x / tileSize);
                int y = static_cast<int>(el.getPosition().y / tileSize);
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
                found = true;
            }
        }
    }

    return found ? sf::Vector2i{maxX - minX + 1, maxY - minY + 1} : sf::Vector2i{0, 0};
}


sf::Vector2i Room::getPosition() const {
    bool found = false;
    sf::Vector2i pos{INT_MAX, INT_MAX};

    for(const auto& row : part){
        for(const auto& el : row){
            if(el.getFillColor() == sf::Color::White) {
                sf::Vector2f pixelPos = el.getPosition();
                pos.x = std::min(pos.x, static_cast<int>(pixelPos.x / tileSize));
                pos.y = std::min(pos.y, static_cast<int>(pixelPos.y / tileSize));
                found = true;
            }
        }
    }

    return found ? pos : sf::Vector2i{0, 0};
}

void Room::setPosition(const sf::Vector2i& newPos) {
    auto oldPos = getPosition();
    auto delta = newPos - oldPos;
    for(auto& row : part){
        for(auto& el : row){
            el.move({static_cast<float>(delta.x * tileSize), static_cast<float>(delta.y * tileSize)});
        }
    }
}

sf::Vector2i Room::getCenterPosition() const {
    auto pos = getPosition();
    auto size = getSize();
    return {pos.x + size.x / 2, pos.y + size.y / 2};
}

void Room::setCenterPosition(const sf::Vector2i newPos) {
    auto oldPos = getCenterPosition();
    auto delta = newPos - oldPos;
    for(auto& row : part){
        for(auto& el : row){
            el.move({static_cast<float>(delta.x * tileSize), static_cast<float>(delta.y * tileSize)});
        }
    }
}

TiledShape& Room::getPart() {
    return part;
}

void WorldGenerator::init() {
    map = TiledShape(mapSize.y, TiledShape::value_type(mapSize.x));
    for(size_t i = 0; i < mapSize.y; i++){
        for(size_t j = 0; j < mapSize.x; j++){
            auto& el = map[i][j];
            el.setPosition({static_cast<float>(i * tileSize), static_cast<float>(j * tileSize)});
            el.setFillColor(sf::Color::Black);
            el.setSize({static_cast<float>(tileSize), static_cast<float>(tileSize)});
        }
    }
}

TiledShape WorldGenerator::generateDungeon() {
    init();

    float rad = generate8Bytes(0, 23);
    rad += 1.0f / (static_cast<int>(generate8Bytes(0, 4)) + 1);

    int numRooms = generate8Bytes(4, 6);

    for(int i = 0; i < numRooms; i++){
        Room part;
        auto& vector = part.getPart();
        size_t attempts = 0;
        while(!isConnected(vector)){
            part.init();
            for(int j = 0; j < 10; j++){
                auto rect = generateRectangle();
                part.addRectangle(rect);
            }
            attempts++;
        }

        part.setCenterPosition({static_cast<int>(mapSize.x / 2 + radius * cosf(rad)),
                                static_cast<int>(mapSize.y / 2 + radius * sinf(rad))});

        addRoom(part);

        addCorridor(mapSize / 2, part.getCenterPosition(), 2);

        rad += 3.14f / 180.f * (360 / numRooms);

    }
    return map;
}

void WorldGenerator::addRoom(Room& room) {
    auto elPos = room.getPart()[0][0].getPosition();
    size_t mapi = 0, mapj = 0;
    for(size_t i = 0; i < mapSize.y; i++){
        auto it = std::find_if(map[i].begin(), map[i].end(), [elPos](const auto& sh) {return sh.getPosition() == elPos; });
        if(it != map[i].end()){
            mapi = it->getPosition().x / tileSize;
            mapj = it->getPosition().y / tileSize;
        }
    }
    for(size_t i = 0; i < limitsTiles.y; i++){
        for(size_t j = 0; j < limitsTiles.x; j++){
            map[mapi + i][mapj + j].setFillColor(room.getPart()[i][j].getFillColor());
        }
    }
}

void WorldGenerator::addCorridor(const sf::Vector2i& startTile, const sf::Vector2i& endTile, int thickness) {
    int dx = abs(endTile.x - startTile.x);
    int dy = abs(endTile.y - startTile.y);
    int sx = startTile.x < endTile.x ? 1 : -1;
    int sy = startTile.y < endTile.y ? 1 : -1;
    int err = dx - dy;

    int x = startTile.x;
    int y = startTile.y;

    while (true) {
        for (int i = -thickness; i <= thickness; ++i) {
            for (int j = -thickness; j <= thickness; ++j) {
                int nx = x + i;
                int ny = y + j;
                if (nx >= 0 && nx < mapSize.x && ny >= 0 && ny < mapSize.y) {
                    map[nx][ny].setFillColor(sf::Color::White);
                }
            }
        }

        if (x == endTile.x && y == endTile.y) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void WorldGenerator::render(sf::RenderTarget& target) {
    for(const auto& row : map){
        for(const auto& sq : row) {
            target.draw(sq);
        }
    }
}

//get radius in pixels
float WorldGenerator::getRadius() {
    return radius * tileSize;
}

sf::Vector2f WorldGenerator::getCenter() {
    return {static_cast<float>(mapSize.x / 2 * tileSize), static_cast<float>(mapSize.y / 2 * tileSize)};
}
