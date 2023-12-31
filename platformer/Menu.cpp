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
  if (keyboard_state[SDL_SCANCODE_RETURN]
      || keyboard_state[SDL_SCANCODE_KP_ENTER]) {
    go_next_ = true;
  }
}
Feedback Menu::Update(float delta_time) {
  if (go_next_) {
    return Feedback::NextStage;
  }
  return Feedback::NoOp;
}
void Menu::Render(ShaderProgram *shader, int life) const {
  background_.Render(shader);
  utility::RenderText("Press enter to start",
                      shader,
                      0.4,
                      glm::vec3(-4.0, 2, 0));
}
Menu::Menu() : background_(Background(std::string("menu_background.png"))) {

}
int Menu::Id() const {
  return 0;
}
glm::vec3 Menu::GetPlayerPosition() const {
  return glm::vec3(0);
}
