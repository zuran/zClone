#pragma once

#include "Actor.h"

#include <SDL.h>
#include <vector>
#include <map>

class Screen;

class Overworld {
 public:
  Overworld();
  ~Overworld();
  void Init(Screen& screen);
  void Update(int dt);
  void Draw(Screen& screen);
  void SetSafeLocationIfColliding(SDL_Rect& gamoraPosRect, MovementDirection direction);

 private:
  SDL_Texture* full_map_;
  SDL_Texture* tiles_;
  SDL_Rect area_rect_;
  SDL_Rect area_draw_rect_;
  int current_area_x = 0;
  int current_area_y = 0;
  std::map<int, std::vector<SDL_Rect>> tile_collisions_;
  std::vector<int> overworld_data_;
  std::vector<int> area_data_;
};
