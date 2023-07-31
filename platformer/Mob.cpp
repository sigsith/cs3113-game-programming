//
// Created by Yifei Yao on 7/31/23.
//

#include "Mob.h"
#include "Utility.h"
#include "Player.h"
void Mob::Update(float delta_t, EntityManager &manager) {
  Dynamic::Update(delta_t, manager);
  if (!is_active_) {
    return;
  }
  switch (behavior_.mob_type) {
    case MobType::Jumper: {
      if (SDL_GetTicks() > timer_) {
        timer_ = SDL_GetTicks() + 3000;
        Jump(2.0, manager);
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
      glm::vec3 direction = manager.player_->position() - this->position_;
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
  if (!is_active_) {
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
      gravity_ = 0;
      break;
    }
    default:break;
  }
}
void Mob::Kill() {
  is_active_ = false;
}
