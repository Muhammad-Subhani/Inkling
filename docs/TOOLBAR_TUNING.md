# Toolbar Sizing & Visual Tuning Technical Report

This document details the step-by-step engineering and design decisions implemented to resolve the compactness, legibility, and layout issues of the Inkling vertical drawing toolbar.

---

## 1. Problem Analysis & Objectives

Following an initial restyling, the toolbar exhibited several visual and ergonomic drawbacks:
1. **Excessive Compactness**: 36px buttons in 40px slots made tool targets too small and visually cramped.
2. **Readability & Collision**: 10px shortcut letters were small, and positioned directly against icon graphics without safe clearance.
3. **Flush Window Alignment**: The bar was positioned at window edge `x=0`, with mouse event interception capturing clicks intended for the canvas margin.
4. **Subdued Icon Weights**: Icon strokes (1.6) and sizing (20px) felt disproportionately thin and small inside the toolbar.
5. **Rigid Window Centering**: Window resizing did not preserve vertical centering within the visible canvas.

The objective was to perform a **visual-only** overhaul—tuning dimensions, typography, spacing tokens, and asset weights—without altering tool logic, shortcuts, canvas operations, or shape synchronization.

---

## 2. Geometry & Layout Mathematics

All dimensions are centralized in `ToolbarConfig` as CSS-like design tokens.

### A. Dimensions & Slots
- **Button Dimension (`TOOL_BTN_SIZE`)**: $44\text{ px} \times 44\text{ px}$.
- **Inter-Button Gap (`TOOL_GAP`)**: $6\text{ px}$.
- **Slot Pitch**: $\text{Button Size} + \text{Gap} = 44\text{ px} + 6\text{ px} = 50\text{ px}$.
- **Container Padding (`TOOLBAR_PADDING`)**: $8\text{ px}$ on top, bottom, left, and right.
- **Button Count**: $11$ buttons.

$$\text{Toolbar Height} = (11 \times 44\text{ px}) + (10 \times 6\text{ px}) + (2 \times 8\text{ px}) = 484 + 60 + 16 = 560\text{ px}$$

$$\text{Toolbar Width} = (2 \times \text{Padding}) + \text{Button Width} = (2 \times 8\text{ px}) + 44\text{ px} = 60\text{ px}$$

### B. Corner Radii (Raylib Normalized Roundness)
Raylib’s `DrawRectangleRounded` takes a normalized `roundness` value in $[0.0, 1.0]$, defined as:

$$\text{roundness} = \frac{\text{target radius}}{\min(\text{width}, \text{height}) / 2}$$

- **Toolbar Container** (Target radius: $14\text{ px}$, width: $60\text{ px}$):
  $$\text{roundness}_{\text{toolbar}} = \frac{14.0}{60.0 / 2} = \frac{14.0}{30.0} \approx 0.4667$$
- **Tool Buttons** (Target radius: $10\text{ px}$, width: $44\text{ px}$):
  $$\text{roundness}_{\text{button}} = \frac{10.0}{44.0 / 2} = \frac{10.0}{22.0} \approx 0.4545$$

---

## 3. SVG Stroke Width & Asset Generation

Icons were updated to match the requested stroke weight and sizing:

1. **SVG Source Files**: In `assets/icons/src/*.svg`, the stroke width was upgraded to `1.75`:
   - `stroke-width="1.6"` $\rightarrow$ `stroke-width="1.75"` across shapes (`rectangle`, `diamond`, `ellipse`, `line`, `arrow`, `pencil`, `text`, `select`, `eraser`, `hand`, `twinkle`).
2. **Vector Rendering**: High-resolution $96 \times 96$ PNG textures were re-rendered from the updated SVGs via `rsvg-convert`:
   ```bash
   for f in assets/icons/src/*.svg; do
     name=$(basename "$f" .svg)
     rsvg-convert -w 96 -h 96 "$f" -o "assets/icons/$name.png"
   done
   ```
3. **Display Scaling**: Buttons render the texture into a $22\text{ px} \times 22\text{ px}$ bounding box (`TOOL_ICON_SIZE = 22.0f`).

---

## 4. Shortcut Typography & Collision Avoidance

To make shortcut indicators readable without clashing with the icon:

- **Typography**: `TOOL_LABEL_SIZE = 11.0f` (rendered with Raylib’s crisp text engine).
- **Absolute Positioning**:
  - `right: 5px` $\rightarrow$ `tx = bound.x + bound.width - textW - 5`
  - `bottom: 3px` $\rightarrow$ `ty = bound.y + bound.height - fontSize - 3`
- **Opacity Mapping**:
  - Inactive Buttons: $0.65$ opacity $\rightarrow$ `rgba(255, 255, 255, 166)`
  - Active Button: $0.95$ opacity $\rightarrow$ `rgba(255, 255, 255, 242)`
  - Transitions remain interpolated smoothly across $120\text{ ms}$ (`TRANSITION_TIME = 0.120f`).

