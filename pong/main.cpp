/**
* Author: Yifei Yao
* Assignment: Pong Clone
* Date due: 2023-06-25, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

// -------- SECTION DO NOT CHANGE --------
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
// -------- SECTION GLOBAL CONSTANTS AND DEFINITIONS --------
#define LOG(argument) std::cout << argument << '\n'
constexpr int WINDOW_WIDTH = 640,
        WINDOW_HEIGHT = 480;
constexpr float BG_RED = 0.5f,
        BG_BLUE = 0.5f,
        BG_GREEN = 0.5f,
        BG_OPACITY = 1.0f;
constexpr int VIEWPORT_X = 0,
        VIEWPORT_Y = 0,
        VIEWPORT_WIDTH = WINDOW_WIDTH,
        VIEWPORT_HEIGHT = WINDOW_HEIGHT;
constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
        F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
constexpr float MILLISECONDS_IN_SECOND = 1000.0;
constexpr int NUMBER_OF_TEXTURES = 1; // to be generated, that is
constexpr GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
constexpr GLint TEXTURE_BORDER = 0;   // this value MUST be zero
constexpr float PADDLE_TOP_BOUND = 3.5f;
constexpr float PADDLE_BOTTOM_BOUND = -3.5f;
constexpr float BALL_TOP_BOUND = 3.75f;
constexpr float BALL_BOTTOM_BOUND = -3.75f;
constexpr float BALL_LEFT_BOUND = -5.00f;
constexpr float BALL_RIGHT_BOUND = 5.00f;
constexpr float PADDLE_HEIGHT = 1.0f;
constexpr float PADDLE_WIDTH = 0.25f;
constexpr float BALL_DIMENSION = 0.2; // The ball is a square.
constexpr auto HALF_BALL_DIMENSION = BALL_DIMENSION / 2;
constexpr auto HALF_PADDLE_HEIGHT = PADDLE_HEIGHT / 2;
constexpr auto HALF_PADDLE_WIDTH = PADDLE_WIDTH / 2;
// -------- SECTION GLOBAL VARIABLES --------
SDL_Window *display_window;
bool game_is_running = true;
ShaderProgram program;
glm::mat4 view_matrix, model_matrix, projection_matrix;
float previous_ticks = 0.0f;
constexpr float PADDLE_SPEED = 1.0f;
constexpr float BALL_SPEED = 1.0f;
glm::vec3 left_paddle_position = glm::vec3(-4.0f, 0.0f, 0.0f);
glm::vec3 right_paddle_position = glm::vec3(4.0f, 0.0f, 0.0f);
glm::vec3 left_paddle_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 right_paddle_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_velocity = glm::vec3(1.0f, 1.0f, 0.0f);
glm::mat4 left_paddle_matrix, right_paddle_matrix, ball_matrix;

// -------- SECTION FUNCTIONS --------
void initialise() {
    SDL_Init(SDL_INIT_VIDEO);
    display_window = SDL_CreateWindow("Pong",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    program.Load(V_SHADER_PATH, F_SHADER_PATH);

    model_matrix = glm::mat4(1.0f);
    view_matrix = glm::mat4(1.0f);
    projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f,
                                   1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.

    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
    program.SetColor(1.0, 1.0, 1.0, 1.0);
    glUseProgram(program.programID);
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
}


void process_input() {
    SDL_Event event;
    // Handle quitting.
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                game_is_running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        game_is_running = false;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
    const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
    // No acceleration. Binary response to input.
    left_paddle_velocity = glm::vec3(0.0f);
    right_paddle_velocity = glm::vec3(0.0f);
    if (key_state[SDL_SCANCODE_W]) {
        left_paddle_velocity.y = 1.0f;
    } else if (key_state[SDL_SCANCODE_S]) {
        left_paddle_velocity.y = -1.0f;
    }
    if (key_state[SDL_SCANCODE_UP]) {
        right_paddle_velocity.y = 1.0f;
    } else if (key_state[SDL_SCANCODE_DOWN]) {
        right_paddle_velocity.y = -1.0f;
    }
}

void update() {
    float ticks = (float) SDL_GetTicks() /
                  MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks -
                       previous_ticks; // the delta time is the difference from the last frame
    previous_ticks = ticks;

    left_paddle_position += left_paddle_velocity * PADDLE_SPEED * delta_time;
    right_paddle_position += right_paddle_velocity * PADDLE_SPEED * delta_time;
    ball_position += ball_velocity * BALL_SPEED * delta_time;

    // Paddle boundary.
    if (left_paddle_position.y > PADDLE_TOP_BOUND)
        left_paddle_position.y = PADDLE_TOP_BOUND;
    if (left_paddle_position.y < PADDLE_BOTTOM_BOUND)
        left_paddle_position.y = PADDLE_BOTTOM_BOUND;
    if (right_paddle_position.y > PADDLE_TOP_BOUND)
        right_paddle_position.y = PADDLE_TOP_BOUND;
    if (right_paddle_position.y < PADDLE_BOTTOM_BOUND)
        right_paddle_position.y = PADDLE_BOTTOM_BOUND;

    // Ball top/bottom bouncing.
    if (ball_position.y > BALL_TOP_BOUND) {
        ball_position.y = BALL_TOP_BOUND;
        ball_velocity.y *= -1;
    }
    if (ball_position.y < BALL_BOTTOM_BOUND) {
        ball_position.y = BALL_BOTTOM_BOUND;
        ball_velocity.y *= -1;
    }

    // Ball paddle collision; detect boundary and bounce.

    const auto ball_left_edge = ball_position.x - HALF_BALL_DIMENSION;
    const auto ball_right_edge = ball_position.x + HALF_BALL_DIMENSION;
    const auto ball_top_edge = ball_position.y + HALF_BALL_DIMENSION;
    const auto ball_bottom_edge = ball_position.y - HALF_BALL_DIMENSION;


    const auto left_paddle_top_edge =
            left_paddle_position.y + HALF_PADDLE_HEIGHT;
    const auto left_paddle_bottom_edge =
            left_paddle_position.y - HALF_PADDLE_HEIGHT;
    const auto left_paddle_right_edge =
            left_paddle_position.x + HALF_PADDLE_WIDTH;
    const auto right_paddle_top_edge =
            right_paddle_position.y + HALF_PADDLE_HEIGHT;
    const auto right_paddle_bottom_edge =
            right_paddle_position.y - HALF_PADDLE_HEIGHT;
    const auto right_paddle_left_edge =
            right_paddle_position.x - HALF_PADDLE_WIDTH;

    if (ball_left_edge < left_paddle_right_edge &&
        !(ball_bottom_edge > left_paddle_top_edge ||
          ball_top_edge < left_paddle_bottom_edge)) {
        ball_velocity.x *= -1;
    }

    if (ball_right_edge > right_paddle_left_edge &&
        !(ball_bottom_edge > right_paddle_top_edge ||
          ball_top_edge < right_paddle_bottom_edge)) {
        ball_velocity.x *= -1;
    }

    if (ball_position.x > BALL_RIGHT_BOUND ||
        ball_position.x < BALL_LEFT_BOUND) {
        game_is_running = false;
    }
    left_paddle_matrix = glm::mat4(1.0f);
    right_paddle_matrix = glm::mat4(1.0f);
    ball_matrix = glm::mat4(1.0f);
    left_paddle_matrix = glm::translate(left_paddle_matrix,
                                        left_paddle_position);
    right_paddle_matrix = glm::translate(right_paddle_matrix,
                                         right_paddle_position);
    ball_matrix = glm::translate(ball_matrix, ball_position);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Vertices for the paddles and the ball
    float paddle_vertices[] = {
            -HALF_PADDLE_WIDTH, -HALF_PADDLE_HEIGHT,
            HALF_PADDLE_WIDTH, -HALF_PADDLE_HEIGHT,
            HALF_PADDLE_WIDTH, HALF_PADDLE_HEIGHT,

            -HALF_PADDLE_WIDTH, -HALF_PADDLE_HEIGHT,
            HALF_PADDLE_WIDTH, HALF_PADDLE_HEIGHT,
            -HALF_PADDLE_WIDTH, HALF_PADDLE_HEIGHT
    };

    float ball_vertices[] = {
            -HALF_BALL_DIMENSION, -HALF_BALL_DIMENSION,
            HALF_BALL_DIMENSION, -HALF_BALL_DIMENSION,
            HALF_BALL_DIMENSION, HALF_BALL_DIMENSION,

            -HALF_BALL_DIMENSION, -HALF_BALL_DIMENSION,
            HALF_BALL_DIMENSION, HALF_BALL_DIMENSION,
            -HALF_BALL_DIMENSION, HALF_BALL_DIMENSION
    };


    // Render left paddle
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, left_paddle_position);
    program.SetModelMatrix(model_matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                          paddle_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);

    // Render right paddle
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, right_paddle_position);
    program.SetModelMatrix(model_matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                          paddle_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);

    // Render ball
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, ball_position);
    program.SetModelMatrix(model_matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                          ball_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);

    SDL_GL_SwapWindow(display_window);
}

void shutdown() { SDL_Quit(); }


int main() {
    initialise();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
