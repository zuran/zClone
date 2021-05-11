#pragma once

#include "Actor.h"
#include <SDL.h>

class Screen;

class Gamora : public Actor {
 public:
  Gamora();
  ~Gamora();
  void Init(Screen& screen, SDL_Surface* spriteSheetSurface);
  virtual void Update(int dt) override;
  virtual void Draw(Screen& screen) override;
  virtual void Stop() override;
  void MoveBy(float x, float y);

 private:
  SDL_Texture* sprite_sheet_;
  SDL_Rect frame_rect_;
  SDL_Rect pos_rect_;
};
