#pragma once

#include <stdint.h>

struct SDL_PixelFormat;

class Color {
 public:
  uint32_t color_;
  static const SDL_PixelFormat* kFormat;

  static Color DoAlphaBlend(const Color& source, const Color& dest);

  Color() : Color(0) {}
  Color(uint32_t color) : color_(color) {}
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

  uint8_t red() const;
  uint8_t green() const;
  uint8_t blue() const;
  uint8_t alpha() const;


 private:
};

