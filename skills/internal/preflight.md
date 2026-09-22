# Preflight - CI lane <-> local mirror

`daslang utils/internal/preflight/main.das` runs the **fast tier**: format, lint,
ast-verify, cpp-syntax, review-md, review-md-tests, md-ascii, hash-refs, untracked, dasgen, ci-das,
ci-matrix (`python3 ci/test_ci_matrix.py`, when the diff touches `.github/` or `ci/`)
and compile-sweep (every program root under `utils/`, `examples/`, `tutorials/`
and the modules' examples and utils, compile-only, in parallel - the per-PR form
of CI's examples and tutorial runs), serially, and a red stops the run. `-- --full`
then runs the **lanes** at once - docs, tests-cpp, tests-interp, tests-jit,
tests-aot, utils-tests - so the wall is the longest lane (`--serial` for
diagnosis). **Module gates** never run from a tier: `--only imgui`, `--only
sequence`, `--only dasllama-model-free` when the module is the work. A gate
with a **reach set** skips, with the reason, when nothing under its paths or the
core (`src/`, `include/`, `daslib/`, `dastest/`, `CMakeLists.txt`, `cmake/`)
changed; `--only` runs a gate whatever changed. `--list-gates` prints tier, reach
and description; `--skip <names>` drops gates. A gate whose host tool or module is missing
reports `SKIP` with an install/rebuild hint. The budget the tiers serve: a full
run fits 20 minutes on the M5 box, or the gate is not in preflight.

Each gate line carries its breakdown indented underneath, on PASS as well as FAIL: the
build/run split for a gate that builds before it sweeps (`tests-aot`, `sequence`, `imgui`),
and dastest's `Top 10 slowest files` table for a suite sweep (preflight passes
`--timing-outliers 10`). The run closes with a time-by-gate table, largest first, so the gate
holding the wall clock names itself.

These gates mirror no CI lane:

- **dasllama-model-free / dasllama-stocked** - `<daslang> -jit modules/dasLLAMA/tests/run.das
  -- --suite model-free` and `--suite stocked` (the module's per-PR gates,
  `modules/dasLLAMA/tests/CLAUDE.md`), module tier: no tier runs them, `--only
  dasllama-model-free` / `--only dasllama-stocked` does, one at a time (two dasLLAMA suites
  never share a box). `model-free` is tens of minutes; `stocked` is tens of minutes on a box
  with models and a run of skips without them. On a failure (or under `--verbose`) the gate
  prints the runner's output; each file's log path is on its `DONE` line.
- **untracked** - `git ls-files --others --exclude-standard` must print nothing:
  commit, delete, or ignore each leftover (`.gitignore` when every clone mints
  it, `.git/info/exclude` for box-local keeps).
- **hash-refs** - a bare `#N` in `origin/master..HEAD` commit messages far below
  the repo's newest issue/PR number, which GitHub mislinks permanently once
  pushed (spelling: `skills/internal/make_pr.md` step 6). SKIPs without `gh`.

**review-md** - the `REVIEW.das` gates of every folder the diff touches, the walk
`utils/internal/review-md/main.das` runs. CI's extended_checks `Run REVIEW.das gates` step runs
every gate in the tree; this gate is that lane's pre-push half, scoped to the diff.

`--full` is Release-only and fails immediately on a Debug host. Debug serves
`--only`/`--skip` subset diagnosis only - never as a substitute when a Windows
MCP process locks the Release runtime DLL: stop the worktree's
`utils/mcp/main.das` host and rebuild Release (the MCP watcher restarts it).

`<daslang>` = `bin/Release/daslang.exe` (Windows MSVC multi-config),
`bin/daslang` (Ninja single-config - what CI's extended_checks uses on all three
OSes), or `build/daslang` (Make/Ninja without `EXECUTABLE_OUTPUT_PATH`).
Commands are platform-neutral unless marked. "WSL" = the verbatim-CI recipe in
`skills/internal/wsl_ci_repro.md` - fresh clone at the CI ref, never a
working-tree copy.

## What CI runs (per-PR + nightly)

