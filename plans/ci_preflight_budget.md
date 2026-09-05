# The pre-merge budget: preflight in 20 minutes, CI in 35 per job

Ruling: on the M5 box `preflight --full` fits in 20 minutes or a gate is not in preflight; a per-PR CI job
fits in 35 minutes or its steps go to the nightly run. What does not fit is retired, not tolerated.

## Baseline (measured 2026-09-04, module-cache follow-up branch)

Local chain (`make-pr` then `preflight --full`), serial on 16 cores:

| Phase | Wall | Composition |
|---|---|---|
| make-pr gates | 12.5 min | sync, review-md walk, stamp-reach, dupes (the bulk), ast-verify, jit-smoke |
| preflight, non-dasLLAMA | ~20 min | tests-jit ~6.5, sphinx docs ~3.3, AOT build+run ~2 (7 cold), interp ~1.7, imgui ~1.4, sequence <1, the fast gates ~3 |
| dasllama-model-free | ~6 min | 63 files, warm caches; ~10 s/file is engine compile the module cache does not reach |
| dasllama-stocked | ~25 min | model wall: test_batch_decode 263 s, test_ple_modes ~560 s, gemma3v 109 s, vision_chat ~110 s |

CI, last master run (9836bcb91): extended_checks linux 75 min (build 23.7, examples 8.4, tutorials 7.8,
dasllama-server 6.9, utils tests 4.2, coverage 3.5, ser/deser 2.8, static 2.8, facade lint 2.3, MCP 2.3),
extended_checks darwin15 46 min (build 16.3, tutorials 10.9, examples 5.6); build lane 55 min (asan 55, windows
Debug 52, tsan 51, windows Release 50, linux Release 46, ubsan 45, bundle_smoke 41 - the build step is the
cost: linux Release 1302 objects in 30 min, cold); CodeQL 42.5. sccache: `SCCACHE_CACHE_SIZE=500M` while one
Release build is 718 MB of objects, so no slot ever holds a build (linux: cold every run; windows: 22% hits);
the repo holds 17 GB of caches, 4.3 GB of it ten per-commit CodeQL databases; the limit is ~25 GB.

Structural causes: everything serial; review-md and ast-verify run in both halves of the chain; no reach
model for module-owned gates; dupes (advisory) on the critical path; the dasLLAMA per-file engine compile.

## Preflight (utils/internal/preflight)

Tiers, printed by `--list-gates`:

- **fast** (serial, first; a red stops the run before the lanes): untracked, format, lint, ast-verify,
  cpp-syntax, review-md, md-ascii, hash-refs, dasgen, ci-das, compile-sweep (every program root under
  `utils/`, `examples/`, `tutorials/`, the modules' examples and utils, `-compile-only`, parallel - 995 roots
  in 18 s here).
- **lane** (`--full`, run concurrently, wall = the longest): tests-cpp, tests-interp, tests-jit, tests-aot,
  docs, utils-tests (`run_utils_tests`, ~100 s here).
- **module** (never in `--full`; `--only <gate>` when working on the module): imgui, sequence,
  dasllama-model-free, dasllama-stocked.
- **reach**: a gate names the path prefixes that reach it; `src/`, `include/`, `daslib/`, `dastest/`,
  `CMakeLists.txt` reach everything. A diff that misses a gate's reach skips it with the reason.
- make-pr's default chain drops review-md and ast-verify (preflight owns them) and dupes (an advisory
  report: `make-pr --only dupes` when wanted); it keeps sync, stamp-reach, jit-smoke, then chains preflight.

Acceptance: a full run with a core change under 20 minutes on the M5 box; a dasLLAMA-only diff in about 5.

Measured (2026-09-04, this branch, a core diff of 10 das / 3 cpp files): `preflight --full` 11.5 min wall.
Fast tier 3.1 min serial (compile-sweep 108 s - 730 light roots in an 18-wide pool 66 s + 25 engine roots
serial through one module cache 42 s; lint 24; cpp-syntax 18 - a header change sweeps 203 TUs; ci-das 12;
format 6; review-md 7). Lanes concurrent, wall = tests-aot 513 s (test_aot build 402 + run 111); docs 336
(sphinx-html 297); utils-tests 186; tests-jit 184; tests-interp 116; tests-cpp 20. The pool width lesson:
`get_total_hw_threads()` is the jobque's worker count (5 on the 18-core M5), so a pool sized by it runs 5
wide; size pools by `get_total_hw_cores()`.

## CI (.github/workflows)

- The matrices are data: `ci/ci_matrix.py build|extended <event>` emits the cells, `pre_job` evaluates it,
  the fan-out job reads `fromJSON(needs.pre_job.outputs.matrix)`; `ci/test_ci_matrix.py` pins both sets
  and the role-condition spelling (`.github/workflows/REVIEW.md`).
- sccache: build.yml's slots were never capped (the default 10G; the compressed tarballs run 104 MB to
  971 MB) - the 500M cap was extended_checks' alone, and it is now 1500M so the nightly save holds a
  complete object set. The nightly-only cells (sanitizers, windows 64 Debug) save no slot: nothing
  restores one (frees ~1.3 GB). CodeQL: master pushes + the weekly cron, no `pull_request` trigger
  (frees the per-commit ~430 MB databases and 20-40 min per PR).
- extended_checks per PR = two darwin15 jobs, `core` and `modules` (the step lists: `skills/internal/preflight.md`
  sec."extended_checks.yml"). Estimated 20-26 min each at a warm sccache. linux and windows extended_checks,
  tutorial dry-runs, the run form of examples, coverage, nano cross-compile: nightly (same workflow,
  `event_name == 'schedule' || 'workflow_dispatch'`, role `all`). daslang_static stays per PR (modules role):
  a step leaves the per-PR path only when a preflight gate mirrors it, and nothing local sweeps the static
  binary. `ci/test_ci_matrix.py` runs in the core role and as preflight's `ci-matrix` gate. Two darwin lane steps that were
  linux-only needed portable shells: `mapfile` (bash 3.2 on the runner) became a read loop, `timeout`
  (no coreutils) became perl's `alarm`.
- build lane per PR: Release + Debug on linux/darwin/windows/linux_arm as today minus asan/tsan/ubsan and
  windows 64 Debug, which are nightly-only cells. Every remaining job is a build step.

Acceptance: every per-PR job under 35 minutes on the first PR after the change (measure with the Actions API:
run, job, step walls; the script in the session scratchpad becomes `utils/internal/ci-timing/` if kept).

## dasLLAMA long tests (after the above)

- Compile dressed as a test: `test_exe_smoke` (108 s, builds an exe), `test_tok_seed` (90 s) and
  `test_parity_pregate` (91 s) require `lcpp_bench` by path and pay its engine compile for a header
  parser and a pregate - the helpers move into a small module the bench requires.
- Model wall, per file: `test_batch_decode` 263 s, `test_ple_modes` ~560 s, `test_gemma3v` 109 s,
  `test_vision_chat` ~110 s, `test_kquant` 82 s, `test_whisper` 80 s, `test_parity` 40 s - fewer carriers,
  one load per file, smaller fixtures.
- The engine compile floor: the module cache surviving into dastest's runtime `compile_file` (per test
  file key; an engine record is ~210 MB - why, first).
