#include "Overworld.h"
#include "Screen.h"

#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

Overworld::Overworld()
    : full_map_(nullptr),
      tiles_(nullptr),
      area_rect_({0, 0, 256, 176}),
      area_draw_rect_({0, 64, 256, 176}),
      current_area_x(0),
      current_area_y(0) {}

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

void Overworld::Init(Screen& screen) {
  // Read overworld json data into a variable
  json j;
  std::string map_area_path =
      static_cast<std::string>(SDL_GetBasePath()) + "../Assets/map_area1.json";
  std::ifstream jsonFile(map_area_path);
  jsonFile >> j;
  std::cout << j << std::endl;

  // Create tiles_ texture from spriteSheetSurface
  std::string overworldSpriteSheet =
      static_cast<std::string>(SDL_GetBasePath()) + "../Assets/" +
      j["tilesets"][0]["image"].get<std::string>();
  SDL_Surface* overworldSpritesImage = IMG_Load(overworldSpriteSheet.c_str());
  tiles_ = screen.CreateTextureFromSurface(overworldSpritesImage);
  SDL_FreeSurface(overworldSpritesImage);

  // Using width, height, and data
  std::vector<int> data = j["layers"][0]["data"].get<std::vector<int>>();
  int dataWidth = j["layers"][0]["width"].get<int>();
  int dataHeight = j["layers"][0]["height"].get<int>();

  // Create full_map_ texture using the width and height
  full_map_ = screen.CreateTexture(dataWidth * 16, dataHeight * 16);

  // Loop through data array and render tiles to the full_map_ texture
  int tilesetColumns = j["tilesets"][0]["columns"].get<int>();
  screen.DrawOverworldTiles(tiles_, full_map_, data, tilesetColumns, dataWidth,
                            dataHeight);

  // Load warp points/doors
  // Load tile collision info

  // Construct a vector of area data for discovering collisions, spawn points,
  //  and other points of interest
}

void Overworld::Update(int dt) {}

void Overworld::Draw(Screen& screen) {
  screen.Draw(full_map_, area_rect_, area_draw_rect_);
}
