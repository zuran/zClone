#include "InputManager.h"
#include "GameController.h"
#include <SDL.h>

InputManager::InputManager() { 
  GameController defaultController = GameController();
  current_controller_ = &defaultController; 
}

void InputManager::Update(int dt) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if(event.type == SDL_QUIT) {
      // Process quit event
    }
  }

  // Process registered keys then movement
  const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
  for(const SDL_Scancode key : current_controller_->registered_keys()) {
    const KeyInfo keyInfo = current_controller_->GetKeyInfo(key);
    keyInfo.action(dt, keyboardState[key], keyInfo.state);
  }
  current_controller_->ProcessMovement(
      dt, keyboardState[SDL_SCANCODE_LEFT], keyboardState[SDL_SCANCODE_RIGHT],
      keyboardState[SDL_SCANCODE_UP], keyboardState[SDL_SCANCODE_DOWN]);
}
