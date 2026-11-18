# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_CODEGEN_IDENTITY.md`, `ARCHITECTURE_DEBUG_INFO.md`,
`ARCHITECTURE_EXE.md`, `ARCHITECTURE_JIT_ENTRY.md`, `ARCHITECTURE_LIB.md`,
`ARCHITECTURE_TARGET_FEATURES.md`, `ARCHITECTURE_VECTOR_MATH.md`. Planned work: `LEDGER.md`.
The `tests/` subfolder has its own checklist, `tests/REVIEW.md`: a `[test]` file, or a file that
exists only for tests to build, run or require (a program root, a client, a fixture module),
answers to it wherever the diff puts it under this module.

- **A diff that changes how the compile handles a function because it carries `[tune]`,
  `[tune_perm]`, `[tune_companion]`, `[tune_scope]`, `[tune_policy]` or `[llvm_code]` - the body
  it emits for it, which permutation it picks for it - is reviewed with `skills/tune.md`.**

- **A diff that changes how a read or a save of `<app>.tune.json` treats a file that is stale
  (its mtime is older than the running binary) or that another machine wrote is reviewed with
  `skills/tune.md`.**

- **A change to the tune framework - `daslib/llvm_tune.das`, `requires_hold` or `any_row_runs`
  in `daslib/llvm_cpu_class.das`, a test under `tests/` here that carries a
  `require llvm/daslib/llvm_tune` line of its own or spawns a child on a script that carries
  one, the `registerModuleGroupMember("tune_framework", ...)` row in `src/dasLLVM.cpp`, or the
  `register_module_group("tune_framework", ...)` row in `.das_module` - is reviewed with
  `skills/internal/llvm_tune_internals.md`.**

- **A change to any file under this module but a `.md` runs the module-owned suite (command
  and build gate: `tests/README.md` here).** CI runs some test files of that suite only nightly,
  so before merge this run is the only one that covers every one of them.

- **A change to a file under `daslib/` here that `EMITTER_FILES`
  (`tests-cpp/small/test_jit_emitter_pin.cpp`, repo root) names also runs
  `dastest/dastest.das -jit -- --test tests` (repo root).**

- **A diff that adds or changes a branch keyed on what `get_platform_name()`,
  `get_architecture_name()`, `cpu_supports()`, or `host_llvm_feature()` returns, directly or
  through a value built from it, runs the module-owned suite on a machine that takes that
  branch, when a build that targets the machine it runs on can take that branch.** A child run
  under a forced CPU feature (`DAS_JIT_ARM64_FORCE_FEATURES`, `DAS_JIT_X64_FORCE_FEATURES`)
  that reaches the branch's code counts as such a machine. A test's own check that skips the
  test on other platforms is not such a branch.

- **A diff that changes which arm a non-host triple selects in a branch keyed on the target
  triple (a key added, changed or removed), or changes code only a cross target's arm reaches,
  records in its PR body the `--jit-target=<triple>` command that reached the changed arm: an
  `-exe` build, or a `--jit-compile-only` run when the arm sits in the code-generation
  permutations a `[tune]` function races, which an `-exe` build replaces with reference
  bodies.** The suite runs on the host, so another target's arm is checked only by a command
  built for it.

- **A PR body that records a `--jit-compile-only` cross-target run also records, beside it, a
  run of that code on a box of the target, or the statement that no such box ran it.** A
  compile-only run shows the code emits, never that it runs.

- **A diff that adds work to, or moves work within, what `run_jit` (`daslib/llvm_jit_run.das`)
  or `run_jit_linked` (`daslib/llvm_jit_link.das`) runs between its first timer and the
  `LLVM JIT time:` report - in its own body or in any function it calls - prints a number that
  covers that work on every path that runs it: a line of its own, or the line of a phase that
  contains it, while that line still prints.** When one compile does the same work twice (a link
  that falls back to the emitter computes the plan a second time), each run prints its own number
  (`ARCHITECTURE.md#jit-timing-contract`).

