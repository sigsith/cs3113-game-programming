/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_ENTITYMANAGER_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_ENTITYMANAGER_H_

#include "Map.h"
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
 public:
  Player *player_;
  explicit EntityManager(Background background,
                         Map map,
                         Player *player,
                         std::vector<Mob *> mobs);
  void RenderAll(ShaderProgram *shader) const;
  void UpdateAll(float delta_t);
  const Map &map() const;
  std::vector<Mob *> &mobs();
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_ENTITYMANAGER_H_
