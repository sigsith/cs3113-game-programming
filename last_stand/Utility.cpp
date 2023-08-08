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
float Length(glm::vec3 glm_vec) {
  const auto x = glm_vec.x;
  const auto y = glm_vec.y;
  const auto z = glm_vec.z;
  return std::sqrt(x * x + y * y + z * 2);
}
glm::vec3 Normalize(glm::vec3 glm_vec) {
  const auto x = glm_vec.x;
  const auto y = glm_vec.y;
  const auto z = glm_vec.z;
  const auto length = std::sqrt(x * x + y * y + z * 2);
  return {x / length, y / length, z / length};
}
glm::vec3 VectorByAngle(float scalar, float angle_in_radians) {
  float x = scalar * glm::cos(angle_in_radians);
  float y = scalar * glm::sin(angle_in_radians);
  return {x, y, 0.0f};
}
void RenderTileObj(float x_px,
                   float y_px,
                   float width,
                   float height,
                   glm::vec3 position,
                   float orientation,
                   ShaderProgram *shader,
                   float scale) {

  static const auto tile_id = LoadTexture("objects.png");
  constexpr float total_width = 782.0;
  constexpr float total_height = 782.0;

  constexpr float vertices[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
  };

  const auto u_start = x_px / total_width;
  const auto u_end = (x_px + width) / total_width;
  const auto v_start = y_px / total_height;
  const auto v_end = (y_px + height) / total_height;
  const float texture_coordinates[] = {
      u_start, v_end, u_end, v_end, u_end, v_start,
      u_start, v_end, u_end, v_start, u_start, v_start
  };
  glVertexAttribPointer(shader->positionAttribute, 2, GL_FLOAT, false, 0,
                        vertices);
  glEnableVertexAttribArray(shader->positionAttribute);
  glVertexAttribPointer(shader->texCoordAttribute, 2, GL_FLOAT, false, 0,
                        texture_coordinates);
  glEnableVertexAttribArray(shader->texCoordAttribute);
  constexpr auto base_matrix = glm::mat4(1.0f);
  auto model_matrix = glm::translate(base_matrix, position);
  model_matrix =
      glm::rotate(model_matrix,
                  orientation - glm::pi<float>() / 2,
                  glm::vec3(0.0f, 0.0f, 1.0f));
  model_matrix = glm::scale(model_matrix,
                            glm::vec3(width / 100.0 * scale,
                                      height / 100.0 * scale,
                                      1.0f));
  shader->SetModelMatrix(model_matrix);
  glBindTexture(GL_TEXTURE_2D, tile_id);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(shader->positionAttribute);
  glDisableVertexAttribArray(shader->texCoordAttribute);
}
float FlipAngle(float scalar) {
  return (scalar + glm::pi<float>());
}
}