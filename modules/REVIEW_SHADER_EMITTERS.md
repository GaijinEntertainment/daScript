# Shared emitter rules - dasSpirv and dasMetal

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasMetal/MASTERPLAN.md` and `modules/dasSpirv/MASTERPLAN.md`.

**These rules bind both emitters; each module's `REVIEW.md` routes here and applies this file
together with its own list.**

- **Kernel bodies are ordinary das: what cannot run on the CPU does not enter the emitter.**
  Every kernel, and every function it calls, runs on the CPU - that CPU run is the test
  oracle.

- **A diff that adds or changes an emitter builtin ships a CPU body whose result matches the
  emitted form's, argument for argument.**

- **Everything outside the lowered subset is a clean compile error naming the construct** -
  never a silently wrong kernel.

- **A shape constant never enters a kernel as a runtime value: tile rows, tile cols,
  cooperating simdgroups, and staged chunk depth are call-site constants, and a K width that
  streams at runtime rides the emitter's runtime-extent descriptor (`dynamic_extent` on
  Metal).** The capability ships a specialization path beside it, or the module's
  `MASTERPLAN.md` records that it cannot.

- **A claim about emitted shape is checked in the emitted words or text, never the das
  source.**

- **A kernel-model capability lands on both emitters, or the asymmetry is recorded** in
  the shared ledger (`modules/dasMetal/MASTERPLAN.md`).

- **A rail diff that consumes a new `daslib/shader_lingua_franca` declaration ships that
  rail's lowering of it, or the fail-closed fixture that rejects it by name, in the same
  change.** A declaration in that module is granted to every shader rail.
