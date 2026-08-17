# Tests in the daslang repo (repo-only)

Read this alongside `skills/writing_tests.md` when adding or moving tests **inside the
daslang repository**. `writing_tests.md` covers how to write a dastest test anywhere —
that half ships in the SDK. This file is the repo's own harness: AOT registration, the
`tests/.das_test` gating filter, and the deep-engine model-test rules. None of it applies
to a test written against an installed SDK.

## AOT registration (REQUIRED for new test directories)

The full `test_aot` binary runs EVERY test under `tests/` with AOT enabled (`fail_on_no_aot`).
It builds+runs on the NIGHTLY CI cron and in `preflight --full` — per-PR CI only builds the
`tests/language` subset (`test_aot_subset`) as a compile gate, so a missing registration
passes PR CI and fails the nightly. Creating a new test directory ⇒ register it in
`tests/aot/CMakeLists.txt` — for a plain suite that is one entry in `set(DAS_AOT_SUITES …)`;
see `skills/internal/aot_testing.md` § "Registering a New Test Directory" for the irregular cases —
or the nightly/preflight fails with `error[50101]: AOT link failed`.

If a specific file genuinely can't AOT (emitter bug, interpreted-only by design): put
`options no_aot` IN THE FILE **and** exclude it from the directory's AOT glob, with a
comment + issue link on both. Glob exclusion alone is NOT enough — test_aot still *runs*
the file and trips 50101 on its missing stubs; `options no_aot` is what makes the runtime
skip AOT linking for it. (2026-06-11: in-file `options no_aot` currently fails in the AOT
hash itself — fix incoming on master; until it lands, interp-only tests are gated by the
directory filter below instead.)

## The `tests/.das_test` directory filter — and its root-path caveat

`tests/.das_test` is a daslang script dastest compiles per run; its `can_visit_folder`
pinvoke gates whole directories per mode — e.g. `no_aot/`, `ast/`, `ast_match/` are
skipped under `--use-aot`, module dirs (dasSQLITE, dasPUGIXML…) skip when the
module isn't built in. **The filter is looked up only at the `--test` ROOT path** —
`--test tests` finds and applies it, but `--test tests/flatten` looks for
`tests/flatten/.das_test` (absent) and walks into `no_aot/` unfiltered, producing
false `error[50101]` / JIT failures. For AOT/JIT validation, sweep `--test tests`
(CI's form) or target individual files — never a subtree that contains gated dirs.

## Per-folder sweep gating (`tests/.das_test`)

`tests/.das_test` defines `can_visit_folder(folder_name, result)` — dastest consults it
per subfolder during file collection (only for the `.das_test` at the `--test <root>`
argument; directly naming a child folder bypasses it). It gates folders on module
availability (`dasHV`, `dasSQLITE`, …) and on sweep mode by scanning argv — `--use-aot`
skips `ast`, `ast_match`, `no_aot`, `jit_tests`, `.jitted_scripts` and `strudel_device`;
`-jit` skips only `gc` (heap_collect can't see heap
pointers whose only reference is a local in a jitted frame — native-stack locals are
invisible to the collector, so GC-semantics tests are interp-only; the other former `-jit`
skips were lifted once `jit_enabled` started triggering daslib/quote lowering). Two traps:
a whole-folder JIT/AOT failure usually means a missing entry here, NOT a per-file fix; and
the `jit_cache_all_tests` prewarm target (utils/CMakeLists.txt — local-use only since CI's
isolated-parallel JIT sweep mints the dll cache itself) does NOT consult it — its
`--exclude` list mirrors the skips manually and must be updated in the same change.
Per-function `[no_jit]` is the finer-grained alternative when only some functions in a
kept folder can't JIT — put it on the function whose CODE diverges under JIT, not just the
`[test]` wrapper (JITted callees replace their SimNode bodies, so an interpreted wrapper
still calls jitted workers). Beware Release-blind divergence: memory bugs (double-free,
reuse-after-collect) only trip the Debug memory_model.h assert, so a green local Release
sweep does NOT prove a lifted skip is sound — Debug CI is the oracle.

## Module-owned test homes

Some modules keep their tests inside the module folder, governed by that folder's
`REVIEW.md`: dasImgui tests live under `modules/dasImgui/tests` (never `tests/dasImgui` —
do not create that folder), dasLLAMA's under `modules/dasLLAMA/tests` (see its
`tests/CLAUDE.md`). When adding a test for such a module, put it in the module's own home.

## Deep-engine model tests (dasLLAMA and friends)

Two hard-won rules for tests that drive a deep JIT engine chain (`forward`/`forward_prefill`/
`generate`):

1. **The test file is the program root under dastest**, so `options stack = 524288` in the test
   file DOES apply (the "main-module-only" rule works in your favor). Without it, driving the
   engine directly trips `stack overflow` deep in the module (reported at some engine function's
   entry line).
2. **Keep heavy helpers free of `T?`** — a `T?` param keeps the helper off the JIT (dastest's
   class), so its engine calls run on the interpreter stack. Structure like `test_parity.das`:
   plain private functions do the model work and return values/arrays; the `[test]` run-lambda
   only asserts. (`modules/dasLLAMA/tests/test_mtp.das` hit both failure modes before landing
   on this.)

## Structural-proof anchors vs the heuristic inliner

`auto_inline_functions` defaults ON, so a small target function whose only call sits in a
test lambda gets auto-inlined and reaped — and a structural test that inspects its
post-patch body (`compiling_module() |> for_each_function`) finds nothing. Mark such probe
anchors `[never_inline]` (or keep them over the `auto_inline_cost` budget). Calls placed
directly in the `[test]` function body are safe — an annotated caller disables the
heuristic tier — but lambda arms (`t |> run(...) @(...)`) are not.

Two more ways a structural anchor vanishes or hides, neither stopped by `[never_inline]`:

- **Const-fold reaping**: a pure anchor called with constant arguments gets evaluated at
  compile time and reaped as unused — behavior checks then pass on the folded constant
  while the body probe finds nothing. Taint the anchor's input through a mutable global
  (`var g_seed = 1`, written somewhere) so it must survive to runtime.
- **Mangled instance names**: a generic instance lands in the compiling module under
  `` origin`name`hash ``, which the exact-name `for_each_function(mod, name)` lookup
  misses (only the empty name iterates everything). Match by substring over
  `for_each_function("")`, and drop the `(` from body-text checks on calls to it — the
  call site spells the mangled name too.

## Shared fixtures and the hyphen trap

Shared test helpers go in `_common.das` module files (e.g. `tests/linq/_common.das`). A
require path cannot contain a hyphen, so a hyphenated tool directory (`utils/detect-dupe/`,
`utils/find-dupe/`) is unreachable by path — those tools keep their `[test]` files *inside*
the directory and require the sibling module by bare name. A test needing a shared fixture
should keep the fixture in its own directory rather than reaching across the tree.

## Running the repo suite

- Single file: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/linq/test_linq_aggregation.das`
- Directory: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/linq/`
- All tests: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/`

Sweep the whole `tests` root for AOT/JIT validation, per the root-path caveat above.
