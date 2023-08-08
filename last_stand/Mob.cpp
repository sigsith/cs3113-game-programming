/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Mob.h"
#include "Utility.h"
#include "Player.h"
void Mob::Update(float delta_t,
                 const Map &map,
                 const Player &player,
                 std::vector<std::unique_ptr<Projectile>> &projectiles) {
  Tank::Update(delta_t, map);
  glm::vec3 direction = player.position() - this->position_;
  const auto distance = utility::Length(direction);
  switch (state_) {
    case MobState::Roaming: {
      if (distance < 6.0) {
        state_ = MobState::Aggro;
      }
      break;
    }
    case MobState::Aggro: {
      if (distance > 7.0) {
        state_ = MobState::Roaming;
        break;
      }
      const auto
          angle = utility::GetTargetAngle(this->position_, player.position());
      if (abs(angle - turret_orientation_) < 0.2) {
        Fire(projectiles);
      } else {
        target_angle = angle;
      }
      break;
    }
  }
  for (auto &proj : projectiles) {
    if (!proj->HasExploded() && !proj->IsSafetyOn()
        && utility::Length(proj->position() - position_) < 0.5) {
      Die();
      return;
    }
  }
}
Mob::Mob(glm::vec3 startpos, float start_orient) :
    Tank(startpos,
         start_orient,
         "tankBody_blue",
         "tankBlue_barrel1",
         "bulletBlue1") {
}
bool Mob::IsAlive() const {
  return is_alive_;
}
void Mob::Die() {
  is_alive_ = false;
}
