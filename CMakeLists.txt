cmake_minimum_required(VERSION 3.10)
project(cs3113_game_programming)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_library(OPENGL_LIBRARY OpenGL)
find_library(SDL2_MIXER_LIBRARY SDL2_mixer)

include_directories(/opt/homebrew/Cellar/sdl2_mixer/2.6.3_1/include/SDL2)
include_directories(common/includes)

function(add_executable_and_link name)
    set(SOURCES ${name}/main.cpp common/src/ShaderProgram.cpp)
    foreach (src ${ARGN})
        list(APPEND SOURCES ${name}/${src})
    endforeach ()
    add_executable(${name} ${SOURCES})
    target_link_libraries(${name} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARY} ${OPENGL_LIBRARY})
endfunction()

add_executable_and_link(simple-2d-scene)
add_executable_and_link(pong)
add_executable_and_link(lunar_lander Entity.cpp)
add_executable_and_link(ai Entity.cpp Map.cpp)
add_executable_and_link(platformer Entity.cpp Map.cpp Utility.cpp Scene.cpp
        Dynamic.cpp Player.cpp Mob.cpp Game.cpp Level.cpp Menu.cpp)
add_executable_and_link(last_stand Entity.cpp Map.cpp Utility.cpp
        Dynamic.cpp Player.cpp Mob.cpp Game.cpp Level.cpp Menu.cpp)