//
// Created by Yifei Yao on 8/9/23.
//

#include "StaticEntity.h"
StaticEntity::StaticEntity(TextureObject texture_object,
                           glm::vec3 position,
                           float orientation,
                           float scale)
    : texture_object_(texture_object),
      position_(position),
      orientation_(orientation),
      scale_(scale) {

}
void StaticEntity::Render(ShaderProgram *program) const {
  texture_object_.Render(position_, orientation_, scale_, program);
}
Box StaticEntity::box() const {
  return Box{
      position_,
      texture_object_.width() / 2, texture_object_.height() / 2, orientation_
  };
}
