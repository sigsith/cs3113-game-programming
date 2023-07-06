/**
 * Author: Yifei Yao
 * Assignment: Lunar Lander
 * Date due: 2023-07-07, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/
/* -----------------------------  FORBID CHANGE ----------------------------- */
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
#include "stb_image.h"
#include "Entity.h"
/* -----------------------------  STD INCLUDES ----------------------------- */
//#include <memory>
/* ---------------------  GLOBAL CONSTANTS AND DEFINES --------------------- */
constexpr auto TOP_BOUNDARY = 3.75f;
constexpr auto RIGHT_BOUNDARY = 5.0f;
/* ---------------------------  GLOBAL VARIABLES --------------------------- */
SDL_Window *display_window;
bool is_game_running = true;
ShaderProgram program;
float previous_ticks = 0.0f;
GLuint saucer_texture_id;
// To avoid default initialization.
std::unique_ptr<ship::Ship> apollo;
float time_accumulator = 0.0;
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
  SDL_Quit();
  return 0;
}
/* -----------------------  FUNCTION IMPLEMENTATIONS ----------------------- */
GLuint LoadTexture(const char *filepath) {
  int width, height, number_of_components;
  unsigned char *image = stbi_load(filepath, &width, &height,
                                   &number_of_components, STBI_rgb_alpha);
  if (image == nullptr) {
    throw std::runtime_error(
        "Unable to load image. Make sure the path is correct.");
  }
  GLuint textureID;
  // The value doesn't seem to matter?
  constexpr int NUMBER_OF_TEXTURES = 1;
  glGenTextures(NUMBER_OF_TEXTURES, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_image_free(image);
  return textureID;
}
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
  program.Load("shaders/vertex_textured.glsl",
               "shaders/fragment_textured.glsl");
  const auto view_matrix = glm::mat4(1.0f);
  const auto
      projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
  program.SetProjectionMatrix(projection_matrix);
  program.SetViewMatrix(view_matrix);
  glUseProgram(program.programID);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  saucer_texture_id = LoadTexture("lunar_lander.png");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  apollo = std::make_unique<ship::Ship>(glm::vec3(-4.0, 2.0, 0.0),
                                        glm::vec3(0.5, -0.8, 0.0),
                                        0.0,
                                        saucer_texture_id);
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
  if (key_state[SDL_SCANCODE_S]) {
    apollo->SetEngine(true);
  } else {
    apollo->SetEngine(false);
  }
  const auto A_PRESSED = key_state[SDL_SCANCODE_A];
  const auto D_PRESSED = key_state[SDL_SCANCODE_D];
  if (A_PRESSED && !D_PRESSED) {
    apollo->SetRcs(1);
  } else if (D_PRESSED && !A_PRESSED) {
    apollo->SetRcs(-1);
  } else {
    apollo->SetRcs(0);
  }
}
void Update() {
  // Calculate delta time.
  constexpr auto MILLISECONDS_IN_SECOND = 1000.0f;
  const auto ticks =
      static_cast<float>(SDL_GetTicks()) / MILLISECONDS_IN_SECOND;
  const auto delta_time = static_cast<float>(ticks - previous_ticks);
  previous_ticks = ticks;

  // ————— FIXED TIMESTEP ————— //
  // STEP 1: Keep track of how much time has passed since last step
  auto epoch = delta_time + time_accumulator;

  // STEP 2: Accumulate the amount of time passed while we're under our fixed
  // timestep
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  if (epoch < FIXED_TIMESTEP) {
    time_accumulator = epoch;
    return;
  }

  // STEP 3: Once we exceed our fixed timestep, apply that elapsed time into the
  // objects' update function invocation
  while (epoch >= FIXED_TIMESTEP) {
    // Notice that we're using FIXED_TIMESTEP as our delta time
    apollo->Update(FIXED_TIMESTEP);
    epoch -= FIXED_TIMESTEP;
  }

  time_accumulator = epoch;
}
void DrawObject(glm::mat4 &object_model_matrix, GLuint &object_texture_id) {
  program.SetModelMatrix(object_model_matrix);
  glBindTexture(GL_TEXTURE_2D, object_texture_id);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  apollo->Render(program);
  SDL_GL_SwapWindow(display_window);
}