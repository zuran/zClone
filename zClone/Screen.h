#pragma once

#include <SDL.h>
#include "Color.h"

class Screen {
 public:
  Screen();
  ~Screen();

  SDL_Window* Init(int w, int h, int mag);
  void SwapBuffer();

 private:
};
