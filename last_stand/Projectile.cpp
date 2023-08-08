//
// Created by Yifei Yao on 8/8/23.
//

#include "Projectile.h"
#include "Utility.h"
Projectile::Projectile(TextureObject text_obj,
                       float orientation,
                       glm::vec3 origin)
    : text_obj_(text_obj),
      orientation_(orientation),
      origin_(origin),
      curr_pos_(origin),
      velocity_(utility::VectorByAngle(1.0, orientation)
      ) {
}
void Projectile::Render(ShaderProgram *program) const {
  text_obj_.Render(curr_pos_, orientation_, 1.0, program);
}
bool Projectile::Update(float delta_t) {
  curr_pos_ += velocity_ * delta_t;
  const auto distance = utility::Length(curr_pos_ - origin_);
  if (distance > 10.0) {
    return false;
  }
  return true;
}
