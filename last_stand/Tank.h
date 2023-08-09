/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
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
#include "Entity.h"
#include "TextureObject.h"
#include "Map.h"
#include "Projectile.h"
class Tank : public Boxed {
 private:
 protected:
  uint fire_time_out = 0;
  glm::vec3 velocity_{};
  glm::vec3 acceleration_{};
  glm::vec3 position_;
  float orientation_ = 0.0;
  float angular_velocity_{};
  float angular_acceleration_{};
  float turret_orientation_{};
  float turret_angular_velocity_{};
  float turret_angular_acceleration_{};
  float target_angle{};
  TextureObject chassis_;
  TextureObject turret_;
  TextureObject shell_;
 public:
  Tank(glm::vec3 start_position,
       float start_orientation,
       const std::string &chassis_name,
       const std::string &turret_name,
       const std::string &shell_name);
  void Update(float delta_t, const Map &map);
  void Render(ShaderProgram *shader) const override;
  Box box() const override {
    return Box{
        position_,
        0.5, 0.5
    };
  }
  void Fire(std::vector<std::unique_ptr<Projectile>> &);
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_DYNAMIC_H_
