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

struct EventFrame;

enum class Feedback {
  NoOp,
  TakeDamage,
  NextStage,
};

class Scene {
 public:
  virtual ~Scene() = default;
  virtual uint Update(float delta_time, const EventFrame &frame) = 0;
  virtual void Render(ShaderProgram *shader) const = 0;
  virtual int Id() const = 0;
  virtual glm::vec3 GetPlayerPosition() const = 0;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_SCENE_H_
