/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
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

static constexpr float SQUARE_VERTICES[12] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f,
    -0.5f, -0.5f,
    0.5f, 0.5f,
    -0.5f, 0.5f
};
static constexpr float FULL_TEX_COORDS[12] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};

class Entity {
 public:
  virtual void Render(ShaderProgram *program) const = 0;
 protected:
  GLuint texture_id_{};
};

struct Box {
  glm::vec3 position;
  float half_width;
  float half_height;
  float XMax() const;
  float XMin() const;
  float YMax() const;
  float YMin() const;
  bool IsCollisionWith(const Box &rhs) const;
  bool IsOnTopOf(const Box &rhs) const;
};

class Boxed : public Entity {
 public:
  virtual Box box() const = 0;
};

class Background : public Entity {
 private:
  const float scale_x_;
  const float scale_y_;
 public:
  explicit Background(const std::string &texture_path,
                      float scale_x = 10.0,
                      float scale_y = 7.5);
  void Render(ShaderProgram *shader) const override;
};
#endif //CS3113_GAME_PROGRAMMING_AI_ENTITY_H_