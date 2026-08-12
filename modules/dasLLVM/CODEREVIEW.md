# dasLLVM Code Review Checklist

Run this list on every dasLLVM change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `ARCHITECTURE.md` for the reason behind it; it may
not require that section to be read before the criterion can be applied. If an entry needs
code-reading or prior knowledge, it is not a review criterion — move it to `ARCHITECTURE.md`
and leave a one-line criterion here.

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how
  to write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything
  that needs a stable reference lives in `ARCHITECTURE.md`, which is numbered for that purpose.
- **Cite files by name; cite `ARCHITECTURE.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside ("the way the X family does it") has no such excuse: nothing keeps it
  in sync with the code, and a stale example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `ARCHITECTURE.md`.** No history, no PR numbers, no direction of travel; planned
  work lives in the follow-up ledgers.

## Pipeline timing

**A diff that adds or moves work inside `run_jit` or `run_split_codegen` (`llvm_jit_run.das`)
or the artifact emitters (`write_dll` / `write_exe` / `write_wasm` / `emit_object_only` /
`link_dll_from_objects` in `llvm_jit_common.das`) keeps that work inside a timed phase of the
`LLVM JIT time:` breakdown** — new work landing between phases or after the last timer is a
defect; the phase inventory the breakdown must match is `ARCHITECTURE.md` §1 (§1.1 carries
the split-mode mapping). Out of scope: a path that runs no pipeline phase and prints no
breakdown (an early return, an empty function set), `run_jit`'s pre-timer option/policy
resolution prologue, and a diagnostic log line.

**A phase split stays split in the log.** When a diff extracts a timed phase into finer
steps, each resulting step reports its own number under a `LLVM JIT time:` label; a parent
entry may keep covering the sum only if the finer steps also print (`ARCHITECTURE.md` §1.1).
Leaving an aggregate label silently absorbing the finer steps of a split it just performed
is a defect; new work added inside an existing phase reports its own number when the phase's
doc names it as a separate step, and rides the aggregate otherwise.

## Codegen identity

**A change under `daslib/` to IR generation, target-machine setup, `[llvm_code]` generators,
or the jit call ABI bumps `LLVM_JIT_CODEGEN_VERSION`** (`llvm_jit_run.das`) — the cache keys
(the DLL key and the split obj-cache partition keys) fold AST hashes but cannot see emitter
behavior, so changed emission without a bump silently serves stale code from cache
(`ARCHITECTURE.md` §2, §2.1).

**A new environment or config cache-key input folds inside `jit_env_salt`** (`llvm_jit_run.das`),
never directly into `jit_dll_basename` or the partition-key chain in `run_split_codegen` — the
helper is shared by both keys, and a config component folded into one key but not the other
lets a config change link stale cached partition objects. Per-function-set inputs (AOT hashes,
name folds, the hint folds) fold into each key directly; they are not salt.

## Overrides

**An override announces itself where it changes the outcome.** The knobs this governs are
those matching `ARCHITECTURE.md` §3's definition — an escape that changes what a run compiles,
tunes, or emits beyond its defaults, under `llvm_jit_run.das` / `llvm_jit_common.das` /
`llvm_tune.das` — and a diff introducing such a knob adds it to §3's inventory in the same
change; at the point one changes
what a run compiles, tunes, or emits, at least one line names the knob — set-but-inert stays
silent, per-scope repeats are fine, and a diff that only exposes the override bit (a
`*_overridden` query) discharges this when the announce lands at the consumer in the same
change. A diff that adds a knob to that surface, or gives one a new effect, without its
announce is a defect.

**No raw environment access outside `llvm_env.das`.** An environment knob is an `[EnvConfig]`
field there: JIT knobs read as `g_env_jit` fields; tune knobs (live-read by the set-then-latch
contract) and ambient names go through `env_value_of` / `env_is_set`, with a declaration or an
`ambient_rows` entry backing every literal name. `get_env_variable` / `has_env_variable` /
literal-name `env_config_*` anywhere else in the module is a defect. `ENVIRONMENT.md` is
generated from the declarations (`harness/gen_env_doc.das`) — hand-editing it is a defect, and
a declaration change regenerates it in the same change; `tests/llvm_env_registry.das` enforces
the lot.
