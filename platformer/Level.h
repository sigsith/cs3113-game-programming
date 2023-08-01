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
class Level : public Scene {
 protected:
  Level(Background background, Map map, std::vector<Mob> mobs, Player player);
  Background background_;
  Map map_;
  std::vector<Mob> mobs_;
  Player player_;
 public:
  void UpdateInput(const Uint8 *keyboard_state) override;
  Feedback Update(float delta_time) override;
  void Render(ShaderProgram *shader, int life) const override;
  void RenderLife(ShaderProgram *shader, int life) const;
  virtual bool ShouldGoNext() const = 0;
};

class Level0 : public Level {
 public:
  Level0();
  static Map BuildMap();
  static std::vector<Mob> BuildMobs();
  bool ShouldGoNext() const override;
  int Id() const override;
};

class Level1 : public Level {
 public:
  Level1();
  static Map BuildMap();
  static std::vector<Mob> BuildMobs();
  bool ShouldGoNext() const override;
  int Id() const override;
};

class Level2 : public Level {
 public:
  Level2();
  static Map BuildMap();
  static std::vector<Mob> BuildMobs();
  bool ShouldGoNext() const override;
  int Id() const override;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_LEVEL_H_
