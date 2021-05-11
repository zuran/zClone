#pragma once

#include <functional>

class GameController;

class InputManager {
 public:
  InputManager();
  inline void Init(const std::function<void()>& quit_action) {
    quit_action_ = quit_action;
  }
  void Update(int dt);
  inline void set_current_controller(GameController* controller) {
    current_controller_ = controller;
  }
 private:
  GameController* current_controller_;
  std::function<void()> quit_action_;
};
