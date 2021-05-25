// Initially the overworld is drawn with a single area. I need to create
// additional tiles before I can complete it, though placeholders should work
// for testing screen transitions and area loading. To start with, the full
// overworld map is drawn to the full_map_ texture after tiles are loaded. Then
// the starting area is loaded (need a LoadArea(x, y) function). In addition
// to overworld area transitions, caves and dungeon entrances need transitions.
// My plan is to add these to the json using special objects. The object will
// need properties to store the name of the area to load (or x and y on another
// plane), as well as the x and y of Gamora's position when the cave or dungeon
// is loaded. I should also pass a "return" location on the overworld map, so
// we know what area and position to place Gamora upon leaving the cave.

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
  overworld_data_ = j["layers"][0]["data"].get<std::vector<int>>();
  int dataWidth = j["layers"][0]["width"].get<int>();
  int dataHeight = j["layers"][0]["height"].get<int>();

  // Create full_map_ texture using the width and height
  full_map_ = screen.CreateTexture(dataWidth * 16, dataHeight * 16);

  // Loop through data array and render tiles to the full_map_ texture
  int tilesetColumns = j["tilesets"][0]["columns"].get<int>();
  screen.DrawOverworldTiles(tiles_, full_map_, overworld_data_, tilesetColumns, dataWidth,
                            dataHeight);

  // Load warp points/doors
  // Load tile collision info
  int numTiles = j["tilesets"][0]["tiles"].size();
  for(int i = 0; i < numTiles; ++i) {
    int numCollisions = j["tilesets"][0]["tiles"][i]["objectgroup"]["objects"].size();
    int tileId = j["tilesets"][0]["tiles"][i]["id"].get<int>();
    std::vector<SDL_Rect> collisionRects;
    for(int o = 0; o < numCollisions; ++o) {
      SDL_Rect collisionRect;
      collisionRect.x = j["tilesets"][0]["tiles"][i]["objectgroup"]["objects"][o]["x"].get<int>();
      collisionRect.y = j["tilesets"][0]["tiles"][i]["objectgroup"]["objects"][o]["y"].get<int>();
      collisionRect.w =
          j["tilesets"][0]["tiles"][i]["objectgroup"]["objects"][o]["width"]
              .get<int>();
      collisionRect.h =
          j["tilesets"][0]["tiles"][i]["objectgroup"]["objects"][o]["height"]
              .get<int>();
      collisionRects.push_back(collisionRect);
    }
    tile_collisions_[tileId] = collisionRects;
  }

  // Construct a vector of area data for discovering collisions, spawn points,
  //  and other points of interest
  current_area_y = 1;
}

void Overworld::Update(int dt) {}

void Overworld::Draw(Screen& screen) {
  screen.Draw(full_map_, area_rect_, area_draw_rect_);
}

void Overworld::SetSafeLocationIfColliding(SDL_Rect& gamoraPosRect, MovementDirection direction) {
  if (direction == MovementDirection::kNone) return;
  // Get overlapping tiles
  int x = gamoraPosRect.x;
  int y = gamoraPosRect.y + 8 - 64;

  int tileCols = 16;

  std::vector<int> overlappingTileIds;
  std::vector<SDL_Point> overlappingTileLocs;

  // tile map is 16x11
  int tileX = x / 16;
  int tileXOffest = x % 16;
  int tileY = y / 16;
  int tileYOffset = y % 16;

  // Always overlapping at least one tile
  overlappingTileLocs.push_back({tileX, tileY});
  overlappingTileIds.push_back(tileCols * tileY + tileX);
  if(tileXOffest > 0) {
    overlappingTileLocs.push_back({tileX + 1, tileY});
    overlappingTileIds.push_back(tileCols * tileY + tileX + 1);
  }
  if(tileYOffset > 8) {
    overlappingTileLocs.push_back({tileX, tileY+1});
    overlappingTileIds.push_back(tileCols * (tileY+1) + tileX);
  }
  if(tileXOffest > 0 && tileYOffset > 8) {
    overlappingTileLocs.push_back({tileX + 1, tileY + 1});
    overlappingTileIds.push_back(tileCols * (tileY + 1) + tileX + 1); 
  }

  // Check terrain collisions
  for(size_t i = 0; i < overlappingTileIds.size(); ++i) {
    int tileId = area_data_[overlappingTileIds[i]] - 1;
    auto it = tile_collisions_.find(tileId);
    if (it == tile_collisions_.end()) continue;

    std::vector<SDL_Rect> collisionRects = it->second;
    SDL_Point tileLoc = overlappingTileLocs[i];
    for(size_t j = 0; j < collisionRects.size(); j++) {
      SDL_Rect collisionRect = collisionRects[j];
      collisionRect.x += tileLoc.x * 16;
      collisionRect.y += tileLoc.y * 16;

      switch (direction) {
        case MovementDirection::kLeft: {
          bool isLeftInside =
              x < (collisionRect.x + collisionRect.w) && x > collisionRect.x;
          bool isTopInside =
              y < (collisionRect.y + collisionRect.h) && y >= collisionRect.y;
          bool isBottomInside =
              (y + 8) <= (collisionRect.y + collisionRect.h) &&
              (y + 8) > collisionRect.y;
          if (isLeftInside && (isTopInside || isBottomInside)) {
            gamoraPosRect.x = collisionRect.x + collisionRect.w;
            return;
          }
          break;
        }
        case MovementDirection::kRight: {
          bool isRightInside = (x + 16) > collisionRect.x &&
                               (x + 16) < (collisionRect.x + collisionRect.w);
          bool isTopInside =
              y < (collisionRect.y + collisionRect.h) && y >= collisionRect.y;
          bool isBottomInside =
              (y + 8) <= (collisionRect.y + collisionRect.h) &&
              (y + 8) > collisionRect.y;
          if (isRightInside && (isTopInside || isBottomInside)) {
            gamoraPosRect.x = collisionRect.x - 16;
            return;
          }
          break;
        }
        case MovementDirection::kUp: {
          bool isTopInside =
              y < (collisionRect.y + collisionRect.h) && y > collisionRect.y;
          bool isLeftInside =
              x < (collisionRect.x + collisionRect.w) && x >= collisionRect.x;
          bool isRightInside =
              (x + 16) <= (collisionRect.x + collisionRect.w) &&
              (x + 16) > collisionRect.x;
          if (isTopInside && (isLeftInside || isRightInside)) {
            gamoraPosRect.y = collisionRect.y + collisionRect.h + 64 - 8;
            return;
          }
          break;
        }
        case MovementDirection::kDown: {
          bool isBottomInside = (y + 8) > collisionRect.y &&
                               (y + 8) < (collisionRect.y + collisionRect.h);
          bool isLeftInside =
              x < (collisionRect.x + collisionRect.w) && x >= collisionRect.x;
          bool isRightInside =
              (x + 16) <= (collisionRect.x + collisionRect.w) &&
              (x + 16) > collisionRect.x;
          if (isBottomInside && (isLeftInside || isRightInside)) {
            gamoraPosRect.y = collisionRect.y - 8 + 64 - 8;
            return;
          }
          break;
        }
      }
    }
  }
}
