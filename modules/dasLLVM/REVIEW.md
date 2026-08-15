# dasLLVM Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `DEBUGGING.md` (§ Roadmap).

- **Work added to or moved within the JIT pipeline — `run_jit`, `run_split_codegen`
  (`llvm_jit_run.das`), or an artifact emitter (`llvm_jit_common.das`) — runs inside a timed
  phase of the `LLVM JIT time:` breakdown** (phase inventory: `ARCHITECTURE.md` §1). Option
  resolution before the first timer and log lines are not pipeline work.

- **A diff that breaks one timed phase into finer steps gives each step its own number in
  the log;** a parent label may keep printing the sum only while its steps print too.

- **New work inside an existing phase prints its own number when `ARCHITECTURE.md` §1 names
  it as a step;** otherwise it rides the phase's number.

- **A change under `modules/dasLLVM/daslib/` to IR generation, target-machine setup,
  `[llvm_code]` generators, or the jit call ABI bumps `LLVM_JIT_CODEGEN_VERSION`**
  (`llvm_jit_run.das`) — the cache self-invalidates on program changes, not emitter changes.

- **A new environment or config input to the cache key folds inside `jit_env_salt`
  (`llvm_jit_run.das`), never directly into either key** — salt feeds both keys, and a config
  folded into one but not the other links stale objects. Inputs that vary per function set
  (AOT hashes) are key material, not salt.

- **A diff introducing an override knob — an env variable or setting that changes what a run
  compiles, tunes, or emits — adds it to `ARCHITECTURE.md` §3's inventory in the same change.**

- **An override announces itself where it takes effect:** at least one logged line names the
  knob. Set-but-inert stays silent; an exposure-only diff defers the announce to its
  consumer, same change. A knob added, or given a new effect, without its announce is a
  defect.

- **No raw environment access outside `llvm_env.das` — declare a knob there instead.**
  `get_env_variable` / `has_env_variable` / literal-name `env_config_*` anywhere else in the
  module is a defect —
  including the non-literal spelling `get_env_variable(expr)`, the case only review catches;
  `tests/llvm_env_registry.das` enforces the rest.

- **`ENVIRONMENT.md` is generated (`harness/gen_env_doc.das`); hand-editing it is a defect.**
  A change to `llvm_env.das` or the repo-root `daslib/env_registry.das` re-runs the generator
  and commits the diff; `tests/llvm_env_registry.das` fails on drift.
