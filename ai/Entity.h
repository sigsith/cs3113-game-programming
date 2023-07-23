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
class ShaderProgram;
#include <SDL.h>
#include <SDL_opengl.h>

#include "Entity.h"
GLuint LoadTexture(const std::string &path);

class Entity {
 public:
  virtual ~Entity() = default;
  virtual void Render(ShaderProgram *program) const = 0;
};

class Background : public Entity {
 private:
  GLuint texture_id_;
 public:
  explicit Background(std::string &texture_path);
  void Render(ShaderProgram *program) const override;
};

#endif //CS3113_GAME_PROGRAMMING_AI_ENTITY_H_