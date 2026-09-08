# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture docs:
`ARCHITECTURE.md`, `ARCHITECTURE_TARGET_FEATURES.md`. Planned work: `DEBUGGING.md` (sec. Roadmap),
`fat_mode_plan.md`.

- **A change under `modules/dasLLVM/` runs the module-owned suite** (command and build gate:
  `tests/README.md` here). The suite is outside the core `tests/` sweep, so no other lane
  covers it.

- **A diff that adds or changes a branch keyed on what `get_platform_name()`,
  `get_architecture_name()`, `cpu_supports()`, or `host_llvm_feature()` returns runs the
  module-owned suite on a machine that takes the new branch.**

- **A test under `tests/` (beside this file) never creates, overwrites, or deletes a
  git-tracked path.**

- **A test under `tests/` here that writes at all - through its own filesystem calls, not a
  child process's - writes only under a directory it created for this process, and removes
  it** - a shared path under `build/` is one two concurrent runs collide on.

- **A test under `tests/` here that spawns a daslang child keeps the child's artifacts inside
  the directory it created for this process: `-output <dir>/...` for a `-exe` build,
  `-no-module-cache` or `-module-cache <dir>/...` for a run that compiles through the front-end
  cache, and `-jit-no-cache` or a pinned `jit_output_path` for a `-jit` run that executes the
  script.** A child writes its caches relative to the cwd otherwise, which is the tree two
  concurrent runs share.

- **A diff that adds or changes a branch on the target triple records in its PR body the
  cross-compile (`write_exe`) for that target that exercised the behavior.** The suite runs on
  the host, so a target-triple branch is checked only by the artifact built for that target.

- **A diff that adds work to, or moves work within, what `run_jit`
  (`daslib/llvm_jit_run.das`) executes - its own body or any callee - also prints an
  `LLVM JIT time:` number for that work: its own line, or the number of a phase that includes
  it, while that phase's line still prints** (phase inventory: `ARCHITECTURE.md` sec.1).
  Option resolution before the first timer, and log lines, are not work.

- **A change that can alter the machine code the JIT's DLL or split-obj cache serves back for
  identical inputs - IR generation, target-machine setup, a `[llvm_code]` generator body, or the
  call ABI the generated code binds: function signatures, the name scheme, the prologue, the
  externs the install phase binds - bumps `LLVM_JIT_CODEGEN_VERSION`**
  (`daslib/llvm_jit_run.das`); selecting among existing generators' `[llvm_code]` arguments, the
  `[tune]` stamping, is not such a change. The caches are addressed by the AST hashes and this
  constant, so such a change without the bump serves the old machine code back
  (`ARCHITECTURE.md` sec.1.2).

- **A diff that adds an environment or config input to a JIT cache key folds it inside
  `jit_env_salt` (`daslib/llvm_jit_run.das`), never directly into either JIT key - the DLL
  key or the split-obj key (`ARCHITECTURE.md` sec.2)** - salt feeds both keys, and a config
  folded into one but not the other links stale objects. Inputs that vary per function set
  (AOT hashes) fold into the key directly, not through the salt.

- **A macro under this module's `daslib/` that reads a file at compile time registers it with
  `add_module_cache_dependency` before any early return, in the same change**
  (`ARCHITECTURE.md` sec.5). An unpinned compile-time file read serves stale macro output
  from the module cache until an unrelated source file changes - silently.

- **A change to a `[tune]`-family annotation is reviewed with `skills/tune.md`.**

- **A change to the tune framework - `daslib/llvm_tune.das` or its tests - is reviewed with
  `skills/internal/llvm_tune_internals.md`.**

- **A test under `tests/` here whose child compiles through the front-end module cache - any
  child but a `-exe`, `-compile-only`, `-documentation`, debugger or AOT run - and asserts a
  line a macro prints at compile time, or its absence, spawns that child with
  `-no-module-cache`; a test whose subject is the cache itself pins its own file with
  `-module-cache <temp>` instead and never takes the flag.** The default cache replays the
  cached AST without re-running a macro, so a macro-time line - the `llvm_tune:` apply lines, a
  `[tune]`-family compile error - lands on the first run and never again and a silence
  assertion passes vacuously, while a line the backend or the runtime guard prints past the
  cache - the `LLVM JIT:` announce, the covered-box announce, `re-tuning (--tune)` - is
  re-produced every run and needs no flag.

- **A diff that adds a top-level section to the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) updates `modules/dasLLAMA/dasllama/dasllama_exchange_schema.das` in
  the same change and keeps `modules/dasLLAMA/tests/test_exchange_schema.das` green** - the
  validator allow-lists sections, so a section it does not know fails every newly minted
  sidecar at submission, and the checked-in corpus the test sweeps cannot show it. A new key
  inside an existing section passes the validator as it stands.

