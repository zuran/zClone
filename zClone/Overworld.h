#pragma once

class Screen;
struct SDL_Surface;
struct SDL_Texture;

class Overworld {
 public:
  Overworld();
  ~Overworld();
  void Init(Screen& screen, SDL_Surface* spriteSheetSurface);
  void Update(int dt);
  void Draw(Screen& screen);

 private:
  SDL_Texture* full_map_;
  SDL_Texture* tiles_;
  int current_area_x;
  int current_area_y;
};
