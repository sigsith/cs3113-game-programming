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
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include "Utility.h"
#include "Player.h"
#include "Mob.h"
#include "Scene.h"

class Game {
 private:
  SDL_Window *display_window_;
  bool is_game_running_ = true;
  ShaderProgram shader_;
  float previous_ticks_ = 0.0f;
  float time_accumulator_ = 0.0;
  Scene *curr_scene_;
  uint immune_time_out = 0;
  uint8_t life_ = 3;
 public:
  Game();
  void Run();
  void ProcessInput();
  void Update();
  void Render();
  void RenderMessage(const std::string &message);
  void RenderLife();
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_GAME_H_
