# dasLLVM Code Review Checklist

Run this list on every dasLLVM change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `README.md` for the reason behind it; it may
not require that section to be read before the criterion can be applied. If an entry needs
code-reading or prior knowledge, it is not a review criterion — move it to `README.md`
and leave a one-line criterion here.

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how
  to write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything
  that needs a stable reference lives in `README.md`, which is numbered for that purpose.
- **Cite files by name; cite `README.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside ("the way the X family does it") has no such excuse: nothing keeps it
  in sync with the code, and a stale example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `README.md`.** No history, no PR numbers, no direction of travel; planned
  work lives in the follow-up ledgers.

## Pipeline timing

**Every jit-backend pipeline phase is timed.** A diff that adds a phase to `run_jit`
(`llvm_jit_run.das`) or to the artifact emitters (`write_dll` / `write_exe` / `write_wasm` in
`llvm_jit_common.das`) also adds that phase's elapsed wall to the `LLVM JIT time:` breakdown
printed under `log_compile_time` — a phase that runs but never appears in the breakdown is a
defect, because an untimed phase is invisible exactly when someone is hunting where compile
time went.

**A phase split stays split in the log.** When a timed section is extracted into separate
steps — emission producing several objects instead of one, a link gaining stages — each
resulting step reports its own number in the `LLVM JIT time:` breakdown; leaving the old
aggregate label covering the new steps is a defect.

## Codegen identity

**An emitter change that alters generated code for fixed inputs bumps
`LLVM_JIT_CODEGEN_VERSION`** (`llvm_jit_run.das`) — jit DLLs are content-addressed and cached,
so changed emission without a bump silently serves stale code from cache.
