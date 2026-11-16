#include "../include/Icons.h"
#include <vector>
Icons::Icons(const std::vector<const char *> &paths) {
  for (const char *p : paths) {
    Texture2D t = LoadTexture(p);
    if (t.id != 0)
      SetTextureFilter(t, TEXTURE_FILTER_BILINEAR);
    textures.push_back(t);
  }
}
Icons::~Icons() {
  for (Texture2D &t : textures)
    if (t.id != 0)
      UnloadTexture(t);
}
