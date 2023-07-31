/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Utility.h"
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
  texture_id_ = utility::LoadTexture((texture_path));
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
bool Box::IsCollisionWith(const Box &rhs) const {
  const auto x_diff = abs(this->position.x - rhs.position.x);
  const auto y_diff = abs(this->position.y - rhs.position.y);
  const auto x_limit = this->half_width + rhs.half_width;
  const auto y_limit = this->half_height + rhs.half_height;
  return x_diff <= x_limit && y_diff <= y_limit;
}
bool Box::IsOnTopOf(const Box &rhs) const {
  return this->position.y > rhs.position.y;
}
