#pragma once

#include <SDL.h>
#include "Color.h"

class Screen {
 public:
  Screen();
  ~Screen();

  SDL_Window* Init(int width, int height, int magnification);
  void Present();
  void Clear();

 private:
  Screen(const Screen& screen);
  Screen& operator=(const Screen& screen);

  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* buffer_;
};
