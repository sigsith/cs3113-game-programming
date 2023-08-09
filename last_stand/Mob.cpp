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
  glm::vec3 direction = player.position() - position();
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
      MoveTowards(player.position());
      const auto
          angle = utility::GetTargetAngle(position(), player.position());
      if (abs(angle - turret_orientation()) < 0.2) {
        auto projectile = TryFire();
        if (projectile) {
          projectiles.push_back(std::move(projectile));
        }
      } else {
        SetTurretTarget(angle);
      }
      break;
    }
  }
  for (auto &proj : projectiles) {
    if (!proj->HasExploded() && !proj->IsSafetyOn()
        && utility::Length(proj->position() - position()) < 0.5) {
      Die();
      return;
    }
  }
}
Mob::Mob(glm::vec3 startpos,
         float start_orient,
         const std::string &chassis_name,
         const std::string &turret_name,
         const std::string &bullet_name) :
    Tank(startpos,
         start_orient,
         chassis_name,
         turret_name,
         bullet_name) {
  SetGear(Mode::Forward, Steering::None);
}
bool Mob::IsAlive() const {
  return is_alive_;
}
void Mob::Die() {
  is_alive_ = false;
}
void Mob::MoveTowards(glm::vec3 target) {
  // Move towards a target.
  // 1. Adjust orientation. (similar to turret)
  const auto target_angle = utility::GetTargetAngle(position(), target);

  auto
      diff = fmod((target_angle - chassis_orientation()),
                  glm::pi<float>() * 2);
  if (diff < 0) {
    diff += glm::pi<float>() * 2;
  }
  auto steer = Steering::None;
  if (diff < glm::pi<float>()) {
    steer = Steering::Left;
  } else {
    steer = Steering::Right;
  }
  // 2. Move
  SetGear(Mode::Forward, steer);
}
