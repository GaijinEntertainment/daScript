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
`LLVM JIT time:` breakdown** — new work landing between phases, before the first timer, or
after the last one is a defect; the phase inventory the breakdown must match is
`ARCHITECTURE.md` §1 (§1.1 carries the split-mode mapping). A path that runs no pipeline
phase and prints no breakdown (an early return, an empty function set) is out of scope, and
a diagnostic log line is not work.

**A phase split stays split in the log.** When a diff extracts a timed phase into finer
steps, each resulting step reports its own number under a `LLVM JIT time:` label; a parent
entry may keep covering the sum only if the finer steps also print (`ARCHITECTURE.md` §1.1).
Leaving an aggregate label silently absorbing new sub-steps is a defect.

## Codegen identity

**A change under `daslib/` to IR generation, target-machine setup, `[llvm_code]` generators,
or the jit call ABI bumps `LLVM_JIT_CODEGEN_VERSION`** (`llvm_jit_run.das`) — the DLL cache
key folds AST hashes but cannot see emitter behavior, so changed emission without a bump
silently serves stale code from cache (`ARCHITECTURE.md` §2).

## Overrides

**An override announces itself where it changes the outcome.** The knobs this governs are the
inventory in `ARCHITECTURE.md` §3 (the `DAS_TUNE_*` family, `--tune`, `tune_suppress_mint`,
under `llvm_jit_run.das` / `llvm_jit_common.das` / `llvm_tune.das`); at the point one changes
what a run compiles, tunes, or emits, at least one line names the knob — set-but-inert stays
silent, per-scope repeats are fine, and a diff that only exposes the override bit (a
`*_overridden` query) discharges this when the announce lands at the consumer in the same
change. A diff that adds a knob to that surface, or gives one a new effect, without its
announce is a defect.
