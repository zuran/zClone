#pragma once

#include <SDL.h>
#include "Screen.h"
#include "InputManager.h"
class Gamora;

class Game {
 public:
  static Game& Singleton();
  bool Init(int width, int height, int magnification);
  void Run();

 private:
  GameController SetupGameController(Gamora& gamora) const;
  void SetupGamora(Gamora& gamora);

  SDL_Window* window_;
  Screen screen_;

  InputManager input_manager_;  
};
