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
#include "Menu.h"
Game::Game()
    : curr_scene_(nullptr),
      projection_matrix_(glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f)),
      view_matrix_(glm::mat4(1.0f)
      ) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  constexpr int WINDOW_WIDTH = 1280,
      WINDOW_HEIGHT = 960;
  constexpr int VIEWPORT_X = 0,
      VIEWPORT_Y = 0,
      VIEWPORT_WIDTH = WINDOW_WIDTH,
      VIEWPORT_HEIGHT = WINDOW_HEIGHT;
  SDL_Init(SDL_INIT_VIDEO);
  display_window_ = SDL_CreateWindow("LAST STAND",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(display_window_);
  SDL_GL_MakeCurrent(display_window_, context
  );
#ifdef _WINDOWS
  glewInit();
#endif
  glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT
  );
  shader_.Load("shaders/vertex_textured.glsl",
               "shaders/fragment_textured.glsl");
  shader_.
      SetProjectionMatrix(projection_matrix_);
  shader_.
      SetViewMatrix(view_matrix_);
  glUseProgram(shader_
                   .programID);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Mix_OpenAudio(
      44100,
      MIX_DEFAULT_FORMAT, 2, 4096
  );
  const auto music = Mix_LoadMUS("background.mp3");
  Mix_PlayMusic(music,
                -1);
  Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
  curr_scene_ = std::make_unique<Menu>();
}
void Game::Update() {
  this->event_frame_.Poll();
  if (event_frame_.any_quit()) {
    is_game_running_ = false;
  }
  const auto ticks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
  const auto delta_time = static_cast<float>(ticks - previous_ticks_);
  previous_ticks_ = ticks;
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  for (time_accumulator_ += delta_time; time_accumulator_ >= FIXED_TIMESTEP;
       time_accumulator_ -= FIXED_TIMESTEP) {
    if (life_ != 0 && !gg) {
      const auto feedback = curr_scene_->Update(FIXED_TIMESTEP, event_frame_);
      switch (feedback) {
        case Feedback::NextStage: {
          GoNextLevel();
          break;
        }
        case Feedback::NoOp: {
          break;
        }
        case Feedback::TakeDamage: {
          const auto curr_ticks = SDL_GetTicks();
          if (curr_ticks > immune_time_out) {
            life_ -= 1;
            std::cout << "Take damage!\n";
            immune_time_out = SDL_GetTicks() + 500;
          }
        }
      }
    }
  }
}
void Game::Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  curr_scene_->Render(&shader_, life_);
  if (life_ == 0) {
    utility::RenderText("You LOST!",
                        &shader_,
                        1.0,
                        glm::vec3(-4.0, 2, 0)
                            + curr_scene_->GetPlayerPosition());
  }
  if (gg) {
    utility::RenderText("You WON!",
                        &shader_,
                        1.0,
                        glm::vec3(-3.5, 2, 0)
                            + curr_scene_->GetPlayerPosition());
  }
  SDL_GL_SwapWindow(display_window_);
}
void Game::Run() {
  while (is_game_running_) {
    Update();
    Render();
  }
}
void Game::GoNextLevel() {
  switch (curr_scene_->Id()) {
    case 0: {
      curr_scene_ = std::make_unique<Level>();
      break;
    }
    case 1: {
      gg = true;
      break;
    }
    default: {
      throw std::runtime_error("Invalid scene id");
    }
  }
}
void EventFrame::Poll() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
      case SDL_WINDOWEVENT_CLOSE: {
        any_quit_ = true;
        break;
      }
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_q: {
            any_quit_ = true;
            break;
          }
          case SDLK_RETURN:
          case SDLK_KP_ENTER: {
            enter_key_down_ = true;
            break;
          }
          case SDLK_SPACE: {
            space_key_down_ = true;
            break;
          }
          default:break;
        }
      case SDL_MOUSEBUTTONDOWN: {
        if (event.button.button == SDL_BUTTON_LEFT) {
          left_mouse_down_ = true;
        }
        break;
      }
      default:break;
    }
  }
}
void EventFrame::Reset() {
  left_mouse_down_ = false;
  space_key_down_ = false;
  enter_key_down_ = false;
  any_quit_ = false;
}
bool EventFrame::left_mouse_down() const {
  return left_mouse_down_;
}
bool EventFrame::space_key_down() const {
  return space_key_down_;
}
bool EventFrame::enter_key_down() const {
  return enter_key_down_;
}
bool EventFrame::any_quit() const {
  return any_quit_;
}
