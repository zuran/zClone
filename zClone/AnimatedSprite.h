#pragma once

#include <SDL.h>
#include <vector>
#include <map>
#include <string>

class Screen;

struct Frame {
  int frame_time;
  int x_offset;
  int y_offset;
  SDL_RendererFlip flip;
};

class AnimatedSprite {
 public:
  AnimatedSprite(int width, int height);
  void Update(int dt);
  void Draw(Screen& screen, SDL_Texture* sprite_sheet, SDL_Rect pos_rect);
  void Stop();
  void Play();

  inline std::vector<Frame> GetAnimation(std::string animationName) {
    return animations_[animationName];
  }
  void SetCurrentAnimation(std::string animationName);
  void AddAnimation(std::string animationName, std::vector<Frame>& frames);

 private:
  std::map<std::string, std::vector<Frame>> animations_;
  int current_frame_time_;
  std::string current_animation_name_;
  int current_frame_;
  SDL_Rect frame_rect_;
  SDL_Point frame_center_;
  bool is_playing_;
};