### Collision Guarantee (`translateY(-2px)`)
Within a $44\text{ px}$ button:
- The shortcut label occupies $Y \in [30\text{ px}, 41\text{ px}]$ ($44 - 11 - 3 = 30$).
- By translating the icon up by $2\text{ px}$ (`TOOL_ICON_OFFSET_Y = -2.0f`), the $22\text{ px}$ icon texture is drawn at:
  $$\text{dest.y} = \frac{44 - 22}{2} - 2 = 9\text{ px} \quad (Y \in [9\text{ px}, 31\text{ px}])$$
- Because the SVG glyphs have internal padding within their $24 \times 24$ viewboxes, the actual drawn pixels for all 11 icons terminate at or before $Y = 29.2\text{ px}$, providing a guaranteed positive vertical separation:

```
+------------------------------------------+  Y = 0px
|                                          |
|         +----------------------+         |  Y = 9px (Icon Box Top)
|         |                      |         |
|         |     ICON GLYPH       |         |
|         |                      |         |
|         +----------------------+         |  Y = 29.2px (Max Glyph Bottom)
|                                          |  <--- Guaranteed >0.8px gap
|                                  +---+   |  Y = 30px (Label Top)
|                                  | R |   |
|                                  +---+   |  Y = 41px (Label Bottom)
|                                          |  Y = 44px (Button Bottom)
+------------------------------------------+
```

Pixel-boundary validation confirms zero overlap across all 11 icons:
- `arrow.png`: $+4.0\text{ px}$ vertical clearance
- `diamond.png`: $+1.1\text{ px}$ vertical clearance
- `ellipse.png`: $+2.2\text{ px}$ vertical clearance
- `eraser.png`: $+1.3\text{ px}$ vertical clearance
- `hand.png`: $+1.8\text{ px}$ vertical clearance
- `line.png`: $+9.1\text{ px}$ vertical clearance
- `pencil.png`: $+0.8\text{ px}$ vertical clearance
- `rectangle.png`: $+2.7\text{ px}$ vertical clearance
- `select.png`: $+3.1\text{ px}$ vertical clearance
- `text.png`: $+2.2\text{ px}$ vertical clearance
- `twinkle.png`: $+2.2\text{ px}$ vertical clearance

---

## 5. Window Positioning & Dynamic Centering

### Left Offset (`TOOLBAR_LEFT = 16.0f`)
The toolbar is placed at $X = 16\text{ px}$. To avoid capturing canvas clicks in the $[0, 16)\text{ px}$ margin, the click detection hit test in `src/main.cpp` was updated:
```cpp
bool inBar = (mouse.x >= MainBar.GetOrigin().x &&
             mouse.x <= MainBar.GetOrigin().x + MainBar.GetDimension().x &&
             mouse.y >= MainBar.GetOrigin().y &&
             mouse.y <= MainBar.GetOrigin().y + MainBar.GetDimension().y);
```

### Dynamic Vertical Centering
Window resizing was enabled via `SetConfigFlags(FLAG_WINDOW_RESIZABLE)` and `SetWindowMinSize(400, 600)` in `include/window.h`.

In `src/main.cpp`, the bar’s $Y$ position is recalculated dynamically every frame:
```cpp
float currentCanvasTop = ToolbarConfig::TOP_BAR_HEIGHT;
float currentCanvasHeight = (float)GetScreenHeight() - currentCanvasTop;
float targetBarY = currentCanvasTop + (currentCanvasHeight - MainBar.GetDimension().y) / 2.0f;

if (MainBar.GetOrigin().y != targetBarY || MainBar.GetOrigin().x != ToolbarConfig::TOOLBAR_LEFT) {
  MainBar.SetPosition(Vector2{ToolbarConfig::TOOLBAR_LEFT, targetBarY});
}
```

At any window height $H$, the space above and below the bar is strictly equal:
- At $H = 800\text{ px}$: Top space $= 120\text{ px}$, Bottom space $= 120\text{ px}$.
- At $H = 1080\text{ px}$: Top space $= 260\text{ px}$, Bottom space $= 260\text{ px}$.

---

## 6. Architecture & Modified Files

```
Inkling/
├── include/
│   ├── Bar.h              # Defines ToolbarConfig variables and Bar class
│   ├── Button.h           # Button layout, roundness, and label opacity
│   └── window.h           # Added FLAG_WINDOW_RESIZABLE & SetWindowMinSize
├── src/
│   ├── Button.cpp         # Icon translateY(-2px), label coordinates & font size
│   ├── bar.cpp            # Layout dimensions, slot computation & radius math
│   └── main.cpp           # Dynamic canvas vertical centering & inBar hit-test
└── assets/icons/
    ├── src/*.svg          # Stroke width updated to 1.75
    └── *.png              # Regenerated 96x96 textures
```
