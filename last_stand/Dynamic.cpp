/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Dynamic.h"

void Dynamic::Update(float delta_t, const Map &map) {
  velocity_ += acceleration_ * delta_t;
  position_ += velocity_ * delta_t;
  Box box = this->box();
//  if (SDL_GetTicks() > collision_time_out && map.IsSolid(box).first
//      && velocity_.y < 0) {
//    velocity_.y = 0;
//    acceleration_.y = 0;
//    position_.y = map.IsSolid(box).second;
//    grounded = true;
//  }
}
Dynamic::Dynamic(glm::vec3 startpos,
                 GLuint text_id,
                 float half_height,
                 float half_width) :
    position_(startpos), half_height_(half_height), half_width_(half_width) {
  texture_id_ = text_id;

}
void Dynamic::MoveLeft() {
  velocity_.x = -horizontal_speed_;
}
void Dynamic::MoveRight() {
  velocity_.x = horizontal_speed_;
}
void Dynamic::Jump(float speed) {
  auto box = this->box();
  if (grounded) {
    velocity_.y += speed;
    collision_time_out = SDL_GetTicks() + 200;
    grounded = false;
  }
}
void Dynamic::StopHorizontal() {
  velocity_.x = 0;
}
