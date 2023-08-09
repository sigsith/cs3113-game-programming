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
  // Update control
  // Update gear
  const auto direction_vec = utility::VectorByAngle(1.0, orientation_);
  const auto speed_in_direction = glm::dot(velocity_, direction_vec);
  switch (mode_) {
    case Mode::Forward: {
      const auto real_acceleration =
          specs_.base_acceleration_forward_
              * std::min((specs_.top_speed_forward - speed_in_direction)
                             / specs_.top_speed_forward, 1.2f);
      acceleration_ = direction_vec * real_acceleration;
      break;
    }
    case Mode::Reverse: {
      const auto real_acceleration =
          specs_.base_acceleration_reverse_
              * std::min((specs_.top_speed_backward + speed_in_direction)
                             / specs_.top_speed_backward, 1.2f);
      acceleration_ = -direction_vec * real_acceleration;
      break;
    }
    case Mode::Halt: {
      acceleration_ = -specs_.cruising_friction * velocity_;
      break;
    }
  }
  switch (steering_) {
    case Steering::None: {
      angular_acceleration_ = -angular_velocity_ * specs_.rotation_friction;
      break;
    }
    case Steering::Left: {
      angular_acceleration_ = specs_.base_angular_acceleration * std::min(
          (specs_.top_rotation_speed - angular_velocity_)
              / specs_.top_rotation_speed, 1.2f);
      break;
    }
    case Steering::Right: {
      angular_acceleration_ = -specs_.base_angular_acceleration * std::min(
          (specs_.top_rotation_speed + angular_velocity_)
              / specs_.top_rotation_speed, 1.2f);
      break;
    }
  }
  // Update turret
  auto
      diff = fmod((target_angle - turret_orientation_),
                  glm::pi<float>() * 2);
  if (diff < 0) {
    diff += glm::pi<float>() * 2;
  }
  if (diff < glm::pi<float>()) {
    turret_angular_velocity_ = 1.0;
  } else {
    turret_angular_velocity_ = -1.0;
  }
  Box box = this->box();
}
Tank::Tank(glm::vec3 start_position,
           float start_orientation,
           const std::string &chassis_name,
           const std::string &turret_name,
           const std::string &shell_name) : position_(start_position),
                                            orientation_(start_orientation),
                                            chassis_(chassis_name),
                                            turret_(turret_name),
                                            shell_(shell_name) {

}
void Tank::Render(ShaderProgram *shader) const {
  chassis_.Render(position_, orientation_, 1.0, shader);
  const auto offset = utility::VectorByAngle(turret_.height() / 100.0 / 2.0,
                                             turret_orientation_);
  turret_.Render(position_ + offset,
                 utility::FlipAngle(turret_orientation_),
                 1.0,
                 shader);
}
Tank::Tank(glm::vec3 start_position,
           float start_orientation,
           Specs specs,
           const Paint &paint) : position_(start_position),
                                 orientation_(start_orientation),
                                 specs_(specs),
                                 chassis_(paint.chassis_name),
                                 turret_(paint.turret_name),
                                 shell_(paint.shell_name) {

}
void Tank::SetTurretTarget(float target_orientation) {
  target_angle = target_orientation;
}
std::unique_ptr<Projectile> Tank::TryFire() {
  if (SDL_GetTicks() > fire_time_out) {
    std::cout << "Fire!\n";
    fire_time_out = SDL_GetTicks() + 2000;
    return std::make_unique<Projectile>(shell_,
                                        TextureObject("explosion2"),
                                        turret_orientation_,
                                        position_);
  }
  return nullptr;//use optional in C++17+
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
