#pragma once

#include <memory>
#include <unordered_map>
#include <fstream>
#include <nlohmann/json.hpp>

class Player;
class GameWorld;

class SaveSystem {
private:

	std::shared_ptr<Player> loadPlayer();
	std::shared_ptr<GameWorld> loadWorld();

	SaveSystem() = default;
	~SaveSystem() = default;

public:

	SaveSystem(const SaveSystem&) = delete;
	SaveSystem& operator=(const SaveSystem&) = delete;

	static SaveSystem& getInstance();

	void init();

	void savePlayer(std::weak_ptr<Player> player);
	void saveGameWorld(GameWorld& gameWorld);
	void saveSettings();

};