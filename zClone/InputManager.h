#pragma once

class GameController;

class InputManager {
 public:
  InputManager();
  void Update(int dt);
  inline void set_current_controller(GameController* controller) {
    current_controller_ = controller;
  }
 private:
  GameController* current_controller_;
};
