/**
* Author: Yifei Yao
* Assignment: Lunar Lander
* Date due: 2023-07-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"
/* ------------------------- SECTION FORBID CHANGE ------------------------- */
// Contract code such as header files, shader path.
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <algorithm>

constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
        F_SHADER_PATH[] = "shaders/fragment.glsl";

/* ------------------ SECTION GLOBAL CONSTANTS AND DEFINES ------------------ */
// Shared compile time configurations. Keep this as small as possible.

/* ------------------------ SECTION GLOBAL VARIABLES ------------------------ */
// Shared states. Keep this as small as possible.
SDL_Window *g_display_window;
bool is_game_running = true;
ShaderProgram program;

/* --------------------------- SECTION FUNCTIONS --------------------------- */
int main() {}
