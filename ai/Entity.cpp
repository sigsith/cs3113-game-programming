/**
* Author: Yifei Yao
* Assignment: Rise of the AI
* Date due: 2023-07-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include "stb_image.h"
void Background::Render(ShaderProgram *shader) const {
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
  constexpr auto scale_factor_x = 10.0f;
  constexpr auto scale_factor_y = 7.5f;
  const auto model_matrix =
      glm::scale(glm::translate(base_matrix, glm::vec3(0.0f, 0.0f, 0.0f)),
                 glm::vec3(scale_factor_x, scale_factor_y, 1.0f));
  shader->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
Background::Background(const std::string &texture_path) {
  texture_id_ = LoadTexture((texture_path));
}
GLuint LoadTexture(const std::string &path) {
  int width, height, nrChannels;
  unsigned char
      *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (!data) {
    throw std::runtime_error("Failed to load texture: " + path);
  }
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
               0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return texture;
}
float Box::XMax() const {
  return position.x + half_width;
}
float Box::XMin() const {
  return position.x - half_width;
}
float Box::YMax() const {
  return position.y + half_height;
}
float Box::YMin() const {
  return position.y - half_height;
}
bool Box::IsCollision(const Box &rhs) const {
  const auto x_diff = abs(this->position.x - rhs.position.x);
  const auto y_diff = abs(this->position.y - rhs.position.y);
  const auto x_limit = this->half_width + rhs.half_width;
  const auto y_limit = this->half_height + rhs.half_height;
  return x_diff <= x_limit && y_diff <= y_limit;
}
