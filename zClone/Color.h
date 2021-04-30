#pragma once

#include <stdint.h>

struct SDL_PixelFormat;

class Color {
 public:
  static const SDL_PixelFormat* kFormat;
  static Color GetAlphaBlend(const Color& source, const Color& destination);

  Color() : Color(0) {}
  Color(uint32_t value) : value_(value) {}
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

  uint8_t red() const;
  uint8_t green() const;
  uint8_t blue() const;
  uint8_t alpha() const;

  inline bool operator==(const Color& c) const { return c.value_ == value_; }
  inline bool operator!=(const Color& c) const { return !(*this == c); }
  inline uint32_t GetPixelColor() const { return value_; }

 private:
   enum class Channel {
     kRed = 0,
     kGreen,
     kBlue,
     kAlpha
  };

   uint32_t value_;

   uint8_t GetChannel(Channel channel) const;
};

