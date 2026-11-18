# Build System, Compiler Toolchain & Debugging

This document outlines the technical environment, compilation toolchain, dependency resolution, compiler flags, and debugging procedures for Inkling. The project is designed to compile directly on Linux workstations using the GNU C++ Compiler (`g++`), modern C++17 language standards, and the raylib multimedia library linked via `pkg-config`.

## Related Files
- Source Directory: [src/](file:///home/subhani/Projects/Inkling/src/)
- Header Directory: [include/](file:///home/subhani/Projects/Inkling/include/)
- Binary Output: [build/app](file:///home/subhani/Projects/Inkling/build/app)

---

## 1. System Requirements & Dependencies

To build and run Inkling, the host system requires:
1. **Operating System**: Linux (x86_64, Wayland or X11 display server).
2. **C++ Compiler**: GCC (`g++`) version 7.1 or higher (supporting ISO C++17).
3. **Raylib**: Version 4.0 or higher development package (`libraylib-dev` on Debian/Ubuntu, `raylib` on Arch Linux / Fedora).
4. **Pkg-Config**: Helper tool used to query compiler and linker flags for installed libraries.

Verify dependencies in your terminal:
```bash
g++ --version
pkg-config --modversion raylib
```

---

## 2. Exact Build & Run Commands

Inkling compiles in a single invocation without requiring external build tools like Make or CMake.

### Standard Build Command
Run this command from the repository root:

```bash
g++ -std=c++17 -Wall -Wextra src/*.cpp $(pkg-config --cflags --libs raylib) -o build/app
```

### Execution Command
Launch the compiled binary from the root directory:

```bash
./build/app
```

> [!IMPORTANT]
> **Working Directory Sensitivity**:
> Always execute `./build/app` from the project root (`/path/to/Inkling`). In [src/main.cpp](file:///home/subhani/Projects/Inkling/src/main.cpp#L18-L23), icon textures are loaded using relative paths (`assets/icons/*.png`). If the binary is launched directly from within `build/` (e.g. `cd build && ./app`), raylib will fail to locate the textures, rendering blank tool buttons.

---

## 3. Compiler Flags Deep-Dive

| Flag | Purpose & Technical Implication |
| :--- | :--- |
| `-std=c++17` | Enforces the ISO C++17 standard. In Inkling, this enables:<br>• `std::make_unique` for memory-safe shape construction.<br>• Constructor inheritance syntax (`using BoundedShape::BoundedShape;`).<br>• `constexpr` definitions within custom namespaces (`ToolbarConfig`).<br>• Nested namespace declarations and range-based loops. |
| `-Wall` | Activates standard compiler diagnostic warnings, catching uninitialized variables, unreachable statements, missing switch cases, and format mismatches. |
| `-Wextra` | Enables supplementary diagnostic checks, including signed/unsigned integer comparisons, unused function parameters, and shadowed variable scopes. |
| `$(pkg-config ...)` | Dynamically expands to the system's raylib compile and link arguments. On Linux, this typically expands to `-lraylib` (or includes dependent system libraries like `-lGL -lm -lpthread -ldl -lrt -lX11`). |
| `-o build/app` | Designates the target output path and filename for the compiled binary. |

---

## 4. Compilation Organization & File Structure

The project uses relative path inclusion rather than `-Iinclude` compiler flags:
- All source files in `src/` include their respective headers using relative paths:
  ```cpp
  #include "../include/Shape.h"
  #include "../include/BoundedShape.h"
  ```
- Because of this layout, passing `src/*.cpp` to `g++` compiles all translation units simultaneously and links them into `build/app` in a single compiler pass.

### Compilation Architecture Trade-offs
- **Advantages**: Zero configuration overhead; no build script dependencies; instantly understandable for students and contributors.
- **Disadvantages**: Lacks incremental compilation. Every rebuild recompiles every translation unit from scratch. As the project expands, introducing a `Makefile` or `CMakeLists.txt` with incremental object caching (`.o` files) is recommended.

---

## 5. Common Warnings & Pitfalls

### 5.1 Member Variable Shadowing (`-Wshadow`)
In [include/Stroke.h](file:///home/subhani/Projects/Inkling/include/Stroke.h#L13-L14):
```cpp
class Stroke : public Shape {
private:
  Color color = WHITE;
  float thickness = 2;
};
```
Both `color` and `thickness` are already declared in the base class [include/Shape.h](file:///home/subhani/Projects/Inkling/include/Shape.h#L18-L19) under `protected:`. Redeclaring them in `Stroke` creates member shadowing. While valid C++, compiling with `-Wshadow` flags these as warnings.

### 5.2 Typographical Inconsistencies
The class name `RecatngleShape` in [include/RectangleShape.h](file:///home/subhani/Projects/Inkling/include/RectangleShape.h#L4) omits the 'n'. Any external code including `RectangleShape.h` must instantiate `RecatngleShape` until refactored.

### 5.3 Division by Zero in Geometric Queries
In [src/Ellipse.cpp](file:///home/subhani/Projects/Inkling/src/Ellipse.cpp#L11-L14) and [src/Diamond.cpp](file:///home/subhani/Projects/Inkling/src/Diamond.cpp#L19-L22):
If the user clicks without dragging, `bound.width` or `bound.height` evaluates to `0.0f`. Dividing by `radiiX` or `radiiY` produces `inf` or `nan`.

---

## 6. Debugging & Diagnostic Environments

### 6.1 Compiling with Debug Symbols (`-g`)
To enable line-by-line debugging and stack trace symbols, append `-g` and disable compiler optimizations:

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 src/*.cpp $(pkg-config --cflags --libs raylib) -o build/app_debug
```

### 6.2 Running Under GDB (GNU Debugger)
Inspect crashes or backtraces:

```bash
gdb ./build/app_debug
```
Common GDB commands:
- `run`: Starts the application.
- `bt` (backtrace): Prints the call stack upon a crash or segmentation fault.
- `break Canvas::PickAt`: Sets a breakpoint at shape picking.
- `print *x`: Inspects dynamic members of a shape pointer.

### 6.3 Memory & Undefined Behavior Sanitizers (`ASan` / `UBSan`)
AddressSanitizer detects memory corruption, dangling pointers, and double-free calls; UndefinedBehaviorSanitizer detects division by zero and null pointer dereferences:

```bash
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined src/*.cpp $(pkg-config --cflags --libs raylib) -o build/app_sanitized
./build/app_sanitized
```

If an invalid pointer or memory access occurs, AddressSanitizer prints a detailed crash report indicating the exact source file and line number.

---

## Assumptions / Unclear
- The project relies on the system raylib installation matching the version expected by the developer. No vendored raylib binaries or submodules are present in the repository.
