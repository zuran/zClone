#pragma once

#include <SDL.h>

class Screen;

class Overworld {
 public:
  Overworld();
  ~Overworld();
  void Init(Screen& screen);
  void Update(int dt);
  void Draw(Screen& screen);

 private:
  SDL_Texture* full_map_;
  SDL_Texture* tiles_;
  SDL_Rect area_rect_;
  SDL_Rect area_draw_rect_;
  int current_area_x = 0;
  int current_area_y = 0;
};
