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

void Level::UpdateInput(const Uint8 *keyboard_state) {
  if (keyboard_state[SDL_SCANCODE_A]) {
    player_.MoveLeft();
  }
  if (keyboard_state[SDL_SCANCODE_D]) {
    player_.MoveRight();
  }
  if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_SPACE]) {
    player_.Jump(2.3);
  }
}
Feedback Level::Update(float delta_time) {
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
Level0::Level0() : Level(BuildMap(),
                         BuildMobs(),
                         Player(glm::vec3(0, 0, 0),
                                utility::LoadTexture(std::string("player.png")))) {}
Map Level0::BuildMap() {
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
  const auto tile_set_id = utility::LoadTexture(std::string("tileset.png"));
  const auto tile_set = SpriteSheetMapping(10, 6, tile_set_id);
  const auto top_left = glm::vec3(-4, 0, 0);
  auto map = Map(index_mapping, tile_set, 0.5, top_left);
  return map;
}
std::vector<Mob> Level0::BuildMobs() {
  const auto mob0_id = utility::LoadTexture(std::string("mob1.png"));
  const auto mob0_config = MobConfig{MobType::Jumper};
  const auto mob0 = Mob(glm::vec3(-3, 1, 0), mob0_id, mob0_config);
  const auto mob2_id = utility::LoadTexture(std::string("mob2.png"));
  const auto mob2_config = MobConfig{MobType::Patroller};
  const auto mob1 = Mob(glm::vec3(1, -1, 0), mob2_id, mob2_config);
  const auto mob3_id = utility::LoadTexture(std::string("mob3.png"));
  const auto mob3_config = MobConfig{MobType::Chaser};
  const auto mob2 = Mob(glm::vec3(2, 2, 0), mob3_id, mob3_config);
  return std::vector<Mob>{
      mob0, mob1, mob2
  };
}
bool Level0::ShouldGoNext() const {
  return player_.position().y < -4;
}
int Level0::Id() const {
  return 1;
}