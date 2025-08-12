# GAME SPELLBOUND

## Content

* [About game](https://github.com/axywego/game-spellbound?tab=readme-ov-file#about-game)
* [Used technologies](https://github.com/axywego/game-spellbound?tab=readme-ov-file#used-technologies)
* [How to compile and run](https://github.com/axywego/game-spellbound?tab=readme-ov-file#how-to-compile)

## About game
This is a 2D top-down game that combines the Rougelike and Action-Adventure genres. The player will have to descend deep into the dungeon, meeting various enemies along the way, the last of which holds the key to the hatch to the next floor of the dungeon. Before the "sortie", the player chooses the class of his hero - Knight, Mage or Archer. During the game, he will encounter various buffs that can increase his characteristics: health, mana, speed, damage, etc. Each dungeon floor and enemy placement are pseudo-randomly generated.

## Used technologies

* Language: C++
* Library: SFML

## How to compile

Write and enter in console
```
git clone https://github.com/axywego/game-spellbound
cd game-spellbound
```
Make directory `build` and go to it
```
mkdir build
cd build
```
Then run the configuration and compile the project using the following commands
```
cmake ..
make
```
To run game enter in console `./SPELLBOUND_GAME`

