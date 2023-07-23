/**
* Author: Yifei Yao
* Assignment: Rise of the AI
* Date due: 2023-07-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Map.h"

Map::Map(LevelMapping level_mapping,
         SpriteSheetMapping sprite_sheet_mapping,
         float tile_size, glm::vec3 top_left)
    : levels_(std::move(level_mapping)) {
  texture_id_ = sprite_sheet_mapping.texture_id_;;
  const auto tile_height = 1.0f / (float) sprite_sheet_mapping.height_;
  const auto tile_width = 1.0f / (float) sprite_sheet_mapping.width_;
  const auto x_offset = top_left.x; // From center of tile
  const auto y_offset = top_left.y; // From center of tile

  for (int y_coord = 0; y_coord < levels_.height_; y_coord++) {
    for (int x_coord = 0; x_coord < levels_.width_; x_coord++) {
      const auto tile =
          levels_.index_mapping_[y_coord * levels_.width_
              + x_coord];
      if (tile == NONE) continue;
      const auto
          v_coord = static_cast<float>(tile
          / sprite_sheet_mapping.width_)
          / static_cast<float>(sprite_sheet_mapping.height_);
      const auto
          u_coord = static_cast<float>(tile
          % sprite_sheet_mapping.width_)
          / static_cast<float>(sprite_sheet_mapping.width_);
      vertices_.insert(vertices_.end(), {
          x_offset + (tile_size * x_coord), y_offset + -tile_size * y_coord,
          x_offset + (tile_size * x_coord),
          y_offset + (-tile_size * y_coord) - tile_size,
          x_offset + (tile_size * x_coord) + tile_size,
          y_offset + (-tile_size * y_coord) - tile_size,
          x_offset + (tile_size * x_coord), y_offset + -tile_size * y_coord,
          x_offset + (tile_size * x_coord) + tile_size,
          y_offset + (-tile_size * y_coord) - tile_size,
          x_offset + (tile_size * x_coord) + tile_size,
          y_offset + -tile_size * y_coord
      });
      tex_coords_.insert(tex_coords_.end(), {
          u_coord, v_coord,
          u_coord, v_coord + (tile_height),
          u_coord + tile_width, v_coord + (tile_height),
          u_coord, v_coord,
          u_coord + tile_width, v_coord + (tile_height),
          u_coord + tile_width, v_coord
      });
    }
  }

}
void Map::Render(ShaderProgram *shader) const {
  glm::mat4 model_matrix = glm::mat4(1.0f);
  shader->SetModelMatrix(model_matrix);

  glUseProgram(shader->programID);

  glVertexAttribPointer(shader->positionAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        vertices_.data());
  glEnableVertexAttribArray(shader->positionAttribute);
  glVertexAttribPointer(shader->texCoordAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        tex_coords_.data());
  glEnableVertexAttribArray(shader->texCoordAttribute);

  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glDrawArrays(GL_TRIANGLES, 0, (int) vertices_.size() / 2);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
SolidStatus Map::CheckSolid(glm::vec3 position, float half_width) {
  return Empty;
}
LevelMapping::LevelMapping(uint width,
                           uint height,
                           std::vector<uint> index_mapping) :
    width_(width), height_(height),
    index_mapping_(std::move(index_mapping)) {
  assert(width_ * height_ == index_mapping_.size());
}
SpriteSheetMapping::SpriteSheetMapping(uint width,
                                       uint height,
                                       GLuint texture_id)
    : width_(width), height_(height), texture_id_(texture_id) {}
