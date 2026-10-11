# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture docs:
`ARCHITECTURE.md`, `ARCHITECTURE_TARGET_FEATURES.md`, `ARCHITECTURE_DEBUG_INFO.md`,
`ARCHITECTURE_JIT_ENTRY.md`, `ARCHITECTURE_EXE.md`, `ARCHITECTURE_LIB.md`,
`ARCHITECTURE_CODEGEN_IDENTITY.md`, `ARCHITECTURE_VECTOR_MATH.md`. Planned work: `LEDGER.md`.

**A `[test]` file under this module that carries a `require dasllama/...` line of its own answers
to `modules/dasLLAMA/REVIEW.md` (repo root) as well** - its out-of-folder ledger row lives there.

**A diff that changes how the compile handles a function because it carries `[tune]`,
`[tune_perm]`, `[tune_companion]`, `[tune_scope]`, `[tune_policy]` or `[llvm_code]` - the body
it emits for it, which permutation it picks for it - or whether a saved `<app>.tune.json` is
applied, refused as stale, or read as another machine's, is reviewed with `skills/tune.md`.**

**A change to the tune framework - `daslib/llvm_tune.das`, its tests, or the descriptor and
C++ rows that join it to a program (`.das_module`, `src/dasLLVM.cpp`) - is reviewed with
`skills/internal/llvm_tune_internals.md`.**

- **A change to any file under this module but a `.md` runs the module-owned suite** (command
  and build gate: `tests/README.md` here). The suite is outside the core `tests/` sweep, so no
  other lane covers it.

- **A diff that adds or changes a branch keyed on what `get_platform_name()`,
  `get_architecture_name()`, `cpu_supports()`, or `host_llvm_feature()` returns, directly or
  through a value built from it, runs the module-owned suite on a machine that takes the new
  branch.**

- **A test under `tests/` (beside this file) never creates, overwrites, or deletes a
  git-tracked path.**

- **A test under `tests/` here that writes at all - through its own filesystem calls, not a
  child process's - writes only to paths it created uniquely for this process (a temp file or a
  temp directory), and removes them** - a shared path under `build/` is one two concurrent runs
  collide on.

- **A test under `tests/` here that spawns a daslang child keeps the child's artifacts inside a
  directory it created uniquely for this process: `-output <dir>/...` for a `-exe` build,
  `-no-module-cache` or `-module-cache <dir>/...` for a run that compiles through the front-end
  cache, and `-jit-no-cache` or a pinned `jit_output_path` for a `-jit` run that executes the
  script.** A child writes its caches relative to the cwd otherwise, which is the tree two
  concurrent runs share.

- **A diff that changes which arm a non-host triple selects in a branch keyed on the target
  triple (a key added, changed or removed), or changes code only a cross target's arm reaches,
  records in its PR body the `--jit-target=<triple>` command that reached the changed arm: an
  `-exe` build, or a `--jit-compile-only` run when the arm sits in `[tune]` grid code, which an
  `-exe` build replaces with reference bodies.** The suite runs on the host, so another target's
  arm is checked only by a command built for it.

- **A PR body that records a `--jit-compile-only` cross-target run also records, beside it, a
  run of that code on a box of the target, or the statement that no such box ran it.** A
  compile-only run shows the code emits, never that it runs.

- **A diff that adds work to, or moves work within, what `run_jit`
  (`daslib/llvm_jit_run.das`) or `run_jit_linked` (`daslib/llvm_jit_link.das`) executes - its
  own body or any callee - also prints an `LLVM JIT time:` number for that work on every path
  that executes it: its own line, or the number of a phase that includes it, while that phase's
  line still prints; a number a second, independent computation of the same work prints (the
  emitter's plan after the link module's) does not cover the first**
  (`ARCHITECTURE.md#jit-timing-contract`). Only work on the path that reaches the report is
  timed: option resolution before the first timer, log lines, and failure-path teardown are not.

- **A change that alters the machine code the JIT emits for a function the JIT cache serves -
  from the DLL cache or the split-obj cache - without changing any input the JIT cache keys fold,
  bumps `LLVM_JIT_CODEGEN_VERSION` (`daslib/llvm_jit_plan.das`)**
  (`ARCHITECTURE.md#codegen-tier`). The folded inputs are what `jit_dll_basename`
  (`daslib/llvm_jit_plan.das`) and the split-partition key in `run_jit`
  (`daslib/llvm_jit_run.das`) fold; a key that does not change serves the old machine code back.

