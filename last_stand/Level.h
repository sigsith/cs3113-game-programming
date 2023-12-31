/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_LEVEL_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_LEVEL_H_

#include "Scene.h"
#include "Mob.h"
#include "Player.h"
#include "Projectile.h"
#include "StaticEntity.h"
struct EventFrame;

class Level : public Scene {
 private:
  Level(Map map, std::vector<Mob> mobs, Player player);
  Map map_;
  std::vector<Mob> mobs_;
  std::vector<StaticEntity> static_entities_;
  Player player_;
  std::vector<std::unique_ptr<Projectile>> projectiles_;
 public:
  Level();
  uint Update(float delta_time, const EventFrame &event_frame) override;
  void Render(ShaderProgram *shader) const override;
  void RenderLife(ShaderProgram *shader, int life) const;
  glm::vec3 GetPlayerPosition() const override;
  static Map BuildMap();
  static std::vector<Mob> BuildMobs();
  bool HasKilledAll() const;
  int Id() const override;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_LEVEL_H_
