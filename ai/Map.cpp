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

Map::Map(LevelMapping level_mapping, SpriteSheetMapping sprite_sheet_mapping) {
  texture_id_ = sprite_sheet_mapping.texture_id_;

}
void Map::Render(ShaderProgram *shader) const {

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
