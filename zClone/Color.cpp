#include "Color.h"
#include <SDL.h>

const SDL_PixelFormat* Color::kFormat =
    SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);

Color Color::GetAlphaBlend(const Color& source, const Color& destination) {
  float sourceAlpha = static_cast<float>(source.alpha()) / 255.0f;
  float destinationAlpha = 1.0f - sourceAlpha;

  uint8_t r = static_cast<uint8_t>(source.red() * sourceAlpha +
                                   destination.red() * destinationAlpha);
  uint8_t g = static_cast<uint8_t>(source.green() * sourceAlpha +
                                   destination.green() * destinationAlpha);
  uint8_t b = static_cast<uint8_t>(source.blue() * sourceAlpha +
                                   destination.blue() * destinationAlpha);
  uint8_t a = 255;
  return Color(r, g, b, a);
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  value_ = SDL_MapRGBA(kFormat, r, g, b, a);
}

uint8_t Color::red() const { return GetChannel(Channel::kRed); }
uint8_t Color::green() const { return GetChannel(Channel::kGreen); }
uint8_t Color::blue() const { return GetChannel(Channel::kBlue); }
uint8_t Color::alpha() const { return GetChannel(Channel::kAlpha); }

uint8_t Color::GetChannel(Channel channel) const { 
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  SDL_GetRGBA(value_, kFormat, &r, &g, &b, &a);
  switch (channel) { 
    case Channel::kRed:
      return r;
    case Channel::kGreen:
      return g;
    case Channel::kBlue:
      return b;
    case Channel::kAlpha:
      return a;
    default:
      return 0;
  }
}
