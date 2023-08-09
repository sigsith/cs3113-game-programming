//
// Created by Yifei Yao on 8/8/23.
//

#include "TextureObject.h"
#include "Utility.h"
TextureObject::TextureObject(const std::string &name) {
  static const auto specs = TextureObject::ParseMapping("object_mapping.txt");
  try {
    *this = specs.at(name);
  } catch (const std::out_of_range &e) {
    std::cerr << "Key not found: " << e.what() << std::endl;
  }
}
void TextureObject::Render(glm::vec3 position,
                           float orientation,
                           float scale,
                           ShaderProgram *shader) const {
  utility::RenderTileObj(x_px_,
                         y_px_,
                         width_,
                         height_,
                         position,
                         orientation,
                         shader,
                         scale);
}
TextureObject::TextureObject() = default;
std::unordered_map<std::string,
                   TextureObject> TextureObject::ParseMapping(const std::string &file) {
  std::unordered_map<std::string, TextureObject> result_map;

  std::ifstream infile(file);
  if (!infile.is_open()) {
    throw std::runtime_error("Failed to open file: " + file);
  }
  std::string line;
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    std::string name;
    TextureObject spec;
    if (!(iss >> name >> spec.x_px_ >> spec.y_px_ >> spec.width_
              >> spec.height_)) {
      throw std::runtime_error("Failed to parse line: " + line);
    }
    result_map[name] = spec;
  }

  infile.close();
  return result_map;
}
float TextureObject::height() const {
  return height_;
}
float TextureObject::width() const {
  return width_;
}
