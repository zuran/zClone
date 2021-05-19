#include "Gamora.h"
#include "Screen.h"
#include "AnimatedSprite.h"
#include <cmath>
#include <vector>

Gamora::Gamora()
    : sprite_sheet_(nullptr),
      pos_rect_({0, 0, 16, 16}),
      gamora_sprite_({16, 16}),
      overworld_(nullptr),
      attack_sprite_({16, 16}) {}

Gamora::~Gamora() {
  if(sprite_sheet_) {
    SDL_DestroyTexture(sprite_sheet_);
    sprite_sheet_ = nullptr;
  }
}

void Gamora::Init(Screen& screen, SDL_Surface* spriteSheetSurface, Overworld& overworld) {
  overworld_ = &overworld;
  sprite_sheet_ = screen.CreateTextureFromSurface(spriteSheetSurface);
  
  // Set up animations
  int frameTime = 250;
  std::vector<Frame> upFrames;
  upFrames.push_back({frameTime, 0, 0, SDL_FLIP_NONE});
  upFrames.push_back({frameTime, 16, 0, SDL_FLIP_NONE});
  gamora_sprite_.AddAnimation("up", upFrames);

  std::vector<Frame> downFrames;
  downFrames.push_back({frameTime, 0, 16, SDL_FLIP_NONE});
  downFrames.push_back({frameTime, 16, 16, SDL_FLIP_NONE});
  gamora_sprite_.AddAnimation("down", downFrames);

  frameTime = 125;
  std::vector<Frame> rightFrames;
  rightFrames.push_back({frameTime, 0, 32, SDL_FLIP_NONE});
  rightFrames.push_back({frameTime, 16, 32, SDL_FLIP_NONE});
  gamora_sprite_.AddAnimation("right", rightFrames);

  std::vector<Frame> leftFrames;
  leftFrames.push_back({frameTime, 0, 32, SDL_FLIP_HORIZONTAL});
  leftFrames.push_back({frameTime, 16, 32, SDL_FLIP_HORIZONTAL});
  gamora_sprite_.AddAnimation("left", leftFrames);
  gamora_sprite_.SetCurrentAnimation("down");
}

void Gamora::Update(int dt) {
  float moveAmount = dt / 1000.0f * speed_;

  gamora_sprite_.Play();
  switch(direction()) { 
  case MovementDirection::kDown:
    gamora_sprite_.SetCurrentAnimation("down");
    MoveBy(0, moveAmount);
    break;
  case MovementDirection::kUp:
    gamora_sprite_.SetCurrentAnimation("up");
    MoveBy(0, -moveAmount);
    break;
  case MovementDirection::kLeft:
    gamora_sprite_.SetCurrentAnimation("left");
    MoveBy(-moveAmount, 0);
    break;
  case MovementDirection::kRight:
    gamora_sprite_.SetCurrentAnimation("right");
    MoveBy(moveAmount, 0);
    break;
  case MovementDirection::kNone:
    gamora_sprite_.Stop();
    break;
  }

  gamora_sprite_.Update(dt);
}

void Gamora::Draw(Screen& screen) {
  gamora_sprite_.Draw(screen, sprite_sheet_, pos_rect_);
}

void Gamora::Stop() {}

// Gamora's absolute position is a floating point vector, but she can only be
// displayed using an integer vector represented by the x and y of pos_rect_.
// pos_rect_'s width and height remain unchanged, and the bounding box is
// just translated as it's needed for drawing anyway.
//
// When Gamora's movement is perpendicular to her previous direction, she's
// realigned to the 8x8 subgrid.
void Gamora::MoveBy(float x, float y) { 
  Actor::set_x_pos(Actor::x_pos() + x);
  Actor::set_y_pos(Actor::y_pos() + y);

  // Align Gamora with the tile subgrid (8x8)
  if(y == 0) {
    float gridY = std::roundf(Actor::y_pos() / 8);
    Actor::set_y_pos(gridY * 8);
  } else {
    float gridX = std::roundf(Actor::x_pos() / 8);
    Actor::set_x_pos(gridX * 8);
  }

  pos_rect_.x = static_cast<int>(Actor::x_pos());
  pos_rect_.y = static_cast<int>(Actor::y_pos());
  int tempX = pos_rect_.x;
  int tempY = pos_rect_.y;
  overworld_->SetSafeLocationIfColliding(pos_rect_, direction());
  if (pos_rect_.x != tempX) Actor::set_x_pos(pos_rect_.x);
  if (pos_rect_.y != tempY) Actor::set_y_pos(pos_rect_.y);
}
