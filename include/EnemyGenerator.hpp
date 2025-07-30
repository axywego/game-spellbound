#ifndef ENEMYGENERATOR_HPP
#define ENEMYGENERATOR_HPP

#include "Tilemap.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class EnemyGenerator {
private:

public:
    static std::vector<std::pair<std::unique_ptr<Enemy>, sf::Vector2f>> generateEnemies(Tilemap& map, Player& player){

        short numEnemies = 100 * 4; //must be (numEnemies mod 4 = 0)
        short numRat = numEnemies / 4;
        short numGoblin = numEnemies / 4;
        short numOgre = numEnemies / 4;
        short numShaman = numEnemies / 4;

        std::vector<std::pair<std::unique_ptr<Enemy>, sf::Vector2f>> res;

        std::vector<sf::Vector2f> positionsFloor;

        for(const auto& row : map.getTiles()){
            for(const auto& tile : row){
                if(tile.getType() == TileType::Floor) {
                    const auto& bounds = tile.getGlobalBounds();
                    const auto& center = WorldGenerator::getCenter();
                    const float radius = WorldGenerator::getRadius() - (limitsTiles.x / 2) * tileSize;

                    float dx = bounds.position.x - center.x;
                    float dy = bounds.position.y - center.y;
                    float squaredDist = dx*dx + dy*dy;
                    if(dx*dx + dy*dy >= radius * radius){
                        positionsFloor.push_back(bounds.position);
                    }
                }
            }
        }

        for(short i = 0; i < numRat; i++){
            auto index = generate8Bytes(0, positionsFloor.size()-1);
            auto e = EnemyFactory::create(EnemyClass::Rat, map, player);

            res.push_back(std::make_pair(std::move(e), positionsFloor[index]));
            positionsFloor.erase(positionsFloor.begin() + index);
        }


        return res;
    }
};

#endif