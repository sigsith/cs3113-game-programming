//
// Created by Yifei Yao on 8/8/23.
//

#include "Projectile.h"
#include "Utility.h"
Projectile::Projectile(TextureObject shell,
                       TextureObject explosion,
                       float orientation,
                       glm::vec3 origin,
                       float speed)
    : shell_(shell),
      explosion_(explosion),
      orientation_(orientation),
      origin_(origin),
      curr_pos_(origin),
      velocity_(utility::VectorByAngle(speed, orientation)),
      speed_(speed) {
}
void Projectile::Render(ShaderProgram *program) const {
  switch (state_) {
    case ProjState::Flying: {
      shell_.Render(curr_pos_, orientation_, 1.0, program);
      break;
    }
    case ProjState::Exploded: {
      explosion_.Render(curr_pos_, 0.0, 1.0, program);
      break;
    }
  }
}
bool Projectile::Update(float delta_t) {
  switch (state_) {
    case ProjState::Flying: {
      curr_pos_ += velocity_ * delta_t;
      const auto distance = utility::Length(curr_pos_ - origin_);
      if (distance > speed_ * 1.0) {
        Explode();
      }
      return true;
    }
    case ProjState::Exploded: {
      if (SDL_GetTicks() > explosion_timeout_) {
        return false;
      }
      return true;
    }
  }
}
void Projectile::Explode() {
  explosion_timeout_ = SDL_GetTicks() + 1000;
  state_ = ProjState::Exploded;
}
glm::vec3 Projectile::position() const {
  return curr_pos_;
}
bool Projectile::HasExploded() {
  return state_ == ProjState::Exploded;
}
bool Projectile::IsSafetyOn() const {
  return utility::Length(position() - origin_) < 0.8;
}
