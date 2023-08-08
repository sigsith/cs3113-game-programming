/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_UTILITY_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_UTILITY_H_
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include <unordered_map>

namespace utility {
GLuint LoadTexture(const std::string &path);
void RenderText(const std::string &text,
                ShaderProgram *shader,
                float tile_size, glm::vec3 top_left
);
void RenderTileObj(float x_px,
                   float y_px,
                   float width,
                   float height,
                   glm::vec3 position,
                   float orientation,
                   float scale,
                   ShaderProgram *shader);
float Length(glm::vec3 glm_vec);
glm::vec3 Normalize(glm::vec3 glm_vec);
glm::vec3 VectorByAngle(float scalar, float angle_in_radians);
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_UTILITY_H_
