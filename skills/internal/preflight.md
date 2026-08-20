# Preflight — CI lane ↔ local mirror

`daslang utils/internal/preflight/main.das` runs the fast tier: format, lint,
clang frontend pass on changed C++ (a full src+tests-cpp sweep when a header
changed). `-- --full` adds the untracked gate, dasgen freshness, the
CI-only-das compile sweep, the doc gates, ctest, the interp/JIT/AOT suites and
the sequence smoke. `--list-gates`; `--only <names>` / `--skip <names>` select
subsets. A gate whose host tool or module is missing reports `SKIP` with an
install/rebuild hint.

Two gates mirror no CI lane:

- **untracked** — `git ls-files --others --exclude-standard` must print nothing:
  commit, delete, or ignore each leftover (`.gitignore` when every clone mints
  it, `.git/info/exclude` for box-local keeps).
- **hash-refs** — a bare `#N` in `origin/master..HEAD` commit messages far below
  the repo's newest issue/PR number, which GitHub mislinks permanently once
  pushed (spelling: `skills/internal/make_pr.md` step 6). SKIPs without `gh`.

`--full` is Release-only and fails immediately on a Debug host. Debug serves
`--only`/`--skip` subset diagnosis only — never as a substitute when a Windows
MCP process locks the Release runtime DLL: stop the worktree's
`utils/mcp/main.das` host and rebuild Release (the MCP watcher restarts it).

`<daslang>` = `bin/Release/daslang.exe` (Windows MSVC multi-config),
`bin/daslang` (Ninja single-config — what CI's extended_checks uses on all three
OSes), or `build/daslang` (Make/Ninja without `EXECUTABLE_OUTPUT_PATH`).
Commands are platform-neutral unless marked. "WSL" = the verbatim-CI recipe in
`skills/internal/wsl_ci_repro.md` — fresh clone at the CI ref, never a
working-tree copy.

## What CI runs (per-PR + nightly)

| Workflow | Trigger | Jobs |
|---|---|---|
| `build.yml` (per-PR) | every PR commit (`pull_request`) + pushes to `master` | `build` matrix (5 targets × Debug/Release/RelWithDebInfo × sanitizers), `bundle_smoke`, `build_linux_gcc` |
| `build.yml` (nightly) | `schedule` cron (daily 02:00 UTC) | `build_windows_mingw` + `build_windows_clangcl` (gated OFF per-PR) **plus the full build matrix, whose Release cells (sanitizers included) run the full AOT sweep** ("Slow Release Tests"). Breaks surface within ~24 h, not at PR time |
| `nightly_imgui.yml` | `schedule` cron (daily 03:00 UTC) + `workflow_dispatch` | dasImgui playwright suite on ubuntu + macos — section below |
| `extended_checks.yml` | every PR | linux + darwin15-arm64 + windows, ALL release modules ON |
| `wasm_build.yml` | every PR | emscripten build of `web/` on 3 OSes + `wasm_cross` |
| `build_eastl.yml` | every PR | EASTL shadow-config build + no-fileio build (linux clang) |
| `doc.yml` | only if `doc/**`, `daslib/**`, `src/builtin/**`, `modules/dasImgui/**`, `modules/dasVulkan/**`, or `modules/dasLLAMA/dasllama/**` changed | the doc gates |
| `playground-e2e.yml` | only if `site/**` / `web/examples/ui/**` changed | Playwright on the web playground |

> A manual **`workflow_dispatch`** of `build.yml` runs the **whole** workflow — every per-PR job, both nightly toolchains, *and* the full AOT sweep. The cron `schedule` runs the two toolchains + the full build matrix; `bundle_smoke` and `build_linux_gcc` are gated off `schedule`.

## build.yml — the build matrix

Per-lane steps: build → JIT sweep (mints its own dll cache) → interpreter sweep
→ `ctest -L small`. Per-PR lanes also build `test_aot_subset` (tests/language,
part of ALL) as a compile+link gate, and run no AOT tests.

