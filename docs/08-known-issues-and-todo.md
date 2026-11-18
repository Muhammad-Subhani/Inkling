# Known Issues, Code Smells & Architectural Debt

This document catalogs real, observable issues identified in the Inkling codebase. Every finding is anchored to its exact file, line number, and function, explaining why the issue matters and providing a concrete engineering fix. No hypothetical issues are included; only bugs and architectural debt visible in the current source code.

## Related Files
- Event Loop: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp)
- Canvas: [src/Canvas.cpp](file:///home/subhani/Projects/Inkling/src/Canvas.cpp), [include/Canvas.h](file:///home/subhani/Projects/Inkling/include/Canvas.h)
- Geometry & Shapes: [src/BoundedShape.cpp](file:///home/subhani/Projects/Inkling/src/BoundedShape.cpp), [src/Ellipse.cpp](file:///home/subhani/Projects/Inkling/src/Ellipse.cpp), [src/Diamond.cpp](file:///home/subhani/Projects/Inkling/src/Diamond.cpp), [include/Stroke.h](file:///home/subhani/Projects/Inkling/include/Stroke.h), [include/RectangleShape.h](file:///home/subhani/Projects/Inkling/include/RectangleShape.h)
- UI & Window: [include/Button.h](file:///home/subhani/Projects/Inkling/include/Button.h), [src/bar.cpp](file:///home/subhani/Projects/Inkling/src/bar.cpp), [include/window.h](file:///home/subhani/Projects/Inkling/include/window.h)

---

## 1. Catalog of Observable Bugs & Flaws

### Issue 1: Reverse Traversal Bug in `Canvas::PickAt`
- **Location**: [src/Canvas.cpp](file:///home/subhani/Projects/Inkling/src/Canvas.cpp#L7-L13) (`Canvas::PickAt`)
- **Code**:
  ```cpp
  Shape *Canvas::PickAt(Vector2 p) const {
    for (auto &x : shapes) {
      if (x->Contains(p))
        return x.get();
    }
    return nullptr;
  }
  ```
- **Why it Matters**: In painter's algorithm rendering ([src/Canvas.cpp](file:///home/subhani/Projects/Inkling/src/Canvas.cpp#L3-L6)), shapes at lower indices are drawn first (bottommost), and shapes at higher indices are drawn last (topmost). A forward `for` loop returns the **bottommost** shape that contains `p`. When shapes overlap, the user clicks on the visible topmost shape, but the application selects the hidden bottom shape underneath.
- **Suggested Fix**: Iterate in reverse order from topmost to bottommost:
  ```cpp
  Shape *Canvas::PickAt(Vector2 p) const {
    for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
      if ((*it)->Contains(p))
        return it->get();
    }
    return nullptr;
  }
  ```

---

### Issue 2: Keyboard Shortcut Collision & Missing Bindings
- **Location**: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L85-L86)
- **Code**:
  ```cpp
  if (IsKeyPressed(KEY_E))
    MainBar.ChangePressed(TOOL_ELLIPSE);
  ```
- **Why it Matters**:
  1. The toolbar tooltip and shortcut label for Ellipse is `"O"` (lines 25, 29). The shortcut label for Eraser is `"E"`.
  2. Pressing `KEY_E` erroneously activates `TOOL_ELLIPSE` instead of `TOOL_ERASER`.
  3. `KEY_O` is never polled, making it impossible to select Ellipse using the advertised `"O"` shortcut.
  4. `KEY_K` (Magic wand) is also completely omitted from keyboard polling.
- **Suggested Fix**: Update `src/main.cpp`:
  ```cpp
  if (IsKeyPressed(KEY_O))
    MainBar.ChangePressed(TOOL_ELLIPSE);
  if (IsKeyPressed(KEY_E))
    MainBar.ChangePressed(TOOL_ERASER);
  if (IsKeyPressed(KEY_K))
    MainBar.ChangePressed(TOOL_TWINKLE);
  ```

---

### Issue 3: Double `CloseWindow()` Call and OpenGL Texture Teardown Order
- **Location**: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L14-L18, #L147), [include/window.h](file:///home/subhani/Projects/Inkling/include/window.h#L10), [src/Icons.cpp](file:///home/subhani/Projects/Inkling/src/Icons.cpp#L11-L15)
- **Code**:
  ```cpp
  // main.cpp:
  int main() {
    Window window(800, 800, "Inkling"); // Window destructor calls CloseWindow()
    ...
    CloseWindow();                      // Explicit call!
  }
  ```
- **Why it Matters**:
  1. **Double Close**: `main()` calls `CloseWindow()` explicitly at line 147, and then the RAII `window` instance goes out of scope and calls `CloseWindow()` a second time in `~Window()`.
  2. **Context Teardown Order**: In `main()`, `Window window` is declared *before* `Icons icons`. In C++, local objects are destroyed in reverse declaration order. When `main()` returns, `CloseWindow()` has already destroyed the OpenGL context. Subsequently, `~Icons()` runs `UnloadTexture()` on GPU handles in a destroyed context, leading to driver errors or exit segmentation faults.
- **Suggested Fix**: Remove the explicit `CloseWindow()` call at line 147, and enclose the application state inside a block or struct so textures unload while the window is still open:
  ```cpp
  int main() {
    Window window(800, 800, "Inkling");
    SetTargetFPS(60);
    {
      Icons icons({...});
      // run loop
    } // icons unload cleanly here
    // Window destructor runs CloseWindow() here
  }
  ```

---

### Issue 4: Typographical Class Name: `RecatngleShape`
- **Location**: [include/RectangleShape.h](file:///home/subhani/Projects/Inkling/include/RectangleShape.h#L4), [src/RectangleShape.cpp](file:///home/subhani/Projects/Inkling/src/RectangleShape.cpp#L3, #L6), [src/ShapeFactory.cpp](file:///home/subhani/Projects/Inkling/src/ShapeFactory.cpp#L8)
- **Code**:
  ```cpp
  class RecatngleShape : public BoundedShape { ... };
  ```
- **Why it Matters**: The file is named `RectangleShape.h`, but the class identifier is misspelled `RecatngleShape` (missing 'n' before 'g'). Any new developer including the header will expect `RectangleShape` and encounter compiler errors.
- **Suggested Fix**: Refactor `RecatngleShape` to `RectangleShape` across `include/RectangleShape.h`, `src/RectangleShape.cpp`, and `src/ShapeFactory.cpp`.

---

### Issue 5: Member Variable Shadowing in `Stroke`
- **Location**: [include/Stroke.h](file:///home/subhani/Projects/Inkling/include/Stroke.h#L13-L14)
- **Code**:
  ```cpp
  class Stroke : public Shape {
  private:
    std::vector<Vector2> points;
    Color color = WHITE;
    float thickness = 2;
  };
  ```
- **Why it Matters**: Base class `Shape` already declares `Color color` and `float thickness` under `protected:` ([include/Shape.h](file:///home/subhani/Projects/Inkling/include/Shape.h#L18-L19)). Re-declaring them in `Stroke` creates member shadowing. Any polymorphic routine in `Shape` querying `color` would read `Shape::color`, not `Stroke::color`. It also produces compiler warnings under `-Wshadow`.
- **Suggested Fix**: Delete lines 13 and 14 in `include/Stroke.h` and use the inherited protected members from `Shape`.

---

### Issue 6: Division by Zero in `Ellipse::Contains` & `Diamond::Contains`
- **Location**: [src/Ellipse.cpp](file:///home/subhani/Projects/Inkling/src/Ellipse.cpp#L11-L14), [src/Diamond.cpp](file:///home/subhani/Projects/Inkling/src/Diamond.cpp#L19-L22)
- **Code**:
  ```cpp
  float radiiX = bound.width / 2;
  float radiiY = bound.height / 2;
  float dx = (p.x - (bound.x + radiiX)) / radiiX; // Div by zero if width == 0
  ```
- **Why it Matters**: If a user single-clicks without dragging, `bound.width` or `bound.height` is `0.0f`. Dividing by zero produces `inf` or `nan`.
- **Suggested Fix**: Add an early exit guard in both functions:
  ```cpp
  if (bound.width <= 0.0f || bound.height <= 0.0f) return false;
  ```

---

### Issue 7: High-Frequency Heap Allocation & ID Counter Churn During Drag Preview
- **Location**: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L142-L143), [src/Shape.cpp](file:///home/subhani/Projects/Inkling/src/Shape.cpp#L3)
- **Code**:
  ```cpp
  if (dragging)
    MakeShape(current, origin, mouse)->Draw(); // live preview
  ```
- **Why it Matters**:
  1. `MakeShape` allocates a heap object via `std::make_unique` 60 times per second during a mouse drag.
  2. Each construction invokes `Shape::Shape()` which increments `nextId++`. A single 2-second drag burns through 120 unique shape IDs before committing only 1 shape to canvas.
- **Suggested Fix**: Render preview outlines directly using raw raylib calls (`DrawRectangleLinesEx`, `DrawEllipseLines`) or maintain a single persistent preview instance updated in-place without heap re-allocation.

---

### Issue 8: Premature Hit Testing on Toolbar Clicks
- **Location**: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L97-L113)
- **Code**:
  ```cpp
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Shape *Clicked = canvas.PickAt(GetMousePosition());
    if (Clicked)
      TraceLog(LOG_INFO, "HIT");
    ...
    if (inBar) {
      MainBar.SelectAbutton(mouse);
    }
  ```
- **Why it Matters**: `canvas.PickAt()` runs before `inBar` is evaluated. Clicking a toolbar button over a region where a canvas shape exists underneath logs a false `"HIT"`.
- **Suggested Fix**: Evaluate `inBar` first; only invoke `canvas.PickAt()` if `!inBar`.

---

### Issue 9: Method Spelling Typo: `Button::getWitdh()`
- **Location**: [include/Button.h](file:///home/subhani/Projects/Inkling/include/Button.h#L48), [src/Button.cpp](file:///home/subhani/Projects/Inkling/src/Button.cpp#L102), [src/bar.cpp](file:///home/subhani/Projects/Inkling/src/bar.cpp#L58, #L113)
- **Code**:
  ```cpp
  float getWitdh() const;
  ```
- **Why it Matters**: Misspelled public getter violates conventions and breaks readability.
- **Suggested Fix**: Rename to `getWidth()` across header, implementation, and callers.

---

### Issue 10: Incomplete / Inert Tools
- **Location**: [src/Tool.cpp](file:///home/subhani/Projects/Inkling/src/Tool.cpp#L15), [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L116-L120)
- **Why it Matters**: Seven buttons on the toolbar (`Hand`, `Select`, `Arrow`, `Line`, `Text`, `Twinkle`, `Eraser`) produce no canvas shapes or actions when clicked or dragged.
- **Suggested Fix**: Implement concrete classes (`LineShape`, `ArrowShape`, `TextShape`) or visually disable/dim unsupported buttons until implemented.

---

### Issue 11: Access Specifier Mismatch in `BoundedShape.h`
- **Location**: [include/BoundedShape.h](file:///home/subhani/Projects/Inkling/include/BoundedShape.h#L11)
- **Code**:
  ```cpp
  protected:
    Rectangle bound;
    bool Contains(Vector2 p) const override;
  ```
- **Why it Matters**: In `include/Shape.h`, `Contains` is declared `public:`. In `BoundedShape.h`, it is placed under `protected:`. If an external caller holds a `BoundedShape*` or reference rather than a `Shape*`, calling `Contains()` fails compile-time access checks.
- **Suggested Fix**: Move `bool Contains(Vector2 p) const override;` into the `public:` section of `BoundedShape.h`.

---

### Issue 12: God-Function Anti-Pattern in `src/main.cpp`
- **Location**: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L14-L148)
- **Why it Matters**: A single 148-line function manages window creation, asset paths, button initialization, dynamic layout calculations, event dispatch, state machine branches, drawing passes, and teardown. This violates the Single Responsibility Principle and hinders automated testing.
- **Suggested Fix**: Encapsulate the application state into an `App` class with separate methods: `Initialize()`, `HandleInput()`, `Update()`, `Render()`, and `Shutdown()`.

---

## Assumptions / Unclear
- No assumptions were made; all issues listed above were verified directly against line numbers and logic in the repository.
