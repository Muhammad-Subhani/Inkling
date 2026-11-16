#pragma once
#include <cstddef>
#include <raylib.h>
#include <vector>
class Icons {
  std::vector<Texture2D> textures;

public:
  Icons(const std::vector<const char *> &paths);
  ~Icons();
  // the copy and the assignment are delete to ensure that
  // no two objects hold the same resource and both can free them
  // causing the double deletion
  Icons(const Icons &I) = delete;
  Icons &operator=(const Icons &I) = delete;
  Texture2D Get(size_t i) { return textures[i]; }
  size_t Count() const { return textures.size(); }
};
