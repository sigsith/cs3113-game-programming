/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Game.h"
#include "Level.h"
Game::Game() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  constexpr int WINDOW_WIDTH = 640,
      WINDOW_HEIGHT = 480;
  constexpr int VIEWPORT_X = 0,
      VIEWPORT_Y = 0,
      VIEWPORT_WIDTH = WINDOW_WIDTH,
      VIEWPORT_HEIGHT = WINDOW_HEIGHT;
  SDL_Init(SDL_INIT_VIDEO);
  display_window_ = SDL_CreateWindow("Generic Platform Game2",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     640, 480,
                                     SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(display_window_);
  SDL_GL_MakeCurrent(display_window_, context);
#ifdef _WINDOWS
  glewInit();
#endif
  glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
  shader_.Load("shaders/vertex_textured.glsl",
               "shaders/fragment_textured.glsl");
  const auto view_matrix = glm::mat4(1.0f);
  const auto
      projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
  shader_.SetProjectionMatrix(projection_matrix);
  shader_.SetViewMatrix(view_matrix);
  glUseProgram(shader_.programID);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  curr_scene_ = new Level0();
  Mix_OpenAudio(
      44100,
      MIX_DEFAULT_FORMAT, 2, 4096
  );
  const auto music = Mix_LoadMUS("background.mp3");
  Mix_PlayMusic(music, -1);
  Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}
void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
      case SDL_WINDOWEVENT_CLOSE:is_game_running_ = false;
        return;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_q:is_game_running_ = false;
            return;
          default:break;
        }
      default:break;
    }
  }
  curr_scene_->UpdateInput(SDL_GetKeyboardState(nullptr));
}
void Game::Update() {
  const auto ticks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
  const auto delta_time = static_cast<float>(ticks - previous_ticks_);
  previous_ticks_ = ticks;
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  for (time_accumulator_ += delta_time; time_accumulator_ >= FIXED_TIMESTEP;
       time_accumulator_ -= FIXED_TIMESTEP) {
    if (life_ != 0) {
      const auto feedback = curr_scene_->Update(FIXED_TIMESTEP);
      switch (feedback) {
        case Feedback::NextStage: {
          break;
        }
        case Feedback::NoOp: {
          break;
        }
        case Feedback::TakeDamage: {
          life_ -= 1;
        }
      }
    }
  }
}
void Game::Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  curr_scene_->Render(&shader_);
  SDL_GL_SwapWindow(display_window_);
}
void Game::Run() {
  while (is_game_running_) {
    ProcessInput();
    Update();
    Render();
  }
}
void Game::RenderMessage(const std::string &message) {
  utility::RenderText(message, &shader_, 1.0, glm::vec3(-3.5, 2, 0));
}
