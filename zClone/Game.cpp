#include "Game.h"
#include "GameController.h"
#include "Gamora.h"
#include <iostream>
#include <SDL_image.h>

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
  int dt = 0;

  Gamora gamora;
  SetupGamora(gamora);

  GameController controller = SetupGameController(gamora);
  input_manager_.Init([&isRunning]() { isRunning = false; });
  input_manager_.set_current_controller(&controller);

  while(isRunning) {
    int frameBegin = SDL_GetTicks();
    
    input_manager_.Update(dt);

    gamora.Draw(screen_);

    screen_.Present();
    SDL_Delay(10);
    int frameEnd = SDL_GetTicks();
    int totalFrameTime = frameEnd - frameBegin;
    dt = totalFrameTime;
    screen_.Clear();
  }
}

GameController Game::SetupGameController(Gamora& gamora) const { 
  GameController controller = GameController();
  KeyAction eAction = [](int dt, Uint8 state, Uint8 previousState) {
    if (state == SDL_PRESSED && previousState == SDL_RELEASED) {
      std::cout << "E" << std::endl;
    }
  };
  controller.RegisterKeyAction(SDL_SCANCODE_E, eAction);
  float speed = 100.0f;
  MovementAction moveAction = [&gamora, speed](int dt, Uint8 leftState, Uint8 rightState,
                                 Uint8 upState, Uint8 downState) {
    float amount = dt / 1000.0f * speed;
    if(downState && !upState) {
      std::cout << "Down" << std::endl;
      gamora.MoveBy(0, amount);
    } else if(!downState && upState) {
      std::cout << "Up" << std::endl;
      gamora.MoveBy(0, -amount);
    } else if(leftState && !rightState) {
      std::cout << "Left" << std::endl;
      gamora.MoveBy(-amount, 0);
    } else if(!leftState && rightState) {
      std::cout << "Right" << std::endl;
      gamora.MoveBy(amount, 0);
    } else {
      //std::cout << "None" << std::endl;
    }
  };
  controller.RegisterMovementAction(moveAction);

  return controller;
}

void Game::SetupGamora(Gamora& gamora) {
  std::string gamoraSpriteSheetPath =
      static_cast<std::string>(SDL_GetBasePath()) + "../Assets/turtle_walk.png";
  SDL_Surface* gamoraMovementImage = IMG_Load(gamoraSpriteSheetPath.c_str());
  gamora.Init(screen_, gamoraMovementImage);
  gamora.MoveBy(kWidth / 2.0f, kHeight / 2.0f);

  SDL_FreeSurface(gamoraMovementImage);
}