- **A change that alters the machine code the JIT emits for a function the JIT cache serves -
  from the DLL cache or the split-obj cache - without changing any input the JIT cache keys fold,
  bumps `LLVM_JIT_CODEGEN_VERSION` (`daslib/llvm_jit_plan.das`)**
  (`ARCHITECTURE.md#codegen-tier`). The folded inputs are what `jit_dll_basename`
  (`daslib/llvm_jit_plan.das`) and the split-partition key in `run_split_codegen`
  (`daslib/llvm_jit_run.das`) fold; a key that does not change serves the old machine code back.

- **A diff that adds or changes an extern slot name - one a `UidNodes` getter
  (`daslib/llvm_dll_utils.das`) or an `FN_JIT_*` name function (`daslib/llvm_jit_plan.das`)
  builds - names the callee or type the slot binds by its mangled name, its mangled-name hash, or
  the key `Type` of the table a table helper serves, never by interpolating an AST node**
  (`ARCHITECTURE_CODEGEN_IDENTITY.md#extern-slot-names-key`). Every slot name folds into the cache
  key, and a node's text changes every run.

- **A diff that adds a kind of extern slot - a `UidNodes` getter (`daslib/llvm_dll_utils.das`) or
  `FN_JIT_*` name function (`daslib/llvm_jit_plan.das`) that `DllExternResolver` binds a slot
  through - adds to `other_slots` in `tests/_slot_names_fixture.das` a statement that makes the
  resolver bind a slot of that kind, in the same change.** `tests/llvm_jit_cache.das` folds
  `other_slots` in a second process and requires the same value, so a name that changes from run
  to run fails there.

- **A diff that adds an input that can differ between two functions of one compile, that the JIT
  emitter reads, and that the function's AOT hash does not cover - an input the interpreter
  never reads - folds it into `fold_jit_only_inputs` (`daslib/llvm_jit_plan.das`) in the same
  change, and the fold's result changes when the input moves from one function to another**
  (`ARCHITECTURE_JIT_ENTRY.md#hint-folds`). A `LLVM_JIT_CODEGEN_VERSION` bump re-keys once, so
  adding or removing the input on a function afterwards still serves the old machine code back.

- **A diff that adds a `require` line naming a `[llvm_code]` generator module outside this
  module to `daslib/llvm_user_modules.das` is a defect; that module's package
  joins the `llvm_code_generator` group from its own descriptor instead.**
  `daslib/llvm_user_modules.das` names no package, so a build that does not carry the package
  registers nothing and compiles unchanged.

- **A diff that puts a function on the jit finalizer's path - `free_jit_context`
  (`daslib/llvm_jit_link.das`) and every function it calls - marks that function `[no_jit]`.**
  The finalizer is program code, so a function without `[no_jit]` on its path joins every
  jitted program's DLL (`ARCHITECTURE_JIT_ENTRY.md#jit-set-exclusions`).

- **A function on the jit finalizer's path - `free_jit_context` (`daslib/llvm_jit_link.das`)
  and every function it calls - never calls a function written in das in another module; it
  calls the C++ externs that function wraps.** A das helper from another module cannot be
  marked `[no_jit]` from this module.

- **A diff that adds to a JIT cache key an input that is the same for every function of one
  compile folds it inside `jit_env_salt` (`daslib/llvm_jit_plan.das`), never directly into
  either JIT key - the DLL key or the split-obj key
  (`ARCHITECTURE_CODEGEN_IDENTITY.md#split-obj-cache`)** - salt feeds both keys, and an input
  folded into one but not the other links stale objects.

- **A macro under this module's `daslib/` that reads a file at compile time registers it with
  `add_module_cache_dependency` before any early return, in the same change**
  (`ARCHITECTURE.md#tune-sidecar-cache-pin`). An unpinned compile-time file read serves stale
  macro output from the module cache until an unrelated source file changes - silently.

- **A diff that adds a `-lib` entry point, or work to one, keeps three behaviors (`<P>` is
  the library's export prefix): a raise reaches the caller as a return value, never as an unwind;
  `<P>_create` returns null instead of aborting; and `<P>_destroy` calls the program's
  `[finalize]` and `[shutdown]` functions itself** (`ARCHITECTURE_LIB.md#lib-runtime-scope`). A
  library is called by code that cannot catch anything, and the runtime's own shutdown finds no
  jitted `[shutdown]` function.

- **A `-lib` build path a diff adds or changes that writes no artifact and exits 0 is a defect -
  exit non-zero instead.** A build rule reads the exit code, and a silent success lets
  it link the previous run's library against this run's header.

- **A diff that adds a top-level section to the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) updates `modules/dasLLAMA/dasllama/dasllama_exchange_schema.das` in
  the same change and keeps `modules/dasLLAMA/tests/test_exchange_schema.das` green** - the
  validator allow-lists sections, so a section it does not know fails every sidecar written
  after the diff at submission, and the checked-in corpus the test sweeps cannot show it.

