# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `DEBUGGING.md` (sec. Roadmap).

- **A change under `modules/dasLLVM/` runs the module-owned suite** (command and build gate:
  `tests/README.md` here). The suite is outside the core `tests/` sweep, so no other lane
  covers it.

- **A change whose behavior depends on the host - a branch on `get_platform_name()`,
  `get_architecture_name()`, or `cpu_supports()` - runs the module-owned suite on a machine
  matching that condition.**

- **A test under `tests/` never creates, overwrites, or deletes a path that git tracks - it
  copies the fixtures it needs into a temp directory it makes and writes to the copy.**

- **A diff that adds or changes a branch on the target triple records in its PR body the
  cross-compile (`write_exe`) for that target that exercised the behavior.** The suite runs on
  the host, so a target-triple branch is checked only by the artifact built for that target.

- **A diff that adds work to, or moves work within, what `run_jit`
  (`daslib/llvm_jit_run.das`) executes - its own body or any callee - also prints an
  `LLVM JIT time:` number for that work: its own line, or the number of a phase that includes
  it, while that phase's line still prints** (phase inventory: `ARCHITECTURE.md` sec.1).
  Option resolution before the first timer, and log lines, are not work.

- **A change that can alter the machine code emitted for identical inputs bumps
  `LLVM_JIT_CODEGEN_VERSION`** (`daslib/llvm_jit_run.das`; what counts as emitting:
  `ARCHITECTURE.md` sec.1.2). Selecting among existing generators' `[llvm_code]` arguments -
  the `[tune]` stamping - is not such a change: stamped arguments fold into the cache keys
  per function.

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

- **A diff that adds a top-level section to the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`), or a new key or value type inside an existing section, updates
  `modules/dasLLAMA/dasllama/dasllama_exchange_schema.das` in the same change and keeps
  `modules/dasLLAMA/tests/test_exchange_schema.das` green** - the validator allow-lists
  sections, so a section it does not know fails every newly minted sidecar at submission, and
  the checked-in corpus the test sweeps cannot show it.

- **A diff introducing an override knob adds it to `ARCHITECTURE.md` sec.3's inventory in the
  same change.** An override knob is supplied at run time - an environment variable, a
  command-line flag, or an exported runtime setter - and changes what a run compiles, tunes,
  or emits beyond its defaults. Anything written in source - `[tune]`-family and `[hint]`
  annotation arguments - is a declaration, not an override.

- **A diff that adds an override knob, or gives one a new effect, also logs at least one line
  naming the knob where it takes effect.** A diff that only exposes the knob puts the line at
  the consumer instead, in that same diff.

- **Never read an environment variable by a computed name - `get_env_variable(expr)` /
  `has_env_variable(expr)` outside `daslib/llvm_env.das`. Spell the name as a literal in
  `env_value_of("NAME")` / `env_is_set("NAME")`, or declare it as an `[EnvConfig]` field in
  `daslib/llvm_env.das` and read the `g_env_*` field, instead** (the literal-name forms are
  scanner-enforced by `tests/llvm_env_registry.das` here; weakening that test is a defect).

- **A host path a diff writes into the tune sidecar (`<app>.tune.json`, written by
  `daslib/llvm_tune.das`) goes through `tilde_home` (`daslib/fio.das`, repo root)** - a
  recorded path is read by other people and must not name the user who minted it. A host path
  a diff passes to a filesystem call stays raw: no filesystem call resolves `~`.
  `tests/llvm_tune_manifest.das` here asserts a minted sidecar carries no home directory.

- **Never call `LLVMSetIsInBounds` - build the GEP in-bounds with `LLVMBuildInBoundsGEP2` (the
  `llvm_boost` wrapper's `inbounds` default) instead.** A constant-folded GEP is a
  `ConstantExpr`, and the setter's cast writes through the wrong type into it
  (`ARCHITECTURE.md#gep-constant-fold`).

- **A feature name used in a `requires=` list or a `g_target_x64_*` gate has its cpuid line in
  `das_cpu_supports` (`src/builtin/module_builtin_runtime.cpp`) in the same diff**
  (`ARCHITECTURE.md#x64-tier-gates`). A name the cpuid table does not know answers false on
  every box, so every perm that requires it silently declines to its fallback and no error names
  the cause.

- **A diff that adds or changes a `build_vector_*` emitter on the inline-polynomial rail
  (`daslib/llvm_jit_intrin.das`) emits every Horner step vecmath writes unfused, through
  `vmath_poly_step`, and reaches for `vmath_fma` only where vecmath itself fuses. It also ships
  that emitter's cells in `tests/llvm_vector_math.das`: a divergence arm comparing the emitted
  form lane for lane with the interpreted twin, at every vector width the emitter serves, and a
  NaN-lane cell asserting the two rails agree on which lanes come back NaN**
  (`ARCHITECTURE.md#vector-poly-fusion`). One contracted step in a sign-alternating chain moves
  the result by several ulp, the rail's point is that the three tiers agree, and a clamp or a
  conversion written with ordered compares replaces a NaN lane with a number that every accuracy
  bound reads as success.

- **Weakening `REVIEW.das` (beside this file) is a defect:** dropping a check, dropping a
  directory from its tracked-fixture list, or a finding text that no longer names what failed.
  What the gate enforces is read from the gate itself; each check's finding text states its own
  rule.
