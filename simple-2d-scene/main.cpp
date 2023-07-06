/**
* Author: Yifei Yao
* Assignment: Simple 2D Scene
* Date due: 2023-06-11, 11:59pm
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
#include "stb_image.h"

enum Coordinate {
  x_coordinate,
  y_coordinate
};

#define LOG(argument) std::cout << argument << '\n'

const int WINDOW_WIDTH = 640,
    WINDOW_HEIGHT = 480;

const float BG_RED = 0.1922f,
    BG_BLUE = 0.549f,
    BG_GREEN = 0.9059f,
    BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
    VIEWPORT_Y = 0,
    VIEWPORT_WIDTH = WINDOW_WIDTH,
    VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
    F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL =
    0;  // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0;   // this value MUST be zero

const char COW_SPRITE_FILEPATH[] = "cow.png";
const char SAUCER_SPRITE_FILEPATH[] = "flying-saucer.png";

const float LEFT_BORDER = -4.6f;
int beam_count = 0;
const float RIGHT_BORDER = 4.6f;
float cow_angle = 0.0;
SDL_Window *display_window;
bool game_is_running = true;
bool is_beaming = false;
float cool_down = 0.0;
double cool_down_small = 0.0;
float turn_cool_down = 0.0;
bool is_going_left = true;
const float FLOOR = -1.0f;

ShaderProgram program;
glm::mat4 view_matrix, cow_matrix, projection_matrix, trans_matrix,
    saucer_matrix;

float previous_ticks = 0.0f;

GLuint cow_texture_id;
GLuint saucer_texture_id;
SDL_Joystick *player_one_controller;

// overall position
glm::vec3 cow_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ship_position = glm::vec3(0.0f, 2.0f, 0.0f);

// movement tracker
glm::vec3 cow_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ship_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

GLuint LoadTexture(const char *filepath) {
  // STEP 1: Loading the image file
  int width, height, number_of_components;
  unsigned char *image = stbi_load(filepath, &width, &height,
                                   &number_of_components, STBI_rgb_alpha);

  if (image == NULL) {
    LOG("Unable to load image. Make sure the path is correct.");
    assert(false);
  }

  // STEP 2: Generating and binding a texture ID to our image
  GLuint textureID;
  glGenTextures(NUMBER_OF_TEXTURES, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height,
               TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

  // STEP 3: Setting our texture filter parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // STEP 4: Releasing our file from memory and returning our texture id
  stbi_image_free(image);

  return textureID;
}

void initialise() {
  // Initialise video and joystick subsystems
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

  // Open the first controller found. Returns null on error
  player_one_controller = SDL_JoystickOpen(0);

  display_window = SDL_CreateWindow("Simple 2d scene",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT,
                                    SDL_WINDOW_OPENGL);

  SDL_GLContext context = SDL_GL_CreateContext(display_window);
  SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
  glewInit();
#endif

  glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

  program.Load(V_SHADER_PATH, F_SHADER_PATH);

  cow_matrix = glm::mat4(1.0f);
  saucer_matrix = glm::mat4(1.0f);
  view_matrix = glm::mat4(
      1.0f);  // Defines the position (location and orientation) of the camera
  projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f,
                                 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.

  program.SetProjectionMatrix(projection_matrix);
  program.SetViewMatrix(view_matrix);
  // Notice we haven't set our model matrix yet!

  glUseProgram(program.programID);

  glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

  cow_texture_id = LoadTexture(COW_SPRITE_FILEPATH);
  saucer_texture_id = LoadTexture(SAUCER_SPRITE_FILEPATH);

  // enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_WINDOWEVENT_CLOSE:
      case SDL_QUIT:game_is_running = false;
        break;
      default:break;
    }
  }
}

void Update() {
  float ticks = (float) SDL_GetTicks() /
      MILLISECONDS_IN_SECOND; // get the current number of ticks
  float delta_time = ticks -
      previous_ticks; // the delta time is the difference from the last frame
  previous_ticks = ticks;

  if (abs(cow_position[0] - ship_position[0]) < 0.1 && ticks > cool_down) {
    is_beaming = true;
    beam_count += 1;
  }
  if (abs(cow_position[1] - ship_position[1]) < 0.5) {
    is_beaming = false;
    cow_velocity[1] = 0.0;
    is_going_left = !is_going_left;
    if (ticks > cool_down) {
      cool_down = ticks + 8.0;
      cool_down_small = ticks + 2.0;
    }
  }
  if (is_beaming || cool_down_small > ticks || cow_position[1] > 0) {
    ship_velocity = glm::vec3(0, 0, 0);
  } else {
    if (ship_velocity[1] > 0) {
      ship_velocity[1] = 0;
    }
    ship_velocity = glm::vec3(cow_position[0] - ship_position[0], 0, 0);
    if (ship_velocity[0] > 0.7) {
      ship_velocity[0] = 0.7;
    } else if (ship_velocity[0] < 0.7) {
      ship_velocity[0] = -0.7;
    }
    ship_position +=
        ship_velocity * delta_time
            * 1.0f;
  }
  saucer_matrix = glm::mat4(1.0f);
  saucer_matrix = glm::translate(saucer_matrix, ship_position);

  cow_matrix = glm::mat4(1.0f);
  const float COW_ROT_ANGLE = glm::radians(0.1f);
  bool was_going = is_going_left;
  float progress = (ship_position[1] - cow_position[1] - 0.5) / 2.0;
  if (is_beaming) {
    if (is_going_left) { cow_angle += COW_ROT_ANGLE; }
    else {
      cow_angle -= COW_ROT_ANGLE;
    }
    cow_velocity = glm::vec3(0.0f, 0.6f, 0.0f);

  } else if (cool_down_small > ticks) {
    cow_angle = 0;
    cow_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  } else if
      (cow_position[1] > FLOOR) {
    cow_angle = 0;
    cow_velocity[0] = beam_count % 2 == 0 ? 1.5 : -1.5;
    cow_velocity[1] -= 4.0 * delta_time;
  } else {
    cow_angle = 0;

    if (turn_cool_down < ticks) {
      if (cow_position[0] < ship_position[0]) {
        is_going_left = true;
      } else
        is_going_left = false;
    }

    if (cow_position[0] < LEFT_BORDER) {
      is_going_left = false;
      turn_cool_down = ticks + 5;
    } else if (cow_position[0] > RIGHT_BORDER) {
      is_going_left = true;
      turn_cool_down = ticks + 5;
    }
    if (is_going_left) {
      cow_velocity[0] -= 0.01;
      if (cow_velocity[0] < -0.9) {
        cow_velocity[0] = -0.9;
      }
    } else {
      cow_velocity[0] += 0.01;
      if (cow_velocity[0] > 0.9) {
        cow_velocity[0] = 0.9;
      }
    }

  }
  cow_position +=
      cow_velocity * delta_time
          * 1.0f;
  if (cow_position[1] < FLOOR) {
    cow_position[1] =
        FLOOR;
    cow_velocity[1] = 0;
  }

  cow_matrix = glm::translate(cow_matrix, cow_position);
  cow_matrix = glm::rotate(cow_matrix, cow_angle,
                           glm::vec3(0.0f, 0.0f, 1.0f));
  if (cool_down_small > ticks) {
    if (int(ticks * 1000) % 1000 == 0) {
      is_going_left = !was_going;
    }
  }
  if (!is_going_left) {
    cow_matrix = glm::scale(cow_matrix, glm::vec3(-1.0f, 1.0f, 1.0f));
  }
  if (is_beaming) {
    cow_matrix = glm::scale(cow_matrix,
                            glm::vec3(1.0f * progress, 1.0f * progress,
                                      1.0f * progress));
  }
}

void DrawObject(glm::mat4 &object_model_matrix, GLuint &object_texture_id) {
  program.SetModelMatrix(object_model_matrix);
  glBindTexture(GL_TEXTURE_2D, object_texture_id);
  glDrawArrays(GL_TRIANGLES, 0,
               6); // we are now drawing 2 triangles, so we use 6 instead of 3
}

void Render() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Vertices
  float vertices[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
  };

  // Textures
  float texture_coordinates[] = {
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
      0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
  };

  glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0,
                        vertices);
  glEnableVertexAttribArray(program.positionAttribute);

  glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0,
                        texture_coordinates);
  glEnableVertexAttribArray(program.texCoordAttribute);

  // Bind texture
  DrawObject(cow_matrix, cow_texture_id);
  DrawObject(saucer_matrix, saucer_texture_id);

  // We disable two attribute arrays now
  glDisableVertexAttribArray(program.positionAttribute);
  glDisableVertexAttribArray(program.texCoordAttribute);

  SDL_GL_SwapWindow(display_window);
}

void shutdown() {
  SDL_JoystickClose(player_one_controller);
  SDL_Quit();
}

/**
 Start here—we can see the general structure of a game loop without worrying too much about the details yet.
 */
int main(int argc, char *argv[]) {
  initialise();

  while (game_is_running) {
    ProcessInput();
    Update();
    Render();
  }

  shutdown();
  return 0;
}
