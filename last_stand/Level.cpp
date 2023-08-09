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

#include "Game.h"

uint Level::Update(float delta_time, const EventFrame &event_frame) {
  const auto
      life_left =
      player_.Update(delta_time, event_frame, map_, mobs_, short_lived_);
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
  if (HasKilledAll()) {
    return 2;
  }
  if (life_left == 0) {
    return 3;
  }
  return 1;
}
void Level::Render(ShaderProgram *shader) const {
  const auto view = glm::translate(glm::mat4(1.0f), -player_.position());
  shader->SetViewMatrix(view);
  map_.Render(shader);
  for (auto &&static_entity : static_entities_) {
    static_entity.Render(shader);
  }
  for (auto &&mob : mobs_) {
    if (mob.IsAlive()) {
      mob.Render(shader);
    }
  }
  player_.Render(shader);
  for (auto &&item : short_lived_) {
    item->Render(shader);
  }
  RenderLife(shader, player_.life());
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
                       Player()) {

  // Add some trees
  for (int i = -6; i <= 6; i += 2) {
    static_entities_.emplace_back(TextureObject("treeGreen_large"),
                                  glm::vec3(-6, i, 0));
    static_entities_.emplace_back(TextureObject("treeGreen_large"),
                                  glm::vec3(6, i, 0));
    static_entities_.emplace_back(TextureObject("treeGreen_large"),
                                  glm::vec3(i, 6, 0));
    static_entities_.emplace_back(TextureObject("treeGreen_large"),
                                  glm::vec3(i, -6, 0));
  }

}
Map Level::BuildMap() {
  const auto index_mapping = LoadTerrain("terrain_mapping.txt");
  const auto tile_set_id = utility::LoadTexture(std::string("terrain.png"));
  const auto tile_set = SpriteSheetMapping(10, 4, tile_set_id);
  const auto top_left = glm::vec3(-1.0 * 49.0 / 2.0, 1.0 * 49.0 / 2.0, 0);
  auto map = Map(index_mapping, tile_set, 1.0, top_left);
  return map;
}
std::vector<Mob> Level::BuildMobs() {
  static const auto common_waypoint1 =
      WaypointLooper(std::vector<glm::vec3>{
                         glm::vec3(-7.0, 7.0, 0.0),
                         glm::vec3(-3.5, 7.0, 0.0),
                         glm::vec3(0.0, 7.0, 0.0),
                         glm::vec3(3.5, 7.0, 0.0),
                         glm::vec3(7.0, 7.0, 0.0),
                         glm::vec3(7.0, 3.5, 0.0),
                         glm::vec3(7.0, 0.0, 0.0),
                         glm::vec3(7.0, -3.5, 0.0),
                         glm::vec3(7.0, -7.0, 0.0),
                         glm::vec3(3.5, -7.0, 0.0),
                         glm::vec3(0.0, -7.0, 0.0),
                         glm::vec3(-3.5, -7.0, 0.0),
                         glm::vec3(-7.0, -7.0, 0.0),
                         glm::vec3(-7.0, -3.5, 0.0),
                         glm::vec3(-7.0, 0.0, 0.0),
                         glm::vec3(-7.0, 3.5, 0.0),
                     }
      );
  static constexpr auto STANDARD_MOB_SPECS =
      Specs{2.0, 1.5,
            1.0, 1.0, 2.0,
            3.0, 2.5, 1.5,
            1.0, 1.5, 5.5, 0.85};
  static const auto small_red = Paint{
      "tankBody_red",
      "tankRed_barrel1",
      "bulletRed1"
  };
  static const auto small_dark = Paint{
      "tankBody_dark",
      "tankDark_barrel1",
      "bulletDark1"
  };
  static const auto small_green = Paint{
      "tankBody_green",
      "tankGreen_barrel2",
      "bulletGreen3"
  };
  static const auto abrams = Paint{
      "tankBody_sand",
      "tankSand_barrel3",
      "bulletSand2"
  };
  static const auto chungus = Paint{
      "tankBody_huge",
      "specialBarrel1",
      "bulletRed2"
  };
  return std::vector<Mob>{
      Mob(glm::vec3(-7.0, 7.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          small_red,
          common_waypoint1),
      Mob(glm::vec3(0.0, 7.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          small_dark,
          common_waypoint1),
      Mob(glm::vec3(7.0, 7.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          small_red,
          common_waypoint1),
      Mob(glm::vec3(7.0, 0.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          chungus,
          common_waypoint1),
      Mob(glm::vec3(7.0, -7.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          small_dark,
          common_waypoint1),
      Mob(glm::vec3(0.0, -7.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          small_red,
          common_waypoint1),
      Mob(glm::vec3(-7.0, -7.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          small_green,
          common_waypoint1),
      Mob(glm::vec3(-7.0, 0.0, 0),
          0.0,
          STANDARD_MOB_SPECS,
          abrams,
          common_waypoint1),
  };
}
bool Level::HasKilledAll() const {
  for (const auto &mob : mobs_) {
    if (mob.IsAlive()) {
      return false;
    }
  }
  return true;
}
int Level::Id() const {
  return 1;
}