//
// Created by Yifei Yao on 7/31/23.
//

#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_DYNAMIC_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_DYNAMIC_H_
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define GL_GLEXT_PROTOTYPES 1
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
class EntityManager;
#include "Entity.h"

class Dynamic : public Boxed {
 private:
  float half_height_;
  float half_width_;
  float horizontal_speed_ = 1.2;
  float vertical_speed = 0.08;
 protected:
  uint collision_time_out = 0;
  bool grounded = false;
  glm::vec3 velocity_{};
  glm::vec3 acceleration_{};
  glm::vec3 position_;
  bool is_active_ = true;
  float gravity_ = -1.2;
 public:
  Dynamic(glm::vec3 startpos,
          GLuint text_id,
          float half_height,
          float half_width);
  virtual void Update(float delta_t, EntityManager &manager);
  virtual void Jump(float speed, const EntityManager &manager);
  void Disable();
  virtual void MoveLeft();
  virtual void MoveRight();
  virtual void StopHorizontal();
  bool IsActive() const;
  Box box() const override {
    return Box{
        position_,
        half_width_, half_height_
    };
  }
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_DYNAMIC_H_
