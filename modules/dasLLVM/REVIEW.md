# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `DEBUGGING.md` (sec. Roadmap).

- **A change under `modules/dasLLVM/` runs the module-owned suite** (command and build gate:
  `tests/README.md` here). The suite is outside the core `tests/` sweep, so no other lane
  covers it.

- **A change gated on a platform - a `get_platform_name()` test or a target-triple branch -
  runs the module-owned suite on that platform.**

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

- **A diff that adds an environment or config input to the cache key folds it inside
  `jit_env_salt` (`daslib/llvm_jit_run.das`), never directly into either cache key - the DLL
  key or the split-obj key (`ARCHITECTURE.md` sec.2)** - salt feeds both keys, and a config
  folded into one but not the other links stale objects. Inputs that vary per function set
  (AOT hashes) are key material, not salt.

- **A change to a `[tune]`-family annotation is reviewed with `skills/tune.md`.** A change to
  the framework itself - `daslib/llvm_tune.das` or its tests - is reviewed with
  `skills/internal/llvm_tune_internals.md`.

- **A diff that adds a new top-level section, or a new value shape inside one, to the tune
  sidecar (`<app>.tune.json`, written by `daslib/llvm_tune.das`) updates
  `modules/dasLLAMA/dasllama/dasllama_exchange_schema.das` in the same change and keeps
  `modules/dasLLAMA/tests/test_exchange_schema.das` green** - the validator allow-lists
  sections, so a section it does not know fails every newly minted sidecar at submission, and
  the checked-in corpus the test sweeps cannot show it.

- **A diff introducing an override knob adds it to `ARCHITECTURE.md` sec.3's inventory in the
  same change.** An override knob is readable from outside the code under review - an
  environment variable, a command-line flag, or an exported runtime setter - and changes what
  a run compiles, tunes, or emits beyond its defaults. `[tune]`-family annotation arguments
  are declarations, not overrides.

- **A diff that adds an override knob, or gives one a new effect, also logs at least one line
  naming the knob where it takes effect.** An override that is set but changes nothing needs
  no line; a diff that only exposes the knob defers the line to its consumer, in the same
  change.

- **Never read an environment variable by a computed name - `get_env_variable(expr)` /
  `has_env_variable(expr)` outside `daslib/llvm_env.das`. Spell the name as a literal through
  the declared forms, or declare the knob, instead** (the literal-name forms are
  scanner-enforced by `tests/llvm_env_registry.das`; weakening that test is a defect).
