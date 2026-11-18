# Documentation Maintenance Guide & Contributor Checklist

Documentation must evolve alongside code to prevent architectural drift. As Inkling expands with selection tools, transformations, and new drawing primitives, contributors and maintainers must ensure technical specifications remain accurate. This guide provides a synchronization matrix mapping codebase changes to their corresponding documentation files, alongside a mandatory pre-commit checklist.

## Related Files
- Documentation Suite: [docs/](file:///home/subhani/Projects/Inkling/docs/)
- System Overview: [docs/00-overview.md](00-overview.md)
- Class Hierarchy: [docs/01-classes-and-hierarchy.md](01-classes-and-hierarchy.md)
- Roadmap: [docs/07-roadmap-selection-feature.md](07-roadmap-selection-feature.md)
- Known Issues: [docs/08-known-issues-and-todo.md](08-known-issues-and-todo.md)

---

## 1. Documentation Synchronization Matrix

When implementing changes in the codebase, consult this matrix to identify every documentation file that must be updated:

| Codebase Change | Primary Doc to Update | Secondary Docs to Update | What to Update |
| :--- | :--- | :--- | :--- |
| **Adding a New Shape**<br>*(e.g., `LineShape`, `ArrowShape`)* | [01-classes-and-hierarchy.md](01-classes-and-hierarchy.md) | [05-hit-testing.md](05-hit-testing.md)<br>[04-tools-and-input-handling.md](04-tools-and-input-handling.md)<br>[00-overview.md](00-overview.md) | • Add class definition and members to catalog.<br>• Update Mermaid class diagram.<br>• Document `Contains` math and worked examples.<br>• Detail drag-creation preview handling.<br>• Update feature list in overview. |
| **Adding a New Tool**<br>*(e.g., Eraser, Text, Panning)* | [04-tools-and-input-handling.md](04-tools-and-input-handling.md) | [03-toolbar-bar-and-buttons.md](03-toolbar-bar-and-buttons.md)<br>[00-overview.md](00-overview.md) | • Update `enum Tool` status table.<br>• Document keyboard shortcut mapping in `main.cpp`.<br>• Document mouse state machine branch.<br>• Update overview feature matrix. |
| **Adding a Toolbar Button / Token**<br>*(e.g., new button, changing gap)* | [03-toolbar-bar-and-buttons.md](03-toolbar-bar-and-buttons.md) | [00-overview.md](00-overview.md) | • Update `ToolbarConfig` tokens table.<br>• Recalculate container height and slot pitch.<br>• Re-verify icon-label collision clearance.<br>• Update button count $N$. |
| **Adding a Virtual Function to `Shape`**<br>*(e.g., `Translate`, `Clone`)* | [01-classes-and-hierarchy.md](01-classes-and-hierarchy.md) | [07-roadmap-selection-feature.md](07-roadmap-selection-feature.md)<br>[02-canvas-and-shape-storage.md](02-canvas-and-shape-storage.md) | • Add function signature to `Shape` interface.<br>• Document overrides in `BoundedShape` and `Stroke`.<br>• Advance milestone status in selection roadmap.<br>• Update canvas dispatch documentation if called via canvas. |
| **Modifying Canvas Storage / Z-Order**<br>*(e.g., reverse traversal in `PickAt`)* | [02-canvas-and-shape-storage.md](02-canvas-and-shape-storage.md) | [05-hit-testing.md](05-hit-testing.md)<br>[08-known-issues-and-todo.md](08-known-issues-and-todo.md) | • Update loop logic and sequence diagrams.<br>• Update `PickAt` code excerpts.<br>• Mark resolved issues in catalog. |
| **Fixing a Bug from Issue Catalog**<br>*(e.g., double `CloseWindow`)* | [08-known-issues-and-todo.md](08-known-issues-and-todo.md) | Relevant topic document | • Mark issue status as resolved.<br>• Update affected code snippets in topic docs. |

---

## 2. Pre-Commit Contributor Checklist

Run through this quality checklist before committing any code or documentation changes:

### A. Compilation & Diagnostic Verification
- [ ] **Clean Build**: Executes with zero errors and zero warnings:
  ```bash
  g++ -std=c++17 -Wall -Wextra src/*.cpp $(pkg-config --cflags --libs raylib) -o build/app
  ```
- [ ] **Sanitizer Validation**: Compiles and executes cleanly under AddressSanitizer and UndefinedBehaviorSanitizer:
  ```bash
  g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined src/*.cpp $(pkg-config --cflags --libs raylib) -o build/app_san
  ./build/app_san
  ```

### B. Functional & Asset Sanity
- [ ] **Launch Directory**: Application launches successfully from the project root (`./build/app`) and displays all 11 toolbar icons without texture loading failures.
- [ ] **Quadrant Dragging**: Dragging shapes in all 4 directions (up-left, up-right, down-left, down-right) produces correctly normalized bounding rectangles.
- [ ] **Hit Testing**: Clicking within the shape interior triggers detection; clicking on empty canvas space does not.

### C. OOP Design Integrity
- [ ] **Destructors**: Any new class in the `Shape` inheritance tree preserves polymorphic destruction (`virtual ~Shape() = default;`).
- [ ] **Pure Virtual Contracts**: Any new derived shape overrides all pure virtual functions (`Draw()` and `Contains()`).
- [ ] **Const Correctness**: Read-only member functions (`Draw() const`, `Contains() const`, `GetId() const`) are strictly marked `const`.

### D. Documentation Alignment
- [ ] **Cross-Links**: All markdown cross-links use valid relative file paths (e.g. `[01-classes-and-hierarchy.md](01-classes-and-hierarchy.md)`).
- [ ] **Real Code Quotes**: Code excerpts match actual source files with exact identifier names, parameters, and line ranges.
- [ ] **Roadmap Status**: Any completed milestones in [07-roadmap-selection-feature.md](07-roadmap-selection-feature.md) have been updated from `TODO` to `DONE`.

---

## Assumptions / Unclear
- No assumptions were made. This document acts as the governing procedure for documentation maintenance in Inkling.
