# dasLLAMA tests - Test Files Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that adds or moves a `[test]` here applies `tests/REVIEW.md` (repo root) too, with this
folder's `REVIEW.das` as its gate.**

**A test or a cell a diff moves to another file, its text unchanged or not, is moved: every rule of
`REVIEW.md` (beside this file) or a companion beside it that a diff adding or moving a test or a
cell triggers binds it, whether or not the move reaches the cell.**

**A `[test]` a diff adds or moves into a file that an `--arm` suite lists (every `run.das` suite but
`model-free`, `gpu-kernels` and `stocked`) gates each model-loading cell on `arm_on(t, name)`
(`_model_tier.das`) with that suite's arm; a test with no arm goes into a `model-free`,
`gpu-kernels` or `stocked` file.** A file an `--arm` suite lists runs only where a run names its
arms - `--arm`, or the `image` suite arms a `--changed` or `--area` plan owns (`area_image_arms`,
`run.das`) - never in a `model-free`, `gpu-kernels` or `stocked` run.

**A `[test]` that reaches no stocked artifact once a diff adds it, moves it, or moves cells out of
it goes into a `model-free` file - a `gpu-kernels` file where it compiles a kernel on a Vulkan or
CUDA device, an `--arm` suite's file where it compiles one on the Metal device or, its cells gated
through `arm_on`, its claim holds only under that suite's arm - never a `stocked` one.** A stocked
file runs only where models are stocked, an `--arm` file only where a run names its arms, so a
model-free test in either leaves the per-PR run; a device's first kernel compile is driver time
the model-free test wall does not admit.

**A diff that weakens the model-free/stocked check of `test_run_suites.das`, which reds a
model-free or `gpu-kernels` file whose text reaches a model file, is a defect.**

**A diff that adds a child process running a `.das` program of this folder adds it as a verb of
the test child - a `_child_<verb>.das` module `_child_root.das` dispatches, spawned through
`child_verb_cmd` (`_child_run.das`) - or as a row of `OWN_ROOTS` in `REVIEW.das` (beside this
file), which holds a program only for a claim a verb cannot carry: the program's own compile, the
process it runs in, a server started by its command line, the runner itself. Weakening that
gate is a defect.** The runner builds the test child once a run as an exe, so a verb is a process
start, where a root of its own is an engine compile inside its test's wall.

**A diff that adds, renames, merges or removes a test file, or moves a cell to another file, keeps
every `--only` / `--exclude` token written in `CLAUDE.md` (beside this file) or `../CLAUDE.md`
matching every file that now holds a test the token reached before, or corrects the token there in
the same change.** A token that stops matching drops its files from the run without a red.

**A diff that registers a test file of this folder in a `CMakeLists.txt` other than in a per-PR
lane of `../CMakeLists.txt` is a defect - list it in a `run.das` suite instead.** A second list
drifts from the runner's tables.

**A diff that adds a test file of this folder to a per-PR lane in `../CMakeLists.txt` adds only a
file in the `model-free` suite that guards something a PR without a pod run can break, and lists it
in the lane's `FILES` (run interpreted) rather than its `JIT_FILES` only when no cell of the file
skips for want of the JIT.** A CMake lane has no models.

**An `[init]` in a test file here, or in a `_*.das` fixture one requires, calls nothing but
`allow_cpu_prefill()`; the value a cell needs - a K/V codec, a prompt window, a mirror context,
a GEMM mode, an MMQ route, a VRAM cap - is set inside its `[test]`, or in the fixture's load
helper the test calls, before the first load or device use.** The runner compiles a batch's files as
one program and runs every module's `[init]` in every batch-mate's context, so a value set at
`[init]` reaches files that never asked for it, and their cells skip or measure the wrong rail;
`allow_cpu_prefill()` only matches the `DASLLAMA_CPU_PREFILL=1` every child runs under.

**A test file here, a file dastest runs, declares no `module` name; a diff that adds a `module`
line to one is a defect, and `REVIEW.das` is the gate.** dastest runs a named root with no
required module's `[init]`, so every table an `[init]` seeds - a kernel census, a dispatch
slot - reads empty in the file's cells, and a cell fails far from the cause. A `_*.das` fixture
is a module a test requires, never a root, and keeps its name.

**A model-gated or compute `[test]` here that neither builds, spawns a child nor waits on a
peer carries no `timeout`.** The runner owns a child's budget, and a per-test cap reds a cell
that runs long on a cold cache.

**A diff that adds a `[test]`, wherever it puts it, that starts a daslang child on an entry file -
the `.das` path the child's command line names as its program - another child of the same test file
also runs starts every child of that file on that entry file in one working directory under
`test_temp_dir()` (`dastest/testing_boost`, repo root), and passes none of them `-no-module-cache`,
`-jit-no-cache` or a `-module-cache` path of its own.** Children sharing one working directory share
its module and JIT caches, so the file compiles that program once instead of once a child.

**A diff that adds a line in this folder starting the daslang binary through a spelling
`check_child_binary_flags` (`REVIEW.das`) does not read - a binary path held in a variable or taken
from the command line - builds that command with `daslang_argv` or puts `child_disable_flags()`
right after the binary (`_daslang_argv.das`); an entry a diff adds to `BINARY_UNFLAGGED` starts a
program whose require closure names no GPU module, optional requires included; a diff that weakens
the check is a defect.** A child without the run's `--disable-module` flags compiles another module
set than the run's records hold and reparses the engine inside its test's wall.

**A `standalone` reason here that says the test loads a model is a defect - the test goes to
its topic's file.** A cell frees its model before it returns.

**A function in a file of this folder that requires a module behind an optional `require ?<mod>`
never names that module's types in its signature - leave a parameter that would carry one
untyped, and drop a return type that would name one.** A signature cannot sit inside a
`static_if`, so a build without the module fails the compile on it.

**A function in a file of this folder that requires a module behind an optional `require ?<mod>`,
and that has no untyped parameter, names anything that module declares only inside a
`static_if` branch that compiles only where `<mod>` exists.** In a build without the module a
fully typed function is inferred anyway; one with an untyped parameter is inferred only at a call
site, which its caller has already guarded.
