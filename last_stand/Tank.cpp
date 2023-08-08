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

void Tank::Update(float delta_t, const Map &map) {
  velocity_ += acceleration_ * delta_t;
  position_ += velocity_ * delta_t;
  orientation_ += angular_velocity_ * delta_t;
  turret_orientation_ += turret_angular_velocity_ * delta_t;
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
void Tank::Fire(std::vector<std::unique_ptr<Projectile>> &short_lived) {
  if (SDL_GetTicks() > fire_time_out) {
    std::cout << "Fire!\n";
    short_lived.push_back(std::make_unique<Projectile>(shell_,
                                                       TextureObject(
                                                           "explosion2"),
                                                       turret_orientation_,
                                                       position_));
    fire_time_out = SDL_GetTicks() + 2000;
  }
}
