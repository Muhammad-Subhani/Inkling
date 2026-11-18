# Inkling: Architecture & System Overview

Inkling is an Excalidraw-style lightweight whiteboard application written in modern C++17 and powered by the raylib graphics library. Developed as an Object-Oriented Programming (OOP) course project, Inkling emphasizes foundational software engineering concepts including polymorphic class hierarchies, abstract base classes, RAII memory management, and decoupled graphical user interface design. This document provides a high-level overview of the application, its technical stack, repository layout, build instructions, current feature matrix, and a navigation index to the detailed technical documentation.

## Related Files
- Source Entry: [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp)
- Application Window: [include/window.h](file:///home/subhani/Projects/Inkling/include/window.h)
- Configuration & Styling: [include/Bar.h](file:///home/subhani/Projects/Inkling/include/Bar.h), [docs/TOOLBAR_TUNING.md](file:///home/subhani/Projects/Inkling/docs/TOOLBAR_TUNING.md)

---

## 1. Project Goals & Vision

The core goals of Inkling are:
1. **Interactive Whiteboard Canvas**: Provide a clean, dark-themed canvas for real-time shape creation, freehand sketching, and visual diagramming.
2. **Object-Oriented Design**: Serve as an exemplar of C++ OOP principles—specifically inheritance, run-time polymorphism via virtual tables (`vtable`), pure virtual contracts, constructor chaining, and standard library container ownership (`std::unique_ptr`).
3. **Excalidraw Aesthetics**: Deliver a vertical floating toolbar inspired by Excalidraw, complete with smooth button transition animations (120ms lerp), bilinear-filtered vector icons, tooltip previews, and single-letter keyboard shortcuts.
4. **Extensibility for Selection & Manipulation**: Lay the algorithmic groundwork for full canvas selection, bounding-box hit testing, group translation, and transformation handles.

---

## 2. Technology Stack

Inkling relies on a minimal, high-performance C++ toolchain:
- **Programming Language**: C++17 (`-std=c++17`). Employs features such as `std::make_unique`, `using Base::Base` constructor inheritance, `constexpr` tokens, and `auto` range-based loops.
- **Graphics & Input Library**: [raylib](https://www.raylib.com/) (linked dynamically or statically via `pkg-config`). Raylib provides cross-platform window initialization, hardware-accelerated 2D rendering, texture management, font measurement, and mouse/keyboard polling.
- **Compiler**: `g++` (GCC) with strict diagnostic flags (`-Wall -Wextra`).
- **Target Platform**: Linux (x86_64 / Wayland / X11).
- **Package Configuration**: `pkg-config` for resolving raylib compilation and linker flags (`-lraylib`).

---

## 3. Directory & Folder Layout

The repository is structured with a clean separation of interface headers, implementation sources, binary assets, and documentation:

```
Inkling/
├── assets/                         # Graphic and media assets
│   ├── icons/                      # Rasterized 96x96 PNG icons
│   │   ├── src/                    # SVG source files (stroke-width: 1.75)
│   │   ├── arrow.png
│   │   ├── diamond.png
│   │   ├── ellipse.png
│   │   ├── eraser.png
│   │   ├── hand.png
│   │   ├── line.png
│   │   ├── pencil.png
│   │   ├── rectangle.png
│   │   ├── select.png
│   │   ├── text.png
│   │   └── twinkle.png
│   ├── twinkle_k.png
│   └── Pasted image.png
├── build/                          # Compiled artifacts
│   └── app                         # Executable binary
├── docs/                           # Technical documentation suite
│   ├── 00-overview.md              # System overview & navigation index
│   ├── 01-classes-and-hierarchy.md # Detailed OOP class hierarchy & mechanics
│   ├── 02-canvas-and-shape-storage.md # Storage, z-order, and ownership model
│   ├── 03-toolbar-bar-and-buttons.md  # UI bar, buttons, tokens, and animations
│   ├── 04-tools-and-input-handling.md # Event loop, state machine, and creation flow
│   ├── 05-hit-testing.md           # Hit testing algorithms, math, and benchmarks
│   ├── 06-build-and-environment.md # Build instructions, compiler flags, and debugging
│   ├── 07-roadmap-selection-feature.md # Feature roadmap for selection & transforms
│   ├── 08-known-issues-and-todo.md # Identified bugs, architectural debt, and fixes
│   ├── 09-maintaining-these-docs.md   # Maintenance guidelines for future contributors
│   ├── TOOLBAR_TUNING.md           # Design tokens & icon collision calculations
│   └── PolyMorphDeign.pdf          # Academic course notes on C++ polymorphism
├── include/                        # Public class declarations & interfaces
│   ├── Bar.h                       # Vertical toolbar container & ToolbarConfig tokens
│   ├── BoundedShape.h              # Intermediate base class for 2D bounding boxes
│   ├── Button.h                    # Toolbar button component (inherits BoundedShape)
│   ├── Canvas.h                    # Shape container, renderer, and hit-picker
│   ├── Diamond.h                   # Rhombus shape primitive
│   ├── Ellipse.h                   # Elliptical shape primitive
│   ├── Icons.h                     # RAII texture loader and container
│   ├── RectangleShape.h            # Rectangle shape primitive (RecatngleShape)
│   ├── ShapeFactory.h              # Factory function MakeShape for bounded shapes
│   ├── Shape.h                     # Abstract polymorphic base class
│   ├── Stroke.h                    # Freehand multi-segment polyline stroke
│   ├── Tool.h                      # Tool enumeration and tool-to-shape mapping
│   └── window.h                    # RAII window lifecycle management struct
├── prototypes/                     # Scratchpad directory for experimental snippets
├── src/                            # Implementation units (.cpp)
│   ├── bar.cpp
│   ├── BoundedShape.cpp
│   ├── Button.cpp
│   ├── Canvas.cpp
│   ├── Diamond.cpp
│   ├── Ellipse.cpp
│   ├── Icons.cpp
│   ├── main.cpp
│   ├── RectangleShape.cpp
│   ├── Shape.cpp
│   ├── ShapeFactory.cpp
│   ├── Stroke.cpp
│   └── Tool.cpp
└── README.md                       # Project attribution & icon credits
```

---

## 4. How to Build and Run

### Compilation
The project does not use a Makefile or CMakeLists; all compilation is executed directly via `g++` invoking the C++17 standard with raylib linking:

```bash
g++ -std=c++17 -Wall -Wextra src/*.cpp $(pkg-config --cflags --libs raylib) -o build/app
```

### Execution
Run the compiled binary directly from the project root:

```bash
./build/app
```

> [!NOTE]
> The working directory must be the project root so that raylib's texture loader finds `assets/icons/*.png`. Running `./app` directly from inside the `build/` directory will fail to locate icon assets.

---

## 5. Current Feature Status

### Implemented & Working
- **Shape Rendering**:
  - Rectangles ([src/RectangleShape.cpp](file:///home/subhani/Projects/Inkling/src/RectangleShape.cpp))
  - Ellipses ([src/Ellipse.cpp](file:///home/subhani/Projects/Inkling/src/Ellipse.cpp))
  - Diamonds ([src/Diamond.cpp](file:///home/subhani/Projects/Inkling/src/Diamond.cpp))
  - Freehand Strokes / Polyline pencil paths ([src/Stroke.cpp](file:///home/subhani/Projects/Inkling/src/Stroke.cpp))
- **Live Drag-Creation Preview**: Live preview during mouse press-and-drag for rectangles, ellipses, diamonds, and freehand pencil paths.
- **Coordinate Normalization**: Safe dragging in all four screen quadrants (up, down, left, right) via `BoundedShape::SetCorners`.
- **Excalidraw-Style Vertical Toolbar**:
  - 11 buttons (`Hand`, `Select`, `Rectangle`, `Diamond`, `Ellipse`, `Arrow`, `Line`, `Draw/Pencil`, `Text`, `Magic wand/Twinkle`, `Eraser`).
  - Smooth 120ms color transitions for hover and active states.
  - Floating container with shadow, border, and automatic vertical centering on window resize.
  - Tooltips rendered on button hover.
  - Custom cursor toggle (`MOUSE_CURSOR_POINTING_HAND` when hovering buttons).
- **Texture Management**: RAII-based texture loader ([include/Icons.h](file:///home/subhani/Projects/Inkling/include/Icons.h)) ensuring clean lifecycle management.
- **Hit Testing Infrastructure**:
  - Point-in-rectangle for `BoundedShape` / `RecatngleShape`.
  - Normalized quadratic test for `Ellipse`.
  - Normalized Manhattan distance test for `Diamond`.
  - Polyline segment collision with pixel tolerance for `Stroke`.
  - `Canvas::PickAt(Vector2)` hit test invocation on canvas click.

### Not Implemented Yet
- **Selection State & Highlight**: Clicking a shape prints a console trace (`HIT`), but shapes cannot yet be selected, stored in a selection list, or outlined with a selection border.
- **Transformations**: No translation (moving), resizing via corner handles, or rotation handles.
- **Marquee Selection**: Dragging a box across empty space does not select enclosed shapes.
- **Inert Tools**: The toolbar buttons for Hand, Select, Arrow, Line, Text, Twinkle, and Eraser currently do not instantiate canvas shapes or tool actions.
- **Undo / Redo & Serialization**: No command stack, history tracking, or file save/load (JSON/binary).

---

## 6. Documentation Index

Detailed documentation is organized into dedicated topic guides:

| Document | Title | Description |
| :--- | :--- | :--- |
| [01-classes-and-hierarchy.md](01-classes-and-hierarchy.md) | Classes & Inheritance Hierarchy | Full breakdown of all classes, members, virtual tables, pure virtual methods, and Mermaid class diagrams. |
| [02-canvas-and-shape-storage.md](02-canvas-and-shape-storage.md) | Canvas & Shape Storage | Ownership model (`std::unique_ptr`), drawing order (z-order), dynamic dispatch, and `PickAt` mechanics. |
| [03-toolbar-bar-and-buttons.md](03-toolbar-bar-and-buttons.md) | Toolbar, Bar & Buttons | UI component design, `ToolbarConfig` tokens, hover/active animation math, and layout algorithms. |
| [04-tools-and-input-handling.md](04-tools-and-input-handling.md) | Tools & Input Handling | The frame lifecycle, tool enumeration, drag-creation state machine, live previews, and keyboard shortcut mapping. |
| [05-hit-testing.md](05-hit-testing.md) | Hit Testing & Mathematics | Geometric collision formulas for bounding boxes, ellipses, rhombuses, and strokes, plus worked numeric examples. |
| [06-build-and-environment.md](06-build-and-environment.md) | Build System & Environment | Compiler flags, raylib dependency resolution, compilation model, warnings, and sanitizers (`ASan`, `gdb`). |
| [07-roadmap-selection-feature.md](07-roadmap-selection-feature.md) | Selection Feature Roadmap | Phased engineering plan for single-click selection, translation, bounding boxes, handles, and rotation. |
| [08-known-issues-and-todo.md](08-known-issues-and-todo.md) | Known Issues & Architectural Debt | Catalog of bugs, reverse picking logic, keyboard shortcut collisions, lifetime bugs, and refactoring tasks. |
| [09-maintaining-these-docs.md](09-maintaining-these-docs.md) | Maintaining These Docs | Contributor checklist and cross-reference table for keeping documentation aligned with future code updates. |

---

## Assumptions / Unclear
- The absence of a formal build script (Makefile or CMakeLists.txt) implies the project is currently built via shell command invocation or direct IDE task runners.
- The `prototypes/` folder is empty and appears reserved for standalone experimental code.
