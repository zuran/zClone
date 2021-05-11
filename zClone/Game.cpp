#include "Game.h"
#include "GameController.h"
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
  GameController controller = SetupGameController();
  input_manager_.set_current_controller(&controller);

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

GameController Game::SetupGameController() const { 
  GameController controller = GameController();
  KeyAction eAction = [](int dt, Uint8 state, Uint8 previousState) {
    if (state == SDL_PRESSED && previousState == SDL_RELEASED) {
      std::cout << "E" << std::endl;
    }
  };
  controller.RegisterKeyAction(SDL_SCANCODE_E, eAction);

  MovementAction moveAction = [](int dt, Uint8 leftState, Uint8 rightState,
                                 Uint8 upState, Uint8 downState) {
    if(downState && !upState) {
      std::cout << "Down" << std::endl;
    } else if(!downState && upState) {
      std::cout << "Up" << std::endl;
    } else if(leftState && !rightState) {
      std::cout << "Left" << std::endl;
    } else if(!leftState && rightState) {
      std::cout << "Right" << std::endl;
    } else {
      std::cout << "None" << std::endl;
    }
  };
  controller.RegisterMovementAction(moveAction);

  return controller;
}
