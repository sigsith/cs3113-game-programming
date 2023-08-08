//
// Created by Yifei Yao on 8/8/23.
//

#ifndef CS3113_GAME_PROGRAMMING_LAST_STAND_PROJECTILE_H_
#define CS3113_GAME_PROGRAMMING_LAST_STAND_PROJECTILE_H_

#include "TextureObject.h"
#include "Entity.h"
class Projectile : public Ephemeral {
 private:
  TextureObject text_obj_;
  glm::vec3 velocity_;
  glm::vec3 curr_pos_;
  float orientation_;
  glm::vec3 origin_;
 public:
  Projectile(TextureObject text_obj, float orientation, glm::vec3 origin);
  void Render(ShaderProgram *program) const override;
  bool Update(float delta_t) override;
};

#endif //CS3113_GAME_PROGRAMMING_LAST_STAND_PROJECTILE_H_
