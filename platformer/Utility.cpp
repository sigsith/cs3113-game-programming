/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Utility.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace utility {
GLuint LoadTexture(const std::string &path) {
  int width, height, nrChannels;
  unsigned char
      *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (!data) {
    throw std::runtime_error("Failed to load texture: " + path);
  }
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
               0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return texture;
}
void RenderText(const std::string &text,
                ShaderProgram *shader,
                float tile_size,
                glm::vec3 top_left) {
  static const auto
      font_mapping =
      SpriteSheetMapping(16,
                         6,
                         utility::LoadTexture(std::string("font-sheet.png")));
  const auto text_map =
      TextMap(text, font_mapping, tile_size, top_left);
  text_map.Render(shader);
}
}