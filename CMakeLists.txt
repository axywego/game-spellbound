cmake_minimum_required(VERSION 3.14)  # Требуется для FetchContent
project(game_project)

set(CMAKE_CXX_STANDARD 23)

# Включаем FetchContent
include(FetchContent)

# Настраиваем SFML (скачиваем с GitHub)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
)

FetchContent_MakeAvailable(SFML nlohmann_json)

# Создаём исполняемый файл
add_executable(game main.cpp 
                include/Camera.hpp 
                include/GameState.hpp 
                include/Inventory.hpp
                include/Mob.hpp 
                include/Player.hpp 
                include/ResourceManager.hpp
                include/SceneManager.hpp
                include/UI.hpp
                include/World.hpp
                include/AnimationController.hpp
                include/Projectile.hpp
                include/Enemy.hpp
                )

# Подключаем SFML к проекту
target_link_libraries(game
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio  # Опционально
    sfml-network  # Опционально
    nlohmann_json::nlohmann_json
)