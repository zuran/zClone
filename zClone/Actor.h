#pragma once

class Screen;

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

  inline MovementDirection direction() const { return direction_; }
  inline void direction(MovementDirection dir) { direction_ = dir; }

 private:
  MovementDirection direction_;
};
