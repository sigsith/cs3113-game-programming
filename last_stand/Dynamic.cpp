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

void Tank::Update(float delta_t, const Map &map) {
  velocity_ += acceleration_ * delta_t;
  position_ += velocity_ * delta_t;
  orientation_ += angular_velocity_ * delta_t;
  angular_velocity_ += angular_acceleration_ * delta_t;
  Box box = this->box();
}
Tank::Tank(glm::vec3 start_position, float start_orientation): position_(start_position),
      orientation_(start_orientation){

}