- **A diff introducing an override knob adds it to `ARCHITECTURE.md` sec.3's inventory in the
  same change.** An override knob is supplied at run time - an environment variable, a
  command-line flag, or an exported runtime setter - and changes what a run compiles, tunes,
  or emits beyond its defaults. Anything written in source - `[tune]`-family and `[hint]`
  annotation arguments - is a declaration, not an override.

- **A diff that adds an override knob, or gives one a new effect, also logs at least one line
  naming the knob where it takes effect.** A diff that only exposes the knob puts the line at
  the consumer instead, in that same diff.

- **Weakening `tests/llvm_env_registry.das` (beside this file) is a defect** - it bans
  environment reads outside `daslib/llvm_env.das`, requires every name read to be a declared
  `[EnvConfig]` field, and bans re-declared env helpers; dropping a scan, narrowing its
  `STRICT_ROOTS`, or widening its `SKIP_FILES` weakens it.

- **A host path a diff writes into the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) goes through `tilde_home` (`daslib/fio.das`, repo root)** - a
  recorded path is read by other people and must not name the user who minted it. A host path
  a diff passes to a filesystem call stays raw: no filesystem call resolves `~`.
  `tests/llvm_tune_manifest.das` here asserts a minted sidecar carries no home directory.

- **Never call `LLVMSetIsInBounds` - build the GEP in-bounds with `LLVMBuildInBoundsGEP2` (the
  `llvm_boost` wrapper's `inbounds` default) instead.** A constant-folded GEP is a
  `ConstantExpr`, and the setter's cast writes through the wrong type into it
  (`ARCHITECTURE.md#gep-constant-fold`).

- **A feature name used in a `requires=` list or a `g_target_*` tier gate
  (`daslib/llvm_jit_common.das`) has its cpuid line in `das_cpu_supports`
  (`src/builtin/module_builtin_runtime.cpp`, repo root) in the same diff**
  (`ARCHITECTURE_TARGET_FEATURES.md#x64-tier-gates`). A name the cpuid table does not know answers false on
  every box, so every perm that requires it silently declines to its fallback and no error names
  the cause.

- **An emitter under `daslib/` that uses a call's name as a KEY - an intrinsic-name fragment, a
  lookup-table key, a branch on one spelling - reads `expr.func.name`, never `expr.name`;
  `expr.name` stays only in a diagnostic or an LLVM value name, where it is what the user
  wrote.** The dispatch tables key off the declaration, so a call site can still spell itself
  module-qualified.

- **A diff that adds or changes a `build_vector_*` emitter (`daslib/llvm_jit_intrin.das`) emits
  each Horner step unfused, through `vmath_poly_step`, and calls `vmath_fma` only for the steps
  vecmath itself writes fused** (`ARCHITECTURE.md#vector-poly-fusion`). One fused step in a
  sign-alternating chain moves the last few bits of the result, and the interpreter and AOT
  answers do not move with it.

- **A diff that changes what an emitter whose daslang body is the reference implementation
  produces - the emitter itself, or which of its arms a call selects - also adds a cell
  comparing the emitted result with the interpreted result over the operand range that emitter
  serves (every vector width for a vector emitter, the full int8 lattice for a dot), and for a
  float emitter one asserting both answer NaN in the same lanes; a lowering only a cross target
  runs states in the PR body the artifact that compared them.** A clamp or a conversion written with
  ordered compares turns a NaN lane into a number, and an accuracy bound reads that as success;
  an IR-shape test names the instruction and never a number.

- **A change that makes `REVIEW.das` (beside this file) report fewer inputs is a defect:**
  dropping a check, shrinking a scanned set or a tracked-fixture directory (a guard over nothing),
  widening an exemption list without naming the exempted input's reason beside it, or a finding
  text that no longer names what failed. What the gate enforces is read from the gate itself.

- **A diff that builds an x64 or aarch64 machine's feature string appends the forced ones
  (`x64_forced_plus_features` / `arm64_forced_plus_features`, `daslib/llvm_jit_common.das`) AFTER
  the detected host features.** LLVM's `SubtargetFeatures` takes the last occurrence of a name, so
  a forced feature placed first is silently overridden by detection. A wasm machine has no force
  knob and no detected features, so the rule does not reach it.

**A `[test]` file under this module that carries a `require dasllama/...` line of its own answers
to `modules/dasLLAMA/tests/REVIEW.md` (repo root) as well** - its out-of-folder ledger row lives
there.
