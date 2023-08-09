//
// Created by Yifei Yao on 8/9/23.
//

#ifndef CS3113_GAME_PROGRAMMING_LAST_STAND_STATICENTITY_H_
#define CS3113_GAME_PROGRAMMING_LAST_STAND_STATICENTITY_H_

#include "TextureObject.h"
#include "Entity.h"
class StaticEntity : Boxed {
 private:
  TextureObject texture_object_;
  glm::vec3 position_;
  float orientation_;
  float scale_;
 public:
  explicit StaticEntity(TextureObject texture_object,
                        glm::vec3 position,
                        float orientation = glm::pi<float>() / 2,
                        float scale = 1.0);
  void Render(ShaderProgram *program) const override;
  Box box() const override;
};

#endif //CS3113_GAME_PROGRAMMING_LAST_STAND_STATICENTITY_H_
