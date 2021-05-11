#include "GameController.h"

GameController::GameController() : movement_action_(nullptr) {}

void GameController::RegisterKeyAction(SDL_Scancode keyCode,
                                       const KeyAction& keyAction) {
  KeyInfo keyInfo = {SDL_RELEASED, keyAction};
  registered_actions_[keyCode] = keyInfo;
  registered_keys_.push_back(keyCode);
}

void GameController::RegisterMovementAction(
    MovementAction& movementAction) {
  movement_action_ = movementAction;
}

void GameController::ProcessMovement(int dt, Uint8 leftState, Uint8 rightState,
                                     Uint8 upState, Uint8 downState) const {
  if (movement_action_ == nullptr) return;
  (movement_action_)(dt, leftState, rightState, upState, downState);
}
