#pragma once

#include "Color.h"
#include <SDL.h>
#include <vector>

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
  void DrawOverworldTiles(SDL_Texture* tileset, SDL_Texture* map,
                          std::vector<int> data, int tilesetColumns,
                          int dataWidth, int dataHeight);
  void DrawCollisionRect(SDL_Rect& collisionRect);

  SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
  SDL_Texture* CreateTexture(int width, int height);

 private:
  Screen(const Screen& screen);
  Screen& operator=(const Screen& screen);

  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* buffer_;
};
