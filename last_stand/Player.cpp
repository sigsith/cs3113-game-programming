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
#include "Utility.h"

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
  const float forward_speed = keyboard_state[SDL_SCANCODE_W] ? 0.5 : 0.0f;
  const float backward_speed = keyboard_state[SDL_SCANCODE_S] ? 0.2 : 0.0f;
  const auto speed = forward_speed - backward_speed;
  velocity_ = utility::VectorByAngle(speed, orientation_);

  int cursor_x, cursor_y;
  SDL_GetMouseState(&cursor_x, &cursor_y);
  const auto
      cursor_pos = glm::vec3(cursor_x - 1280 / 2, -(cursor_y - 960 / 2), 0);
  target_angle = utility::GetTargetAngle(glm::vec3(0, 0, 0), cursor_pos);

  Tank::Update(delta_t, map);
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
Player::Player() : Tank(glm::vec3(0, 0, 0),
                        glm::pi<float>() / 2,
                        "tankBody_blue",
                        "tankBlue_barrel1", "bulletBlue1") {

}
glm::vec3 Player::position() const {
  return position_;
}