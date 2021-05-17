#pragma once

#include "Actor.h"
#include "AnimatedSprite.h"
#include "Overworld.h"
#include <SDL.h>

class Screen;

class Gamora : public Actor {
 public:
  const static SDL_Rect kLeftFrame;

  Gamora();
  ~Gamora();
  void Init(Screen& screen, SDL_Surface* spriteSheetSurface, Overworld& overworld);
  virtual void Update(int dt) override;
  virtual void Draw(Screen& screen) override;
  virtual void Stop() override;
  void MoveBy(float x, float y);

 private:
  SDL_Texture* sprite_sheet_;
  SDL_Rect pos_rect_;

  AnimatedSprite sprite_;

  float speed_ = 100.0f;
  Overworld* overworld_;
};
