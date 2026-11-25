#include "../include/Logo.h"
#include "../include/Bar.h"
#include <algorithm>
#include <raylib.h>

namespace {
constexpr const char *LOGO_IMAGE_PATH = "assets/logo/inkling.png";
constexpr const char *LOGO_FONT_DIR = "assets/fonts";
constexpr const char *LOGO_TEXT = "Inkling";

constexpr float LOGO_IMAGE_HEIGHT = 26.0f;
constexpr float LOGO_FONT_SIZE = 28.0f;
constexpr float LOGO_TEXT_SPACING = 1.0f;
constexpr float LOGO_PADDING_X = 14.0f;
constexpr float LOGO_PADDING_Y = 8.0f;
constexpr float LOGO_GAP = 8.0f;
constexpr float LOGO_ROUNDNESS = 1.0f;
constexpr int LOGO_ROUND_SEGMENTS = 16;

constexpr int ASCII_START = 32;
constexpr int ASCII_END = 126;
constexpr int ASCII_COUNT = ASCII_END - ASCII_START + 1;
} // namespace

Logo::Logo(Texture2D fallbackIcon) {
  if (FileExists(LOGO_IMAGE_PATH)) {
    Texture2D loaded = LoadTexture(LOGO_IMAGE_PATH);
    if (loaded.id != 0) {
      SetTextureFilter(loaded, TEXTURE_FILTER_BILINEAR);
      texture = loaded;
      ownsTexture = true;
    }
  }
  if (!ownsTexture) {
    texture = fallbackIcon;
  }

  if (DirectoryExists(LOGO_FONT_DIR)) {
    FilePathList files = LoadDirectoryFilesEx(LOGO_FONT_DIR, ".ttf", false);
    if (files.count > 0) {
      int codepoints[ASCII_COUNT];
      for (int i = 0; i < ASCII_COUNT; i++) {
        codepoints[i] = ASCII_START + i;
      }
      Font loadedFont = LoadFontEx(files.paths[0], (int)LOGO_FONT_SIZE,
                                   codepoints, ASCII_COUNT);
      if (loadedFont.texture.id != 0 && loadedFont.glyphCount > 0) {
        SetTextureFilter(loadedFont.texture, TEXTURE_FILTER_BILINEAR);
        font = loadedFont;
        ownsFont = true;
      }
    }
    UnloadDirectoryFiles(files);
  }
  if (!ownsFont) {
    font = GetFontDefault();
  }

  if (texture.id != 0 && texture.height > 0) {
    imageWidth =
        LOGO_IMAGE_HEIGHT * ((float)texture.width / (float)texture.height);
  } else {
    imageWidth = LOGO_IMAGE_HEIGHT;
  }

  Vector2 textSize =
      MeasureTextEx(font, LOGO_TEXT, LOGO_FONT_SIZE, LOGO_TEXT_SPACING);
  float contentHeight = std::max(LOGO_IMAGE_HEIGHT, textSize.y);

  bounds = {ToolbarConfig::TOOLBAR_LEFT, ToolbarConfig::TOOLBAR_LEFT,
            LOGO_PADDING_X * 2.0f + imageWidth + LOGO_GAP + textSize.x,
            LOGO_PADDING_Y * 2.0f + contentHeight};
}

Logo::~Logo() { Unload(); }

void Logo::Unload() {
  if (ownsTexture && texture.id != 0) {
    UnloadTexture(texture);
    texture = {};
    ownsTexture = false;
  }
  if (ownsFont && font.texture.id != 0) {
    UnloadFont(font);
    font = {};
    ownsFont = false;
  }
}

void Logo::Draw() const {
  DrawRectangleRounded(bounds, LOGO_ROUNDNESS, LOGO_ROUND_SEGMENTS,
                       ToolbarConfig::PANEL_BG);

  if (texture.id != 0) {
    Rectangle src = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    Rectangle dst = {bounds.x + LOGO_PADDING_X,
                     bounds.y + (bounds.height - LOGO_IMAGE_HEIGHT) / 2.0f,
                     imageWidth, LOGO_IMAGE_HEIGHT};
    DrawTexturePro(texture, src, dst, {0.0f, 0.0f}, 0.0f, WHITE);
  }

  Vector2 textSize =
      MeasureTextEx(font, LOGO_TEXT, LOGO_FONT_SIZE, LOGO_TEXT_SPACING);
  Vector2 textPos = {bounds.x + LOGO_PADDING_X + imageWidth + LOGO_GAP,
                     bounds.y + (bounds.height - textSize.y) / 2.0f};
  DrawTextEx(font, LOGO_TEXT, textPos, LOGO_FONT_SIZE, LOGO_TEXT_SPACING,
             ToolbarConfig::PANEL_TEXT);
}

bool Logo::Contains(Vector2 point) const {
  return CheckCollisionPointRec(point, bounds);
}
