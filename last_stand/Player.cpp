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

PlayerFeedback Player::Update(float delta_t,
                              const Map &map,
                              std::vector<Mob> &mobs) {
  const auto keyboard_state = SDL_GetKeyboardState(nullptr);
  constexpr float BASE_ROTATION_SPEED = 0.5f;
  const float angular_v_left =
      (keyboard_state[SDL_SCANCODE_A]) ? BASE_ROTATION_SPEED : 0.0f;
  const float angular_v_right =
      (keyboard_state[SDL_SCANCODE_D]) ? -BASE_ROTATION_SPEED : 0.0f;
  angular_velocity_ = angular_v_left + angular_v_right;
  if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_SPACE]) {
  }
  Dynamic::Update(delta_t, map);
  const auto player_box = this->box();
  for (auto &mob : mobs) {
    if (!mob.IsAlive()) {
      continue;
    }
    const auto mob_box = mob.box();
    if (player_box.IsCollisionWith(mob_box)) {
      if (player_box.IsOnTopOf(mob_box)) {
        mob.Die();
      } else {
        return PlayerFeedback::TakeDamage;
      }
      std::cout << "Box hit!\n";
    }
  }
  return PlayerFeedback::NoOp;
}
Player::Player(glm::vec3 startpos, GLuint text_id) : Dynamic(startpos,
                                                             text_id,
                                                             0.3,
                                                             0.15) {

}
void Player::Render(ShaderProgram *shader) const {
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

  auto model_matrix = glm::translate(base_matrix, position_);
  model_matrix =
      glm::rotate(model_matrix,
                  orientation_ - glm::pi<float>() / 2,
                  glm::vec3(0.0f, 0.0f, 1.0f));
  model_matrix = glm::scale(model_matrix, glm::vec3(0.65f, 0.65f, 1.0f));

  shader->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
glm::vec3 Player::position() const {
  return position_;
}