/**
* Author: Yifei Yao
* Assignment: Lunar Lander
* Date due: 2023-07-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef LUNAR_LANDER_ENTITY_H
#define LUNAR_LANDER_ENTITY_H

#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Entity {
 public:
  // The value should be set to a small fix number for all instances.
  virtual void Update(float delta_time) = 0;

  virtual void Render(ShaderProgram &program) const = 0;
};

namespace ship {
// Model of movement:
// Three keys to control the movement of a lunar lander:
// S: when pressed, engine turns on; not pressed, turns off. (It's a very good engine).
// A: when pressed, RCS is turned on and rotate counter-clockwise; not pressed, turns off.
// D: when pressed, RCS is turned on and rotate clockwise; not pressed, turns off.
// When engine is on, because it's such a good engine, the acceleration discretely
// changed to the combination of gravity and a constant thrust (towards the direction
// of the orientation), and when it's off, switched to gravity only immediately.
// For RCS, when pressed a constant angular acceleration is applied towards the
// intended direction, when not pressed, removed immediately. Also, when both A
// and D are pressed, nothing happens.

// This value is used for acceleration when no other forces are around.
constexpr glm::vec3 GRAVITY = glm::vec3(0.0f, -0.1f, 0.0f);
// THRUST and RCS_THRUST here are acceleration and angular acceleration
constexpr float THRUST = 0.1f;
constexpr float RCS_THRUST = 1.0f;

class Ship : public Entity {
 private:
  glm::vec3 _position{};
  glm::vec3 _velocity{};
  glm::vec3 _acceleration = GRAVITY;

  // Orientation in DEGREE in the Cartesian convention. Always modular by 360.
  float _orientation{};
  float _angular_velocity = 0; // In degree / second.
  float _angular_acceleration = 0; // In degree / second^2;

  int _rcs_state = 0;  // -1, 0, 1
  bool _is_engine_on = false;
  GLuint _texture_id{};
 public:
  // Assuming the ship is drifting in idle initially.
  Ship(glm::vec3 position, glm::vec3 velocity, float orientation,
       GLuint texture_id);

  Ship();

  void SetEngine(bool is_on);

  void SetRcs(int state);

  void Update(float delta_time) override;

  void Render(ShaderProgram &program) const override;
};

glm::vec3 VectorByAngle(float scalar, float angle_in_degree);
}

#endif //LUNAR_LANDER_ENTITY_H
