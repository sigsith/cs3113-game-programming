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
Tank::Tank(glm::vec3 startpos,
           GLuint text_id,
           float half_height,
           float half_width) :
    position_(startpos), half_height_(half_height), half_width_(half_width) {
  texture_id_ = text_id;

}
