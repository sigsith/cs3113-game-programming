/**
 * Author: Yifei Yao
 * Assignment: Lunar Lander
 * Date due: 2023-07-07, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/

#include "Entity.h"
namespace ship {
void Ship::Update(float delta_time) {}
void Ship::Render(ShaderProgram &program) const {
  const float vertices[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
  };
  const float texture_coordinates[] = {
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  };
  glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                        vertices);
  glEnableVertexAttribArray(program.positionAttribute);
  glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0,
                        texture_coordinates);
  glEnableVertexAttribArray(program.texCoordAttribute);
  const auto saucer_matrix = glm::mat4(1.0f);
  program.SetModelMatrix(saucer_matrix);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(program.positionAttribute);
  glDisableVertexAttribArray(program.texCoordAttribute);
}
Ship::Ship(GLuint texture_id) : _texture_id(texture_id) {}
}