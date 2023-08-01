/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Menu.h"
void Menu::UpdateInput(const Uint8 *keyboard_state) {
  if (keyboard_state[SDLK_RETURN]) {
    go_next_ = true;
  }
}
Feedback Menu::Update(float delta_time) {
  if (go_next_) {
    return Feedback::NextStage;
  }
  return Feedback::NoOp;
}
void Menu::Render(ShaderProgram *shader) const {
  background_.Render(shader);
  utility::RenderText("Press enter to start",
                      shader,
                      1.0,
                      glm::vec3(-3.5, 2, 0));
}
Menu::Menu() : background_(Background(std::string("background1.png"))) {

}
