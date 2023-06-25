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

constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
        F_SHADER_PATH[] = "shaders/fragment.glsl";

// -------- SECTION GLOBAL CONSTANTS AND DEFINITIONS --------
// Compile time configurations.
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

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

// -------- SECTION GLOBAL VARIABLES --------
// Game level variable states; to be extensively shared.
// Keep this as small as possible.
SDL_Window *g_display_window;
bool g_game_is_running = true;
ShaderProgram program;

// -------- SECTION FUNCTIONS --------
void initialise() {
    SDL_Init(SDL_INIT_VIDEO);
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
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            g_game_is_running = false;
        }
    }
}

void update() {
    // float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    // float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
    // g_previous_ticks = ticks;

    // g_triangle_x += 1.0f * delta_time;
    // g_triangle_rotate += DEGREES_PER_SECOND * delta_time; // 90-degrees per second


    // /* Translate -> Rotate */
    // g_model_matrix = glm::translate(g_model_matrix, glm::vec3(g_triangle_x, 0.0f, 0.0f));
    // g_model_matrix = glm::rotate(g_model_matrix, glm::radians(g_triangle_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
}

void render_rectangle(float half_width, float half_height, glm::vec3 position) {
    constexpr auto base_matrix = glm::mat4(1.0f);
    const auto model_matrix = glm::translate(base_matrix, position);
    program.SetModelMatrix(model_matrix);
    float vertices[] =
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
    render_rectangle(1, 2, glm::vec3(1, -1, 0));
    render_rectangle(1, 1, glm::vec3(-1, -1, 0));
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }

/**
 Start here—we can see the general structure of a game loop without worrying too much about the details yet.
 */
int main() {
    initialise();

    while (g_game_is_running) {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
