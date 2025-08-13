#include "SceneManager.hpp"

SceneManager::SceneManager(sf::RenderWindow& window_): window(window_) {
    // initializing;
    worlds.insert({"test", {std::make_shared<GameWorld>(WorldGenerator::generateDungeon())}});

    //worlds.insert({"mapa_main", {std::make_shared<GameWorld>("map.json")}});

    player = PlayerFactory::create(PlayerClass::Mage, worlds["test"]->getTilemap());

    initScenes();
}

void SceneManager::initScenes() {

    addScene<GameLevelScene>("main", *worlds["test"], getPlayer(), window, [this]() { pause(); });

    addScene<MenuScene>("menu", window,
                        [this]() { switchTo("main"); },
                        [this]() { window.close(); }
    );

    addScene<PauseScene>("pause", window, [this]() { resume(); }, [this]() { switchTo("menu"); });

    switchTo("menu");
}

std::weak_ptr<Player> SceneManager::getPlayer() {
    return player;
}

Scene& SceneManager::getScene(const std::string& name){
    return *scenes[name];
}

template<typename T, typename... Args>
void SceneManager::addScene(const std::string& name, Args&&... args){
    scenes[name] = std::make_unique<T>(std::forward<Args>(args)...);
}

void SceneManager::switchTo(const std::string& sceneName) {
    auto it = scenes.find(sceneName);
    if(it != scenes.end()){
        if (sceneName == "main") {
            isPaused = false;
            pausedScene = nullptr;
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
    transitionShape.setPosition( {1920.f / 2, 1080.f / 2} );
    transitionShape.setFillColor(sf::Color::Black);
    transitionShape.setScale( { 0.f, 0.f } );
}

void SceneManager::pause() {
    // Пауза только из игровой сцены
    if(currentScene && !isPaused && currentScene == scenes["main"]) {
        pausedScene = currentScene;
        switchTo("pause");
        isPaused = true;
    }
}

void SceneManager::resume() {
    if(isPaused){
        isPaused = false;
        pausedScene = nullptr;
        switchTo("main");
    }
}

void SceneManager::update(const float& dt) {
    if(currentScene){
        currentScene->update(dt);
    }
    if(isTransition){
        transitionTime += dt;
        float progress = transitionTime / maxTransitionTime;
        if(progress < 0.5f){
            float scale = progress * 2.f;
            transitionShape.setScale( { scale, scale } );
        }
        else {
            float scale = 2.f - progress * 2.f;
            transitionShape.setScale( { scale, scale } );

            if (nextScene && progress >= 0.5f && currentScene != nextScene) {
                currentScene = nextScene;
                currentScene->load();
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
