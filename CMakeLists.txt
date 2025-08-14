cmake_minimum_required(VERSION 3.30)
project(SPELLBOUND_GAME)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
)

FetchContent_MakeAvailable(SFML)

message(STATUS "SFML include dir: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML libraries: ${SFML_LIBRARIES}")

add_executable(SPELLBOUND_GAME main.cpp
        src/core/AnimationController.cpp
        src/core/Camera.cpp
        src/core/ResourceManager.cpp
        src/entities/enemy/Enemy.cpp
        src/entities/enemy/EnemyFactory.cpp
        src/entities/enemy/Rat.cpp
        src/entities/enemy/Shaman.cpp
        src/entities/mob/Mob.cpp
        src/entities/mob/MobCollision.cpp
        src/entities/mob/Projectile.cpp
        src/entities/player/Archer.cpp
        src/entities/player/Knight.cpp
        src/entities/player/Mage.cpp
        src/entities/player/Player.cpp
        src/entities/player/PlayerFactory.cpp
        src/generators/EnemyGenerator.cpp
        src/generators/WorldGenerator.cpp
        src/scenes/GameLevelScene.cpp
        src/scenes/MenuScene.cpp
        src/scenes/PauseScene.cpp
        src/scenes/SceneManager.cpp
        src/world/GameWorld.cpp
        src/world/Tilemap.cpp
        src/core/StringFormat.cpp
        src/core/Generator.cpp
        src/core/Raycast.cpp
        src/core/Raycast.hpp
        src/core/Utils.cpp
        src/core/Utils.hpp
        src/core/TransformAnimation.cpp
        src/core/TransformAnimation.hpp
        src/core/UI/UIObject.cpp
        src/core/UI/UIObject.hpp
        src/core/UI/Button.cpp
        src/core/UI/Button.hpp
        src/core/UI/HUD.cpp
        src/core/UI/HUD.hpp
        src/scenes/SettingsScene.cpp
        src/scenes/SettingsScene.hpp
        src/core/UI/Slider.cpp
        src/core/UI/Slider.hpp
)

target_link_libraries(SPELLBOUND_GAME
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
)

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SFML_DLLS
                "${CMAKE_BINARY_DIR}/_deps/sfml-build/bin/sfml-system-d-3.dll"
                "${CMAKE_BINARY_DIR}/_deps/sfml-build/bin/sfml-graphics-d-3.dll"
                "${CMAKE_BINARY_DIR}/_deps/sfml-build/bin/sfml-window-d-3.dll"
                "${CMAKE_BINARY_DIR}/_deps/sfml-build/bin/sfml-audio-d-3.dll"
                "${CMAKE_BINARY_DIR}/_deps/sfml-build/bin/sfml-network-d-3.dll"
        )

        foreach(DLL ${SFML_DLLS})
            if(EXISTS ${DLL})
                add_custom_command(TARGET SPELLBOUND_GAME POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        ${DLL}
                        $<TARGET_FILE_DIR:SPELLBOUND_GAME>
                        COMMENT "Copying ${DLL}"
                )
            else()
                message(WARNING "DLL not found: ${DLL}")
            endif()
        endforeach()
    endif()


    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(SPELLBOUND_GAME PROPERTIES LINK_FLAGS "-Wl,--subsystem,console")
    else()
        set_target_properties(SPELLBOUND_GAME PROPERTIES LINK_FLAGS "-Wl,--subsystem,windows")
    endif()


endif()