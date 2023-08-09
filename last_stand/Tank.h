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



struct Specs {
  const float top_speed_forward = 2.0;
  const float base_acceleration_forward_ = 1.5;
  const float base_acceleration_reverse_ = 1.0;
  const float top_speed_backward = 1.0;
  const float forward_friction = 2.0;
  const float lateral_friction = 3.0;
  const float rotation_friction = 2.5;
  const float base_angular_acceleration = 1.5;
  const float top_rotation_speed = 1.0;
  const float turret_turn_rate = 2.0;
  const float projectile_speed = 6.0;
  const float fire_rate = 1.0;
};

struct Paint {
  const std::string chassis_name;
  const std::string turret_name;
  const std::string shell_name;
};

enum class Mode {
  Forward,
  Halt,
  Reverse,
};

enum class Steering {
  None,
  Left,
  Right,
};

class Tank : public Boxed {
 private:
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
  Mode mode_{};
  Steering steering_{};
  Specs specs_{};
  TextureObject chassis_;
  TextureObject turret_;
  TextureObject shell_;
 public:
  Tank(glm::vec3 start_position,
       float start_orientation,
       const std::string &chassis_name,
       const std::string &turret_name,
       const std::string &shell_name);
  Tank(glm::vec3 start_position,
       float start_orientation,
       Specs specs,
       const Paint &paint);
  void Update(float delta_t, const Map &map);
  void Render(ShaderProgram *shader) const override;
  void SetTurretTarget(float target_orientation);
  void SetGear(Mode mode, Steering steering);
  std::unique_ptr<Projectile> TryFire();
  glm::vec3 position() const;
  float turret_orientation() const;

  Box box() const override {
    return Box{
        position_,
        0.5, 0.5
    };
  }
//  void Fire(std::vector<std::unique_ptr<Projectile>> &);
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_DYNAMIC_H_
