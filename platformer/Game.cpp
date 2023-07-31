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
  const auto background = Background(std::string("background.png"));
  const auto mapping =
      std::vector<uint>{6, 7, 8, 6, 7, 8, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE,
                        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE,
                        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
                        NONE, NONE, NONE,
                        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
  const auto index_mapping = LevelMapping(12, 5, mapping);
  const auto tile_set_id = utility::LoadTexture(std::string("tileset.png"));
  const auto tile_set = SpriteSheetMapping(10, 6, tile_set_id);
  const auto top_left = glm::vec3(-4, 0, 0);
  const auto map = Map(index_mapping, tile_set, 0.5, top_left);
  const auto player_id = utility::LoadTexture(std::string("player.png"));
  const auto player = new Player(glm::vec3(0, 0, 0), player_id);
  const auto mob0_id = utility::LoadTexture(std::string("mob1.png"));
  const auto mob0_config = MobConfig{MobType::Jumper};
  const auto mob0 = new Mob(glm::vec3(-3, 1, 0), mob0_id, mob0_config);
  const auto mob2_id = utility::LoadTexture(std::string("mob2.png"));
  const auto mob2_config = MobConfig{MobType::Patroller};
  const auto mob2 = new Mob(glm::vec3(1, -1, 0), mob2_id, mob2_config);
  const auto mob3_id = utility::LoadTexture(std::string("mob3.png"));
  const auto mob3_config = MobConfig{MobType::Chaser};
  const auto mob3 = new Mob(glm::vec3(2, 2, 0), mob3_id, mob3_config);
  manager_ = std::make_unique<EntityManager>(background,
                                             map,
                                             player,
                                             std::vector<Mob *>{
                                                mob0, mob2, mob3
                                            });
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
  const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
  if (key_state[SDL_SCANCODE_A]) {
    manager_->player_->MoveLeft();
  }
  if (key_state[SDL_SCANCODE_D]) {
    manager_->player_->MoveRight();
  }
  if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_SPACE]) {
    manager_->player_->Jump(2.3, *manager_);
  }
}
void Game::Update() {
  const auto ticks = static_cast<float>(SDL_GetTicks()) / 1000.0f;
  const auto delta_time = static_cast<float>(ticks - previous_ticks_);
  previous_ticks_ = ticks;
  constexpr auto FIXED_TIMESTEP = 1.0f / 60.0f;
  for (time_accumulator_ += delta_time; time_accumulator_ >= FIXED_TIMESTEP;
       time_accumulator_ -= FIXED_TIMESTEP) {
    manager_->UpdateAll(FIXED_TIMESTEP);
  }
}
void Game::Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  // Render all
  manager_->RenderAll(&shader_);
  SDL_GL_SwapWindow(display_window_);
}
void Game::Run() {
  while (is_game_running_) {
    ProcessInput();
    Update();
    Render();
  }
}