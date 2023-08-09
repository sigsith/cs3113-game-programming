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
  const auto model_matrix =
      glm::scale(glm::translate(base_matrix, glm::vec3(0.0f, 0.0f, 0.0f)),
                 glm::vec3(scale_x_, scale_y_, 1.0f));
  shader->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
Background::Background(const std::string &texture_path,
                       float scale_x,
                       float scale_y) : scale_x_(scale_x), scale_y_(scale_y) {
  texture_id_ = utility::LoadTexture((texture_path));
}
bool Box::IsPointCollisionWith(glm::vec3 point_pos) const {
  point_pos -= position;
  glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f),
                                                   -orientation,
                                                   glm::vec3(0.0f,
                                                             0.0f,
                                                             1.0f)));
  glm::vec3 localPoint = rotationMatrix * point_pos;
  return localPoint.x >= -half_width && localPoint.x <= half_width &&
      localPoint.y >= -half_height && localPoint.y <= half_height;

}
bool Box::IsCollisionWith(const Box &rhs) const {
  const auto distance = utility::Length(position - rhs.position);
  const auto sum_half_thickness =
      (half_height + half_width + rhs.half_width + rhs.half_height) / 2;
  return distance < sum_half_thickness;
}
