#ifndef ENEMYGENERATOR_HPP
#define ENEMYGENERATOR_HPP

#include "Tilemap.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class EnemyGenerator {
private:

public:
    static std::vector<std::pair<std::unique_ptr<Enemy>, sf::Vector2f>> generateEnemies(Tilemap& map, Player& player){

        short numEnemies = 40; //must be (numEnemies mod 4 = 0)
        short numRat = numEnemies / 4;
        short numGoblin = numEnemies / 4;
        short numOgre = numEnemies / 4;
        short numShaman = numEnemies / 4;

        std::vector<std::pair<std::unique_ptr<Enemy>, sf::Vector2f>> res;

        std::vector<sf::Vector2f> positionsFloor;

        for(const auto& row : map.getTiles()){
            for(const auto& tile : row){
                if(tile.getType() == TileType::Floor) {
                    auto pos = tile.getGlobalBounds().position;
                    if(std::sqrtf(
                        std::powf(pos.x - WorldGenerator::getRadius(), 2) + std::powf(pos.y - WorldGenerator::getRadius(), 2)
                     ) >= WorldGenerator::getRadius()){
                        positionsFloor.push_back(pos);
                    }
                }
            }
        }

        std::cout << positionsFloor.size() << '\n';
        for(short i = 0; i < numRat; i++){
            auto index = generate8Bytes(0, positionsFloor.size()-1);
            auto e = EnemyFactory::create(EnemyClass::Rat, map, player);

            res.push_back(std::make_pair(std::move(e), positionsFloor[index]));
            positionsFloor.erase(positionsFloor.begin() + index);

            std::cout << (res[i].first ? "da" : "net") << '\n';

            std::cout << "kakashka222\n";
            std::cout << res[i].first->getIsAlive() << '\n';
            std::cout << "kakashka\n";

        }


        return res;
    }
};

#endif