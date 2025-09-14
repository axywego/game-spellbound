#include "SceneManager.hpp"

#include "CharacterSelectScene.hpp"
#include "SettingsScene.hpp"

SceneManager::SceneManager(sf::RenderWindow& window_): window(window_) {
    // initializing;
    worlds.insert({"1_dungeon_world", {std::make_shared<GameWorld>(WorldGenerator::generateDungeon())}});
    worlds.insert({"2_dungeon_world", {std::make_shared<GameWorld>(WorldGenerator::generateDungeon())}});
    //player = PlayerFactory::create(PlayerClass::Mage, worlds["test"]->getTilemap());
    //player->setPosition({50 * 16 * 5 + 16 / 2, 50 * 16 * 5 + 16 / 2});
    initScenes();

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

            addScene<GameLevelScene>(window,
                "1_dungeon_scene",
                *worlds["1_dungeon_world"],
                getPlayer(),
                [this]() { pause(); },
                [this]() {
                    switchTo("2_dungeon_scene");
                    player->setPosition({50 * 16 * 5 + 16.f / 2, 50 * 16 * 5 + 16.f / 2});
                    player->setTilemap(worlds["2_dungeon_world"]->getTilemap());
                });
            addScene<GameLevelScene>(window,
                "2_dungeon_scene",
                *worlds["2_dungeon_world"],
                getPlayer(),
                [this]() { pause(); },
                [this]() {
                    switchTo("3_dungeon_scene");
                    player->setPosition({50 * 16 * 5 + 16.f / 2, 50 * 16 * 5 + 16.f / 2});
                    player->setTilemap(worlds["3_dungeon_world"]->getTilemap());
                });

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
    if (const auto sceneName = currentScene->getName(); sceneName.contains("dungeon")) {

        const size_t underscore_pos = sceneName.find('_');
        try {
            const int current_number = std::stoi(sceneName.substr(0, underscore_pos));
            const int next_number = current_number + 2;

            const std::string next_world_key = std::format("{}_dungeon_world", next_number);
            const std::string next_scene_key = std::format("{}_dungeon_scene", next_number);

            worlds[next_world_key] = std::make_shared<GameWorld>(WorldGenerator::generateDungeon());
            addScene<GameLevelScene>(window,
                next_scene_key,
                *worlds[next_world_key],
                getPlayer(),
                [this]() { pause(); },
                [this, next_number]() {
                    const std::string next_scene = std::to_string(next_number + 1) + "_dungeon_scene";
                    const std::string next_world = std::to_string(next_number + 1) + "_dungeon_world";

                    switchTo(next_scene);
                    player->setTilemap(worlds[next_world]->getTilemap());
                    player->setPosition({50 * 16 * 5 + 16.f / 2, 50 * 16 * 5 + 16.f / 2});
                }
            );
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to update game levels: " << e.what() << std::endl;
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