- **A diff that adds an input only the backend reads - an annotation, or an annotation argument,
  that changes emitted code and leaves the function's AOT hash unchanged - folds it, after the
  carrying function's mangled name, into the DLL key and the split-obj key through a
  `fold_*_hints` function in `daslib/llvm_jit_plan.das`, in the same change**
  (`ARCHITECTURE_JIT_ENTRY.md#hint-folds`). A `LLVM_JIT_CODEGEN_VERSION` bump re-keys once, so
  adding or removing the input on a function afterwards still serves the old machine code back.

- **A diff that adds a `require` line naming a `[llvm_code]` generator module outside this
  module to `daslib/llvm_user_modules.das` is a defect; that module's package
  joins the `llvm_code_generator` group from its own descriptor instead.** The wiring module
  names no package, so a build that does not carry the package registers nothing and compiles
  unchanged; the generators this module ships stay named.

- **A diff that puts a function on the jit finalizer's path - `free_jit_context`
  (`daslib/llvm_jit_link.das`) and every function it calls - marks that function `[no_jit]`; a
  helper from another module (`macro_context_of` and the rest of `daslib/cross_context`, repo
  root) is never called there - call the externs it wraps directly.** The finalizer is program
  code, so a function without `[no_jit]` on its path joins every jitted program's DLL (the
  block-passing helpers cannot be lowered: `ARCHITECTURE_JIT_ENTRY.md#jit-set-exclusions`).

- **A diff that adds to a JIT cache key an input that is the same for every function of one
  compile folds it inside `jit_env_salt` (`daslib/llvm_jit_plan.das`), never directly into
  either JIT key - the DLL key or the split-obj key
  (`ARCHITECTURE_CODEGEN_IDENTITY.md#split-obj-cache`)** - salt feeds both keys, and an input
  folded into one but not the other links stale objects.

- **A macro under this module's `daslib/` that reads a file at compile time registers it with
  `add_module_cache_dependency` before any early return, in the same change**
  (`ARCHITECTURE.md#tune-sidecar-cache-pin`). An unpinned compile-time file read serves stale
  macro output from the module cache until an unrelated source file changes - silently.

- **A diff that adds a `-lib` entry point, or work to one, keeps the C boundary's three promises:
  a raise reaches the caller as a return value and never an unwind, `<P>_create` answers null
  rather than aborting, and `<P>_destroy` runs what the runtime's own shutdown cannot find**
  (`ARCHITECTURE_LIB.md#lib-runtime-scope`). A library is called by code that cannot catch anything.

- **A `-lib` build path a diff adds or changes that writes no artifact and exits 0 is a defect -
  exit non-zero instead.** A build rule reads the exit code, and a silent success lets
  it link the previous run's library against this run's header.

- **A test under `tests/` here whose child compiles through the front-end module cache - any
  child but a `-exe`, `-compile-only`, `-documentation`, debugger or AOT run - and asserts a
  line a macro prints at compile time, or its absence, spawns that child with
  `-no-module-cache`; a test whose subject is the cache itself pins its own file with
  `-module-cache <temp>` instead and never takes the flag.** A cache hit runs no macro, so a
  macro-time line prints on the first run only (`ARCHITECTURE.md#macro-line-cache-replay`).

- **A diff that adds a top-level section to the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) updates `modules/dasLLAMA/dasllama/dasllama_exchange_schema.das` in
  the same change and keeps `modules/dasLLAMA/tests/test_exchange_schema.das` green** - the
  validator allow-lists sections, so a section it does not know fails every newly minted
  sidecar at submission, and the checked-in corpus the test sweeps cannot show it. A new key
  inside an existing section passes the validator as it stands.

- **A diff that adds an override knob, or gives one a new effect, adds the knob or the effect
  to the inventory in `ARCHITECTURE.md#override-knobs` in the same change.** An override knob is
  supplied at run time - an environment variable, a command-line flag, or an exported runtime
  setter - and changes what a run compiles, tunes, or emits beyond its defaults. Anything
  written in source - `[tune]`-family and `[hint]` annotation arguments - is a declaration, not
  an override.

- **A diff that adds an override knob, or gives one a new effect, also logs at least one line
  naming the knob where it takes effect.** A diff that only exposes the knob puts the line at
  the consumer instead, in that same diff.

- **Weakening `tests/llvm_env_registry.das` (beside this file) is a defect** - dropping a scan,
  narrowing its `STRICT_ROOTS`, or widening its `SKIP_FILES` weakens it.

- **A host path a diff writes into the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) goes through `tilde_home` (`daslib/fio.das`, repo root); a host path
  it passes to a filesystem call stays raw.** No filesystem call resolves `~`, and a recorded
  path is read by other people, so it must not name the user who minted it.