| CI step | Local mirror | Notes |
|---|---|---|
| Interpreter sweep | `<daslang> dastest/dastest.das -- --color --failures-only --max-file-time 30 --timeout 1800 --test tests` | fails if a completed test file exceeded 30 s |
| JIT sweep | `<daslang> dastest/dastest.das -jit -- --jit-opt-level=3 --color --failures-only --max-file-time 30 --isolated-mode --batch 4 --timeout 1800 --test tests` | Windows-local `clang-cl` link failures are env noise; the catchable class is LLVM verifier errors, end-to-end JIT needs WSL/mac. 2-test smoke: `skills/internal/make_pr.md` §2.5 |
| Small C++ tests | `ctest --test-dir build --build-config Release -L small --output-on-failure` | drop `--build-config` on single-config generators. **Run after touching `tests-cpp/`** — MSVC tolerates C++ clang/gcc reject; `skills/internal/writing_cpp_tests.md` |
| AOT sweep (full) | `cmake --build build --config Release --target test_aot`, then `bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --color --failures-only --max-file-time 30 --timeout 1800 --test tests` | nightly + manual dispatch only, so this is the **only** pre-push gate for AOT regressions outside tests/language — don't skip it |
| AOT subset gate | `cmake --build build --config Release --target test_aot_subset` (add `--target run_tests_aot_subset` to sweep tests/language too) | what per-PR lanes build |
| Debug lanes | `cmake --build build --config Debug --target daslang`, then the sweep against `bin/Debug/daslang.exe` — Debug coexists in-checkout with Release (`bin/Debug/`, `_debug.shared_module`) | Debug bypasses the fused interpreter permutations: a fused-path-only fix passes Release everywhere and trips Debug, and fused-path bugs need Release. Touched `src/simulate/simulate_fusion_*`? run both |
| Sanitizer lanes (linux Release asan/tsan/ubsan) | WSL: `CC=clang CXX=clang++ cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_USE_SANITIZER=<asan\|tsan\|ubsan>`, then the JIT sweep on `tests/language` | not mirrorable on Windows/mac. CI applies LSan suppressions (`format_error`, `uriParseSingleUriA`, `uriMakeOwner`) |
| linux_arm / darwin lanes | mac: same commands as linux; from Windows not mirrorable | ARM reds (LLVM SelectionDAG, alignment) are CI-only signals |

## build.yml — bundle_smoke (linux)

Release-modules build → `cmake --install --prefix ./daslang_bundle --strip` →
`bash ci/smoke_test_bundle.sh ./daslang_bundle`. WSL-mirrorable verbatim; the
install-layout gate — run it when touching CMake `install(...)` rules,
`ci/release_modules.txt`, or module loading.

## build.yml — build_windows_mingw (nightly)

msys2 CLANG64 build with dasClangBind + dasLLVM ON, full interp/JIT/AOT sweeps,
plus two things no other lane runs: the `bind_clangbind.das` self-binder
freshness check (`git diff --exit-code -- modules/dasClangBind/src/`) and
`test_const_preproc.das`. No msys2 mirror — use the clang-cl frontend pass
below. After regenerating dasClangBind bindings run the self-binder per
`skills/internal/clang_bind_build.md`.

## build.yml — build_windows_clangcl (nightly)

preflight's `cpp-syntax` gate mirrors this lane's frontend: clang-cl `/Zs`
(parse + semantic analysis + template instantiation, no codegen) on changed C++,
escalating to ALL ~160 src+tests-cpp TUs (~15-30 s) when a core header changed
(a header edit breaks instantiation in untouched TUs). It catches neither
link-stage nor codegen-only divergence.

A full local mirror is **destructive in the main checkout**: all build dirs of
one source tree share `bin/`, `lib/`, and `modules/<X>/*.shared_module` (only
Debug gets a `_debug` suffix), so the configure+build below overwrites your MSVC
Release binaries and shared modules. Separate clone/worktree only:

```powershell
# CI drives clang-cl through Ninja, NOT the VS generator / "ClangCL" toolset (spurious MSB8066 -1 reds)
cmake -B build-clangcl -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl
cmake --build build-clangcl --parallel
```

Single file, from a VS dev prompt with clang-cl on PATH
(mac/WSL: `clang -fsyntax-only -std=c++17 ...`):

```powershell
clang-cl /Zs /EHsc /std:c++17 -Iinclude -I3rdparty/fmt/include -I3rdparty/uriparser/include -Itests-cpp/3rdparty -Ibuild/include <changed>.cpp
```

`-Ibuild/include` matters — the configure-generated `modules/external_*.inc`
headers live there, and TUs like `src/simulate/fs_file_info.cpp` include them.

## extended_checks.yml

**CI configures with ALL release modules ON** — `ci/release_modules.txt` flips
`DAS_HV/LLVM/AUDIO/PUGIXML/SQLITE/GLFW_DISABLED=OFF`. A local build with several
OFF compiles none of the module-gated `.das` and C++ (dasOpenGL helpers,
dasHV-dependent daslib) that CI does. Mirror the configure for any change to
daslib generics or the type system:

```bash
cmake -B build -DDAS_HV_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF -DDAS_AUDIO_DISABLED=OFF \
  -DDAS_PUGIXML_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF -DDAS_GLFW_DISABLED=OFF
```

