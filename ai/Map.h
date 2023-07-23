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

const uint NONE = std::numeric_limits<uint>::max();

struct LevelMapping {
  uint width_;
  uint height_;
  // the index corresponds to the flattened sprit sheet index.
  std::vector<uint> index_mapping_;
  LevelMapping(uint width_,
               uint height_,
               std::vector<uint> index_mapping_);
};

struct SpriteSheetMapping {
  uint width_;
  uint height_;
  GLuint texture_id_;
  SpriteSheetMapping(uint width_,
                     uint height_,
                     GLuint texture_id_);
};

class Map : public Entity {
 private:
  std::vector<float> vertices_;
  std::vector<float> tex_coords_;
  LevelMapping levels_;
  float min_x_;
  float max_x_;
  float min_y_;
  float max_y_;
  float tile_size_;
 public:
  Map(LevelMapping level_mapping,
      SpriteSheetMapping sprite_sheet_mapping,
      float m_tile_size, glm::vec3 top_left
  );
  std::pair<bool, float> IsSolid(Box &box) const;
  void Render(ShaderProgram *shader) const override;
};

#endif //CS3113_GAME_PROGRAMMING_AI_MAP_H_