- **Weakening `tests/llvm_tune_manifest.das` (beside this file) is a defect.**

- **A diff that emits an instruction into the entry block of a function whose body das
  statements emit - the impl half of a generated function pair (the body the wrapper half calls),
  or a block body - gives it no debug location, and
  one it emits into a loop's latch (the block that jumps back to the loop head) gives it the
  loop's own line** (`daslib/llvm_jit.das`). Those are the two places the emitter fills out of
  das statement order, and an instruction that keeps whatever location was current when it was
  emitted moves a `break file:line` stop onto a statement that has not run
  (`ARCHITECTURE_DEBUG_INFO.md#di-variable-locations`).

- **A diff that adds a teardown step to `reset_jit_globals_after_failure`
  (`daslib/llvm_jit_run.das`) puts it after the `di_finalize()` call.** A DIBuilder writes into
  the LLVM module it was created for, so finalizing one whose module an earlier step already
  disposed of reads freed memory.

- **Never call `LLVMSetIsInBounds` - build the GEP in-bounds with `LLVMBuildInBoundsGEP2` (the
  `llvm_boost` wrapper's `inbounds` default) instead.** A constant-folded GEP is a
  `ConstantExpr`, and the setter's cast writes through the wrong type into it
  (`ARCHITECTURE.md#gep-constant-fold`).

- **A feature name used in a `requires=` list or a `g_target_*` tier gate
  (`daslib/llvm_jit_common.das`) has its cpuid line in `das_cpu_supports`
  (`src/builtin/module_builtin_runtime.cpp`, repo root) in the same diff**
  (`ARCHITECTURE_TARGET_FEATURES.md#x64-tier-gates`). A name the cpuid table does not know
  answers false on every box, so every perm that requires it silently declines to its fallback
  and no error names the cause.

- **An emitter under `daslib/` that uses a call's name as a KEY - an intrinsic-name fragment, a
  lookup-table key, a branch on one spelling - reads `expr.func.name`, never `expr.name`;
  `expr.name` stays only in a diagnostic or an LLVM value name, where it is what the user
  wrote.** The dispatch tables key off the declaration, so a call site can still spell itself
  module-qualified.

- **A diff that adds or changes a `build_vector_*` emitter (`daslib/llvm_jit_intrin.das`) emits
  each Horner step unfused, through `vmath_poly_step`, and calls `vmath_fma` only for the steps
  vecmath itself writes fused** (`ARCHITECTURE_VECTOR_MATH.md#vector-poly-fusion`). One fused
  step in a sign-alternating chain moves the last few bits of the result, and the interpreter
  and AOT answers do not move with it.

- **A diff that changes the machine code a twinned emitter produces - its body, which of its
  arms a call selects, or the feature set its output is lowered under - leaves it covered by a
  cell comparing the emitted result with the interpreted result over the operand range that
  emitter serves (every vector width for a vector emitter, the full int8 lattice for a dot),
  adding the cell in the same change when none covers that range.** A twinned emitter is one the
  interpreter also computes, through a das body or a builtin: every `build_vector_*` in
  `daslib/llvm_jit_intrin.das` and every `[llvm_code]` generator under this module.

- **An IR-shape test asserts the instruction by name, never a numeric value.**

- **A cell comparing a float emitter's emitted and interpreted results also asserts both answer
  NaN in the same lanes.** A clamp or a conversion written with ordered compares turns a NaN lane
  into a number, and an accuracy bound reads that as success.

- **A change that makes `REVIEW.das` (beside this file) report fewer inputs is a defect:**
  dropping a check, shrinking a scanned set or a tracked-fixture directory (a guard over nothing),
  widening an exemption list without naming the exempted input's reason beside it, or a finding
  text that no longer names what failed. What the gate enforces is read from the gate itself.

- **A walk over the program's modules in this module's `daslib/` that leaves a module out by
  `moduleFlags.builtIn` alone is a defect - a promoted das module (`module X shared`) is
  builtIn too; a walk that means the C++ modules tests `builtIn && !promoted`**
  (`ARCHITECTURE_EXE.md#exe-global-init-walk`). A promoted module's global initializers are program
  code, and a walk that skips them leaves the address globals they need null in the exe.

- **A diff that builds a feature string for a machine that has a force knob - a
  `*_forced_plus_features` function in `daslib/llvm_jit_common.das` - appends the forced names
  AFTER the detected host features.** LLVM's `SubtargetFeatures` takes
  the last occurrence of a name, so a forced feature placed first is silently overridden by
  detection.
