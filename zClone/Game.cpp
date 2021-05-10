#include "Game.h"
#include <iostream>

static const int kWidth = 256;
static const int kHeight = 240;
static const int kMagnification = 4;

int main(int argc, char** args) {
  bool didInit =
      Game::Singleton().Init(kWidth, kHeight, kMagnification);
  if(didInit) {
    Game::Singleton().Run();
  }
  return 0;
}

Game& Game::Singleton() {
  static Game game;
  return game;
}

bool Game::Init(int width, int height, int magnification) { 
  window_ = screen_.Init(width, height, magnification);
  if (!window_) return false;
  return true;
}

void Game::Run() { 
  bool isRunning = true;  
  float dt = 0.0f;

  input_manager_.Init([&isRunning]() { isRunning = false; });

  while(isRunning) {
    int frameBegin = SDL_GetTicks();
    
    input_manager_.Update(dt);

    SDL_Delay(10);
    int frameEnd = SDL_GetTicks();
    int totalFrameTime = frameBegin - frameEnd;
    dt = totalFrameTime / 1000.0f;
    screen_.Clear();
  }
}
