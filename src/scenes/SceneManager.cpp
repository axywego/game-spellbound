#include "SceneManager.hpp"

#include "CharacterSelectScene.hpp"
#include "SettingsScene.hpp"
#include "../core/SettingsManager.hpp"

SceneManager::SceneManager(sf::RenderWindow& window_): window(window_) {
    worlds.insert({"1_dungeon_world", {std::make_shared<GameWorld>(WorldGenerator::generateDungeon())}});
    initScenes();
}

void SceneManager::createDungeonScene(int levelNumber) {
    std::string worldKey = std::format("{}_dungeon_world", levelNumber);
    std::string sceneKey = std::format("{}_dungeon_scene", levelNumber);

    auto newWorld = std::make_shared<GameWorld>(WorldGenerator::generateDungeon());
    worlds[worldKey] = std::move(newWorld);

    auto onExitForScene = [this, levelNumber]() {
        createDungeonScene(levelNumber + 1);

        std::string newWorldKey = std::format("{}_dungeon_world", levelNumber + 1);
        std::string newSceneKey = std::format("{}_dungeon_scene", levelNumber + 1);

        player->setPosition({50 * 16 * 5 + 16.f / 2, 50 * 16 * 5 + 16.f / 2});
        player->setTilemap(worlds[newWorldKey]->getTilemap());

        switchTo(newSceneKey);

        std::cout << newWorldKey << ' ' << newSceneKey << std::endl;
    };

    auto onPauseForScene = [this]() {
        this->pause();
    };

    addScene<GameLevelScene>(
        window, sceneKey, *worlds[worldKey], getPlayer(), onPauseForScene, std::move(onExitForScene)
    );
}

void SceneManager::initScenes() {

    addScene<MenuScene>(window, "menu",
                        [this]() { switchTo("select_character"); },
                        [this]() { switchTo("settings"); },
                        [this]() { window.close(); }
    );

    addScene<CharacterSelectScene>(window, "select_character",
        [this](){ switchTo("menu"); },
        [&](const PlayerClass playerClass) {

            player = PlayerFactory::create(playerClass, worlds["1_dungeon_world"]->getTilemap());
            player->setPosition({50 * 16 * 5 + 16.f / 2, 50 * 16 * 5 + 16.f / 2});

            createDungeonScene(1);

            player->setTilemap(worlds["1_dungeon_world"]->getTilemap());
            switchTo("1_dungeon_scene");
        }
    );

    addScene<SettingsScene>(window, "settings",
        [this]() {
            switchTo("menu");
        },
        [this](float vol) {
            music->setVolume(vol);
        }
    );

    addScene<PauseScene>(window, "pause", [this]() { resume(); }, [this]() { switchTo("menu"); });

    switchTo("menu");

    music->setVolume(SettingsManager::getInstance().getMusicValue() * 100.f);
    music->play();
}

std::weak_ptr<Player> SceneManager::getPlayer() {
    return player;
}

Scene& SceneManager::getScene(const std::string& name){
    return *scenes[name];
}

template<typename T, typename... Args>
void SceneManager::addScene(Args&&... args){
    auto scene = std::make_unique<T>(std::forward<Args>(args)...);
    scenes[scene->getName()] = std::move(scene);
}

void SceneManager::switchTo(const std::string& sceneName) {
    if(const auto it = scenes.find(sceneName); it != scenes.end()){
        if (it->first.contains("dungeon")) {
            isPaused = false;
            pausedScene = {"", nullptr};
            music->pause();
            updateGameLevels();
        }
        nextScene = it->second;
        startTransition();
    }
}

void SceneManager::startTransition() {
    isTransition = true;
    transitionTime = 0.f;

    transitionShape.setRadius(1500);
    transitionShape.setOrigin( { transitionShape.getRadius(), transitionShape.getRadius() } );
    transitionShape.setPosition({1920.f / 2.f, 1080.f / 2.f});
    transitionShape.setFillColor(sf::Color::Black);
    transitionShape.setScale( { 0.f, 0.f } );
}

void SceneManager::pause() {

    for (const auto& [fst, snd]: scenes) {
        if (currentScene && !isPaused && currentScene == snd) {
            pausedScene = {fst, snd};
            switchTo("pause");
            isPaused = true;
            return;
        }
    }
}

void SceneManager::resume() {
    if(isPaused){
        isPaused = false;
        switchTo(pausedScene.first);
        pausedScene.second = nullptr;
    }
}

void SceneManager::updateGameLevels() {

    if (currentScene && currentScene->getName().find("dungeon") != std::string::npos) {
        std::string currentSceneName = currentScene->getName();

        if (const auto underscorePos = currentSceneName.find('_'); underscorePos != std::string::npos) {
            const int currentLevelNumber = std::stoi(currentSceneName.substr(0, underscorePos));
            int nextLevelNumber = currentLevelNumber + 1;

            if (const std::string nextSceneKey = std::format("{}_dungeon_scene", nextLevelNumber); !scenes.contains(nextSceneKey)) {
                createDungeonScene(nextLevelNumber);
            }
        }
    }
}

void SceneManager::update(const float& dt) {
    if(currentScene){
        currentScene->update(dt);
    }
    if(isTransition){
        transitionTime += dt;
        const float progress = transitionTime / maxTransitionTime;
        if(progress < 0.5f){
            float scale = progress * 2.f;
            transitionShape.setScale( { scale, scale } );

            if (currentScene)
                transitionShape.setPosition( currentScene->getCameraCenter() );
        }
        else {
            float scale = 2.f - progress * 2.f;
            transitionShape.setScale( { scale, scale } );

            if (nextScene && progress >= 0.5f && currentScene != nextScene) {
                currentScene = nextScene;
                currentScene->load();
                transitionShape.setPosition( currentScene->getCameraCenter() );
            }
        }
        if (progress >= 1.f){
            isTransition = false;
        }
    }
}

void SceneManager::render(sf::RenderTarget& renderTarget) const {
    if(currentScene) currentScene->render(renderTarget);
    if(isTransition) renderTarget.draw(transitionShape);
}

void SceneManager::handleEvent(const std::optional<sf::Event>& event) const {
    if(currentScene && !isTransition) {
        currentScene->handleEvent(event);
    }
}
