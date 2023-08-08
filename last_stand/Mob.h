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
class Mob : public Tank {
 private:
  MobState state_;
  MobConfig behavior_;
  uint timer_ = 0;
  bool is_alive_ = true;
 public:
  void Update(float delta_t, const Map &map, const Player &player);
  void Render(ShaderProgram *shader) const override;
  Mob(glm::vec3 startpos, GLuint text_id, MobConfig config);
  void Die();
  bool IsAlive() const;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_MOB_H_
