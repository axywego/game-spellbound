#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Player.hpp"
#include "UI.hpp"
#include "GameWorld.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "Enemy.hpp"
#include <unordered_map>
#include <memory>
#include <print>
#include <algorithm>
#include <ranges>
#include "EnemyGenerator.hpp"

class Scene {
protected:
    sf::RenderWindow& window;

    sf::Vector2f lastPlayerPos;

public:
    Scene(sf::RenderWindow& window_): window(window_) {}

    virtual void load() = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget& renderTarget) = 0;
    virtual void handleEvent(const std::optional<sf::Event>& event) = 0;
    virtual sf::Vector2f getCameraCenter() const = 0;

    virtual ~Scene() = default;
};

class MenuScene: public Scene {
private:

    sf::View view;
    sf::Vector2f target;

    sf::CircleShape circle;
    float circleScale { 1.f };
    float transitionSpeed { 2.f };
    bool isTransition { true };

    sf::Texture textureBackground;
    sf::Sprite backgroundImage;

    Button startButton;
    Button settingsButton;
    Button exitButton;

    std::function<void()> onStartClick;
    std::function<void()> onExitClick;

public:
    MenuScene(sf::RenderWindow& window_, std::function<void()> startCallback, std::function<void()> exitCallback): Scene(window_), textureBackground("img/backgroundMenu.png"),
     backgroundImage(textureBackground), startButton(sf::Texture("img/startBtn.png"), window), settingsButton(sf::Texture("img/settingsBtn.png"), window),
     exitButton(sf::Texture("img/exitBtn.png"), window), onStartClick(startCallback), onExitClick(exitCallback) {
        target = { 960.f, 540.f };
        view.setSize({1920.f, 1080.f});
        view.setCenter(target);
    
        circle.setRadius(1200.f);
        circle.setFillColor(sf::Color::Black);
        circle.setOrigin( {circle.getRadius(), circle.getRadius()} );
        circle.setPosition(target);

        startButton.setPosition({50.f, 500.f});
        settingsButton.setPosition({50.f, 600.f});
        exitButton.setPosition({50.f, 700.f});
    }

    void load() override {

    }

    void update(const float& dt) override {
        if(isTransition) {
            circleScale -= transitionSpeed * dt;

            if(circleScale <= 0.f){
                circleScale = 0.f;
                isTransition = false;
            }
            circle.setScale( {circleScale, circleScale} );
            
        }

    }

    void render(sf::RenderTarget& renderTarget) override {
        renderTarget.setView(view);

        renderTarget.draw(backgroundImage);

        startButton.render(renderTarget);
        settingsButton.render(renderTarget);
        exitButton.render(renderTarget);

        if(isTransition) renderTarget.draw(circle);
    }

    void handleEvent(const std::optional<sf::Event>& event) override {
        if(startButton.isClicked(event)) {
            onStartClick();
        }

        if(exitButton.isClicked(event)) {
            onExitClick();
        }

        if(event){
            if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
                if(key->code == sf::Keyboard::Key::Escape)
                    onExitClick();
            }
        }
    }

    sf::Vector2f getCameraCenter() const override {
        return view.getCenter();
    }
};

class GameLevelScene : public Scene {
private:
    Player& player;
    Camera camera;

    std::function<void()> requestPause;

    GameWorld& gameWorld;

    std::vector<std::unique_ptr<Enemy>>& enemies;

public:
    GameLevelScene(GameWorld& world_, Player& player_, sf::RenderWindow& window_, std::function<void()> pauseCallback):
     Scene(window_), player(player_), gameWorld(world_), camera(&(player.getSprite()), world_.getTilemap()), requestPause(pauseCallback), enemies(world_.getEnemies()) {}

    void load() override {

        auto generatedEnemies = EnemyGenerator::generateEnemies(gameWorld.getTilemap(), player);
        
        for(auto& [enemy, pos] : generatedEnemies){
            gameWorld.addEnemy(std::move(enemy), pos);
        }

        player.setPosition({50 * 16 * 5 + 16 / 2, 50 * 16 * 5 + 16 / 2});

        std::println("все заебись");
    }

    void update(const float& dt) override {

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& e) { 
                    return !e->getIsAlive();
                }),
            enemies.end()
        );

        player.update(dt);
 
        camera.update(dt); 

        player.updateProjectiles(dt);

        for(const auto& enemy : enemies) {

            //Player has distance damage
            for (auto& projectile : player.getProjectiles()) {
                if (projectile->getCollisionRect().findIntersection(enemy->getCollisionRect())) {
                    projectile->onHit(*enemy);
                }
            }

            //Player has melee damage
            if(auto area = player.getAttackArea()){
                if(area->findIntersection(enemy->getCollisionRect())){
                    enemy->takeDamage(player.getDamage());
                }
            }

            enemy->update(dt);
        }

        lastPlayerPos = player.getSprite().getPosition();

        HUD::update(player, camera.getCenter());
    }

    void render(sf::RenderTarget& renderTarget) override {
        camera.applyTo(renderTarget);

        gameWorld.render(renderTarget);

        player.render(renderTarget);

        for(const auto& enemy : enemies) {
            enemy->render(renderTarget);
        }

        HUD::render(renderTarget);
    }

    void handleEvent(const std::optional<sf::Event>& event) override {
        if(event){
            if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
                if(key->code == sf::Keyboard::Key::Escape)
                    requestPause();
                
                if(key->code == sf::Keyboard::Key::Space)
                    player.attack();
            }
        }
    }
    sf::Vector2f getCameraCenter() const override {
        return camera.getCenter();
    }
};

