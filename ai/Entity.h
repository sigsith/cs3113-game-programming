/**
* Author: Yifei Yao
* Assignment: Rise of the AI
* Date due: 2023-07-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#ifndef CS3113_GAME_PROGRAMMING_AI_ENTITY_H_
#define CS3113_GAME_PROGRAMMING_AI_ENTITY_H_

class Entity {
 public:
  virtual ~Entity() = default;
  virtual void render(ShaderProgram *program) const = 0;
};

#endif //CS3113_GAME_PROGRAMMING_AI_ENTITY_H_