//
// Created by Yifei Yao on 8/8/23.
//

#ifndef CS3113_GAME_PROGRAMMING_LAST_STAND_TEXTUREOBJECT_H_
#define CS3113_GAME_PROGRAMMING_LAST_STAND_TEXTUREOBJECT_H_
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include <unordered_map>
#include <string>
class TextureObject {
 private:
  float x_px_{};
  float y_px_{};
  float width_{};
  float height_{};
  std::unordered_map<std::string,
                     TextureObject> ParseMapping(const std::string &file);
 public:
  explicit TextureObject(const std::string &name);
  void Render(glm::vec3 position,
              float orientation,
              float scale,
              ShaderProgram *shader) const;
  TextureObject();
  float width() const;
  float height() const;
};

#endif //CS3113_GAME_PROGRAMMING_LAST_STAND_TEXTUREOBJECT_H_
