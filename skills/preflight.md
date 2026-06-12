# Preflight — CI lane ↔ local mirror

Read this before pushing a non-trivial PR, or when a CI lane goes red and you
need to reproduce it. Every CI failure during the fixed-array rework (PR #3095)
was an **oracle mismatch** — a gate CI enforces that no local step mirrored —
not a wrong change. This file maps every PR-triggered lane to its exact local
mirror, or says honestly that there isn't one.

**`utils/preflight` automates these gates.** `daslang utils/preflight/main.das`
runs the fast tier (format + lint + clang frontend pass on changed C++ —
escalating to a full src+tests-cpp sweep when a header changed; seconds);
`-- --full` adds dasgen freshness, the CI-only-das compile sweep, the six doc
gates, ctest, the interp/JIT/AOT suites, and the sequence smoke. `--list-gates`
shows the menu; `--only <names>` / `--skip <names>` select subsets. Gates whose
host tool or module is missing report `SKIP` with an install/rebuild hint. The
tables below remain the reference for what each gate mirrors and for running
any step by hand.

**Conventions.** `<daslang>` = your compiler binary: `bin/Release/daslang.exe`
(Windows MSVC multi-config), `bin/daslang` (Ninja single-config — what CI's
extended_checks uses on all three OSes), or `build/daslang` (Make/Ninja
without `EXECUTABLE_OUTPUT_PATH`). Commands are platform-neutral unless marked.
"WSL" means the verbatim-CI recipe in `skills/wsl_ci_repro.md` — fresh clone at
the CI ref, never a working-tree copy.

## What runs on every PR

| Workflow | Trigger | Jobs |
|---|---|---|
| `build.yml` | every PR | `build` matrix (5 targets × Debug/Release/RelWithDebInfo × sanitizers), `bundle_smoke`, `build_windows_mingw`, `build_windows_clangcl` |
| `extended_checks.yml` | every PR | linux + darwin15-arm64 + windows, ALL release modules ON |
| `wasm_build.yml` | every PR | emscripten build of `web/` on 3 OSes + `wasm_cross` |
| `build_eastl.yml` | every PR | EASTL shadow-config build + no-fileio build (linux clang) |
| `doc.yml` | only if `doc/**`, `daslib/**`, or `src/builtin/**` changed | six doc gates |
| `playground-e2e.yml` | only if `site/**` / `web/examples/ui/**` changed | Playwright on the web playground |

## build.yml — the build matrix

Per-lane steps: build → JIT prewarm → JIT test sweep → interpreter sweep →
`ctest -L small` → AOT sweep (Release lanes only).

| CI step | Local mirror | Notes |
|---|---|---|
| Interpreter sweep | `<daslang> dastest/dastest.das -- --color --failures-only --timeout 900 --test tests` | |
| JIT sweep | `<daslang> dastest/dastest.das -jit -- --jit-opt-level=3 --color --failures-only --timeout 900 --test tests` | Windows-local `clang-cl` link failures are env noise — the catchable class is LLVM verifier errors; full end-to-end JIT needs WSL/mac. See `skills/make_pr.md` §2.5 for the 2-test smoke version |
| Small C++ tests | `ctest --test-dir build --build-config Release -L small --output-on-failure` | drop `--build-config` on single-config generators. **Run this after touching `tests-cpp/`** — and remember MSVC tolerates C++ that clang/gcc reject (the doctest bit-field incident); see `skills/writing_cpp_tests.md` |
| AOT sweep | `cmake --build build --config Release --target test_aot`, then `bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --color --failures-only --timeout 900 --test tests` | Release lanes only in CI |
| Debug lanes | `cmake --build build --config Debug --target daslang`, then the sweep against `bin/Debug/daslang.exe` — safe in-checkout: Debug coexists with Release by design (`bin/Debug/`, `_debug.shared_module` suffix) | Debug bypasses the fused interpreter permutations — a fix that lands only in the fused path passes Release everywhere and trips Debug; conversely fused-path bugs need Release. If you touched `src/simulate/simulate_fusion_*`, run both configs |
| Sanitizer lanes (linux Release asan/tsan/ubsan) | WSL: `CC=clang CXX=clang++ cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_USE_SANITIZER=<asan\|tsan\|ubsan>`, then the JIT sweep on `tests/language` | not mirrorable on Windows/mac. CI applies LSan suppressions (`format_error`, `uriParseSingleUriA`, `uriMakeOwner`) — see the workflow's Test step |
| linux_arm / darwin lanes | mac: same commands as linux. From Windows: not mirrorable | ARM-specific reds (LLVM SelectionDAG, alignment) are CI-only signals |

## build.yml — bundle_smoke (linux)

Release-modules build → `cmake --install --prefix ./daslang_bundle --strip` →
`bash ci/smoke_test_bundle.sh ./daslang_bundle`. WSL-mirrorable verbatim; run
it when touching CMake `install(...)` rules, `ci/release_modules.txt`, or
module loading. This is the lane that catches install-layout regressions.

## build.yml — build_windows_mingw

msys2 CLANG64 build with dasClangBind + dasLLVM ON, full interp/JIT/AOT
sweeps, plus two things no other lane runs: the `bind_clangbind.das`
self-binder freshness check (`git diff --exit-code -- modules/dasClangBind/src/`)
and `test_const_preproc.das`. A local msys2 mirror is possible but rarely worth
it — the 80/20 for "compiles under MSVC, breaks under clang" is a clang
syntax-only pass on your changed C++ (see clang-cl below). If you regenerated
dasClangBind bindings, run the self-binder locally per `skills/clang_bind_build.md`.

## build.yml — build_windows_clangcl

The preflight `cpp-syntax` gate mirrors this lane's frontend: a clang-cl `/Zs`
pass (parse + semantic analysis + template instantiation — no codegen) on your
changed C++, escalating to ALL ~160 src+tests-cpp TUs (~15-30 s) when any core
header changed, since a header edit can break template instantiation in TUs
the diff never touched. What it cannot catch — link-stage divergence and
codegen-only issues — is rare, arrives batched in one CI log, and is cheap to
fix post-CI.

A full local mirror is possible but **destructive in the main checkout**: all
build dirs of one source tree share `bin/`, `lib/`, and the
`modules/<X>/*.shared_module` outputs (only Debug gets a `_debug` suffix), so
CI's verbatim configure+build below overwrites your MSVC Release binaries and
Release shared modules. Run it in a separate clone or worktree only:

```powershell
# SEPARATE clone/worktree only — clobbers bin/Release + Release .shared_modules otherwise
cmake -B build-clangcl -G "Visual Studio 17 2022" -A x64 -T ClangCL -DCMAKE_BUILD_TYPE=Release
cmake --build build-clangcl --config Release --parallel
```

Manual single-file check (what the gate automates):

```powershell
# from a VS dev prompt (clang-cl on PATH)
clang-cl /Zs /EHsc /std:c++17 -Iinclude -I3rdparty/fmt/include -I3rdparty/uriparser/include -Itests-cpp/3rdparty -Ibuild/include <changed>.cpp
```

(mac/WSL equivalent: `clang -fsyntax-only -std=c++17 ...`.) `-Ibuild/include`
matters — the configure-generated `modules/external_*.inc` headers live there,
and TUs like `src/simulate/fs_file_info.cpp` include them.

## extended_checks.yml

**The defining divergence: CI configures with ALL release modules ON** —
`ci/release_modules.txt` flips `DAS_HV/LLVM/AUDIO/PUGIXML/SQLITE/GLFW_DISABLED=OFF`.
A typical local dev build has several OFF, so module-gated `.das` and C++
(dasOpenGL helpers, dasHV-dependent daslib code) is compiled by CI and by
nothing on your machine. For any change to daslib generics or the type system,
mirror the configure:

```bash
cmake -B build -DDAS_HV_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF -DDAS_AUDIO_DISABLED=OFF \
  -DDAS_PUGIXML_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF -DDAS_GLFW_DISABLED=OFF
```

| CI step | Local mirror | Notes |
|---|---|---|
| dasgen freshness | `<daslang> dasgen/gen_bind.das` then `git diff --exit-code -- include/daScript/builtin/` | regen + commit if dirty; see `skills/visitor_gen_bind.md` |
| Run examples | `cmake --build build --config Release --target run_examples` | |
| Utils tests | `cmake --build build --config Release --target run_utils_tests` | |
| Tutorial dry-runs | `cmake --build build --config Release --target dry_run_tutorials` | catches compile rot in `tutorials/` — run after daslib API changes |
| Standalone exes | `cmake --build build --config Release --target all_utils_exe`, plus `<daslang> -exe -output bin/das-fmt utils/das-fmt/dasfmt.das` and `... bin/das-lint utils/lint/main.das` | `-exe` needs dasLLVM + lld-link on PATH |
| Sequence smoke | Windows: `pwsh examples/games/sequence/ci_smoke_test.ps1 "$(pwd)"`; linux/mac: `bash examples/games/sequence/ci_smoke_test.sh "$(pwd)"` | build the runtime modules first: `cmake --build build --config Release --target dasModuleGlfw dasModuleLiveHost dasModuleHV dasModuleAudio dasModulePUGIXML dasModuleStbImage`. **This is the only pre-merge lane that compiles GLFW-gated `.das` like dasOpenGL** — run it for type-system / daslib-generics changes |
| Formatter `--verify` | the pre-push hook runs it exactly (tracked files via `--files-from`); manual: `<daslang> utils/das-fmt/dasfmt.das -- --path ./ --verify` | name trap: a locally built `bin/Release/das-fmt.exe` is the **v1→v2 converter** (`utils/dasFormatter/`), not the formatter — CI overwrites its copy with an `-exe`-compiled `dasfmt.das` before verifying |
| Lint changed `.das` | the pre-push hook, or `git diff --name-only origin/master..HEAD -- '*.das' \| xargs <daslang> utils/lint/main.das -- --quiet` | zero warnings required |
| daslang_static sweep | `cmake --build build --config Release --target daslang_static`, then `bin/Release/daslang_static.exe dastest/dastest.das -- --color --failures-only --test tests` | rarely built locally; catches static-registration / no-dynamic-modules divergence |
| Ser/deser sweep | `<daslang> dastest/dastest.das -- --test tests --ser serialized.bin` then `... --deser serialized.bin` | run after touching AST serialization (`ast_serializer.cpp`, flag-bit additions) |
| MCP tools test | `<daslang> dastest/dastest.das -- --color --failures-only --test utils/mcp/test_tools.das` | linux-only in CI but runs anywhere; MCP signature changes break it silently — run after editing `utils/mcp/` |
| dasImgui install | `<daslang> utils/daspkg/main.das -- install dasImgui --branch master` | **the externals-coupling gate**: CI builds external dasImgui from ITS master against YOUR branch — an ABI break vs externals reds this on an unrelated-looking step. See `skills/abi_break_sweep.md` |
| Coverage | `<daslang> dastest/dastest.das -- --cov-path coverage.lcov --color --test tests/language --timeout 1800` + `dascov` | rarely needed locally |

## doc.yml — the six gates

Only triggered when `doc/**`, `daslib/**`, or `src/builtin/**` changed — but
`daslib/**` means **any daslib edit** runs all six. CI stops at the FIRST
das2rst panic, so one CI round can hide N-1 further issues — loop gate 1
locally until clean. Needs a daslang built with `DAS_HV_DISABLED=OFF` and
`DAS_PUGIXML_DISABLED=OFF` (das2rst documents those modules). Step-by-step
workflow: `skills/make_pr.md` §4; conventions: `skills/documentation_rst.md`.

| # | Gate | Local mirror |
|---|---|---|
| 1 | das2rst runs clean (positional handmade-doc validation panics on count mismatch) | `<daslang> doc/reflections/das2rst.das` — repeat until no panic |
| 2 | no `// stub` in handmade docs | `grep -rl '// stub' doc/source/stdlib/handmade/` → must be empty |
| 3 | no `Uncategorized` sections | `grep -rl '^Uncategorized$' doc/source/stdlib/generated/` → must be empty; fix via `group_by_regex` in das2rst.das |
| 4 | no untracked generated RST | `git ls-files --others --exclude-standard doc/source/stdlib/` → must be empty; `git add` the new files |
| 5 | LaTeX sphinx, warnings-as-errors | `sphinx-build -W --keep-going -b latex -d doc/sphinx-build doc/source build/latex` |
| 6 | HTML sphinx, warnings-as-errors | `sphinx-build -W --keep-going -b html -d doc/sphinx-build doc/source build/site` — delete `doc/sphinx-build` first; cached builds hide errors |

(The PDF compile after gate 6 is `continue_on_error` — not a gate.)

## wasm_build.yml

`wasm_build`: emsdk build of `web/` + a Node hello-world. `wasm_cross`:
cross-compiles utility mains to wasm32 via dasLLVM and runs them under
wasmtime, with emscripten **pinned to 5.0.3** (newer clang crashes on
`utils/dasFormatter/ds_parser.cpp` diagnostics). At risk when touching the
parser (`.ypp` → huge generated `.cpp`), `web/`, or the dasLLVM cross-compile
pipeline. Local mirror = emsdk in WSL following the workflow verbatim; for
most changes, let CI carry this lane.

## build_eastl.yml

Builds daslang against EASTL (`cmake/das_config_eastl/` shadow config) and a
second time with `DAS_NO_FILEIO=1`, where a shadow `<filesystem>` header
`#error`s on any stray include. At risk when adding `#include <filesystem>`
outside fio, new vsnprintf/locale dependencies, or touching
`cmake/das_config*`. WSL-mirrorable with the workflow's exact commands;
otherwise just keep `<filesystem>` includes inside the fio layer.
