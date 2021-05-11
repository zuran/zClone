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

void Screen::Draw(SDL_Texture* spriteSheet, const SDL_Rect& spriteRect,
                  SDL_Rect& targetRect) {
  SDL_RenderCopy(renderer_, spriteSheet, &spriteRect, &targetRect);
}

void Screen::Draw(SDL_Texture* spriteSheet, const SDL_Rect& spriteRect,
                  SDL_Rect& targetRect, float rotation, SDL_Point& center,
                  SDL_RendererFlip flip) {}

SDL_Texture* Screen::CreateTextureFromSurface(SDL_Surface* surface) {
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
  if (texture) return texture;
  return texture;
}

Screen& Screen::operator=(const Screen& screen) { return *this; }