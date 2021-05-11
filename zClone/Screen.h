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

  void Draw(SDL_Texture* spriteSheet, const SDL_Rect& spriteRect,
            SDL_Rect& targetRect);
  void Draw(SDL_Texture* spriteSheet, const SDL_Rect& spriteRect,
            SDL_Rect& targetRect, float rotation, SDL_Point& center,
            SDL_RendererFlip flip);
  SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);

 private:
  Screen(const Screen& screen);
  Screen& operator=(const Screen& screen);

  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* buffer_;
};
