/**
* Author: Yifei Yao
* Assignment: Pong Clone
* Date due: 2023-06-25, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
// -------- SECTION FORBID CHANGE --------
// Contract code such as header files, shader path.
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <algorithm>

constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
        F_SHADER_PATH[] = "shaders/fragment.glsl";

// -------- SECTION GLOBAL CONSTANTS AND DEFINITIONS --------
// Shared compile time configurations. Keep this as small as possible.
constexpr auto HALF_BALL_DIMENSION = 0.1f;   // The ball is a square.
constexpr auto HALF_PADDLE_HEIGHT = 0.5f;
constexpr auto HALF_PADDLE_WIDTH = 0.125f;
constexpr auto PADDLE_X_AXIS = 4.0f;

// -------- SECTION GLOBAL VARIABLES --------
// Shared states. Keep this as small as possible.
SDL_Window *g_display_window;
bool is_game_running = true;
ShaderProgram program;
glm::vec3 left_paddle_position = glm::vec3(-PADDLE_X_AXIS, 0.0f, 0.0f);
glm::vec3 right_paddle_position = glm::vec3(PADDLE_X_AXIS, 0.0f, 0.0f);
glm::vec3 left_paddle_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 right_paddle_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_velocity = glm::vec3(1.0f, 1.0f, 0.0f);
double previous_ticks = 0.0f;

// -------- SECTION FUNCTIONS --------
void initialise() {
    SDL_Init(SDL_INIT_VIDEO);
    constexpr int WINDOW_WIDTH = 640,
            WINDOW_HEIGHT = 480;

    constexpr float BG_RED = 0.0f,
            BG_BLUE = 0.0f,
            BG_GREEN = 0.0f,
            BG_OPACITY = 1.0f;

    constexpr int VIEWPORT_X = 0,
            VIEWPORT_Y = 0,
            VIEWPORT_WIDTH = WINDOW_WIDTH,
            VIEWPORT_HEIGHT = WINDOW_HEIGHT;

    constexpr double TRIANGLE_RED = 1.0,
            TRIANGLE_BLUE = 1.0,
            TRIANGLE_GREEN = 1.0,
            TRIANGLE_OPACITY = 1.0;
    g_display_window = SDL_CreateWindow("Hello, Pong!",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    program.Load(V_SHADER_PATH, F_SHADER_PATH);
    auto view_matrix = glm::mat4(
            1.0f);  // Defines the position (location and orientation) of the camera
    const auto projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f,
                                              -1.0f,
                                              1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
    // Notice we haven't set our model matrix yet!

    program.SetColor(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN,
                     TRIANGLE_OPACITY);

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
                is_game_running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        is_game_running = false;
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

template<typename T>
void inplace_clamp(T &value, const T &lower, const T &upper) {
    value = std::max(lower, std::min(value, upper));
}

template<typename T>
bool within(T &value, const T &lower, const T &upper) {
    return value >= lower && value <= upper;
}

void update() {
    // Calculate delta time.
    constexpr auto MILLISECONDS_IN_SECOND = 1000.0;
    const auto ticks = SDL_GetTicks() /
                       MILLISECONDS_IN_SECOND;
    const auto delta_time = static_cast<float>(ticks - previous_ticks);
    previous_ticks = ticks;

    // Update positions.
    constexpr auto PADDLE_SPEED = 6.0f;
    left_paddle_position += left_paddle_velocity * PADDLE_SPEED * delta_time;
    right_paddle_position += right_paddle_velocity * PADDLE_SPEED * delta_time;
    constexpr auto BALL_SPEED = 2.5f;
    ball_position += ball_velocity * BALL_SPEED * delta_time;

    // Check if ball is lost.
    constexpr auto RIGHT_BOUNDARY = 5.0f;
    constexpr auto LEFT_BOUNDARY = -RIGHT_BOUNDARY;
    if (!within(ball_position.x, LEFT_BOUNDARY, RIGHT_BOUNDARY)) {
        is_game_running = false;
        return;
    }

    // Absolute screen boundary.
    constexpr auto TOP_BOUNDARY = 3.75f;
    constexpr auto BOTTOM_BOUNDARY = -TOP_BOUNDARY;

    // Keep paddles position in boundary.
    constexpr auto PADDLE_TOP_BOUND = TOP_BOUNDARY - HALF_PADDLE_HEIGHT;
    constexpr auto PADDLE_BOTTOM_BOUND = BOTTOM_BOUNDARY + HALF_PADDLE_HEIGHT;
    inplace_clamp(left_paddle_position.y, PADDLE_BOTTOM_BOUND,
                  PADDLE_TOP_BOUND);
    inplace_clamp(right_paddle_position.y, PADDLE_BOTTOM_BOUND,
                  PADDLE_TOP_BOUND);

    // Ball top/bottom bouncing.
    constexpr auto BALL_TOP_BOUND = TOP_BOUNDARY - HALF_BALL_DIMENSION;
    constexpr auto BALL_BOTTOM_BOUND = BOTTOM_BOUNDARY + HALF_BALL_DIMENSION;
    if (!within(ball_position.y, BALL_BOTTOM_BOUND, BALL_TOP_BOUND)) {
        inplace_clamp(ball_position.y, BALL_BOTTOM_BOUND, BALL_TOP_BOUND);
        ball_velocity.y *= -1;
    }

    // Ball-paddle collision. Only consider collision with the inner longer side of the
    // paddles. Only reference the central left and central right points of the ball
    const auto central_left_x = ball_position.x - HALF_BALL_DIMENSION;
    const auto central_right_x = ball_position.x + HALF_BALL_DIMENSION;
    constexpr auto LEFT_PADDLE_INNER = -PADDLE_X_AXIS + HALF_PADDLE_WIDTH;
    constexpr auto RIGHT_PADDLE_INNER = PADDLE_X_AXIS - HALF_PADDLE_WIDTH;
    const auto left_paddle_top = left_paddle_position.y + HALF_PADDLE_HEIGHT;
    const auto left_paddle_bottom = left_paddle_position.y - HALF_PADDLE_HEIGHT;
    const auto right_paddle_top = right_paddle_position.y + HALF_PADDLE_HEIGHT;
    const auto right_paddle_bottom =
            right_paddle_position.y - HALF_PADDLE_HEIGHT;

    // See if the central left reference point is inside left paddle.
    if (central_left_x < LEFT_PADDLE_INNER &&
        within(ball_position.y,
               left_paddle_bottom, left_paddle_top)) {
        ball_position.x = LEFT_PADDLE_INNER + HALF_BALL_DIMENSION;
        ball_velocity.x *= -1;
        return;
    }
    // See if the central right reference point is inside right paddle.
    if (central_right_x > RIGHT_PADDLE_INNER &&
        within(ball_position.y, right_paddle_bottom, right_paddle_top)) {
        ball_position.x = RIGHT_PADDLE_INNER - HALF_BALL_DIMENSION;
        ball_velocity.x *= -1;
        return;
    }
}

void render_rectangle(float half_width, float half_height, glm::vec3 position) {
    constexpr auto base_matrix = glm::mat4(1.0f);
    const auto model_matrix = glm::translate(base_matrix, position);
    program.SetModelMatrix(model_matrix);
    const float vertices[] =
            {
                    -half_width, -half_height,
                    half_width, -half_height,
                    half_width, half_height,
                    -half_width, -half_height,
                    half_width, half_height,
                    -half_width, half_height,
            };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                          vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);

}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_rectangle(HALF_PADDLE_WIDTH, HALF_PADDLE_HEIGHT,
                     left_paddle_position);
    render_rectangle(HALF_PADDLE_WIDTH, HALF_PADDLE_HEIGHT,
                     right_paddle_position);
    render_rectangle(HALF_BALL_DIMENSION, HALF_BALL_DIMENSION,
                     ball_position);
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }

/**
 Start here—we can see the general structure of a game loop without worrying too much about the details yet.
 */
int main() {
    initialise();

    while (is_game_running) {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
