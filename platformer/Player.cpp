//
// Created by Yifei Yao on 7/31/23.
//
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define GL_GLEXT_PROTOTYPES 1
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Player.h"
#include "Mob.h"

void Player::Update(float delta_t, EntityManager &manager) {
  Dynamic::Update(delta_t, manager);
  if (!is_active_) {
    return;
  }
  const auto player_box = this->box();
  for (auto &&mob : manager.mobs()) {
    if (!mob->IsActive()) {
      continue;
    }
    const auto mob_box = mob->box();
    if (player_box.IsCollisionWith(mob_box)) {
      if (player_box.IsOnTopOf(mob_box)) {
        mob->Kill();
      } else {
        this->Die();
        return;
      }
      std::cout << "Box hit!\n";
    }
  }
}
Player::Player(glm::vec3 startpos, GLuint text_id) : Dynamic(startpos,
                                                             text_id,
                                                             0.3,
                                                             0.15) {

}
void Player::Render(ShaderProgram *shader) const {
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
void Player::Die() {
  is_active_ = false;
}
glm::vec3 Player::position() const {
  return position_;
}