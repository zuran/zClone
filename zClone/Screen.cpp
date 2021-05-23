// 
// Screen uses a texture, buffer_, as a back buffer, which is the target of all
// draw calls. During Present(), which is called during the game loop, the back
// buffer is copied to the (scaled) window surface. Clear() is only called on
// the back buffer.
//
// Screen also does drawing to other textures such as the overworld map.
// Perhaps I should move this to its own class where the renderer is retrieved.
//

#include "Screen.h"

Screen::Screen() : window_(nullptr), renderer_(nullptr), buffer_(nullptr) {}

Screen::~Screen() {
  if(buffer_) {
    SDL_DestroyTexture(buffer_);
    buffer_ = nullptr;
  }

  if(renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }

  if(window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
  SDL_Quit();
}

SDL_Window* Screen::Init(int width, int height, int magnification) {
  bool didInitVideo = SDL_Init(SDL_INIT_EVERYTHING) >= 0;
  if (!didInitVideo) return nullptr;  
  
  window_ = SDL_CreateWindow("Gamora", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, width * magnification,
                             height * magnification, SDL_WINDOW_SHOWN);
  if (!window_) return nullptr;
  
  Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                       SDL_RENDERER_TARGETTEXTURE;
  renderer_ = SDL_CreateRenderer(window_, -1, renderFlags);
  if (!renderer_) return nullptr;
  
  buffer_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, width, height);
  if (!buffer_) return nullptr;

  SDL_SetRenderTarget(renderer_, buffer_);
  return window_;
}

void Screen::Present() { 
  SDL_SetRenderTarget(renderer_, nullptr);
  SDL_RenderCopy(renderer_, buffer_, nullptr, nullptr);
  SDL_RenderPresent(renderer_);
  SDL_SetRenderTarget(renderer_, buffer_);
}

void Screen::Clear() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_); 
}

// spriteSheet: texture atlas
// spriteRect: bounds for the sprite to draw from the texture atlas
// targetRect: bounds for the draw target location on the back buffer
void Screen::Draw(SDL_Texture* spriteSheet, const SDL_Rect& spriteRect,
                  SDL_Rect& targetRect) {
  SDL_RenderCopy(renderer_, spriteSheet, &spriteRect, &targetRect);
}

void Screen::Draw(SDL_Texture* spriteSheet, const SDL_Rect& spriteRect,
                  SDL_Rect& targetRect, float rotation, SDL_Point& center,
                  SDL_RendererFlip flip) {
  SDL_RenderCopyEx(renderer_, spriteSheet, &spriteRect, &targetRect, rotation,
                   &center, flip);
}

// Draw overworld tiles to the full map texture
void Screen::DrawOverworldTiles(SDL_Texture* tileset, SDL_Texture* map,
                                std::vector<int> data, int tilesetColumns,
                                int dataWidth, int dataHeight) {
  SDL_SetRenderTarget(renderer_, map);
  SDL_Rect tileRect = {0, 0, 16, 16};
  SDL_Rect drawRect = {0, 0, 16, 16};
  int dataIndex = 0;
  for (int row = 0; row < dataHeight; ++row) {
    drawRect.x = 0;
    drawRect.y = row * 16;
    for(int col = 0; col < dataWidth; ++col, ++dataIndex) {
      if (data[dataIndex] == 0) continue;
      drawRect.x = col * 16;
      tileRect.x = (data[dataIndex] - 1) % tilesetColumns * 16;
      tileRect.y = (data[dataIndex] - 1) / tilesetColumns * 16;
      Draw(tileset, tileRect, drawRect);
    }
  }
  SDL_SetRenderTarget(renderer_, buffer_);
}

SDL_Texture* Screen::CreateTextureFromSurface(SDL_Surface* surface) {
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
  return texture;
}

SDL_Texture* Screen::CreateTexture(int width, int height) { 
  SDL_Texture* texture = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, width, height);
  return texture;
}

Screen& Screen::operator=(const Screen& screen) { return *this; }