/**
 * Author: Yifei Yao
 * Assignment: Lunar Lander
 * Date due: 2023-07-07, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 **/
#ifndef CS3113_GAME_PROGRAMMING_LUNAR_LANDER_ENTITY_H_
#define CS3113_GAME_PROGRAMMING_LUNAR_LANDER_ENTITY_H_
#include "ShaderProgram.h"
class Entity {
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
// of the orientation), and when it's off, back to gravity only immediately.
// For RCS, when pressed a constant angular acceleration is applied towards the
// intended direction, when not pressed, removed immediately. Also, when both A
// and D are pressed, nothing happens.

// This value is used for acceleration when no other forces are around.
constexpr glm::vec3 GRAVITY = glm::vec3(0.0f, -0.1f, 0.0f);
// THRUST and RCS_THRUST here are simply acceleration and angular acceleration
constexpr float THRUST = 0.2f;
constexpr float RCS_THRUST = 1.0f;
class Ship : public Entity {
 private:
  const GLuint _texture_id;
  int _rcs_state = 0;  // -1, 0, 1
  bool _is_engine_on = false;
  glm::vec3 _position;
  glm::vec3 _velocity;
  glm::vec3 _acceleration = GRAVITY;

  // Orientation in radian in the standard Cartesian convention.
  float _orientation;
  float _angular_velocity{}; // In radian / second.
  float _angular_acceleration{}; // In radian / second^2;

  int ComputeState() const;
 public:
  void Update(float delta_time) override;

  void Render(ShaderProgram &program) const override;

  explicit Ship(glm::vec3 position, glm::vec3 velocity, float orientation,
                GLuint texture_id);

  void SetEngine(bool is_on);

  void SetRcs(int state);
};
glm::vec3 VectorByAngle(float scalar, float angle_in_radians);
}
#endif //CS3113_GAME_PROGRAMMING_LUNAR_LANDER_ENTITY_H_