#include "Color.h"
#include <SDL.h>

const SDL_PixelFormat* Color::kFormat =
    SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);

Color Color::DoAlphaBlend(const Color& source, const Color& dest) {
  
  return Color();
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  color_ = SDL_MapRGBA(kFormat, r, g, b, a);
}

uint8_t Color::red() const { 
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  SDL_GetRGBA(color_, kFormat, &r, &g, &b, &a);
  return r;
}

uint8_t Color::green() const {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  SDL_GetRGBA(color_, kFormat, &r, &g, &b, &a);
  return g;
}

uint8_t Color::blue() const {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  SDL_GetRGBA(color_, kFormat, &r, &g, &b, &a);
  return b;
}

uint8_t Color::alpha() const {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  SDL_GetRGBA(color_, kFormat, &r, &g, &b, &a);
  return a;
}
