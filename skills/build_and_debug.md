# Build & Debug

Repo-internal build, run, and crash-diagnostic reference. Read this when running `cmake`, debugging a `daslang` crash, flipping module flags, or tracking down an AOT hash mismatch. Day-to-day editing doesn't require it — only operations that touch the build system or runtime exit codes.

The repo builds on **Windows, Linux, macOS, iOS, Android, and WASM** (CI runs the full matrix). Most commands below are platform-neutral. Where a platform detail differs, it's noted.

## Build & run

- **Build system:** CMake. On Windows the default generator is MSVC (Visual Studio 2022); on Linux/macOS use Ninja or Make; on Android/iOS/WASM see the platform docs.
- **Generate** (Windows convenience):
  - `generate_msvc_2022.bat` → creates `build/DAS.sln`
  - On Linux/macOS: `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release` (or omit `-G Ninja` to use the system default)
- **Build:** `cmake --build build --config Release -j 64 -- /nodeReuse:false` (the `/nodeReuse:false` is MSBuild-only; drop it on Ninja/Make)
- **Compiler binary:**
  - Windows MSVC (multi-config): `bin/Release/daslang.exe`
  - Linux/macOS (single-config): `build/daslang`
- **Live-reload host:** `bin/Release/daslang-live.exe` (Windows) / `build/daslang-live` (Linux/macOS) — same script runs in both; see `utils/daslang-live/main.cpp`
- **Run a script:** `<daslang-binary> path/to/script.das`
- **Compile-only check:** `<daslang-binary> -compile-only path/to/script.das` — compiles without simulation or execution, useful for syntax/type checking without needing a window or GL context. Use `-dry-run` to also simulate (but not execute).
- **Run tests:** `<daslang-binary> dastest/dastest.das -- --test path/to/test.das`
- **AOT tests:** `cmake --build build --config Release --target test_aot` then `<test_aot-binary> -use-aot dastest/dastest.das -- --use-aot --test tests`
- **IMPORTANT:** When adding a new test directory under `tests/`, register it in `tests/aot/CMakeLists.txt` for AOT compilation. See `skills/aot_testing.md` for the step-by-step pattern. CI runs ALL tests with AOT enabled — unregistered test directories cause `error[50101]: AOT link failed`

This skill uses `bin/Release/daslang.exe` in examples below (the dominant local-dev case); substitute the right path on other platforms.

## Build timing

- **Builds are slow** — clean builds take **15-25 minutes**, incremental builds take **2-10 minutes** depending on what changed
- **Always use `timeout: 0`** (no timeout) when running `cmake --build` commands. A build that hasn't finished is not stuck or broken, it's just compiling
- **Do not assume build failure** from lack of output — MSVC and most generators are silent during compilation and only print when there are warnings/errors or when it finishes
- For incremental builds after editing a single `.cpp` file, expect ~2-5 minutes. For changes touching headers, expect longer

## Debugging runtime crashes

- **Always check the exit code** after running `daslang` — a crash may produce no output, looking like a silent success
  - Windows / PowerShell: `$LASTEXITCODE`
  - Bash / zsh (Linux, macOS, Git Bash): `$?`
- Exit code `-1073741819` (`0xC0000005`) on Windows = **Access Violation** — native crash (segfault). On Linux/macOS the equivalent shows up as exit code `139` (SIGSEGV) or similar. The bug is in C++ bindings or smart-pointer misuse
- **Don't truncate output** with `head`/`tail` — daslang stack traces and `DAS_GC_BREAK_ON_ID` traces are easily clipped. Capture full output, then `grep` if needed
- **`options log_infer_passes`** — append at the end of a failing `.das` file to dump per-pass infer activity (which generics got reified, when finalize ran, where lookups missed). Smaller and more targeted than `options log` for template/generic reification bugs. `options log` stays the right tool when you need the final program text

## Build configurations (module flags)

Optional modules are controlled by CMake flags (`DAS_*_DISABLED`). The active configuration lives in `.vscode/settings.json` under `cmake.configureSettings` (the "WIP" block is the active one; others are commented-out presets).

Key flags (all default to `ON` = disabled in CMakeLists.txt):
- `DAS_HV_DISABLED` — dasHV (HTTP/WebSocket via libhv)
- `DAS_PUGIXML_DISABLED` — dasPUGIXML (XML parsing)
- `DAS_GLFW_DISABLED` — GLFW (OpenGL windowing)
- `DAS_IMGUI_DISABLED` — ImGui
- `DAS_LLVM_DISABLED` — LLVM JIT
- `DAS_CLANG_BIND_DISABLED` — Clang bindings
- `DAS_AUDIO_DISABLED`, `DAS_STBIMAGE_DISABLED`, `DAS_STBTRUETYPE_DISABLED`, `DAS_STDDLG_DISABLED`, `DAS_SQLITE_DISABLED`

**To change modules:** edit the active `cmake.configureSettings` in `.vscode/settings.json`, then reconfigure. Or pass `-DFLAG=VALUE` to your `cmake -B build ...` command. VSCode CMake Tools will pick up settings changes automatically.

**Documentation generation** (`doc/reflections/das2rst.das`) requires `DAS_HV_DISABLED=OFF` and `DAS_PUGIXML_DISABLED=OFF` because it documents all modules. Temporarily enable them, rebuild `daslang`, run das2rst, then revert settings.

## AOT hash mismatches

When AOT fails with `error[50101]: AOT link failed`, the issue is a **semantic hash mismatch** between the generated C++ stubs and runtime. Each generated `.cpp` file has hash comments showing function hashes and dependency hashes. The runtime error also prints the same breakdown. Compare them to find the diverging function or dependency.

For the full debugging workflow, see `skills/aot_hash_desync_debugging.md` (side-by-side SimNode dumps via `options log_nodes`/`log_nodes_aot_hash`, common shapes, C++ debug switches).

The AOT C++ emitter lives in **`daslib/aot_cpp.das`** (the old `src/ast/ast_aot_cpp.cpp` was emptied by commit `581363ebc`). When codegen output diverges, edit `daslib/aot_cpp.das` — not the C++ stub in `src/ast/`.
