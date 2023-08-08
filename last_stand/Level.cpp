/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Level.h"
#include <sstream>

Feedback Level::Update(float delta_time) {
  const auto keyboard_state = SDL_GetKeyboardState(nullptr);
  if (keyboard_state[SDL_SCANCODE_A]) {
    player_.MoveLeft();
  }
  if (keyboard_state[SDL_SCANCODE_D]) {
    player_.MoveRight();
  }
  if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_SPACE]) {
    player_.Jump(2.3);
  }
  const auto player_feedback = player_.Update(delta_time, map_, mobs_);
  for (auto &&mob : mobs_) {
    if (mob.IsAlive()) {
      mob.Update(delta_time, map_, player_);
    }
  }
  if (player_feedback == PlayerFeedback::TakeDamage) {
    return Feedback::TakeDamage;
  }
  if (ShouldGoNext()) {
    return Feedback::NextStage;
  }
  return Feedback::NoOp;
}
void Level::Render(ShaderProgram *shader, int life) const {
  const auto view = glm::translate(glm::mat4(1.0f), -player_.position());
  shader->SetViewMatrix(view);
  map_.Render(shader);
  for (auto &&mob : mobs_) {
    mob.Render(shader);
  }
  RenderLife(shader, life);
  player_.Render(shader);
}
Level::Level(Map map,
             std::vector<Mob> mobs,
             Player player)
    : map_(std::move(map)),
      mobs_(std::move(mobs)),
      player_(std::move(player)) {
}
void Level::RenderLife(ShaderProgram *shader, int life) const {
  std::ostringstream oss;
  oss << "LIFE: " << static_cast<int>(life);
  const auto
      top_left = player_.position() + glm::vec3(-2.0, 3.5, 0);
  utility::RenderText(oss.str(), shader, 0.5, top_left);
}
glm::vec3 Level::GetPlayerPosition() const {
  return player_.position();
}
Level::Level() : Level(BuildMap(),
                         BuildMobs(),
                         Player(glm::vec3(0, 0, 0),
                                utility::LoadTexture(std::string("player.png")))) {}
Map Level::BuildMap() {
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
  const auto tile_set_id = utility::LoadTexture(std::string("objects.png"));
  const auto tile_set = SpriteSheetMapping(10, 6, tile_set_id);
  const auto top_left = glm::vec3(-4, 0, 0);
  auto map = Map(index_mapping, tile_set, 0.5, top_left);
  return map;
}
std::vector<Mob> Level::BuildMobs() {
  return std::vector<Mob>{};
}
bool Level::ShouldGoNext() const {
  return player_.position().y < -4;
}
int Level::Id() const {
  return 1;
}