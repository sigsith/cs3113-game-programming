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
void Mob::Update(float delta_t, const Map &map, const Player &player) {
  Dynamic::Update(delta_t, map);
  if (!is_alive_) {
    return;
  }
  switch (behavior_.mob_type) {
    case MobType::Jumper: {
      if (SDL_GetTicks() > timer_) {
        timer_ = SDL_GetTicks() + 3000;
      }
      break;
    }
    case MobType::Patroller: {
      if (SDL_GetTicks() > timer_) {
        timer_ = SDL_GetTicks() + 4000;
        velocity_.x = -velocity_.x;
      }
      break;
    }
    case MobType::Chaser: {
      glm::vec3 direction = player.position() - this->position_;
      this->acceleration_ = 1.5f * utility::Normalize(direction);
      if (utility::Length(this->velocity_) > 2.0) {
        this->velocity_ = utility::Normalize(this->velocity_) * 2.0f;
      }
      break;
    }
    default:break;
  }
}
void Mob::Render(ShaderProgram *shader) const {
  if (!is_alive_) {
    return;
  }
  glBindTexture(GL_TEXTURE_2D, this->texture_id_);
  glVertexAttribPointer(shader->positionAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        SQUARE_VERTICES);
  glEnableVertexAttribArray(shader->positionAttribute);
  glVertexAttribPointer(shader->texCoordAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        FULL_TEX_COORDS);
  glEnableVertexAttribArray(shader->texCoordAttribute);
  constexpr auto base_matrix = glm::mat4(1.0f);
  constexpr auto scale_factor_x = 1.0;
  constexpr auto scale_factor_y = 1.0f;
  const auto model_matrix =
      glm::scale(glm::translate(base_matrix, position_),
                 glm::vec3(scale_factor_x, scale_factor_y, 1.0f));
  shader->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
Mob::Mob(glm::vec3 startpos, GLuint text_id, MobConfig config) :
    Dynamic(startpos, text_id, 0.3, 0.15), behavior_(config),
    state_(MobState::Idle) {

  switch (config.mob_type) {
    case MobType::Patroller: {
      velocity_.x = -1.0;
      timer_ = SDL_GetTicks() + 4000;
      break;
    }
    case MobType::Chaser: {
      break;
    }
    default:break;
  }
}
bool Mob::IsAlive() const {
  return is_alive_;
}
void Mob::Die() {
  is_alive_ = false;
}
