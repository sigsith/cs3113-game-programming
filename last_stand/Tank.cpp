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

void Tank::Update(float delta_t, const Map &map) {
  velocity_ += acceleration_ * delta_t;
  position_ += velocity_ * delta_t;
  orientation_ += angular_velocity_ * delta_t;
  angular_velocity_ += angular_acceleration_ * delta_t;
  Box box = this->box();
}
Tank::Tank(glm::vec3 start_position,
           float start_orientation,
           const std::string &chassis_name,
           const std::string &turret_name) : position_(start_position),
                                             orientation_(start_orientation),
                                             chassis_(chassis_name),
                                             turret_(turret_name) {

}
void Tank::Render(ShaderProgram *shader) const {
  chassis_.Render(position_, orientation_, 1.0, shader);
  const auto offset = utility::VectorByAngle(0.2, orientation_)
      + utility::VectorByAngle(turret_.height() / 100.0 / 2.0,
                               turret_orientation_);
  turret_.Render(position_ + offset,
                 utility::FlipAngle(turret_orientation_),
                 1.0,
                 shader);
}
