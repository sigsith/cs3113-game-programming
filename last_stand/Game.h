/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_GAME_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_GAME_H_

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define GL_GLEXT_PROTOTYPES 1
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <deque>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include "Utility.h"
#include "Player.h"
#include "Mob.h"
#include "Scene.h"

class EventFrame {
  bool left_mouse_down_{};
  bool space_key_down_{};
  bool enter_key_down_{};
  bool any_quit_{};
 public:
  void Poll();
  void Reset();
  bool left_mouse_down() const;
  bool space_key_down() const;
  bool enter_key_down() const;
  bool any_quit() const;
};

class Game {
 private:
  SDL_Window *display_window_;
  bool is_game_running_ = true;
  ShaderProgram shader_;
  float previous_ticks_ = 0.0f;
  uint curr_stage = 0;
  float time_accumulator_ = 0.0;
  std::unique_ptr<Scene> curr_scene_;
  glm::mat4 view_matrix_;
  glm::mat4 projection_matrix_;
  EventFrame event_frame_{};
 public:
  Game();
  void Run();
  void Update();
  void Render();
  void GoNextLevel();
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_GAME_H_
