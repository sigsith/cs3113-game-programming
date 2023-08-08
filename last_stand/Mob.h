/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_MOB_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_MOB_H_

#include "Tank.h"
class Player;

enum class MobState {
  Roaming,
  Aggro,
};

class Mob : public Tank {
 private:
  MobState state_ = MobState::Roaming;
  bool is_alive_ = true;
 public:
  void Update(float delta_t, const Map &map, const Player &player,  std::vector<std::unique_ptr<Projectile>> &);
  Mob(glm::vec3 startpos, float start_orient, const std::string& chassis_name,
      const std::string& turret_name, const std::string& bullet_name);
  void Die();
  bool IsAlive() const;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_MOB_H_
