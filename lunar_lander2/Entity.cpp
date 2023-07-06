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
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace ship {
void Ship::Update(float delta_time) {
  _position += _velocity * delta_time;
  _velocity += _acceleration * delta_time;
  _acceleration =
      _is_engine_on ? GRAVITY + VectorByAngle(THRUST, _orientation) : GRAVITY;
  _orientation += _angular_velocity * delta_time;
  _angular_velocity += _angular_acceleration * delta_time;
  _angular_acceleration = static_cast<float>(_rcs_state) * RCS_THRUST;
}
void Ship::Render(ShaderProgram &program) const {
  constexpr float vertices[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
  };
  constexpr auto sprite_width = 1.0f / 6.0f;
  constexpr float sprite_height = 1.0f;
  const auto index = ComputeState();
  const auto u_start = static_cast<float>(index) * sprite_width;
  const auto u_end = u_start + sprite_width;
  constexpr auto v_start = 0.0f;
  constexpr auto v_end = sprite_height;
  const float texture_coordinates[] = {
      u_start, v_end, u_end, v_end, u_end, v_start,
      u_start, v_end, u_end, v_start, u_start, v_start
  };
  glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                        vertices);
  glEnableVertexAttribArray(program.positionAttribute);
  glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0,
                        texture_coordinates);
  glEnableVertexAttribArray(program.texCoordAttribute);
  constexpr auto base_matrix = glm::mat4(1.0f);
  auto model_matrix = glm::translate(base_matrix, _position);
  model_matrix =
      glm::rotate(model_matrix,
                  _orientation - glm::pi<float>() / 4,
                  glm::vec3(0.0f, 0.0f, 1.0f));
  program.SetModelMatrix(model_matrix);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(program.positionAttribute);
  glDisableVertexAttribArray(program.texCoordAttribute);
}
Ship::Ship(glm::vec3 position, glm::vec3 velocity, float orientation,
           GLuint texture_id) : _position(position),
                                _velocity(velocity),
                                _orientation(orientation),
                                _texture_id(texture_id) {}
int Ship::ComputeState() const {
  return _rcs_state + (_is_engine_on ? 4 : 1);
}
void Ship::SetEngine(bool is_on) { _is_engine_on = is_on; }

void Ship::SetRcs(int state) { _rcs_state = state; }

glm::vec3 VectorByAngle(float scalar, float angle_in_radians) {
  float x = scalar * glm::cos(angle_in_radians);
  float y = scalar * glm::sin(angle_in_radians);
  return {x, y, 0.0f};
}
}