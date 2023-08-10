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
#include "StaticEntity.h"
class Player;

enum class MobState {
  Roaming,
  Aggro,
};

class WaypointLooper {
  std::vector<glm::vec3> waypoints_;
  size_t current_waypoint_;
 public:
  WaypointLooper() = delete;
  explicit WaypointLooper(std::vector<glm::vec3> waypoints_);
  glm::vec3 current() const;
  void Proceed();
  void SwitchToClosest(glm::vec3 reference);
};

class Mob : public Tank {
 private:
  MobState state_ = MobState::Roaming;
  bool is_alive_ = true;
  WaypointLooper looper_;
 public:
  void Update(float delta_t,
              const Map &map,
              const Player &player,
              std::vector<std::unique_ptr<Projectile>> &,
              const std::vector<StaticEntity> &);
  Mob(glm::vec3 startpos, float start_orient, Specs specs, Paint paint,
      WaypointLooper looper);
  void Die();
  bool IsAlive() const;
  void MoveTowards(glm::vec3 target);
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_MOB_H_
