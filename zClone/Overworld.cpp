#include "Overworld.h"
#include <SDL.h>
#include "json.hpp"

using json = nlohmann::json;

Overworld::Overworld() : full_map_(nullptr), tiles_(nullptr) {}

Overworld::~Overworld() {
  if(full_map_) {
    SDL_DestroyTexture(full_map_);
    full_map_ = nullptr;
  }

  if (tiles_) {
    SDL_DestroyTexture(tiles_);
    tiles_ = nullptr;
  }
}

void Overworld::Init(Screen& screen, SDL_Surface* spriteSheetSurface) {
  json j;
  j["test"] = 3.5;
  
  // Create tiles_ texture from spriteSheetSurface
  // Read overworld json data into a variable
  // Using width, height, and data
  // Create full_map_ texture using the width and height
  // Loop through data array and render tiles to the full_map_ texture

  // Load warp points/doors
  // Load tile collision info

  // Construct a vector of area data for discovering collisions, spawn points,
  //  and other points of interest
}
