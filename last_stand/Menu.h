/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_MENU_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_MENU_H_

#include "Scene.h"
struct EventFrame;

class Menu : public Scene {
 private:
  Background background_;
  bool go_next_ = false;
 public:
  Menu();
  uint Update(float delta_time, const EventFrame &event_frame) override;
  void Render(ShaderProgram *program) const override;
  int Id() const override;
  glm::vec3 GetPlayerPosition() const override;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_MENU_H_
