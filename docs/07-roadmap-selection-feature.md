# Selection Feature & Transformation Roadmap

This document outlines the architectural roadmap for implementing an Excalidraw-style interactive selection, manipulation, and transformation system in Inkling. While basic spatial hit testing is currently operational, shapes on the canvas cannot yet be selected, moved, resized, or rotated. Below is an engineering plan structured across eight sequential milestones, detailing the design intent, required OOP interfaces, and specific classes affected by each step.

## Related Files
- Hit Testing & Geometry: [include/Shape.h](file:///home/subhani/Projects/Inkling/include/Shape.h), [include/BoundedShape.h](file:///home/subhani/Projects/Inkling/include/BoundedShape.h), [include/Stroke.h](file:///home/subhani/Projects/Inkling/include/Stroke.h)
- Canvas Management: [include/Canvas.h](file:///home/subhani/Projects/Inkling/include/Canvas.h), [src/Canvas.cpp](file:///home/subhani/Projects/Inkling/src/Canvas.cpp)
- Interaction Loop: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp)
- Hit Testing Reference: [docs/05-hit-testing.md](05-hit-testing.md)

---

## 1. Roadmap Overview & Status Matrix

The selection feature roadmap is divided into eight incremental stages:

| Milestone | Feature Description | Status in Codebase | Target OOP Principles / Interfaces |
| :---: | :--- | :---: | :--- |
| **1** | **Hit Testing (`Contains`)** | **DONE** | Pure virtual `Shape::Contains`; overrides in `BoundedShape`, `Ellipse`, `Diamond`, `Stroke`. |
| **2** | **Single Click Selection & Border** | **TODO** | `Canvas` tracks active selection (`Shape*`); renders visual bounding box around target. |
| **3** | **Move by Dragging (`Translate`)** | **TODO** | Pure virtual `Shape::Translate(Vector2 delta)`; polymorphic translation across all shapes. |
| **4** | **Marquee (Drag-Box) Selection** | **TODO** | Transient marquee rectangle; `Shape::IsEnclosedBy(Rectangle)` enclosure queries. |
| **5** | **Group Move & Union Bounding Box** | **TODO** | Multi-selection container; composite bounding box calculation; synchronized translation. |
| **6** | **Alt + Drag Duplication (`Clone`)** | **TODO** | Prototype Pattern via virtual `Shape::Clone() const = 0`; duplicate on drag initiation. |
| **7** | **Resize via Corner Handles** | **TODO** | 4-point bounding box handles; directional stretching and dimension recalculation. |
| **8** | **Rotation Handle (Single Shape)** | **TODO** | Angular rotation handle; matrix rotation in `Draw()`; inverse coordinate mapping for `Contains()`. |

---

## 2. Milestone Breakdown & Technical Specifications

### Milestone 1: Hit Testing (`Contains`)
- **Status**: **DONE** *(requires traversal order fix in `PickAt`)*
- **Design Intent**: Establish mathematical boundary tests to determine whether cursor coordinate $P$ falls within any canvas element.
- **Current Reality**:
  - Implemented across all shapes ([src/BoundedShape.cpp](file:///home/subhani/Projects/Inkling/src/BoundedShape.cpp), [src/Ellipse.cpp](file:///home/subhani/Projects/Inkling/src/Ellipse.cpp), [src/Diamond.cpp](file:///home/subhani/Projects/Inkling/src/Diamond.cpp), [src/Stroke.cpp](file:///home/subhani/Projects/Inkling/src/Stroke.cpp)).
  - [src/Canvas.cpp](file:///home/subhani/Projects/Inkling/src/Canvas.cpp#L7-L13) provides `PickAt(Vector2 p)`.
- **Refinement Needed**: Reverse iteration order in `Canvas::PickAt` so the topmost shape is returned when shapes overlap.

---

### Milestone 2: Single Click Selection & Selection Highlight
- **Status**: **TODO**
- **Design Intent**: Allow clicking a shape with `TOOL_SELECT` to mark it as the active selection, visually highlighting it with an Excalidraw-style bounding border. Clicking on empty canvas space clears the selection.
- **Classes & Functions Touched**:
  1. `include/Shape.h`:
     - Add `virtual Rectangle GetBoundingBox() const = 0;` so any shape can provide an enclosing boundary.
  2. `include/BoundedShape.h`:
     - Implement `Rectangle GetBoundingBox() const override { return bound; }`.
  3. `include/Stroke.h`:
     - Implement `Rectangle GetBoundingBox() const override;` computing the min/max extents of all points in `points`.
  4. `include/Canvas.h` & `src/Canvas.cpp`:
     - Add member: `std::vector<Shape*> selectedShapes;` (non-owning observer pointers).
     - Add methods: `void Select(Shape* s);`, `void ClearSelection();`, `const std::vector<Shape*>& GetSelection() const;`.
     - Add `void DrawSelectionOverlay() const;`: Iterates selected shapes, gets their bounding box, pads it by 4px, and draws a subtle blue outline (`DrawRectangleLinesEx`) with corner accents.
  5. `src/main.cpp`:
     - When `MainBar.GetPressed() == TOOL_SELECT` and mouse is pressed:
       - Query `Shape* target = canvas.PickAt(mouse);`.
       - If non-null: `canvas.ClearSelection(); canvas.Select(target);`.
       - If null: `canvas.ClearSelection();`.

---

### Milestone 3: Move by Dragging via Virtual `Translate`
- **Status**: **TODO**
- **Design Intent**: Enable moving selected shapes across the canvas by dragging the mouse. The translation logic must be purely polymorphic, delegating coordinate displacement to each shape subclass.
- **Classes & Functions Touched**:
  1. `include/Shape.h`:
     - Declare pure virtual:
       ```cpp
       virtual void Translate(Vector2 delta) = 0;
       ```
  2. `include/BoundedShape.h` & `src/BoundedShape.cpp`:
     - Implement:
       ```cpp
       void BoundedShape::Translate(Vector2 delta) {
         bound.x += delta.x;
         bound.y += delta.y;
       }
       ```
     - Automatically handles `RecatngleShape`, `Ellipse`, and `Diamond`.
  3. `include/Stroke.h` & `src/Stroke.cpp`:
     - Implement:
       ```cpp
       void Stroke::Translate(Vector2 delta) {
         for (auto &pt : points) {
           pt.x += delta.x;
           pt.y += delta.y;
         }
       }
       ```
  4. `src/main.cpp`:
     - When dragging a selected shape with `TOOL_SELECT`:
       - Calculate `Vector2 delta = Vector2Subtract(mouse, prevMouse);`.
       - Invoke `shape->Translate(delta);`.

---

### Milestone 4: Marquee (Drag-Box) Selection
- **Status**: **TODO**
- **Design Intent**: When `TOOL_SELECT` is active and the user presses and drags across empty canvas space, render a transient translucent selection marquee (`rgba(100, 150, 255, 40)` with a 1px dashed or solid border). On mouse release, select all shapes whose bounding boxes are fully enclosed within the marquee rectangle.
- **Classes & Functions Touched**:
  1. `include/Shape.h`:
     - Add `virtual bool IsEnclosedBy(Rectangle r) const = 0;`.
  2. `include/BoundedShape.h` & `src/BoundedShape.cpp`:
     - Test whether `bound` is entirely contained within `r` (`bound.x >= r.x && bound.x + bound.width <= r.x + r.width && ...`).
  3. `include/Stroke.h` & `src/Stroke.cpp`:
     - Test whether all vertices in `points` fall within `r`.
  4. `include/Canvas.h`:
     - Add `void SelectEnclosed(Rectangle marquee);`.
  5. `src/main.cpp`:
     - Track `marqueeOrigin`, `isMarqueeDragging`, render live preview during drag, and dispatch on release.

---

### Milestone 5: Group Move & Union Bounding Box
- **Status**: **TODO**
- **Design Intent**: When multiple shapes are selected via marquee or Shift-click, render a single composite bounding box encompassing all selected items. Dragging anywhere inside the composite box translates all selected shapes simultaneously.
- **Classes & Functions Touched**:
  1. `include/Canvas.h`:
     - Add helper `Rectangle GetSelectionUnionBounds() const;`: Computes the minimum bounding box encompassing all shapes in `selectedShapes`.
  2. `src/main.cpp`:
     - Group translation loop:
       ```cpp
       for (Shape* s : canvas.GetSelection()) {
         s->Translate(delta);
       }
       ```

---

### Milestone 6: Alt + Drag Duplication via Virtual `Clone`
- **Status**: **TODO**
- **Design Intent**: Implement the Prototype design pattern. If the user holds `Alt` while beginning a drag on an active selection, duplicate each selected shape in place, add the clones to `Canvas`, and switch the active selection to the clones.
- **Classes & Functions Touched**:
  1. `include/Shape.h`:
     - Declare virtual prototype constructor:
       ```cpp
       virtual std::unique_ptr<Shape> Clone() const = 0;
       ```
  2. Concrete Derived Classes (`RecatngleShape`, `Ellipse`, `Diamond`, `Stroke`):
     - Each overrides `Clone()` using copy construction:
       ```cpp
       std::unique_ptr<Shape> RecatngleShape::Clone() const override {
         return std::make_unique<RecatngleShape>(*this);
       }
       ```
  3. `src/main.cpp`:
     - Check `IsKeyDown(KEY_LEFT_ALT)` upon drag initiation.
     - Clone shapes, add to `Canvas`, and translate the newly created clones.

---

### Milestone 7: Resize via Corner Handles
- **Status**: **TODO**
- **Design Intent**: Display 4 square grab handles ($8 \times 8\text{ px}$) at the corners of the selected shape's bounding box. Hovering a handle changes the cursor (`MOUSE_CURSOR_RESIZE_NWSE` or `NESW`). Dragging a handle updates the shape's corner points via `SetCorners`.
- **Classes & Functions Touched**:
  1. `include/BoundedShape.h`:
     - Expose handle coordinate accessors: `TopLeft()`, `TopRight()`, `BottomLeft()`, `BottomRight()`.
     - Handle drag updates by recalculating opposite anchor and active corner.
  2. `include/Canvas.h`:
     - Render corner handle squares in `DrawSelectionOverlay()`.
  3. `src/main.cpp`:
     - Handle hit testing for the 4 handle rectangles prior to shape body picking.

---

### Milestone 8: Rotation Handle (Single Shape Only)
- **Status**: **TODO**
- **Design Intent**: Display a circular rotation handle connected by a vertical stem $20\text{ px}$ above the top edge of a selected shape's bounding box. Dragging the handle updates `rotationAngle` (in degrees or radians).
- **Technical Considerations**:
  - **Rendering**: Must wrap raylib drawing in local transformation matrix operations:
    ```cpp
    rlPushMatrix();
    rlTranslatef(center.x, center.y, 0.0f);
    rlRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    rlTranslatef(-center.x, -center.y, 0.0f);
    // Draw shape
    rlPopMatrix();
    ```
  - **Hit Testing**: Rotated hit testing requires transforming incoming click point $P$ into the unrotated local frame using an inverse 2D rotation matrix before evaluating `Contains()`.
- **Classes Touched**:
  - `BoundedShape` (add `float angle = 0.0f;`), `Canvas::PickAt`, `src/main.cpp`.

---

## Assumptions / Unclear
- It is assumed that multi-shape group rotation is out of scope for this coursework project, as stated in the requirements (Step 5 specifies "Group move with a group bounding box, no group rotation"). Rotation is restricted to single shapes only.
