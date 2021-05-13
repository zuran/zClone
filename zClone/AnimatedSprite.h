#pragma once

#include <vector>
#include <map>
#include <string>

struct Frame {
};

class AnimatedSprite {
 public:
 private:
  std::map<std::string, std::vector<Frame>> animations_;
};
