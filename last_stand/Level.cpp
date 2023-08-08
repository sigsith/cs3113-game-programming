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
  const auto
      player_feedback = player_.Update(delta_time, map_, mobs_, short_lived_);
  for (auto &&mob : mobs_) {
    if (mob.IsAlive()) {
      mob.Update(delta_time, map_, player_, short_lived_);
    }
  }
  for (size_t i = 0; i < short_lived_.size();) {
    if (!short_lived_[i]->Update(delta_time)) {
      std::swap(short_lived_[i], short_lived_.back());
      short_lived_.pop_back();
    } else {
      ++i;
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
    if (mob.IsAlive()) {
      mob.Render(shader);
    }
  }
  RenderLife(shader, life);
  player_.Render(shader);
  for (auto &&item : short_lived_) {
    item->Render(shader);
  }
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
                       Player()) {}
Map Level::BuildMap() {
  const auto mapping =
      std::vector<uint>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const auto index_mapping = LevelMapping(12, 12, mapping);
  const auto tile_set_id = utility::LoadTexture(std::string("terrain.png"));
  const auto tile_set = SpriteSheetMapping(10, 4, tile_set_id);
  const auto top_left = glm::vec3(-7, 8, 0);
  auto map = Map(index_mapping, tile_set, 1.0, top_left);
  return map;
}
std::vector<Mob> Level::BuildMobs() {
  return std::vector<Mob>{
      Mob(glm::vec3(-4, 4, 0),
          0.0,
          "tankBody_red",
          "tankRed_barrel2",
          "bulletRed1"),
      Mob(glm::vec3(4, 5, 0),
          0.0,
          "tankBody_dark",
          "tankDark_barrel3",
          "bulletSand1"),
      Mob(glm::vec3(-6, 1, 0),
          0.0,
          "tank_huge",
          "specialBarrel1",
          "bulletRed3")};
}
bool Level::ShouldGoNext() const {
  return player_.position().y < -4;
}
int Level::Id() const {
  return 1;
}