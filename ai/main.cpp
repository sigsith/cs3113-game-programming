/**
* Author: Yifei Yao
* Assignment: Rise of the AI
* Date due: 2023-07-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
/* -------------------------------  CONTRACT ------------------------------- */
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define GL_GLEXT_PROTOTYPES 1
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
//#include "stb_image.h"
#include "Entity.h"
#include "Map.h"
/* -----------------------------  STD INCLUDES ----------------------------- */
#include <memory>
#include <iostream>
#include <cmath>
/* ---------------------  GLOBAL CONSTANTS AND DEFINES --------------------- */
constexpr auto TOP_BOUNDARY = 3.75f;
constexpr auto RIGHT_BOUNDARY = 5.0f;
/* ---------------------------  GLOBAL VARIABLES --------------------------- */
SDL_Window *display_window;
bool is_game_running = true;
ShaderProgram shader;
float previous_ticks = 0.0f;
float time_accumulator = 0.0;
std::unique_ptr<EntityManager> manager;
/* --------------------------  FUNCTION SIGNATURES -------------------------- */
void Initialize();
GLuint LoadTexture(const char *filepath);
void ProcessInput();
void Update();
void Render();
/* ---------------------------------  MAIN --------------------------------- */
int main() {
  Initialize();
  while (is_game_running) {
    ProcessInput();
    Update();
    Render();
  }
}
/* -----------------------  FUNCTION IMPLEMENTATIONS ----------------------- */
void Initialize() {
  constexpr int WINDOW_WIDTH = 640,
      WINDOW_HEIGHT = 480;
  constexpr int VIEWPORT_X = 0,
      VIEWPORT_Y = 0,
      VIEWPORT_WIDTH = WINDOW_WIDTH,
      VIEWPORT_HEIGHT = WINDOW_HEIGHT;
  SDL_Init(SDL_INIT_VIDEO);
  display_window = SDL_CreateWindow("Lunar Lander",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    640, 480,
                                    SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(display_window);
  SDL_GL_MakeCurrent(display_window, context);
#ifdef _WINDOWS
  glewInit();
#endif
  glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
  shader.Load("shaders/vertex_textured.glsl",
              "shaders/fragment_textured.glsl");
  const auto view_matrix = glm::mat4(1.0f);
  const auto
      projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
  shader.SetProjectionMatrix(projection_matrix);
  shader.SetViewMatrix(view_matrix);
  glUseProgram(shader.programID);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  const auto background = std::string("background.png");
  manager = std::make_unique<EntityManager>(background);
}
void ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
      case SDL_WINDOWEVENT_CLOSE:is_game_running = false;
        return;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_q:is_game_running = false;
            return;
          default:break;
        }
      default:break;
    }
  }
  const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
}
void Update() {
  const auto ticks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
  const auto delta_time = static_cast<float>(ticks - previous_ticks);
  previous_ticks = ticks;
  auto epoch = delta_time + time_accumulator;
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  if (epoch < FIXED_TIMESTEP) {
    time_accumulator = epoch;
    return;
  }
  while (epoch >= FIXED_TIMESTEP) {
    // Update all with fixed timestep
  }
  time_accumulator = epoch;
}
void DrawObject(glm::mat4 &object_model_matrix, GLuint &object_texture_id) {
  shader.SetModelMatrix(object_model_matrix);
  glBindTexture(GL_TEXTURE_2D, object_texture_id);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  // Render all
  manager->RenderAll(&shader);
  SDL_GL_SwapWindow(display_window);
}