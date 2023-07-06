/**
* Author: Yifei Yao
* Assignment: Lunar Lander
* Date due: 2023-07-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"
/* ------------------------- SECTION FORBID CHANGE ------------------------- */
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

/* ------------------ SECTION GLOBAL CONSTANTS AND DEFINES ------------------ */
// Shared compile time configurations. Keep this as small as possible.

/* ------------------------ SECTION GLOBAL VARIABLES ------------------------ */
// Shared states. Keep this as small as possible.
SDL_Window *g_display_window;
bool is_game_running = true;
ShaderProgram program;
//glm::vec3 position, glm::vec3 velocity, float orientation
ship::Ship lunar = ship::Ship(glm::vec3(0), glm::vec3(1, 0, 0), 0);
float time_accumulator = 0.0;
float previous_ticks = 0.0;

/* --------------------------- SECTION FUNCTIONS --------------------------- */
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
    g_display_window = SDL_CreateWindow("Lunar Lander",
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
    auto view_matrix = glm::mat4(1.0f);
    const auto projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f,
                                              1.0f);
    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
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
    if (key_state[SDL_SCANCODE_S]) {
        lunar.SetEngine(true);
    } else {
        lunar.SetEngine(false);
    }
    const auto A_PRESSED = key_state[SDL_SCANCODE_A];
    const auto D_PRESSED = key_state[SDL_SCANCODE_D];
    if (A_PRESSED && !D_PRESSED) {
        lunar.SetRcs(1);
    } else if (D_PRESSED && !A_PRESSED) {
        lunar.SetRcs(-1);
    } else {
        lunar.SetRcs(0);
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
    constexpr auto MILLISECONDS_IN_SECOND = 1000.0f;
    const auto ticks = static_cast<float>(SDL_GetTicks()) /
                       MILLISECONDS_IN_SECOND;
    const auto delta_time = static_cast<float>(ticks - previous_ticks);
    previous_ticks = ticks;

    // ————— FIXED TIMESTEP ————— //
    // STEP 1: Keep track of how much time has passed since last step
    auto epoch = delta_time + time_accumulator;

    // STEP 2: Accumulate the amount of time passed while we're under our fixed timestep
    constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
    if (epoch < FIXED_TIMESTEP) {
        time_accumulator = epoch;
        return;
    }

    // STEP 3: Once we exceed our fixed timestep, apply that elapsed time into the objects' update function invocation
    while (epoch >= FIXED_TIMESTEP) {
        // Notice that we're using FIXED_TIMESTEP as our delta time
        lunar.Update(FIXED_TIMESTEP);
        epoch -= FIXED_TIMESTEP;
    }

    time_accumulator = epoch;
}

void
render_rectangle(float half_width, float half_height, glm::vec3 position) {
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
    lunar.Render(program);
    SDL_GL_SwapWindow(g_display_window);
}

// To play this game, use A, S, and D keys.
int main() {
    initialise();
    while (is_game_running) {
        process_input();
        update();
        render();
    }
    SDL_Quit();
    return 0;
}