- **A diff that adds an override knob, or gives one a new effect, adds the knob or the effect
  to the inventory in `ARCHITECTURE.md#override-knobs` and logs a line naming the knob where it
  takes effect (where a caller acts on it, when the diff only adds a way to read it) in the
  same change.** An override knob is an environment variable, a command-line flag, or an
  exported runtime setter that changes what a build compiles, tunes, or emits beyond the
  defaults of its output kind (`-jit`, `-exe`, `-lib`). Choosing the output kind is not a knob;
  choosing the target machine, or making a build write nothing, is. A `[tune]`-family or
  `[hint]` argument, written in source, is not a knob.

- **A host path a diff writes into the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) goes through `tilde_home` (`daslib/fio.das`, repo root); a host path
  it passes to a filesystem call stays raw.** No filesystem call resolves `~`, and a recorded
  path is read by other people, so it must not name the user whose run wrote it.

- **A diff that emits an instruction into the entry block of a function whose body das
  statements emit - the impl half of a generated function pair (the body the wrapper half
  calls), or a block body - gives it no debug location, and one it emits into a loop's latch
  (the block that jumps back to the loop head) gives it the loop's own line**
  (`daslib/llvm_jit.das`). Those are the two places the emitter fills out of das statement
  order, and an instruction that keeps whatever location was current when it was emitted moves a
  `break file:line` stop onto a statement that has not run
  (`ARCHITECTURE_DEBUG_INFO.md#di-variable-locations`).

- **A diff that adds a teardown step to `reset_jit_globals_after_failure`
  (`daslib/llvm_jit_run.das`) puts it after the `di_finalize()` call.** A DIBuilder writes into
  the LLVM module it was created for, so finalizing one whose module an earlier step already
  disposed of reads freed memory.

- **Never call `LLVMSetIsInBounds` - build the GEP in-bounds with `LLVMBuildInBoundsGEP2` (the
  `llvm_boost` wrapper's `inbounds` default) instead.** A constant-folded GEP is a
  `ConstantExpr`, and the setter's cast writes through the wrong type into it
  (`ARCHITECTURE.md#gep-constant-fold`).

- **A feature name a file under this module uses in a `requires=` list or a `g_target_*` tier
  gate (`daslib/llvm_jit_common.das`) has its cpuid line in `das_cpu_supports`
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
  `[test]` comparing the emitted result with the interpreted result over the operand range that
  emitter serves (every vector width for a vector emitter, every operand value for one over
  8-bit operands, each operand class its arms branch on otherwise), adding the comparison in
  the same change when none covers that range.** A twinned emitter is a `build_vector_*`
  emitter (`daslib/llvm_jit_intrin.das`) or an `[llvm_code]` generator under this module: the
  interpreter computes the same answer through a das body or a builtin, so the two can drift
  apart.

- **Weakening `REVIEW.das` (beside this file) is a defect** - dropping a check, shrinking a
  scanned set or the tracked-fixture directory list, widening an exemption list without the
  reason for the exempted input beside it, or a finding text that no longer names what failed.

- **A walk over the program's modules in this module's `daslib/` that leaves a module out by
  `moduleFlags.builtIn` alone is a defect - a promoted das module (`module X shared`) is
  builtIn too; a walk that means the C++ modules tests `builtIn && !promoted`**
  (`ARCHITECTURE_EXE.md#exe-global-init-walk`). A promoted module's global initializers are program
  code, and a walk that skips them leaves the address globals they need null in the exe.

- **A diff that builds a feature string for a machine that has a force knob - a
  `*_forced_plus_features` function in `daslib/llvm_jit_common.das` - appends the forced names
  AFTER the detected host features.** LLVM's `SubtargetFeatures` takes the last occurrence of a
  name, so a forced feature placed first is silently overridden by detection.
