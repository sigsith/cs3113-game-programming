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
#include "Map.h"
/* -----------------------------  STD INCLUDES ----------------------------- */
#include <memory>
#include <iostream>
#include <cmath>
#include <utility>
/* ---------------------  GLOBAL CONSTANTS AND DEFINES --------------------- */
constexpr auto TOP_BOUNDARY = 3.75f;
constexpr auto RIGHT_BOUNDARY = 5.0f;

/* -------------------------  FORWARD DECLARATIONS ------------------------- */
class EntityManager {
 private:
  Background background_;
  Map map_;
 public:
  explicit EntityManager(Background background, Map map);
  void RenderAll(ShaderProgram *shader) const;
  const Map &map() const;
};

class Dynamic : public Boxed {
 private:
  bool is_active_ = true;
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::vec3 acceleration_;
  float half_height_;
  float half_width_;
  float horizontal_velocity_ = 0.5;

 public:
  Dynamic(glm::vec3 startpos, GLuint text_id);
  virtual void Update(float delta_t, const EntityManager &manager);
  void Disable();
  virtual void MoveLeft();
  virtual void MoveRight();
  Box box() const override {
    return Box{
        position_,
        half_width_, half_height_
    };
  }
};
void Dynamic::Update(float delta_t, const EntityManager &manager) {
  if (!(is_active_)) {
    return;
  }
  velocity_ += acceleration_ * delta_t;
  position_ += velocity_ * delta_t;
  Box box = this->box();
  if (manager.map().IsSolid(box)) {
    velocity_.y = 0;
    acceleration_.y = 0;
  } else {
    acceleration_.y = -1.0;
  }
}
void Dynamic::Disable() {
  is_active_ = false;
}
Dynamic::Dynamic(glm::vec3 startpos, GLuint text_id) :
    position_(startpos) {
  texture_id_ = text_id;

}
void Dynamic::MoveLeft() {
  velocity_.x = -horizontal_velocity_;
}
void Dynamic::MoveRight() {
  velocity_.x = horizontal_velocity_;
}
class Player : public Dynamic {
 public:
  void Update(float delta_t, const EntityManager &manager) override;
};
void Player::Update(float delta_t, const EntityManager &manager) {
  Dynamic::Update(delta_t, manager);
}

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
  display_window = SDL_CreateWindow("Generic Platform Game",
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
  const auto background = Background(std::string("background.png"));
  const auto mapping =
      std::vector<uint>{6, 7, 8, 6, 7, 8, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE,
                        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE,
                        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE,
                        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
  const auto index_mapping = LevelMapping(12, 5, mapping);
  const auto tile_set_id = LoadTexture(std::string("tileset.png"));
  const auto tile_set = SpriteSheetMapping(10, 6, tile_set_id);
  const auto top_left = glm::vec3(-4, 0, 0);
  const auto map = Map(index_mapping, tile_set, 0.5, top_left);
  manager = std::make_unique<EntityManager>(background, map);
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
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  for (time_accumulator += delta_time; time_accumulator >= FIXED_TIMESTEP;
       time_accumulator -= FIXED_TIMESTEP) {
    // Update things
  }
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
EntityManager::EntityManager(Background background, Map map)
    : background_(std::move(
    background)), map_(std::move(map)) {

}
void EntityManager::RenderAll(ShaderProgram *shader) const {
  background_.Render(shader);
  map_.Render(shader);
}
const Map &EntityManager::map() const {
  return map_;
}