| CI step | Local mirror | Notes |
|---|---|---|
| dasgen freshness | `<daslang> utils/internal/dasgen/gen_bind.das` then `git diff --exit-code -- include/daScript/builtin/` | regen + commit if dirty; `skills/internal/visitor_gen_bind.md` |
| Run examples | `cmake --build build --config Release --target run_examples` | |
| Utils tests | `cmake --build build --config Release --target run_utils_tests` | |
| Tutorial dry-runs | `cmake --build build --config Release --target dry_run_tutorials` | compile rot in `tutorials/` after daslib API changes |
| Standalone exes | `cmake --build build --config Release --target all_utils_exe`, plus `<daslang> -exe -output bin/das-fmt utils/das-fmt/dasfmt.das` and `... bin/das-lint utils/lint/main.das` | `-exe` needs dasLLVM + lld-link on PATH |
| Sequence smoke | Windows: `pwsh examples/games/sequence/ci_smoke_test.ps1 "$(pwd)"`; linux/mac: `bash examples/games/sequence/ci_smoke_test.sh "$(pwd)"` | build the runtime modules first: `cmake --build build --config Release --target dasModuleGlfw dasModuleLiveHost dasModuleHV dasModuleAudio dasModulePUGIXML dasModuleStbImage`. **The only pre-merge lane compiling GLFW-gated `.das` like dasOpenGL** — run it for type-system / daslib-generics changes |
| Formatter `--verify` | preflight's `format` gate runs it exactly (tracked files via `--files-from`); manual: `<daslang> utils/das-fmt/dasfmt.das -- --path ./ --verify` | CI's second verify pass uses an `-exe`-compiled `bin/das-fmt.exe` |
| Lint changed `.das` | preflight's `lint` gate — THREE rails: host-flavor interp, the LINUX-lane mirror (`--disable-module dasMetal` — CI's verdict comes only from linux, where dasMetal's platform `static_if` halves compile out so its requires/args read unused there and nowhere else; dasVulkan is in-tree on linux, so NOT disabled), and the `-exe`-compiled `das-lint` (different module resolution). Single rail: `git diff --name-only origin/master..HEAD -- '*.das' \| xargs <daslang> utils/lint/main.das -- --quiet` | zero warnings on ALL rails; a mirror-only STYLE030/LINT012 takes the both-worlds `nolint:...,LINT019` spelling |
| ast-verify changed `.das` | preflight's `ast-verify` gate — `<daslang> -dry-run --ast-verify-batch <file>` per changed `.das` plus the `tests/linq/test_linq_fold.das` qmacro canary, parallel, 300 s per-file timeout, skipping `cant_`/`failed_`/`invalid_` and `utils/internal/ast-fuzz/selftest/`. An `AST verify` line, crash or timeout fails; a compile error belongs to whoever owns the file; a file inside the verifier's own require closure (`daslib/ast*.das`, `daslib/rtti.das`, `daslib/strings_boost.das` — `error[20510]` under the force-include) is reported *not verifiable*, never clean | mirrors the workflow's "Run ast-verify on changed .das files". Batch mode is the ruled gate form (`skills/das_macros.md`); with no pre-infer walk, a tree a macro breaks mid-inference surfaces as a compiler crash instead of a located report — hence crash = red, and plain `--ast-verify` on that file locates it. Each item is a whole-engine compile (2-3× a plain one). Width is physical cores halved; `-j` only lowers it |
| REVIEW.das gates | `<daslang> utils/internal/review-md/all.das` | every `REVIEW.das` in the tree, fail-fix; also run per-diff in the make_pr step-0a walk |
| dastest own suite | `<daslang> dastest/dastest.das -- --failures-only --test dastest/tests` | framework suite + `review_gate` library tests; whole-directory, so a new file needs no CI row |
| daslang_static sweep | `cmake --build build --config Release --target daslang_static`, then `bin/Release/daslang_static.exe dastest/dastest.das -- --color --failures-only --test tests` | catches static-registration / no-dynamic-modules divergence |
| Ser/deser sweep | `<daslang> dastest/dastest.das -- --test tests --ser serialized.bin` then `... --deser serialized.bin` | after touching AST serialization (`ast_serializer.cpp`, flag-bit additions) |
| AST verify tree sweep — **not a PR gate** (the per-PR arm is the row above) | `find tests -name '*.das' ! -name 'cant_*' ! -name 'failed_*' ! -name 'invalid_*' -print0 \| xargs -0 -P8 -n1 timeout 120 <daslang> --ast-verify-batch -compile-only` — an `AST verify` line is a failure; compile errors are expected (many tests assert one). This one-liner attributes neither a crash (`CRASH:` banner) nor a timeout (rc 124) to its file — for those copy the step's `/tmp/ast_verify_one.sh` helper out of the workflow | runs on `extended_checks.yml`'s 04:00 cron: one daslang process per test file, each re-parsing daslib. Force it early with `gh workflow run extended_checks.yml`. Run locally after touching macro or AST-building code — `skills/das_macros.md` |
| Authored-doc code blocks — **not a PR gate** | `<daslang> utils/internal/doc-verify/main.das` (exit 0 = every authored RST page's das blocks compile; report at `build/doc_verify/report.json`) | nightly cron + `workflow_dispatch`, posix cells only: ~35 min, one daslang spawn per page. Run locally after editing `doc/source/reference/**` or `doc/source/stdlib/handmade/**`, or after daslib/module API changes docs quote — `skills/internal/doc_sweep.md` |
| MCP tools test | `<daslang> dastest/dastest.das -- --color --failures-only --test utils/mcp/test_tools.das` | linux-only in CI, runs anywhere; MCP signature changes break it silently — run after editing `utils/mcp/` |
| dasImgui build | nothing to install — dasImgui is in-tree (`modules/dasImgui`), built like any default-ON module | external ABI canaries (dasImguiImplot, dasImguiNodeEditor + the rest of the daspkg-index) run in `nightly_daspkg_index.yml`; `skills/internal/abi_break_sweep.md` |
| Coverage | `<daslang> dastest/dastest.das -- --cov-path coverage.lcov --color --test tests/language --timeout 1800` + `dascov` | |

## doc.yml — the gates

**Any** daslib edit runs them all. Needs a daslang built with
`DAS_HV_DISABLED=OFF` and `DAS_PUGIXML_DISABLED=OFF` (das2rst documents those
modules). CI stops at the FIRST das2rst panic, so one CI round can hide N-1
further issues — loop gate 1 locally until clean. Workflow:
`skills/internal/make_pr.md` §4; conventions:
`skills/internal/documentation_rst.md`.

| # | Gate | Local mirror |
|---|---|---|
| 1 | das2rst runs clean (positional handmade-doc validation panics on count mismatch) | `<daslang> -documentation doc/reflections/das2rst.das` — repeat until no panic; the host policy keeps per-box transforms inert |
| 2 | imgui2rst regenerates clean | `<daslang> modules/dasImgui/utils/imgui2rst.das` |
| 3 | vulkan2rst regenerates clean | `<daslang> modules/dasVulkan/utils/vulkan2rst.das` |
| 4 | no `// stub` in handmade docs | `grep -rl '// stub' doc/source/stdlib/handmade/` → must be empty |
| 5 | no `Uncategorized` sections | `grep -rl '^Uncategorized$' doc/source/stdlib/generated/` → must be empty; fix via `group_by_regex` in das2rst.das |
| 6 | no untracked generated RST | `git ls-files --others --exclude-standard doc/source/stdlib/` → must be empty; `git add` the new files |
| 7 | HTML sphinx, warnings-as-errors | `sphinx-build -W --keep-going -b html -d doc/sphinx-build doc/source build/site` — delete `doc/sphinx-build` first; cached builds hide errors |

preflight finds sphinx-build on PATH, then in `~/Library/Python/*/bin` +
`~/.local/bin`, and stages the docs-assets release once per tree: a `.. video::`
whose recording is missing from `doc/source/_static/tutorials/` is a `-W`
warning like any other, and the gate detail says so when staging failed and the
build then went red.

## wasm_build.yml

`wasm_build`: emsdk build of `web/` + a Node hello-world. `wasm_cross`:
cross-compiles utility mains to wasm32 via dasLLVM and runs them under wasmtime,
emscripten **pinned to 5.0.3** (newer clang crashes on
`utils/gen1-to-gen2/ds_parser.cpp` diagnostics). Mirror = emsdk in WSL following
the workflow verbatim; for most changes let CI carry the lane.

## build_eastl.yml

Builds daslang against EASTL (`cmake/das_config_eastl/` shadow config) and again
with `DAS_NO_FILEIO=1`, where a shadow `<filesystem>` header `#error`s on any
stray include. WSL-mirrorable with the workflow's exact commands; otherwise keep
`<filesystem>` includes inside the fio layer.

## nightly_imgui.yml

Each of the ~151 tests spawns a `daslang-live` subprocess hosting a feature app
from `modules/dasImgui/examples/` and drives it over the HTTP live API. The
`modules/dasImgui/tests/.das_test` gate keeps the directory out of per-PR
`--test tests/` sweeps; targeting the folder directly (nightly, and the mirror
below) bypasses the gate. Dispatch `nightly_imgui.yml` itself to exercise an
imgui-touching branch.

Local mirror — also `preflight --full`'s `imgui` gate, the suite's only pre-push
check:

```bash
cmake --build build --config Release --target daslang daslang-live
bin/Release/daslang dastest/dastest.das -- --test modules/dasImgui/tests --headless \
  --isolated-mode --isolated-mode-threads 4 --timeout 600 \
  --exclude glfw_synth --exclude key_hud --exclude embedded_terminal
```
