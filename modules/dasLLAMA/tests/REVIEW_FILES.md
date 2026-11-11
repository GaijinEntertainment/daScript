# dasLLAMA tests - where a test lives

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** The parent
checklist is `REVIEW.md` (beside this file); the runner's tables are `run.das`.

**A diff that adds or moves a `[test]` here applies `tests/REVIEW.md` (repo root) too, with this
folder's `REVIEW.das` and the `check_tests_index` target over `TESTS.md` (beside this file) as
its gates; a diff that weakens either is a defect.**

**A `[test]` a diff adds or moves into a file that an `--arm` suite lists (every `run.das` suite
but `model-free` and `stocked`) gates each model-loading cell on `arm_on(t, name)`
(`_model_tier.das`) with that suite's arm; a test with no arm goes into a `model-free` or
`stocked` file.** A file an `--arm` suite lists runs only when someone names its suite with
`--arm`, never in a per-PR run.

**A `[test]` a diff adds or moves that reaches no stocked artifact goes into a `model-free`
file, never a `stocked` one; a diff that weakens the model-free/stocked check of
`test_run_suites.das` is a defect.** A stocked file runs only where the models are stocked, so
a model-free test inside it stops running on every other box; `test_run_suites.das` reds the
reverse, a model-free file whose text reaches a model file.

**A diff that adds, renames, merges or removes a test file keeps every `--only` / `--exclude`
token written in `CLAUDE.md` (beside this file) or `../CLAUDE.md` matching every file that now
holds a test the token reached before, or corrects the token there in the same change.** A
token that stops matching drops its files from the run without a red.

**A diff that registers a test file of this folder in a `CMakeLists.txt` is a defect - list it
in a `run.das` suite instead. The per-PR lanes in `../CMakeLists.txt` are the exception: a diff
adds a file there only when the file is in the `model-free` suite and guards something a PR
without a pod run can break.** A second list drifts from the runner's tables, and a CMake lane
has no models. A lane may run such a file interpreted (`FILES`, not `JIT_FILES`), outside the
`-jit` rule of `REVIEW.md`, only when no cell of the file skips for want of the JIT.

**An `[init]` in a test file here, or in a `_*.das` fixture one requires, calls nothing but
`allow_cpu_prefill()`; the value a cell needs - a K/V codec, a prompt window, a mirror context,
a GEMM mode, an MMQ route, a VRAM cap - is set inside its `[test]`, or in the fixture's load
helper the test calls, before the first load or device use.** The runner compiles ten files as
one program and runs every module's `[init]` in every batch-mate's context, so a value set at
`[init]` reaches files that never asked for it, and their cells skip or measure the wrong rail;
`allow_cpu_prefill()` only matches the `DASLLAMA_CPU_PREFILL=1` every child runs under.

**A model-gated or compute `[test]` here that neither builds, spawns a child nor waits on a
peer carries no `timeout`.** The runner owns a child's budget, and a per-test cap reds a cell
that runs long on a cold cache.

**A `standalone` reason here that says the test loads a model is a defect - the test goes to
its topic's file.** A cell frees its model before it returns.
