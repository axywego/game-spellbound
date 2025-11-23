#pragma once
#include <memory>

class GameWorld;
class Player;
class Tilemap;

class SaveLoaderManager {
private:
	SaveLoaderManager() = default;
	~SaveLoaderManager() = default;
public:
	SaveLoaderManager(const SaveLoaderManager&) = delete;
	SaveLoaderManager& operator=(const SaveLoaderManager&) = delete;

	static SaveLoaderManager& getInstance();

	Tilemap getSavedTilemap();

	std::shared_ptr<GameWorld> getSavedGameWorld(const std::weak_ptr<Player> &player);

	std::shared_ptr<Player> getSavedPlayer();

};

