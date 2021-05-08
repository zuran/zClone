#pragma once

#include "AnimatedSprite.h"

class Screen;
class CollisionRect;

enum class MovementDirection {
  kNone = 0,
  kUp,
  kLeft,
  kDown,
  kRight
};

class Actor {
 public:
  virtual ~Actor() {}
  virtual void Update(int dt);
  virtual void Draw(Screen &screen);
  virtual void Stop();

  inline MovementDirection direction() const { return direction_; }
  inline void set_direction(MovementDirection direction) {
    direction_ = direction;
  }

 private:
  MovementDirection direction_;
  AnimatedSprite sprite_;
};
