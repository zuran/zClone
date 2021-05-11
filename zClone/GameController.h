#pragma once

#include <SDL.h>
#include <map>
#include <vector>
#include <functional>

using KeyAction = std::function<void(int dt, Uint8 state, Uint8 previousState)>;
using MovementAction = std::function<void(int dt, Uint8 leftState, Uint8 rightState, Uint8 upState, Uint8 downState)>;

struct KeyInfo {
  Uint8 state;
  KeyAction action;
};

class GameController {
 public:
  GameController();
  void RegisterKeyAction(SDL_Scancode keyCode, const KeyAction& keyAction);
  void RegisterMovementAction(MovementAction& movementAction);
  void ProcessMovement(int dt, Uint8 leftState, Uint8 rightState, Uint8 upState,
                       Uint8 downState) const;
  inline KeyInfo& GetKeyInfo(SDL_Scancode keyCode) {
    return registered_actions_[keyCode];
  }
  inline std::vector<SDL_Scancode> registered_keys() const {
    return registered_keys_;
  }

 private:
  std::map<SDL_Scancode, KeyInfo> registered_actions_;
  std::vector<SDL_Scancode> registered_keys_;
  MovementAction* movement_action_;
};
