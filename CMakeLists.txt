cmake_minimum_required(VERSION 3.24)
project(cs3113_game_programming)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_library(OPENGL_LIBRARY OpenGL)

include_directories(common/includes)

function(link_all name)
  target_link_libraries(${name} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
endfunction()

add_executable(simple_2d_scene simple-2d-scene/main.cpp common/src/ShaderProgram.cpp)
link_all(simple_2d_scene)

add_executable(pong pong/main.cpp common/src/ShaderProgram.cpp)
link_all(pong) 