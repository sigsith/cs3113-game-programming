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
#include "Projectile.h"
#include "Game.h"

uint Player::Update(float delta_t, const EventFrame &event_frame,
                    const Map &map,
                    std::vector<Mob> &mobs,
                    std::vector<std::unique_ptr<Projectile>> &projectiles) {
  const auto keyboard_state = SDL_GetKeyboardState(nullptr);
  Steering steering = Steering::None;
  if (keyboard_state[SDL_SCANCODE_A] && !keyboard_state[SDL_SCANCODE_D]) {
    steering = Steering::Left;
  }
  if (keyboard_state[SDL_SCANCODE_D] && !keyboard_state[SDL_SCANCODE_A]) {
    steering = Steering::Right;
  }
  Mode mode = Mode::Halt;
  if (keyboard_state[SDL_SCANCODE_W] && !keyboard_state[SDL_SCANCODE_S]) {
    mode = Mode::Forward;
  }
  if (keyboard_state[SDL_SCANCODE_S] && !keyboard_state[SDL_SCANCODE_W]) {
    mode = Mode::Reverse;
  }
  SetGear(mode, steering);

  int cursor_x, cursor_y;
  SDL_GetMouseState(&cursor_x, &cursor_y);
  const auto
      cursor_pos = glm::vec3(cursor_x - 1280 / 2, -(cursor_y - 960 / 2), 0);
  const auto
      target_angle = utility::GetTargetAngle(glm::vec3(0, 0, 0), cursor_pos);
  SetTurretTarget(target_angle);

  if (event_frame.space_key_down() || event_frame.left_mouse_down()) {
    auto projectile = this->TryFire();
    if (projectile) {
      projectiles.push_back(std::move(projectile));
    }
  }

  Tank::Update(delta_t, map);
  const auto player_box = this->box();
//  for (auto &mob : mobs) {
//    if (!mob.IsAlive()) {
//      continue;
//    }
//    const auto mob_box = mob.box();
//    if (player_box.IsCollisionWith(mob_box)) {
//      if (player_box.IsOnTopOf(mob_box)) {
//        mob.Die();
//      } else {
//        return PlayerFeedback::TakeDamage;
//      }
//      std::cout << "Box hit!\n";
//    }
//  }
  for (auto &proj : projectiles) {
    if (!proj->HasExploded() && !proj->IsSafetyOn()
        && box().IsCollisionWith(proj->position())) {
      proj->Explode();
      life_ -= 1;
      break;
    }
  }
  return life_;
}
constexpr Specs
    PLAYER_SPECS =
    {2.5, 1.875, 1.0,
     1.0, 2.0, 3.0,
     2.5, 2.25, 1.5,
     2.5, 6.0, 1.0};
Player::Player() : Tank(glm::vec3(0, 0, 0),
                        glm::pi<float>() / 2,
                        PLAYER_SPECS,
                        Paint{"tankBody_blue",
                              "tankBlue_barrel1", "bulletBlue1"}) {

}
uint8_t Player::life() const {
  return life_;
}