| Workflow | Trigger | Jobs |
|---|---|---|
| `build.yml` (per-PR) | every PR commit (`pull_request`) + pushes to `master` | `build` matrix (`ci/ci_matrix.py build`: Debug + Release on linux, linux_arm, darwin15, darwin26; windows 32 Release, windows 64 Release), `bundle_smoke`, `build_linux_gcc` |
| `build.yml` (nightly) | `schedule` cron (daily 02:00 UTC) | `build_windows_mingw` + `build_windows_clangcl` (gated OFF per-PR) **plus the full build matrix - the per-PR cells, the sanitizer cells (linux Release asan/tsan/ubsan), the fast-math cell (linux Release `-DDAS_FAST_MATH=ON`) and windows 64 Debug - whose Release cells run the full AOT sweep** ("Slow Release Tests") **and the three backend sweeps** ("Nightly Backend Sweeps"). Breaks surface within ~24 h, not at PR time |
| `nightly_imgui.yml` | `schedule` cron (daily 03:00 UTC) + `workflow_dispatch` | dasImgui playwright suite on ubuntu + macos - section below |
| `extended_checks.yml` (per-PR) | every PR | two darwin15-arm64 jobs, `core` and `modules` (`ci/ci_matrix.py extended`), ALL release modules ON - section below |
| `extended_checks.yml` (nightly) | `schedule` cron (daily 04:00 UTC) + `workflow_dispatch` | one job each on linux, darwin15 and windows running every step (role `all`), including the ones too slow for a PR: tutorial dry-runs, the run form of examples, coverage, the nano cross-compile, the AST verify tree sweep, doc-verify |
| `codeql.yml` | every PR and `master` push touching `src/`, `include/`, `modules/`, `tests-cpp/` + a weekly cron | CodeQL over the C++ surface, ~20 min on a PR; no local mirror, so it stays per PR |
| `wasm_build.yml` | every PR | emscripten build of `web/` on 3 OSes, the vecmath backend battery and `tests/language` under node, + `wasm_cross` |
| `build_eastl.yml` | every PR | EASTL shadow-config build + no-fileio build (linux clang) |
| `doc.yml` | only if `doc/**`, `daslib/**`, `src/builtin/**`, `modules/dasImgui/**`, `modules/dasVulkan/**`, or `modules/dasLLAMA/dasllama/**` changed | the doc gates |
| `playground-e2e.yml` | only if `site/**` / `web/examples/ui/**` changed | Playwright on the web playground |
| `dasweb-verify-browser.yml` | `pull_request` touching `utils/internal/dasweb-verify/browser/**`, `web/examples/ui/samples/data.json`, or the workflow file itself; `workflow_dispatch` | `node_test`: `node --test` in `utils/internal/dasweb-verify/browser` - section below |
| `dasllama_server_release.yml` | `release: prereleased`, `workflow_dispatch` (`publish` input), and a branch push that edits the file itself or what the bundle carries (`utils/dasllama-server/**`, `utils/watchdog/**`, `utils/daspkg/**`, `daslib/daspkg.das`, `modules/dasHV/**`, `modules/dasLLAMA/benchmarks/lcpp_bench.das`, `modules/dasLLAMA/dasllama/dasllama_bench.das`; `.md` edits excepted) | four cells (linux x86_64 and arm64 on ubuntu-22.04, darwin arm64, windows x64): daslang with the release modules, `daspkg release --fat x86-avx2 \| arm-neon` of `utils/dasllama-server` (the server, the watchdog, the `dasllama-bench` companion), smoke, package; a release or a `publish` dispatch uploads to the rolling `dasllama-server` release (and the daslang release being cut). Local mirror: `bin/daslang utils/daspkg/main.das -- release --fat <class> --root utils/dasllama-server --out <dir>` on the box, then the smoke by hand - the server on a spare port answering `/v1/stats` in setup mode, `dasllama-bench -m none.gguf --help` exiting 0 |
| `release.yml` | `release: prereleased` and `workflow_dispatch` (build + smoke; publishes nothing) | four cells (linux x86_64, linux arm64, darwin26 arm64, windows x86_64): build, the test suite under `-jit`, bundle + smoke, `.deb` / `.rpm` / pip wheel, sha256 per asset, the `.rpm` and wheel smokes, upload; then `pypi_route` + `publish_pypi` by tag shape - section below |
| `nightly_issue.yml` | `workflow_call`, from every lane with a cron (`build.yml`, `extended_checks.yml`, `codeql.yml`, `nightly_daspkg_index.yml`, `nightly_imgui.yml`, `nightly_lint.yml`, `nightly_playground.yml`, `nightly_vulkan.yml`) once a `schedule` run of that lane has a failed job | keeps ONE open `nightly-failure` issue for the whole nightly, titled `The nightly is red`: the first red lane files it, every lane after that files nothing while it is open. Close the issue when the nightly is green. No local mirror |

