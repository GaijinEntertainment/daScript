# dasLLAMA tests - Test Files Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that adds or moves a `[test]` here applies `tests/REVIEW.md` (repo root) too, with this
folder's `REVIEW.das` as its gate; a diff that weakens it is a defect.**

**A `[test]` a diff adds or moves into a file that an `--arm` suite lists (every `run.das` suite but
`model-free`, `gpu-kernels` and `stocked`) gates each model-loading cell on `arm_on(t, name)`
(`_model_tier.das`) with that suite's arm; a test with no arm goes into a `model-free`,
`gpu-kernels` or `stocked` file.** A file an `--arm` suite lists runs only where a run names its
arms - `--arm`, or the `image` suite arms a `--changed` or `--area` plan owns (`area_image_arms`,
`run.das`) - never in a `model-free`, `gpu-kernels` or `stocked` run.

**A `[test]` a diff adds or moves that reaches no stocked artifact goes into a `model-free`
file - into a `gpu-kernels` file where it compiles a kernel on a Vulkan or CUDA device - never a
`stocked` one; a diff that weakens the model-free/stocked check of `test_run_suites.das` is a
defect.** A stocked file runs only where the models are stocked, so a model-free test inside it
stops running on every other box; a device's first compile of a kernel is the driver's time,
which the model-free suite's test wall does not admit; `test_run_suites.das` reds a model-free or
`gpu-kernels` file whose text reaches a model file.

**A diff that adds a child process running a `.das` program of this folder adds it as a verb of
the test child - a `_child_<verb>.das` module `_child_root.das` dispatches, spawned through
`child_verb_cmd` (`_child_run.das`) - or as a row of `OWN_ROOTS` in `REVIEW.das` (beside this
file), which holds a program only for a claim a verb cannot carry: the program's own compile, the
process it runs in, a server started by its command line, the runner itself. Weakening that
gate is a defect.** The runner builds the test child once a run as an exe, so a verb is a process
start, where a root of its own is an engine compile inside its test's wall.

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
helper the test calls, before the first load or device use.** The runner compiles a batch's files as
one program and runs every module's `[init]` in every batch-mate's context, so a value set at
`[init]` reaches files that never asked for it, and their cells skip or measure the wrong rail;
`allow_cpu_prefill()` only matches the `DASLLAMA_CPU_PREFILL=1` every child runs under.

**A model-gated or compute `[test]` here that neither builds, spawns a child nor waits on a
peer carries no `timeout`.** The runner owns a child's budget, and a per-test cap reds a cell
that runs long on a cold cache.

**A diff that adds a `[test]`, wherever it puts it, that starts a daslang child on an entry file -
the `.das` path the child's command line names as its program - another child of the same test file
also runs starts every child of that file on that entry file in one working directory under
`test_temp_dir()` (`dastest/testing_boost`, repo root), and passes none of them `-no-module-cache`,
`-jit-no-cache` or a `-module-cache` path of its own.** Children sharing one working directory share
its module and JIT caches, so the file compiles that program once instead of once a child.

**A `standalone` reason here that says the test loads a model is a defect - the test goes to
its topic's file.** A cell frees its model before it returns.
