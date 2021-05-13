#include "Gamora.h"
#include "Screen.h"
#include "AnimatedSprite.h"
#include <cmath>
#include <vector>

Gamora::Gamora() : sprite_sheet_(nullptr), pos_rect_({0,0,0,0}), sprite_({16,16}) {}

Gamora::~Gamora() {
  if(sprite_sheet_) {
    SDL_DestroyTexture(sprite_sheet_);
    sprite_sheet_ = nullptr;
  }
}

void Gamora::Init(Screen& screen, SDL_Surface* spriteSheetSurface) {
  sprite_sheet_ = screen.CreateTextureFromSurface(spriteSheetSurface);
  pos_rect_ = {0, 0, 16, 16};

  // Set up animation
  int frameTime = 250;

  std::vector<Frame> upFrames;
  upFrames.push_back({frameTime, 0, 0, false});
  upFrames.push_back({frameTime, 16, 0, false});
  sprite_.AddAnimation("up", upFrames);

  std::vector<Frame> downFrames;
  downFrames.push_back({frameTime, 0, 16, false});
  downFrames.push_back({frameTime, 16, 16, false});
  sprite_.AddAnimation("down", downFrames);

  frameTime = 125;
  std::vector<Frame> rightFrames;
  rightFrames.push_back({frameTime, 0, 32, false});
  rightFrames.push_back({frameTime, 16, 32, false});
  sprite_.AddAnimation("right", rightFrames);

  std::vector<Frame> leftFrames;
  leftFrames.push_back({frameTime, 0, 32, true});
  leftFrames.push_back({frameTime, 16, 32, true});
  sprite_.AddAnimation("left", leftFrames);
  sprite_.SetCurrentAnimation("down");
}

void Gamora::Update(int dt) {
  float moveAmount = dt / 1000.0f * speed_;

  sprite_.Play();
  switch(direction()) { 
  case MovementDirection::kDown:
    sprite_.SetCurrentAnimation("down");
    MoveBy(0, moveAmount);
    break;
  case MovementDirection::kUp:
    sprite_.SetCurrentAnimation("up");
    MoveBy(0, -moveAmount);
    break;
  case MovementDirection::kLeft:
    sprite_.SetCurrentAnimation("left");
    MoveBy(-moveAmount, 0);
    break;
  case MovementDirection::kRight:
    sprite_.SetCurrentAnimation("right");
    MoveBy(moveAmount, 0);
    break;
  case MovementDirection::kNone:
    sprite_.Stop();
    break;
  }

  sprite_.Update(dt);
}

void Gamora::Draw(Screen& screen) {
  sprite_.Draw(screen, sprite_sheet_, pos_rect_);
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
}
