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
  Screen(const Screen& screen);
  Screen& operator=(const Screen& screen);

  void Clear();

  SDL_Renderer* renderer_;
  SDL_Texture* texture_;
  SDL_PixelFormat* pixel_format_;
};