class PauseScene : public Scene {
private:
    sf::View view;
    
    //sf::Sprite backgroundImage;

    sf::RectangleShape overlay;
    Button resumeButton;
    Button menuButton;

    std::function<void()> onResumeClick;
    std::function<void()> onMenuClick;
public:
    PauseScene(sf::RenderWindow& window_,
     std::function<void()> resumeCallback, std::function<void()> menuCallback):
     Scene(window_), onResumeClick(resumeCallback), onMenuClick(menuCallback),
     resumeButton(sf::Texture("img/startBtn.png"), window),
     menuButton(sf::Texture("img/exitBtn.png"), window)   {

        view.setSize({1920.f, 1080.f});
        view.setCenter({960.f, 540.f});
    
        overlay.setSize({1920.f, 1080.f});
        overlay.setFillColor(sf::Color(255, 255, 255, 150));
    
        resumeButton.setPosition({760.f, 400.f});
        menuButton.setPosition({760.f, 500.f});

    }

    void load() override {

    }

    void update(const float& dt) override {

    }

    void render(sf::RenderTarget& renderTarget) override {
        renderTarget.setView(view);

        renderTarget.draw(overlay);

        resumeButton.render(renderTarget);
        menuButton.render(renderTarget);
    }

    void handleEvent(const std::optional<sf::Event>& event) override {
        if(event){
            if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
                if(key->code == sf::Keyboard::Key::Escape)
                    onResumeClick();
            }
        }

        if (resumeButton.isClicked(event)) {
            onResumeClick();
        }
        if (menuButton.isClicked(event)) {
            onMenuClick();
        }
    }
    sf::Vector2f getCameraCenter() const override {
        return view.getCenter();
    }
};

class SceneManager {
private:

    sf::RenderWindow& window;

    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::unordered_map<std::string, std::shared_ptr<GameWorld>> worlds;
    std::shared_ptr<Scene> currentScene, nextScene;

    GameState gameState;
    std::unique_ptr<Player> player;

    sf::CircleShape transitionShape;
    bool isTransition = false;
    float transitionTime = 0.f;
    const float maxTransitionTime = 0.5f;

    bool isPaused = false;
    std::shared_ptr<Scene> pausedScene;

public:
    SceneManager(sf::RenderWindow& window_): window(window_) {
        // initializing;
        worlds.insert({"test", {std::make_shared<GameWorld>(WorldGenerator::generateDungeon())}});

        //worlds.insert({"mapa_main", {std::make_shared<GameWorld>("map.json")}});
        
        player = PlayerFactory::create(PlayerClass::Mage, worlds["test"]->getTilemap());

        initScenes();
    }

    void initScenes() {

        addScene<GameLevelScene>("main", *worlds["test"], getPlayer(), window, [this]() { pause(); });

        addScene<MenuScene>("menu", window,
            [this]() { switchTo("main"); }, 
            [this]() { window.close(); }
        );

        addScene<PauseScene>("pause", window, [this]() { resume(); }, [this]() { switchTo("menu"); });
        
        switchTo("menu");
    }   

    Player& getPlayer() {
        return *player;
    }

    Scene& getScene(const std::string& name){
        return *scenes[name];
    }

    template<typename T, typename... Args>
    void addScene(const std::string& name, Args&&... args){
        scenes[name] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    void switchTo(const std::string& sceneName) {
        auto it = scenes.find(sceneName);
        if(it != scenes.end()){
            nextScene = it->second;
            startTransition();
        }
    }

    void startTransition() {
        isTransition = true;
        transitionTime = 0.f;
        
        transitionShape.setRadius(1500);
        transitionShape.setOrigin( { transitionShape.getRadius(), transitionShape.getRadius() } );
        transitionShape.setPosition( {1920.f / 2, 1080.f / 2} );
        transitionShape.setFillColor(sf::Color::Black);
        transitionShape.setScale( { 0.f, 0.f } );
    }

    void pause() {
        if(currentScene && !isPaused){
            pausedScene = currentScene;
            switchTo("pause");
            isPaused = true;
        }
    }

    void resume() {
        if(isPaused){
            currentScene = pausedScene;
            //pausedScene = nullptr;
            isPaused = false;
        }
    }

    void update(const float& dt) {
        if(currentScene && !isPaused){
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

    void render(sf::RenderTarget& renderTarget){
        if(currentScene) currentScene->render(renderTarget);
        if(isTransition) renderTarget.draw(transitionShape);
    }

    void handleEvent(const std::optional<sf::Event>& event) {
        if(!isTransition) {
            if(currentScene && event.has_value()) currentScene->handleEvent(event);
        }
    }

};

#endif