> A manual **`workflow_dispatch`** of `build.yml` runs the **whole** workflow - every per-PR job, both nightly toolchains, *and* the full AOT sweep. The cron `schedule` runs the two toolchains, the full build matrix and `bundle_smoke` (the cron run is what seeds its sccache slot); `build_linux_gcc` is gated off `schedule`.

## build.yml - the build matrix

Per-lane steps: build -> JIT sweep (mints its own dll cache) -> interpreter sweep
-> `ctest -L small`. Per-PR lanes also build `test_aot_subset` (tests/language,
part of ALL) as a compile+link gate, and run no AOT tests.

**The build matrix runs four targets: `run_tests_sweeps`, `test-small` and, on the
nightly, `run_tests_slow` and `run_backend_sweeps`.** Which sweeps `run_tests_sweeps` runs is
decided in cmake, not in the workflow: no JIT where dasLLVM is off (the sweep targets live under
`if(NOT DAS_LLVM_DISABLED)`) or on Debug; no interpreter sweep under a sanitizer.

**The configure is one command for every cell.** The flags a cell varies by value -
`CMAKE_BUILD_TYPE`, `DAS_LLVM_DISABLED`, `DAS_USE_SANITIZER`, `DAS_FAST_MATH` - are on that
line; the rest is `matrix.cmake_extra`, one string per cell in `ci/ci_matrix.py`: the compiler
on posix, the arm module flags, the darwin architecture, and the sccache launchers plus the
vcpkg toolchain on windows. Mirror a cell locally by pasting its `cmake_extra` into your own
configure.

| CI step | Local mirror | Notes |
|---|---|---|
| Interpreter sweep | `cmake --build build --config Release --target run_tests_sweeps` - the whole per-PR sweep, JIT then interpreter, each retried isolated; alone: `cmake --build build --config Release --target run_tests_interpreter` | fails if a completed test file exceeded 30 s |
| JIT sweep | `cmake --build build --config Release --target run_tests_jit` (`run_tests_sweeps` runs it first, then the interpreter sweep) | Windows-local `clang-cl` link failures are env noise; the catchable class is LLVM verifier errors, end-to-end JIT needs WSL/mac. 2-test smoke: `skills/internal/make_pr.md` sec.2.5 |
| Small C++ tests | `cmake --build build --config Release --target test-small` | drop `--build-config` on single-config generators. **Run after touching `tests-cpp/`** - MSVC tolerates C++ clang/gcc reject; `skills/internal/writing_cpp_tests.md` |
| AOT sweep (full) | `cmake --build build --config Release --target run_tests_slow` - the full `test_aot` build, the AOT sweep over tests/, then the big C++ tests | nightly + manual dispatch only, so this is the **only** pre-push gate for AOT regressions outside tests/language - don't skip it |
| AOT subset gate | `cmake --build build --config Release --target test_aot_subset` (add `--target run_tests_aot_subset` to sweep tests/language too) | what per-PR lanes build |
| LLVM-AOT sweep | `cmake --build build --config Release --target run_backend_sweeps` runs all three; alone: `cmake --build build --config Release --target run_tests_llvm_aot` | nightly + dispatch. Emits a native `.o` per test through the LLVM backend instead of the C++ proxy, then sweeps tests/ against them |
| Standalone sweep - C++ (`-ctx`) | `cmake --build build --config Release --target run_backend_sweeps`; alone: `cmake --build build --config Release --target run_standalone_sweep_aot` | nightly + dispatch. Emits, compiles, links and launches a standalone context per file of test_aot's corpus. `-DDAS_STANDALONE_SWEEP_FILTER=<regex>` narrows it |
| Standalone sweep - LLVM (`-lib`) | `cmake --build build --config Release --target run_backend_sweeps`; alone: `cmake --build build --config Release --target standalone_sweep_jit` | nightly + dispatch. Same corpus through `daslang -lib`: emits each as a native library, loads it back through its generated bindings and drives it. Needs LLVM |
| Debug lanes | `cmake --build build --config Debug --target daslang`, then the sweep against `bin/Debug/daslang.exe` - Debug coexists in-checkout with Release (`bin/Debug/`, `_debug.shared_module`) | Debug bypasses the fused interpreter permutations: a fused-path-only fix passes Release everywhere and trips Debug, and fused-path bugs need Release. Touched `src/simulate/simulate_fusion_*`? run both |
| Fast-math lane (linux Release, `-ffast-math`) - nightly | Linux/WSL: `CC=gcc CXX=g++ cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_FAST_MATH=ON`, then `--target run_tests_interpreter` and `ctest -L small` | the tree gets the host's relaxed-float flag, the way an embedder passing `-ffast-math` builds it (dagor does). gcc, not clang: clang's `-Wnan-infinity-disabled` fires on every inf literal the tree and its vendored third-party spell, and neither is ours to patch. Vector-vs-scalar bit equality legitimately diverges here - gate such an arm on the `HOST_FAST_MATH` constant and skip. Nightly + dispatch only |
| Sanitizer lanes (linux Release asan/tsan/ubsan) - nightly | WSL: `CC=clang CXX=clang++ cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_USE_SANITIZER=<asan\|tsan\|ubsan>`, then the JIT sweep on `tests/language` | not mirrorable on Windows/mac. CI applies LSan suppressions (`format_error`, `uriParseSingleUriA`, `uriMakeOwner`). Nightly + dispatch only (40-55 minute jobs); force one early with `gh workflow run build.yml` |
| linux_arm / darwin lanes | mac: same commands as linux; from Windows not mirrorable | ARM reds (LLVM SelectionDAG, alignment) are CI-only signals |

