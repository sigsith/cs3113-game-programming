/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_SCENE_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_SCENE_H_

#include "Utility.h"
#include "ShaderProgram.h"
enum class Feedback {
  NoOp,
  TakeDamage,
  NextStage,
};

class Scene {
 public:
  virtual void UpdateInput(const Uint8 *keyboard_state) = 0;
  virtual Feedback Update(float delta_time) = 0;
  virtual void Render(ShaderProgram *shader, int life) const = 0;
  virtual int Id() const = 0;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_SCENE_H_
