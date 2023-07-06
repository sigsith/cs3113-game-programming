cmake_minimum_required(VERSION 3.10)
project(cs3113_game_programming)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_library(OPENGL_LIBRARY OpenGL)

include_directories(common/includes)

function(add_executable_and_link name)
    add_executable(${name} ${name}/main.cpp common/src/ShaderProgram.cpp ${name}/${ARGN})
    target_link_libraries(${name} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
endfunction()

add_executable_and_link(simple-2d-scene)
add_executable_and_link(pong)
add_executable_and_link(lunar_lander2)