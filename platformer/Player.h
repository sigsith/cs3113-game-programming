/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_PLAYER_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_PLAYER_H_

#include "Dynamic.h"
#include "EntityManager.h"
class Player : public Dynamic {
 public:
  void Update(float delta_t, EntityManager &manager) override;
  void Render(ShaderProgram *shader) const override;
  void Die();
  glm::vec3 position() const;
  Player(glm::vec3 startpos, GLuint text_id);
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_PLAYER_H_
