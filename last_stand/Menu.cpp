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
#include "Game.h"
uint Menu::Update(float delta_time, const EventFrame &event_frame) {
  if (event_frame.enter_key_down()) {
    go_next_ = true;
  }
  if (go_next_) {
    return 1;
  }
  return 0;
}
void Menu::Render(ShaderProgram *shader) const {
  background_.Render(shader);
  utility::RenderText("Press enter to start",
                      shader,
                      0.4,
                      glm::vec3(-4.0, 2, 0));
  utility::RenderText("WASD to move",
                      shader,
                      0.2,
                      glm::vec3(-4.0, 1.0, 0));
  utility::RenderText("Cursor to turret",
                      shader,
                      0.2,
                      glm::vec3(-4.0, 0.5, 0));
  utility::RenderText("SPACE/LeftClick to fire",
                      shader,
                      0.2,
                      glm::vec3(-4.0, 0.0, 0));
}
Menu::Menu() : background_(Background(std::string("menu_background.png"))) {

}
int Menu::Id() const {
  return 0;
}
glm::vec3 Menu::GetPlayerPosition() const {
  return glm::vec3(0);
}
