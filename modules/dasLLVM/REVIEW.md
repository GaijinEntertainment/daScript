# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `DEBUGGING.md` (§ Roadmap).

- **A change under `modules/dasLLVM/` runs the module-owned suite** — `bin/daslang -jit
  dastest/dastest.das -- --test modules/dasLLVM/tests` on an LLVM-enabled build. The suite is
  outside the core `tests/` sweep, so no other lane covers it; the build gate lives in
  `tests/README.md` here.

- **Work added to or moved within the JIT pipeline — `run_jit`, `run_split_codegen`
  (`llvm_jit_run.das`), or an artifact emitter (`llvm_jit_common.das`) — runs inside a timed
  phase of the `LLVM JIT time:` breakdown** (phase inventory: `ARCHITECTURE.md` §1). Option
  resolution before the first timer and log lines are not pipeline work.

- **A diff that breaks one timed phase into finer steps gives each step its own number in
  the log;** a parent label may keep printing the sum only while its steps print too.

- **New work inside an existing phase prints its own number when `ARCHITECTURE.md` §1 names
  it as a step;** otherwise it rides the phase's number.

- **A change to code that EMITS machine code — IR generation, target-machine setup, an
  `[llvm_code]` generator body, the jit call ABI — bumps `LLVM_JIT_CODEGEN_VERSION`**
  (`llvm_jit_run.das`). A change that only SELECTS among existing generators' `[llvm_code]`
  arguments — the `[tune]` stamping — needs no bump: stamped arguments fold into the cache
  keys per function.

- **A new environment or config input to the cache key folds inside `jit_env_salt`
  (`llvm_jit_run.das`), never directly into either key** — salt feeds both keys, and a config
  folded into one but not the other links stale objects. Inputs that vary per function set
  (AOT hashes) are key material, not salt.

- **A change to a `[tune]`-family annotation is reviewed with `skills/tune.md`** — the
  family's reference.

- **A change to what the tune sidecar EMITS — a new top-level section, or a value shape —
  updates the exchange validator in the same change and keeps its test green.**
  `modules/dasLLAMA/performance/exchange_schema.das` allow-lists exactly `kernels` /
  `runtime` / `provenance` / `race`; an emitted section it does not know makes every newly
  minted sidecar unsubmittable, and the checked-in corpus swept by
  `modules/dasLLAMA/tests/test_exchange_schema.das` cannot show it. A new provenance key
  needs no schema change — any identifier key with a render-safe string value validates.

- **A diff introducing an override knob adds it to `ARCHITECTURE.md` §3's inventory in the
  same change.** An override knob is readable from outside the code under review — an
  environment variable, a command-line flag, or an exported runtime setter — and changes what
  a run compiles, tunes, or emits beyond its defaults. `[tune]`-family annotation arguments
  are declarations, not overrides.

- **An override announces itself where it takes effect:** at least one logged line names the
  knob. Set-but-inert stays silent; an exposure-only diff defers the announce to its
  consumer, same change. A knob added, or given a new effect, without its announce is a
  defect.

- **An environment knob is an `[EnvConfig]` field in `llvm_env.das`, read as a `g_env_jit` /
  `g_env_tune` field — declare knobs there, never read raw.** The load-once/arm-children
  mechanism is `ARCHITECTURE.md` §3.

- **An ambient name — an environment variable the module reads but does not own — goes
  through `env_value_of` / `env_is_set`, with an `ambient_rows` entry backing every literal
  name; a `set_env_variable` names only a declared knob.**

- **`get_env_variable` / `has_env_variable` / literal-name `env_config_*` anywhere outside
  `llvm_env.das` in the module is a defect** — including the non-literal spelling
  `get_env_variable(expr)`, which only review catches; `tests/llvm_env_registry.das` (run
  per-PR by the extended checks) enforces the literal-name forms.

- **`ENVIRONMENT.md` is generated (`harness/gen_env_doc.das`); hand-editing it is a defect.**
  A change to `llvm_env.das` or the repo-root `daslib/env_registry.das` re-runs the generator
  and commits the diff; `tests/llvm_env_registry.das` fails on drift.
