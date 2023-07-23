/**
* Author: Yifei Yao
* Assignment: Rise of the AI
* Date due: 2023-07-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#ifndef CS3113_GAME_PROGRAMMING_AI_MAP_H_
#define CS3113_GAME_PROGRAMMING_AI_MAP_H_

#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Entity.h"

class LevelMapping {
 private:
  uint width_;
  uint height_;
  std::vector<uint> index_mapping_;
 public:
  LevelMapping(uint width_,
               uint height_,
               std::vector<uint> index_mapping_);
};

class Map : public Entity {
 public:
  explicit Map(const std::string &tile_set_path, const LevelMapping &mapping);
  void Render(ShaderProgram *shader) const override;
};

#endif //CS3113_GAME_PROGRAMMING_AI_MAP_H_