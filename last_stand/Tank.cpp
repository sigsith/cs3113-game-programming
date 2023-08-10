/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Tank.h"
#include "Utility.h"
#include "Projectile.h"
#include <cmath>

void Tank::Update(float delta_t, const Map &map) {
  // Apply inertia
  position_ += velocity_ * delta_t;
  velocity_ += acceleration_ * delta_t;
  orientation_ += angular_velocity_ * delta_t;
  angular_velocity_ += angular_acceleration_ * delta_t;
  turret_orientation_ += turret_angular_velocity_ * delta_t;
  turret_angular_velocity_ += turret_angular_acceleration_ * delta_t;
  // Update control
  // Update gear
  const auto direction_vec = utility::VectorByAngle(1.0, orientation_);
  const auto speed_parallel = glm::dot(velocity_, direction_vec);
  const auto velocity_parallel = speed_parallel * direction_vec;
  const auto velocity_perpendicular = velocity_ - velocity_parallel;
  switch (mode_) {
    case Mode::Forward: {
      const auto real_acceleration =
          specs_.base_acceleration_forward_
              * std::min(
                  (specs_.top_speed_forward * speed_cap_ - speed_parallel)
                      / specs_.top_speed_forward * speed_cap_, 1.2f);
      acceleration_ = direction_vec * real_acceleration
          - specs_.lateral_friction * velocity_perpendicular;
      break;
    }
    case Mode::Reverse: {
      const auto real_acceleration =
          specs_.base_acceleration_reverse_
              * std::min(
                  (specs_.top_speed_backward * speed_cap_ + speed_parallel)
                      / specs_.top_speed_backward * speed_cap_, 1.2f);
      acceleration_ = -direction_vec * real_acceleration
          - specs_.lateral_friction * velocity_perpendicular;
      break;
    }
    case Mode::Halt: {
      acceleration_ = -specs_.forward_friction * velocity_parallel
          - specs_.lateral_friction * velocity_perpendicular;
      break;
    }
  }
  int rotation_direction = 0;
  if ((steering_ == Steering::Left && mode_ != Mode::Reverse)
      || (steering_ == Steering::Right) && mode_ == Mode::Reverse) {
    rotation_direction = 1;
  }
  if ((steering_ == Steering::Right && mode_ != Mode::Reverse)
      || (steering_ == Steering::Left) && mode_ == Mode::Reverse) {
    rotation_direction = -1;
  }
  switch (rotation_direction) {
    case 0: {
      angular_acceleration_ = -angular_velocity_ * specs_.rotation_friction;
      break;
    }
    case 1: {
      angular_acceleration_ = specs_.base_angular_acceleration * std::min(
          (specs_.top_rotation_speed - angular_velocity_)
              / specs_.top_rotation_speed, 1.2f);
      break;
    }
    case -1: {
      angular_acceleration_ = -specs_.base_angular_acceleration * std::min(
          (specs_.top_rotation_speed + angular_velocity_)
              / specs_.top_rotation_speed, 1.2f);
      break;
    }
    default: {
      throw std::runtime_error("Nonexistant rotation");
    }
  }
  // Update turret
  auto
      diff = fmod((target_angle - turret_orientation_),
                  glm::pi<float>() * 2);
  if (diff < 0) {
    diff += glm::pi<float>() * 2;
  }
  float targetAngularVelocity;
  if (diff < glm::pi<float>()) {
    targetAngularVelocity = specs_.top_turn_speed * (diff / glm::pi<float>());
    turret_angular_acceleration_ = specs_.base_turn_acceleration
        * (targetAngularVelocity - turret_angular_velocity_);
  } else {
    diff = glm::pi<float>() * 2 - diff;  // Convert the difference to a positive value between 0 and pi
    targetAngularVelocity = -specs_.top_turn_speed * (diff / glm::pi<float>());
    turret_angular_acceleration_ = specs_.base_turn_acceleration
        * (targetAngularVelocity - turret_angular_velocity_);
  }
  Box box = this->box();
}
void Tank::Render(ShaderProgram *shader) const {
  chassis_.Render(position_, orientation_, 1.0, shader);
  const auto offset = utility::VectorByAngle(turret_.height() / 100.0 / 2.0,
                                             turret_orientation_);
  turret_.Render(position_ + offset,
                 utility::FlipAngle(turret_orientation_),
                 1.0,
                 shader);
  if (SDL_GetTicks() < flash_time_out) {
    fire_smoke_.Render(position_ + 3.1f * offset,
                       utility::FlipAngle(turret_orientation_),
                       1.0,
                       shader);
  }
}
Tank::Tank(glm::vec3 start_position,
           float start_orientation,
           Specs specs,
           const Paint &paint) : position_(start_position),
                                 orientation_(start_orientation),
                                 specs_(specs),
                                 chassis_(paint.chassis_name),
                                 turret_(paint.turret_name),
                                 shell_(paint.shell_name),
                                 fire_smoke_(paint.smoke_name) {
  half_width = chassis_.width() / 100.0f / 2.0;
  half_height = chassis_.height() / 100.0f / 2.0;
}
void Tank::SetTurretTarget(float target_orientation) {
  target_angle = target_orientation;
}

std::unique_ptr<Projectile> Tank::TryFire() {
  if (SDL_GetTicks() > fire_time_out) {
    std::cout << "Fire!\n";
    fire_time_out =
        SDL_GetTicks() + static_cast<uint>(2000.0 / specs_.fire_rate);
    flash_time_out = SDL_GetTicks() + 50;
    static const auto thump = Mix_LoadWAV("tank_fire.wav");
    Mix_VolumeChunk(thump, MIX_MAX_VOLUME / 4);
    Mix_PlayChannel(-1, thump, 0);
    velocity_ -= utility::VectorByAngle(1.0, turret_orientation_);
    return std::make_unique<Projectile>(shell_,
                                        TextureObject("explosion2"),
                                        turret_orientation_,
                                        position_,
                                        specs_.projectile_speed);

  }
  return nullptr;
}
void Tank::SetGear(Mode mode, Steering steering) {
  mode_ = mode;
  steering_ = steering;
}
glm::vec3 Tank::position() const {
  return position_;
}
float Tank::turret_orientation() const {
  return turret_orientation_;
}
float Tank::chassis_orientation() const {
  return orientation_;
}
void Tank::SetSpeedCap(float cap) {
  speed_cap_ = cap;
}
glm::vec3 Tank::velocity() const {
  return velocity_;
}
void Tank::UpdateVelocity(glm::vec3 new_val) {
  velocity_ = new_val;
}
