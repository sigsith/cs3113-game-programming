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
class Ship : public Entity {
 private:
  const GLuint _texture_id;
  int _rcs_state = 0;  // -1, 0, 1
  bool _is_engine_on = false;
  int ComputeState() const;
 public:
  void Update(float delta_time) override;

  void Render(ShaderProgram &program) const override;

  explicit Ship(GLuint texture_id);

  void SetEngine(bool is_on);

  void SetRcs(int state);
};
}
#endif //CS3113_GAME_PROGRAMMING_LUNAR_LANDER_ENTITY_H_