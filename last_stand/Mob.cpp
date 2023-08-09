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
        break;
      }
      const auto
          waypoint_distance = utility::Length(looper_.current() - position());
      if (waypoint_distance < 1.5) {
        looper_.Proceed();
      }
      MoveTowards(looper_.current());
      break;
    }
    case MobState::Aggro: {
      if (distance > 7.0) {
        state_ = MobState::Roaming;
        looper_.SwitchToClosest(position());
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
         const std::string &bullet_name,
         WaypointLooper looper) : looper_(looper),
                                  Tank(startpos,
                                       start_orient,
                                       chassis_name,
                                       turret_name,
                                       bullet_name
                                  ) {
  SetGear(Mode::Forward, Steering::None
  );
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
  SetSpeedCap(
      glm::pi<float>() / (abs(diff - glm::pi<float>()) / 2 + glm::pi<float>()));
  // 2. Move
  SetGear(Mode::Forward, steer);
}
WaypointLooper::WaypointLooper(std::vector<glm::vec3> waypoints) : waypoints_(
    std::move(waypoints)), current_waypoint_(0) {
  if (waypoints_.empty()) {
    throw std::runtime_error("Waypoints cannot be empty!");
  }
}
glm::vec3 WaypointLooper::current() const {
  return waypoints_[current_waypoint_];
}
void WaypointLooper::Proceed() {
  current_waypoint_ += 1;
  if (current_waypoint_ >= waypoints_.size()) {
    current_waypoint_ = 0;
  }
}
void WaypointLooper::SwitchToClosest(glm::vec3 reference) {
  float min_dis = utility::Length(reference - waypoints_[0]);
  size_t closest = 0;
  for (size_t i = 1; i < waypoints_.size(); ++i) {
    if (utility::Length(reference - waypoints_[i]) < min_dis) {
      closest = i;
      min_dis = utility::Length(reference - waypoints_[i]);
    }
  }
  current_waypoint_ = closest;
}
