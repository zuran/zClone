#include "Gamora.h"
#include "Screen.h"
#include <cmath>

Gamora::Gamora() : sprite_sheet_(nullptr), pos_rect_({0,0,0,0}), frame_rect_({0,0,0,0}) {}

Gamora::~Gamora() {
  if(sprite_sheet_) {
    SDL_DestroyTexture(sprite_sheet_);
    sprite_sheet_ = nullptr;
  }
}

void Gamora::Init(Screen& screen, SDL_Surface* spriteSheetSurface) {
  sprite_sheet_ = screen.CreateTextureFromSurface(spriteSheetSurface);
  pos_rect_ = {0, 0, 16, 16};
  frame_rect_ = {0, 0, 16, 16};
}

void Gamora::Update(int dt) {}

void Gamora::Draw(Screen& screen) {
  screen.Draw(sprite_sheet_, frame_rect_, pos_rect_);
}

void Gamora::Stop() {}

void Gamora::MoveBy(float x, float y) { 
  Actor::set_x_pos(Actor::x_pos() + x);
  Actor::set_y_pos(Actor::y_pos() + y);

  if(y == 0) {
    float gridY = std::roundf(Actor::y_pos() / 8);
    Actor::set_y_pos(gridY * 8);
  } else {
    float gridX = std::roundf(Actor::x_pos() / 8);
    Actor::set_x_pos(gridX * 8);
  }

  pos_rect_.x = static_cast<int>(Actor::x_pos());
  pos_rect_.y = static_cast<int>(Actor::y_pos());
}
