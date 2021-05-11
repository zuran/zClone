#pragma once

#include <SDL.h>
#include "Screen.h"
#include "InputManager.h"

class Game {
 public:
  static Game& Singleton();
  bool Init(int width, int height, int magnification);
  void Run();

 private:
  GameController SetupGameController() const;

  SDL_Window* window_;
  Screen screen_;

  InputManager input_manager_;  
};
