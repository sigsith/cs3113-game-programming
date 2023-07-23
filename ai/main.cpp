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
#include <SDL_mixer.h>
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
class Player;
class Mob;

enum class GameResult {
  OnGoing,
  Win,
  Lose
};

class EntityManager {
 private:
  Background background_;
  Map map_;
  std::vector<Mob *> mobs_;
  GameResult result = GameResult::OnGoing;
  TextMap winning_;
  TextMap losing_;
 public:
  Player *player_;
  explicit EntityManager(Background background,
                         Map map,
                         Player *player,
                         std::vector<Mob *> mobs,
                         TextMap winning,
                         TextMap losing);
  void RenderAll(ShaderProgram *shader) const;
  void UpdateAll(float delta_t);
  const Map &map() const;
  std::vector<Mob *> &mobs();
};

class Dynamic : public Boxed {
 private:
  float half_height_;
  float half_width_;
  float horizontal_speed_ = 1.2;
  float vertical_speed = 0.08;
 protected:
  uint collision_time_out = 0;
  bool grounded = false;

  glm::vec3 velocity_{};
  glm::vec3 acceleration_{};
 protected:
  glm::vec3 position_;
  bool is_active_ = true;
  float gravity_ = -1.2;
 public:
  Dynamic(glm::vec3 startpos,
          GLuint text_id,
          float half_height,
          float half_width);
  virtual void Update(float delta_t, EntityManager &manager);
  virtual void Jump(float speed, const EntityManager &manager);
  void Disable();
  virtual void MoveLeft();
  virtual void MoveRight();
  virtual void StopHorizontal();
  bool IsActive() const;
  Box box() const override {
    return Box{
        position_,
        half_width_, half_height_
    };
  }
};
void Dynamic::Update(float delta_t, EntityManager &manager) {
  if (!(is_active_)) {
    return;
  }
  velocity_ += acceleration_ * delta_t;
  position_ += velocity_ * delta_t;
  Box box = this->box();
  if (SDL_GetTicks() > collision_time_out && manager.map().IsSolid(box).first) {
    velocity_.y = 0;
    acceleration_.y = 0;
    position_.y = manager.map().IsSolid(box).second;
    grounded = true;
  } else {
    acceleration_.y = gravity_;
  }
}
void Dynamic::Disable() {
  is_active_ = false;
}
Dynamic::Dynamic(glm::vec3 startpos,
                 GLuint text_id,
                 float half_height,
                 float half_width) :
    position_(startpos), half_height_(half_height), half_width_(half_width) {
  texture_id_ = text_id;

}
void Dynamic::MoveLeft() {
  velocity_.x = -horizontal_speed_;
}
void Dynamic::MoveRight() {
  velocity_.x = horizontal_speed_;
}
void Dynamic::Jump(float speed, const EntityManager &manager) {
  auto box = this->box();
  if (grounded) {
    velocity_.y += speed;
    collision_time_out = SDL_GetTicks() + 200;
    grounded = false;
  }
}
void Dynamic::StopHorizontal() {
  velocity_.x = 0;
}
bool Dynamic::IsActive() const {
  return is_active_;
}
enum class MobType {
  Patroller,
  Jumper,
  Chaser
};
enum class MobState {
  Idle,
  Aggro,
  Attack,
};
struct MobConfig {
  MobType mob_type;
};
class Mob : public Dynamic {
 private:
  MobState state_;
  MobConfig behavior_;
  uint timer_ = 0;
 public:
  void Update(float delta_t, EntityManager &manager) override;
  void Render(ShaderProgram *shader) const override;
  Mob(glm::vec3 startpos, GLuint text_id, MobConfig config);
  void Kill();
};
class Player : public Dynamic {
 public:
  void Update(float delta_t, EntityManager &manager) override;
  void Render(ShaderProgram *shader) const override;
  void Die();
  glm::vec3 position() const;
  Player(glm::vec3 startpos, GLuint text_id);
};
float Length(glm::vec3 glm_vec) {
  const auto x = glm_vec.x;
  const auto y = glm_vec.y;
  const auto z = glm_vec.z;
  return std::sqrt(x * x + y * y + z * 2);
}
glm::vec3 Normalize(glm::vec3 glm_vec) {
  const auto x = glm_vec.x;
  const auto y = glm_vec.y;
  const auto z = glm_vec.z;
  const auto length = std::sqrt(x * x + y * y + z * 2);
  return {x / length, y / length, z / length};
}
void Mob::Update(float delta_t, EntityManager &manager) {
  Dynamic::Update(delta_t, manager);
  if (!is_active_) {
    return;
  }
  switch (behavior_.mob_type) {
    case MobType::Jumper: {
      if (SDL_GetTicks() > timer_) {
        timer_ = SDL_GetTicks() + 3000;
        Jump(2.0, manager);
      }
      break;
    }
    case MobType::Patroller: {
      if (SDL_GetTicks() > timer_) {
        timer_ = SDL_GetTicks() + 4000;
        velocity_.x = -velocity_.x;
      }
      break;
    }
    case MobType::Chaser: {
      glm::vec3 direction = manager.player_->position() - this->position_;
      this->acceleration_ = 1.5f * Normalize(direction);
      if (Length(this->velocity_) > 2.0) {
        this->velocity_ = Normalize(this->velocity_) * 2.0f;
      }
      break;
    }
    default:break;
  }
}
void Mob::Render(ShaderProgram *shader) const {
  if (!is_active_) {
    return;
  }
  glBindTexture(GL_TEXTURE_2D, this->texture_id_);
  glVertexAttribPointer(shader->positionAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        SQUARE_VERTICES);
  glEnableVertexAttribArray(shader->positionAttribute);
  glVertexAttribPointer(shader->texCoordAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        FULL_TEX_COORDS);
  glEnableVertexAttribArray(shader->texCoordAttribute);
  constexpr auto base_matrix = glm::mat4(1.0f);
  constexpr auto scale_factor_x = 1.0;
  constexpr auto scale_factor_y = 1.0f;
  const auto model_matrix =
      glm::scale(glm::translate(base_matrix, position_),
                 glm::vec3(scale_factor_x, scale_factor_y, 1.0f));
  shader->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
Mob::Mob(glm::vec3 startpos, GLuint text_id, MobConfig config) :
    Dynamic(startpos, text_id, 0.3, 0.15), behavior_(config),
    state_(MobState::Idle) {

  switch (config.mob_type) {
    case MobType::Patroller: {
      velocity_.x = -1.0;
      timer_ = SDL_GetTicks() + 4000;
      break;
    }
    case MobType::Chaser: {
      gravity_ = 0;
      break;
    }
    default:break;
  }
}
void Mob::Kill() {
  is_active_ = false;
}

void Player::Update(float delta_t, EntityManager &manager) {
  Dynamic::Update(delta_t, manager);
  if (!is_active_) {
    return;
  }
  const auto player_box = this->box();
  for (auto &&mob : manager.mobs()) {
    if (!mob->IsActive()) {
      continue;
    }
    const auto mob_box = mob->box();
    if (player_box.IsCollisionWith(mob_box)) {
      if (player_box.IsOnTopOf(mob_box)) {
        mob->Kill();
      } else {
        this->Die();
        return;
      }
      std::cout << "Box hit!\n";
    }
  }
}
Player::Player(glm::vec3 startpos, GLuint text_id) : Dynamic(startpos,
                                                             text_id,
                                                             0.3,
                                                             0.15) {

}
void Player::Render(ShaderProgram *shader) const {
  if (!is_active_) {
    return;
  }
  glBindTexture(GL_TEXTURE_2D, this->texture_id_);
  glVertexAttribPointer(shader->positionAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        SQUARE_VERTICES);
  glEnableVertexAttribArray(shader->positionAttribute);
  glVertexAttribPointer(shader->texCoordAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        FULL_TEX_COORDS);
  glEnableVertexAttribArray(shader->texCoordAttribute);
  constexpr auto base_matrix = glm::mat4(1.0f);
  constexpr auto scale_factor_x = 1.0;
  constexpr auto scale_factor_y = 1.0f;
  const auto model_matrix =
      glm::scale(glm::translate(base_matrix, position_),
                 glm::vec3(scale_factor_x, scale_factor_y, 1.0f));
  shader->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
void Player::Die() {
  is_active_ = false;
}
glm::vec3 Player::position() const {
  return position_;
}

/* ---------------------------  GLOBAL VARIABLES --------------------------- */
SDL_Window *display_window;
bool is_game_running = true;
ShaderProgram shader;
float previous_ticks = 0.0f;
float time_accumulator = 0.0;
std::unique_ptr<EntityManager> manager;
GLuint font_id;
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
  const auto player_id = LoadTexture(std::string("player.png"));
  const auto player = new Player(glm::vec3(0, 0, 0), player_id);
  const auto mob0_id = LoadTexture(std::string("mob1.png"));
  const auto mob0_config = MobConfig{MobType::Jumper};
  const auto mob0 = new Mob(glm::vec3(-3, 1, 0), mob0_id, mob0_config);
  const auto mob2_id = LoadTexture(std::string("mob2.png"));
  const auto mob2_config = MobConfig{MobType::Patroller};
  const auto mob2 = new Mob(glm::vec3(1, -1, 0), mob2_id, mob2_config);
  const auto mob3_id = LoadTexture(std::string("mob3.png"));
  const auto mob3_config = MobConfig{MobType::Chaser};
  const auto mob3 = new Mob(glm::vec3(2, 2, 0), mob3_id, mob3_config);
  font_id = LoadTexture(std::string("font_sheet.png"));
  const auto
      font_mapping =
      SpriteSheetMapping(16, 6, LoadTexture(std::string("font-sheet.png")));

  TextMap winning =
      TextMap(std::string("You Win"), font_mapping, 1.0, glm::vec3(-3.5, 2, 0));
  TextMap losing =
      TextMap(std::string("You Lose"), font_mapping, 1.0, glm::vec3(-3.5, 2, 0));
  manager = std::make_unique<EntityManager>(background,
                                            map,
                                            player,
                                            std::vector<Mob *>{
                                                mob0, mob2, mob3
                                            }, winning, losing);
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
  if (key_state[SDL_SCANCODE_A]) {
    manager->player_->MoveLeft();
  }
  if (key_state[SDL_SCANCODE_D]) {
    manager->player_->MoveRight();
  }
  if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_SPACE]) {
    manager->player_->Jump(2.0, *manager);
  }
}
void Update() {
  const auto ticks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
  const auto delta_time = static_cast<float>(ticks - previous_ticks);
  previous_ticks = ticks;
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  for (time_accumulator += delta_time; time_accumulator >= FIXED_TIMESTEP;
       time_accumulator -= FIXED_TIMESTEP) {
    manager->UpdateAll(FIXED_TIMESTEP);
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
EntityManager::EntityManager(Background background,
                             Map map,
                             Player *player,
                             std::vector<Mob *> mobs,
                             TextMap winning,
                             TextMap losing)
    : background_(std::move(
    background)),
      map_(std::move(map)),
      player_(player),
      mobs_(std::move(mobs)),
      winning_(std::move(winning)),
      losing_(std::move(losing)) {

}
void EntityManager::RenderAll(ShaderProgram *shader) const {
  background_.Render(shader);
  map_.Render(shader);
  for (auto &&mob : mobs_) {
    mob->Render(shader);
  }
  player_->Render(shader);
  switch (result) {
    case GameResult::OnGoing: {

      break;
    }
    case GameResult::Win: {
      winning_.Render(shader);
      break;
    };

    case GameResult::Lose: {
      losing_.Render(shader);
      break;
    };
  }

}
const Map &EntityManager::map() const {
  return map_;
}
void EntityManager::UpdateAll(float delta_t) {
  player_->Update(delta_t, *this);
  if (!player_->IsActive()) {
    result = GameResult::Lose;
    return;
  }
  bool all_killed = true;
  for (auto &&mob : mobs_) {
    if (mob->IsActive()) {
      all_killed = false;
      mob->Update(delta_t, *this);
    }
  }
  if (all_killed) {
    result = GameResult::Win;
    return;
  }
}
std::vector<Mob *> &EntityManager::mobs() {
  return mobs_;
}
