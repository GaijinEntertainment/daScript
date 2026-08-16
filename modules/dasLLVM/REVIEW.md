# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `DEBUGGING.md` (§ Roadmap).

- **A change under `modules/dasLLVM/` runs the module-owned suite** (command and build gate:
  `tests/README.md` here). The suite is outside the core `tests/` sweep, so no other lane
  covers it.

- **Work added to or moved within the JIT pipeline — `run_jit`, `run_split_codegen`
  (`daslib/llvm_jit_run.das`), or an artifact emitter (`daslib/llvm_jit_common.das`) —
  runs inside a timed phase of the `LLVM JIT time:` breakdown** (phase inventory:
  `ARCHITECTURE.md` §1). Option resolution before the first timer and log lines are not
  pipeline work.

- **Work added to or split out of a timed phase prints its own `LLVM JIT time:` number, or
  the phase's number covers it and the phase's line still prints.**

- **A change to code that EMITS machine code — any file in
  `tests-cpp/small/test_jit_emitter_pin.cpp`'s `EMITTER_FILES` set (repo root) — bumps
  `LLVM_JIT_CODEGEN_VERSION`** (`daslib/llvm_jit_run.das`). A change that only SELECTS
  among existing generators' `[llvm_code]` arguments — the `[tune]` stamping — needs no
  bump: stamped arguments fold into the cache keys per function. What counts as emitting is
  `ARCHITECTURE.md` §1.2.

- **Weakening `tests-cpp/small/test_jit_emitter_pin.cpp` (repo root) is a defect** — it is
  what enforces the bump trigger: a change to an `EMITTER_FILES` file bumps
  `LLVM_JIT_CODEGEN_VERSION`.

- **A new environment or config input to the cache key folds inside `jit_env_salt`
  (`daslib/llvm_jit_run.das`), never directly into either cache key — the DLL key or the
  split-obj key (`ARCHITECTURE.md` §2)** — salt feeds both keys, and a config
  folded into one but not the other links stale objects. Inputs that vary per function set
  (AOT hashes) are key material, not salt.

- **A change to a `[tune]`-family annotation is reviewed with `skills/tune.md`** — the
  family's reference. A change to the framework itself — `daslib/llvm_tune.das` or its
  tests — is reviewed with `skills/internal/llvm_tune_internals.md`.

- **A diff that adds a new top-level section, or a new value shape inside one, to the tune
  sidecar (`<app>.tune.json`, written by `daslib/llvm_tune.das`) updates
  `modules/dasLLAMA/performance/exchange_schema.das` in the same change and keeps
  `modules/dasLLAMA/tests/test_exchange_schema.das` green** — the validator allow-lists
  sections, so one it does not know fails every newly minted sidecar at submission, and
  the checked-in corpus the test sweeps cannot show it.

- **A diff introducing an override knob adds it to `ARCHITECTURE.md` §3's inventory in the
  same change.** An override knob is readable from outside the code under review — an
  environment variable, a command-line flag, or an exported runtime setter — and changes what
  a run compiles, tunes, or emits beyond its defaults. `[tune]`-family annotation arguments
  are declarations, not overrides.

- **An override announces itself where it takes effect:** at least one logged line names the
  knob. Set-but-inert stays silent; an exposure-only diff defers the announce to its
  consumer, same change. A knob added, or given a new effect, without its announce is a
  defect.

- **A computed-name env read — `get_env_variable(expr)` / `has_env_variable(expr)` outside
  `daslib/llvm_env.das` — is a defect only review catches: spell the name as a literal
  through the declared forms, or declare the knob, instead** (the literal-name forms are
  scanner-enforced by `tests/llvm_env_registry.das`; weakening that test is a defect).
