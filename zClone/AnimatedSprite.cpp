#include "AnimatedSprite.h"
#include "Screen.h"

AnimatedSprite::AnimatedSprite(int width, int height) {
  is_playing_ = false;
  current_frame_ = 0;
  current_frame_time_ = 0;
  frame_rect_ = {0, 0, width, height};
  frame_center_ = {width / 2, height / 2};
}

void AnimatedSprite::Update(int dt) {
  if (!is_playing_) return;

  current_frame_time_ += dt;
  if (current_frame_time_ >
      animations_[current_animation_name_][current_frame_].frame_time) {
    current_frame_time_ = 0;
    current_frame_ =
        (current_frame_ + 1) % animations_[current_animation_name_].size();
    frame_rect_.x = animations_[current_animation_name_][current_frame_].x_offset;
    frame_rect_.y = animations_[current_animation_name_][current_frame_].y_offset;
  }
}

void AnimatedSprite::Draw(Screen& screen, SDL_Texture* sprite_sheet,
                          SDL_Rect pos_rect) {
  screen.Draw(sprite_sheet, frame_rect_, pos_rect, 0.0f, frame_center_,
              animations_[current_animation_name_][current_frame_].flip);
}

void AnimatedSprite::Stop() { is_playing_ = false; }
// maybe implement pause, so if animation is paused, resume, otherwise restart
void AnimatedSprite::Play() { is_playing_ = true; }

void AnimatedSprite::SetCurrentAnimation(std::string animationName) {
  if (current_animation_name_ == animationName) return;
  current_animation_name_ = animationName;
  current_frame_ = 0;
  current_frame_time_ = 0;
  frame_rect_.x = animations_[current_animation_name_][current_frame_].x_offset;
  frame_rect_.y = animations_[current_animation_name_][current_frame_].y_offset;
}

void AnimatedSprite::AddAnimation(std::string animationName,
                                  std::vector<Frame>& frames) {
  animations_[animationName] = frames;
}