## build.yml - bundle_smoke (linux)

Release-modules build -> `cmake --install --prefix ./daslang_bundle --strip` ->
`bash ci/smoke_test_bundle.sh ./daslang_bundle`. WSL-mirrorable verbatim; the
install-layout gate - run it when touching CMake `install(...)` rules,
`ci/release_modules.txt`, or module loading.

## build.yml - build_windows_mingw (nightly)

msys2 CLANG64 build with dasClangBind + dasLLVM ON, full interp/JIT/AOT sweeps,
plus two things no other lane runs: the `bind_clangbind.das` self-binder
freshness check (`git diff --exit-code -- modules/dasClangBind/src/`) and
`test_const_preproc.das`. No msys2 mirror - use the clang-cl frontend pass
below. After regenerating dasClangBind bindings run the self-binder per
`skills/internal/clang_bind_build.md`.

## build.yml - build_windows_clangcl (nightly)

preflight's `cpp-syntax` gate mirrors this lane's frontend: clang-cl `/Zs`
(parse + semantic analysis + template instantiation, no codegen) on changed C++,
escalating to every tracked `src` + `tests-cpp` TU (~15-30 s) when a core header changed
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

`-Ibuild/include` matters - the configure-generated `modules/external_*.inc`
headers live there, and TUs like `src/simulate/fs_file_info.cpp` include them.

## extended_checks.yml

Per PR the workflow is two darwin15 jobs, `extended_checks (darwin15, core)` and
`extended_checks (darwin15, modules)`, each inside the 35-minute budget: `core` runs the
tree's own gates - dasgen, utils tests, standalone exes, formatter, lint, ast-verify, the
python gates, review-md discovery, dastest's own suite, the REVIEW.das gates, ci-das -
and `modules` the module and service suites - ser/deser, MCP tools, boulder-dash,
dasllama-server, env-knob registries, the dasLLVM vector-math rail, facade lint,
dasweb-playground, dasllama-ladder, dasweb-buildd, dasweb-verify, the sequence smoke, the
dasGlfw touch layer, the daslang_static sweep. The role names a group target, so the nightly job
(role `all`, one each on linux, darwin15, windows) runs every check plus the
nightly-only ones: tutorial dry-runs and the run form of examples (preflight's `compile-sweep`
is their per-PR mirror), the AST verify tree sweep and doc-verify (policy), coverage (a report,
not a gate), and the nano cross-compile - platform-bound: its arm-none-eabi toolchain is an apt
package no darwin cell and no developer box carries, so it has no per-PR cell and no local
mirror. The cells are `ci/ci_matrix.py extended <event>`; `ci/test_ci_matrix.py` pins them and
the condition spelling.

**CI configures with ALL release modules ON** - `ci/release_modules.txt` flips
`DAS_HV/LLVM/AUDIO/PUGIXML/SQLITE/GLFW_DISABLED=OFF`. A local build with several
OFF compiles none of the module-gated `.das` and C++ (dasOpenGL helpers,
dasHV-dependent daslib) that CI does. Mirror the configure for any change to
daslib generics or the type system:

```bash
cmake -B build -DDAS_HV_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF -DDAS_AUDIO_DISABLED=OFF \
  -DDAS_PUGIXML_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF -DDAS_GLFW_DISABLED=OFF
```

