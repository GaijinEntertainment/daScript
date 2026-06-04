# Building dasClangBind + regenerating native bindings (libclang)

Read this before enabling the `dasClangBind` module, installing/updating the libclang SDK it needs, or running any of the `bind_*.das` self-binders that regenerate a module's C++ glue (`bind_clangbind`, `bind_imgui`, `bind_llvm`, `bind_sqlite`, …).

`dasClangBind` wraps **libclang** (the stable `clang-c/Index.h` C API) and registers the `cbind` daslang module. The binders under `modules/dasClangBind/bind/` parse a C/C++ header with libclang and emit the `*.inc` + `func_*.cpp` files that bind a library to daslang. The module is **disabled by default** (`DAS_CLANG_BIND_DISABLED` defaults `ON`) and is only built when you have a matching libclang SDK on disk.

## 1. Install the matching libclang SDK

The CMake hard-pins the version: `find_package(Clang <major>.<minor> ...)` in [modules/dasClangBind/CMakeLists.txt](../modules/dasClangBind/CMakeLists.txt) (currently **22.1**). You need an SDK whose `ClangConfigVersion.cmake` reports that `major.minor` (any patch ≥ the pin works — 22.1.5 satisfies 22.1).

**Get the right package.** From the LLVM release for that version (e.g. <https://github.com/llvm/llvm-project/releases/tag/llvmorg-22.1.5>):

- **Windows (MSVC): use the `clang+llvm-<ver>-x86_64-pc-windows-msvc.tar.xz` archive — NOT the `LLVM-<ver>-win64.exe` installer.** The installer ships `libclang.dll`/`.lib` + headers but **no `lib/cmake/clang`**, so `find_package(Clang)` fails (`Clang_DIR-NOTFOUND`). The `clang+llvm-…-windows-msvc` archive is the full SDK (`bin/`, `lib/`, `include/`, **`lib/cmake/clang/`**) and is MSVC-ABI-built, so it links into daslang's MSVC build. Its `libclang.dll` is self-contained (LLVM statically linked), so no companion DLLs are needed at runtime.
- **Linux / macOS:** the distro/`apt.llvm.org` `libclang-dev` + `llvm-<ver>-dev` packages, or the matching `clang+llvm-<ver>-<triple>` archive.
- **Windows (mingw/clang64):** msys2's `mingw-w64-clang-x86_64-toolchain` ships libclang + its cmake package; the libclang there is dynamically linked, so the `.shared_module` needs the full `libclang + libLLVM + libc++ + …` chain copied next to `daslang.exe` (the CMake POST_BUILD handles this).

**Where it goes.** `PATH_TO_LIBCLANG` defaults to `<daslang_root>/../libclang` (a sibling folder of the checkout). Extract the archive there (so `../libclang/lib/cmake/clang/ClangConfig.cmake` exists), or point at it explicitly with `-DPATH_TO_LIBCLANG=<dir>`.

## 2. Enable + build

```
cmake -B build -DDAS_CLANG_BIND_DISABLED=OFF
cmake --build build --config Release --target daslang dasModuleClangBind -j 64
```

Watch the configure log for `dasClangBind module included.` and `LIBCLANG_LIB at <path>`. On Windows the `dasModuleClangBind` POST_BUILD copies `libclang.dll` next to `daslang.exe` (daslang loads `.shared_module`s with restricted `LoadLibraryEx` search flags — `PATH` is not searched, so the DLL must live in `bin/`).

## 3. Run a binder to regenerate

The self-binder regenerates a module's committed glue from the live headers:

```
# dasClangBind's own bindings (from clang-c/Index.h)
bin/Release/daslang.exe modules/dasClangBind/bind/bind_clangbind.das -- --clang_path <libclang-include-dir>/

# dasImgui (parses imgui headers; -i input dir, -o output src dir)
bin/Release/daslang.exe modules/dasImgui/bind/bind_imgui.das -- -i <imgui-headers>/ -o <module-src>/
```

CI guards staleness with `git diff --exit-code -- <module>/src/` after running the binder — if your committed `*.inc`/`func_*.cpp` don't match a fresh regen, the build fails with a "generated files are out of date" message. Always re-run the binder and commit the result after touching a binder or bumping the libclang version.

## 4. Bumping the libclang version

When `find_package(Clang X.Y)` moves (e.g. 16 → 22.1), the `clang-c` API changes (enum renames, ~hundreds of additions), so `dasClangBind/src/*` must be regenerated against the new headers in the **same** commit. Worked example: `git show 17d1b035a` ("dasClangBind: regen bindings for libclang 22.1.5"). Update: the `find_package` pin, the CI `--clang_path`, and re-run `bind_clangbind.das`.

The generated output is **patch-version-sensitive** — regenerating with a different patch (e.g. 22.1.5 vs the 22.1.4 CI uses) produces a small enum delta. Regenerate with the *same* libclang patch CI uses, or the `git diff --exit-code` self-binder check will fail.

## Gotchas

- **`LNK1104: cannot open … .dll` on relink** — a running `daslang`/`daslang-live`/MCP-server process holds the runtime DLL. Enabling clang-bind regenerates the module registry (`external_*.inc`), which forces a relink of `libDaScriptDyn_runtime.dll`. Kill the holders first (`Get-Process daslang | Stop-Process -Force`), then rebuild.
- **`C1001` / `LNK1000` during "Generating code"** — MSVC's link-time-codegen (LTCG) choking on a **stale incremental database** (`.ipdb`/`.iobj`) from prior builds (`"no usable IPDB/IOBJ … fall back to full compilation"` confirms it). Not a code bug — clean-rebuild the offending target: `cmake --build build --target <name> --clean-first`. Common after a config change forces a recompile in a long-lived `build/`. (See `skills/build_and_debug.md`.)
- **Too-old SDK is silently skipped** — the `find_package(Clang 22.1)` has no `EXACT`, so it accepts any `22.1.x` or newer; a too-old SDK (e.g. 16.0.6 against a 22.1 pin) is rejected and the whole module is skipped with no error (you just get no `cbind`). A *too-new* major can satisfy the pin yet drift from the committed bindings' `clang-c` API — keep the SDK on the pinned `major.minor` (see version-bump note above).
- **CRT match (static deps only)** — a *static* C/C++ lib linked into a module must use the same MSVC CRT as daslang or you get `LNK2038: RuntimeLibrary mismatch`. daslang uses the dynamic CRT (`/MD`) by default and the static CRT (`/MT`) only when `DAS_USE_STATIC_STD_LIBS=ON` (CMakeCommon.txt); a static dep must mirror that — see the `DAS_USE_STATIC_STD_LIBS`-gated `BUILD_FOR_MT` in [modules/dasHV/CMakeLists.txt](../modules/dasHV/CMakeLists.txt). libclang is exempt: it is consumed as a DLL via its import lib, so its internal CRT is sealed behind the C API and never has to match.
