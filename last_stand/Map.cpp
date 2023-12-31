/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Map.h"
#include <unordered_map>

Map::Map(LevelMapping level_mapping,
         SpriteSheetMapping sprite_sheet_mapping,
         float tile_size, glm::vec3 top_left)
    : levels_(std::move(level_mapping)), tile_size_(tile_size) {
  texture_id_ = sprite_sheet_mapping.texture_id_;;
  const auto tile_height = 1.0f / (float) sprite_sheet_mapping.height_;
  const auto tile_width = 1.0f / (float) sprite_sheet_mapping.width_;
  const auto x_offset = top_left.x; // From center of tile
  const auto y_offset = top_left.y; // From center of tile
  min_x_ = x_offset;
  max_x_ = x_offset + static_cast<float>(level_mapping.width_) * tile_size;
  max_y_ = y_offset;
  min_y_ = y_offset - static_cast<float>(level_mapping.height_) * tile_size;

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

LevelMapping LoadTerrain(const std::string &config_file) {
  std::unordered_map<std::string, std::vector<std::vector<uint>>> sectors;
  std::ifstream file(config_file);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + config_file);
  }
  std::string line;
  while (std::getline(file, line)) {
    if (line == "FINAL") {
      return {49, 49, BuildFinalMap(file, sectors)};
    } else if (!line.empty()) {
      FillMapping(file, line, sectors);
    }
  }
  throw std::runtime_error("Failed to build map");
}
void FillMapping(std::ifstream &file,
                 std::string &line,
                 std::unordered_map<std::string,
                                    std::vector<std::vector<uint>>> &sectors) {
  std::string title = line;
  std::vector<std::vector<uint>> tiles;
  for (int i = 0; i < 7; i++) {
    std::getline(file, line);
    std::stringstream ss(line);
    std::vector<uint> row;
    uint val;
    while (ss >> val) {
      row.push_back(val);
    }
    tiles.push_back(row);
  }
  sectors[title] = tiles;
}
std::vector<uint> BuildFinalMap(std::ifstream &file,
                                const std::unordered_map<std::string,
                                                         std::vector<std::vector<
                                                             uint>>> &sectors) {
  std::string line;
  std::vector<std::vector<uint>> final_map_2d;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string sectorName;
    std::vector<std::vector<uint>> rowTiles;
    while (ss >> sectorName) {
      auto& sectorTiles = sectors.at(sectorName);
      if (rowTiles.empty()) {
        rowTiles = sectorTiles;
      } else {
        for (int i = 0; i < 7; i++) {
          rowTiles[i].insert(rowTiles[i].end(), sectorTiles[i].begin(), sectorTiles[i].end());
        }
      }
    }
    final_map_2d.insert(final_map_2d.end(), rowTiles.begin(), rowTiles.end());
  }
  std::vector<uint> finalMap;
  for (const auto& row : final_map_2d) {
    finalMap.insert(finalMap.end(), row.begin(), row.end());
  }
  return finalMap;
}