**`extended_checks.yml` runs one target per job:** `run_extended_${{ matrix.role }}`, so
`run_extended_core` and `run_extended_modules` per PR and `run_extended_all` on the nightly.
`ci/CMakeLists.txt` holds the three membership lists and defines every check as its own target,
so the local mirror is either the group or a single check by name. A platform that does not
define a member drops it, and the configure prints which. A new check lands as a target there
and joins a list; `ci/test_ci_matrix.py` pins all three sets, so one that joins none is a red
test rather than a check that silently stopped running.

| CI step | Local mirror | Notes |
|---|---|---|
| Markdown ASCII gate | preflight's `md-ascii` gate (fast tier, runs when the diff touches any `.md`); the CI target is `cmake --build build --config Release --target check_md_ascii`, fix in place with `python3 ci/fix_md_ascii.py` | em-dashes/arrows/ellipses in new markdown are the usual trip |
| dasgen freshness | `<daslang> utils/internal/dasgen/gen_bind.das` then `git diff --exit-code -- include/daScript/builtin/` | regen + commit if dirty; `skills/internal/visitor_gen_bind.md` |
| Run examples - **nightly** | `cmake --build build --config Release --target run_examples`; per PR, preflight's `compile-sweep` gate compiles every example root | the run form is 5-8 minutes a lane |
| Utils tests | `cmake --build build --config Release --target run_utils_tests` - preflight's `utils-tests` lane | |
| Tutorial dry-runs - **nightly** | `cmake --build build --config Release --target dry_run_tutorials`; per PR, preflight's `compile-sweep` gate compiles every tutorial root | compile rot in `tutorials/` after daslib API changes; the run form is 8-11 minutes a lane |
| Standalone exes | `cmake --build build --config Release --target check_standalone_exes` | `-exe` needs dasLLVM + lld-link on PATH; das-lint is built, not run - an exe carries the host's module paths without the modules, so its lint world is not one in-tree files answer to; the shipped `lint.exe` is exercised by the bundle smoke test on daslib |
| Sequence smoke | `cmake --build build --config Release --target run_sequence_smoke` | build the runtime modules first: `cmake --build build --config Release --target dasModuleGlfw dasModuleLiveHost dasModuleHV dasModuleAudio dasModulePUGIXML dasModuleStbImage`. **The only pre-merge lane compiling GLFW-gated `.das` like dasOpenGL** - run it for type-system / daslib-generics changes |
| Formatter `--verify` | preflight's `format` gate runs it exactly (tracked files via `--files-from`); the CI targets are `cmake --build build --config Release --target check_format` then `check_format_exe` | CI's second verify pass uses an `-exe`-compiled `bin/das-fmt.exe`; the mask skips generated `.das` under the build dir (nightly doc-verify extracts RST snippets there) |
| Lint changed `.das` and `.md` | preflight's `lint` gate - TWO rails: host-flavor interp, then the LINUX-lane mirror (`--disable-module dasMetal` - CI's verdict comes only from linux, where dasMetal's platform `static_if` halves compile out so its requires/args read unused there and nowhere else; dasVulkan is in-tree on linux, so NOT disabled). The CI target is `DAS_CI_BASE_REF=<base> cmake --build build --config Release --target check_lint_changed` | zero warnings on BOTH rails; a mirror-only STYLE030/LINT012 takes the both-worlds `nolint:...,LINT019` spelling |
| ast-verify changed `.das` | preflight's `ast-verify` gate; the CI target is `DAS_CI_BASE_REF=<base> cmake --build build --config Release --target check_ast_verify_changed` - `-dry-run --ast-verify-batch` per changed `.das` plus the `tests/linq/test_linq_fold.das` qmacro canary, 300 s per file, skipping `cant_`/`failed_`/`invalid_` and `utils/internal/ast-fuzz/selftest/`. An `AST verify` line, crash or timeout fails; a compile error belongs to whoever owns the file; a file inside the verifier's own require closure (`daslib/ast*.das`, `daslib/rtti.das`, `daslib/strings_boost.das` - `error[20510]` under the force-include) is reported *not verifiable*, never clean | mirrors the workflow's "Run ast-verify on changed .das files". Batch mode is the ruled gate form (`skills/das_macros.md`); with no pre-infer walk, a tree a macro breaks mid-inference surfaces as a compiler crash instead of a located report - hence crash = red, and plain `--ast-verify` on that file locates it. Each item is a whole-engine compile (2-3x a plain one). Width is physical cores halved; `-j` only lowers it |
| REVIEW.das gates | `cmake --build build --config Release --target check_review_gates` | every `REVIEW.das` in the tree, fail-fix; also run per-diff in the make_pr step-0a walk |
| review-md discovery | the CI target is `cmake --build build --config Release --target run_tests_review_md` | `test_walkers.das` copies `utils/REVIEW.das` into a planted fixture, so a gate that starts reading a new file is red only there - the fixture grows with the gate |
| dastest own suite | `cmake --build build --config Release --target run_tests_dastest` | framework suite + `review_gate` library tests; whole-directory, so a new file needs no CI row |
| daslang_static sweep | `cmake --build build --config Release --target run_tests_static` | the `modules` role; catches static-registration / no-dynamic-modules divergence, which no preflight gate mirrors |
| CI matrix test | preflight's `ci-matrix` gate (fast tier, reach `.github/` and `ci/`); the CI target is `cmake --build build --config Release --target check_workflow_matrix` | the per-event cells of `ci/ci_matrix.py` and the `matrix.role != '<other>'` spelling of every `extended_checks.yml` step condition |
| Ser/deser sweep | `cmake --build build --config Release --target run_tests_serdeser` | after touching AST serialization (`ast_serializer.cpp`, flag-bit additions) |
| AST verify tree sweep - **not a PR gate** (the per-PR arm is the row above) | `cmake --build build --config Release --target check_ast_verify_tree` - one daslang per file, 120 s each; an `AST verify` line, a crash or a timeout fails, compile errors are expected (many tests assert one) | runs on `extended_checks.yml`'s 04:00 cron: one daslang process per test file, each re-parsing daslib. Force it early with `gh workflow run extended_checks.yml`. Run locally after touching macro or AST-building code - `skills/das_macros.md` |
| Authored-doc code blocks - **not a PR gate** | `cmake --build build --config Release --target check_doc_verify` (exit 0 = every authored RST page's das blocks compile; report at `build/doc_verify/report.json`) | nightly cron + `workflow_dispatch`, posix cells only: ~35 min, one daslang spawn per page. Run locally after editing `doc/source/reference/**` or `doc/source/stdlib/handmade/**`, or after daslib/module API changes docs quote - `skills/internal/doc_sweep.md` |
| MCP tools test | `cmake --build build --config Release --target run_tests_mcp_tools` (it builds `tree_sitter_daslang` first - the grammar library plus the `sgconfig.yml` its post-build step stamps, without which ast-grep knows no `daslang` language) | the `modules` role; MCP signature changes break it silently - run after editing `utils/mcp/` |
| daspkg suite | `cmake --build build --config Release --target run_tests_daspkg` | the `modules` role; the release gates (`--fat` among them) |
| DAP MCP bridge | `cmake --build build --config Release --target run_tests_dap_bridge` | posix only |
| boulder-dash samples | `cmake --build build --config Release --target run_tests_boulder_dash` | both copies - `examples/games/` and the playground's |
| Example games wired to the site | `cmake --build build --config Release --target check_example_games` | `examples/games/REVIEW.das`; the card id every place it is written down |
| Shipped-skills gate | `cmake --build build --config Release --target check_shipped_skills` | |
| pip wheel repack | `cmake --build build --config Release --target check_wheel_repack` | |
| rpm spec render | `cmake --build build --config Release --target check_rpm_spec` | the spec `rpm_build.sh` emits under `--spec-only` - no rpmbuild, so it runs on every cell |
| Benchmark results updater | `cmake --build build --config Release --target run_tests_bench_updater` | |
| pr-babysit verdict core | `cmake --build build --config Release --target run_tests_pr_babysit` | |
| dasllama.io news + metadata | `cmake --build build --config Release --target check_dasllama_site` | needs python `markdown` |
| Browser model-set mint | `cmake --build build --config Release --target check_model_mint` | needs python `numpy` |
| dasllama-server suites | `cmake --build build --config Release --target run_tests_dasllama_server` | the `modules` role; the model-gated suites are `--compile-only` |
| env-knob registries | `cmake --build build --config Release --target run_tests_dasllama_env_registry` | |
| dasLLVM inline-polynomial rail | `cmake --build build --config Release --target run_tests_llvm_vector_math` | the darwin15 cell pins vecmath's answers; needs `-jit` |
| dasllama facade lint | `cmake --build build --config Release --target run_tests_dasllama_facade_lint` | the DASLLAMA001 smokes |
| dasweb-playground / ladder / buildd / verify | `cmake --build build --config Release --target run_tests_dasweb_playground`, `run_tests_dasllama_ladder`, `run_tests_dasweb_buildd`, `run_tests_dasweb_verify` | in-dir suites `--test ./tests` never reaches; each server suite binds a loopback port and shuts itself down |
| CI-only das surface | `cmake --build build --config Release --target check_gated_das` | reads the same `ci_only_das.txt` preflight's `ci-das` gate does |
| nano cortex-m4 cross-compile - **nightly** (linux) | `cmake --build build --config Release --target check_nano_arm` | its arm-none-eabi toolchain is an apt package no darwin cell and no developer box carries |
| Bundle smoke (build.yml) | `cmake --build build --config Release --target check_bundle_smoke` | installs to `<build>/daslang_bundle` and runs `ci/smoke_test_bundle.sh` |
| dasClangBind self-binder (mingw nightly) | `DAS_CLANG_INCLUDE=<libclang include> cmake --build build --config Release --target check_clangbind_fresh`, then `run_tests_cbind_const_preproc` | needs `DAS_CLANG_BIND_DISABLED=OFF` |
| dasGlfw touch layer | `cmake --build build --config Release --target run_tests_glfw_touch` | the `modules` role; the EM_JS bodies run under node against fake GLFW/Browser fixtures, no browser and no window |
| dasImgui build | nothing to install - dasImgui is in-tree (`modules/dasImgui`), built like any default-ON module | external ABI canaries (dasImguiImplot, dasImguiNodeEditor + the rest of the daspkg-index) run in `nightly_daspkg_index.yml`; `skills/internal/abi_break_sweep.md` |
| Coverage - **nightly** (linux) | `cmake --build build --config Release --target run_coverage` | |

## doc.yml - the gates

**Any** daslib edit runs them all. Needs a daslang built with
`DAS_HV_DISABLED=OFF` and `DAS_PUGIXML_DISABLED=OFF` (das2rst documents those
modules). CI stops at the FIRST das2rst panic, so one CI round can hide N-1
further issues - loop gate 1 locally until clean. Workflow:
`skills/internal/make_pr.md` sec.4; conventions:
`skills/internal/documentation_rst.md`.

| # | Gate | Local mirror |
|---|---|---|
| 1 | das2rst runs clean (positional handmade-doc validation panics on count mismatch) | `<daslang> -documentation doc/reflections/das2rst.das` - repeat until no panic; the host policy keeps per-box transforms inert |
| 2 | imgui2rst regenerates clean | `<daslang> modules/dasImgui/utils/imgui2rst.das` |
| 3 | vulkan2rst regenerates clean | `<daslang> modules/dasVulkan/utils/vulkan2rst.das` |
| 4-6 | the generated docs are fresh: no `// stub` in handmade docs, no `Uncategorized` sections, no untracked generated RST | `cmake --build build --target check_docs_fresh` - names the offending file and the fix (a `group_by_regex` in the owning generator for Uncategorized, `git add` for an untracked page). This is the same target doc.yml runs |
| 7 | HTML sphinx, warnings-as-errors | `sphinx-build -W --keep-going -b html -d doc/sphinx-build doc/source build/site` - delete `doc/sphinx-build` first; cached builds hide errors |

preflight finds sphinx-build on PATH, then in `~/Library/Python/*/bin` +
`~/.local/bin`, and stages the docs-assets release once per tree: a `.. video::`
whose recording is missing from `doc/source/_static/tutorials/` is a `-W`
warning like any other, and the gate detail says so when staging failed and the
build then went red.

## wasm_build.yml

`wasm_build`: emsdk build of `web/` (emsdk `latest`), then under the emsdk node
(`"$EMSDK_NODE"`, the system node may be older) two things - the `tests/language` suite
through `web/test/dastest_wasm.js`, and the vecmath backend battery, which runs the same C++
rows twice: `test_vecmath_native` on the wasm SIMD128 backend and `test_vecmath_scalar` on the
per-lane fallback it is checked against. Build both in `web/cmake_temp`
(`ninja test_vecmath_native test_vecmath_scalar` - `web/` adds the repo tree
`EXCLUDE_FROM_ALL`, so plain `ninja` builds neither), then from `web/`:

```
"$EMSDK_NODE" --experimental-wasm-exnref output/tests/test_vecmath_native.js
"$EMSDK_NODE" --experimental-wasm-exnref output/tests/test_vecmath_scalar.js
```

`wasm_cross`: cross-compiles utility mains to wasm32 via dasLLVM and runs them under
wasmtime, emscripten **pinned to 5.0.3** (newer clang crashes on
`utils/gen1-to-gen2/ds_parser.cpp` diagnostics). Mirror, either lane = that job's own emsdk
commands verbatim, its version included (on Windows `EMSDK_PYTHON` must point at a python
>= 3.10, the emsdk-bundled one is older); for most changes let CI carry the lane.

## build_eastl.yml

Builds daslang against EASTL (`cmake/das_config_eastl/` shadow config) and again
with `DAS_NO_FILEIO=1`, where a shadow `<filesystem>` header `#error`s on any
stray include. WSL-mirrorable with the workflow's exact commands; otherwise keep
`<filesystem>` includes inside the fio layer.

## release.yml

Each Release cell: release-modules build -> `dastest -jit` over `tests/` -> `cmake --install
--prefix ./daslang_bundle --strip` + zip -> "Smoke-test installed bundle" -> "Build .deb"
(linux x86_64) -> "Build .rpm" + "Smoke-test .rpm" (both linux cells; `rpm` and `cpio` come
from apt, no Fedora runner) -> "Set up Python for the wheel" -> "Build pip wheel" ->
"Checksum every release asset" ->
"Smoke-test pip wheel" -> "Upload wheel for the PyPI publish job" (every run) -> "Upload
release assets" (release events only). `pypi_route` reads the tag shape, `publish_pypi`
uploads the wheel set. The packaging scripts are `ci/packaging/`
(`README.md` there is the per-release ritual).

| CI step | Local mirror |
|---|---|
| Install binaries + Smoke-test installed bundle | `cmake --build build --config Release --target check_bundle_smoke` (the `build.yml` `bundle_smoke` gate) |
| Build .deb | linux only: `bash ci/packaging/deb_build.sh build/daslang_bundle <tag> <out>` |
| Build .rpm + Smoke-test .rpm | linux with `rpm`: `bash ci/packaging/rpm_build.sh build/daslang_bundle <tag> <out>`, then the smoke by hand - `rpm -qpl` naming `/usr/bin/daslang` and `/usr/bin/daslang-live`, `rpm2cpio <rpm> \| cpio -idm` in a scratch dir, `./opt/daslang/bin/daslang --version`, `readlink usr/bin/daslang` = `/opt/daslang/bin/daslang`; the spec alone, any OS: `cmake --build build --config Release --target check_rpm_spec` |
| Build pip wheel + Smoke-test pip wheel | `python ci/packaging/wheel_build.py build/daslang_bundle <tag> <out>` + `pip install <out>/*.whl` in a venv; the repack rules alone: `cmake --build build --config Release --target check_wheel_repack` |
| Checksum every release asset, the uploads, `pypi_route`, `publish_pypi` | none - runner-side plumbing over the assets above |

The workflow's wiring - step order, conditions, the runner-side smokes - has no per-PR lane;
the packaging scripts do (`check_rpm_spec`, `check_wheel_repack`, `check_bundle_smoke` above).
Prove a wiring change with a `workflow_dispatch` of `release.yml` on the branch - it builds and
smokes every cell and publishes nothing - or with the next RC cut.

## nightly_imgui.yml

Each test spawns a `daslang-live` subprocess hosting a feature app
from `modules/dasImgui/examples/` and drives it over the HTTP live API. The
`modules/dasImgui/tests/.das_test` gate keeps the directory out of per-PR
`--test tests/` sweeps; targeting the folder directly (nightly, and the mirror
below) bypasses the gate. Dispatch `nightly_imgui.yml` itself to exercise an
imgui-touching branch.

Local mirror - also `preflight --full`'s `imgui` gate, the suite's only pre-push
check:

```bash
cmake --build build --config Release --target daslang daslang-live
bin/Release/daslang dastest/dastest.das -- --test modules/dasImgui/tests --headless \
  --isolated-mode --isolated-mode-threads 4 --timeout 600 \
  --exclude glfw_synth --exclude key_hud --exclude embedded_terminal
```

## dasweb-verify-browser.yml

`node_test` (ubuntu-latest, node 22): "SCM Checkout", `actions/setup-node`, "Install runner
dependencies", "Unit tests". `node --test` runs every `*.test.mjs` in the folder; one case fails
when a sample in `web/examples/ui/samples/data.json` has no `expectations.json` row, which is why
that manifest is a trigger path. The suite launches no browser and makes no network request; the
browser run against the deployed daslang.io is `nightly_playground.yml`.

| CI step | Local mirror |
|---|---|
| Install runner dependencies + Unit tests | `npm ci && node --test` in `utils/internal/dasweb-verify/browser` |
