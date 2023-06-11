cmake_minimum_required(VERSION 3.24)
project(cs3113_game_programming)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_library(OPENGL_LIBRARY OpenGL)

add_executable(cs3113_game_programming simple-2d-scene/main.cpp simple-2d-scene/ShaderProgram.cpp)
target_link_libraries(cs3113_game_programming ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})