#pragma once

#include <memory>
#include <unordered_map>
#include <fstream>
#include <nlohmann/json.hpp>

class Player;
class Scene;

class SaveSystem {
private:

	std::shared_ptr<Player> loadPlayer();
	std::shared_ptr<Scene> loadScene();

	SaveSystem() = default;
	~SaveSystem() = default;

public:

	SaveSystem(const SaveSystem&) = delete;
	SaveSystem& operator=(const SaveSystem&) = delete;

	static SaveSystem& getInstance();

	void init();

	void savePlayer(const Player& player);
	void saveGameWorldScene(const std::shared_ptr<Scene>& scene);
	void saveSettings();

};