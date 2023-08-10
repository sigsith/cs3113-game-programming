cmake_minimum_required(VERSION 3.10)
project(cs3113_game_programming)

set(CMAKE_CXX_STANDARD 14)

# Include Conan settings
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Use the Conan targets for your libraries
include_directories(common/includes)

function(add_executable_and_link name)
    set(SOURCES ${name}/main.cpp common/src/ShaderProgram.cpp)
    foreach (src ${ARGN})
        list(APPEND SOURCES ${name}/${src})
    endforeach ()
    
    add_executable(${name} ${SOURCES})
    target_link_libraries(${name} CONAN_PKG::sdl CONAN_PKG::sdl_image CONAN_PKG::sdl_mixer CONAN_PKG::opengl)
endfunction()

add_executable_and_link(last_stand Entity.cpp Map.cpp Utility.cpp
        Tank.cpp Player.cpp Mob.cpp Game.cpp Level.cpp Menu.cpp TextureObject.cpp
        Projectile.cpp StaticEntity.cpp)
