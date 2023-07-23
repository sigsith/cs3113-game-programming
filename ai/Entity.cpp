/**
* Author: Yifei Yao
* Assignment: Rise of the AI
* Date due: 2023-07-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include "stb_image.h"
void Background::Render(ShaderProgram *program) const {
  glBindTexture(GL_TEXTURE_2D, this->texture_id_);
  glVertexAttribPointer(program->positionAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        SQUARE_VERTICES);
  glEnableVertexAttribArray(program->positionAttribute);
  glVertexAttribPointer(program->texCoordAttribute,
                        2,
                        GL_FLOAT,
                        false,
                        0,
                        FULL_TEX_COORDS);
  glEnableVertexAttribArray(program->texCoordAttribute);
  const auto base_matrix = glm::mat4(1.0f);
  const auto scale_factor_x = 10.0f;
  const auto scale_factor_y = 3.0f;
  const auto model_matrix =
      glm::scale(glm::translate(base_matrix, glm::vec3(0.0f, -2.6f, 0.0f)),
                 glm::vec3(scale_factor_x, scale_factor_y, 1.0f));
  program->SetModelMatrix(model_matrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(program->positionAttribute);
  glDisableVertexAttribArray(program->texCoordAttribute);
}
Background::Background(std::string &texture_path) {
  texture_id_ = LoadTexture((texture_path));
}
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
