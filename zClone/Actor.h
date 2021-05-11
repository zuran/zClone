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

  inline float x_pos() const { return x_pos_; }
  inline float y_pos() const { return y_pos_; }
  inline void set_x_pos(float xPos) { x_pos_ = xPos; }
  inline void set_y_pos(float yPos) { y_pos_ = yPos; }

 private:
  MovementDirection direction_;
  AnimatedSprite sprite_;
  float x_pos_;
  float y_pos_;
};
