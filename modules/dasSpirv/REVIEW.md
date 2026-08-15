# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`MASTERPLAN.md`.

- **A new emitter capability ships its word-level tests in `tests/spirv/`:** a `_golden/`
  disassembly or equivalence fixture, and presence in the opcode census.

- **A new rejection path ships its `_fail_closed/` fixture,** with its error needle asserted
  in `tests/spirv/test_fail_closed.das`.

- **A capability that serves a downstream consumer lands with a behavioral arm there** —
  dasVulkan `tests/integration` or the dasLLAMA vulkan kernel suite — where the kernel runs
  on a device against its CPU oracle.

- **The opcode census is two-directional:** the union of opcodes emitted across all fixtures
  equals the declared supported set — a new opcode joins the declared set AND some fixture
  emits it.

- **Kernel bodies are ordinary das:** every kernel, and every function it calls, runs on the
  CPU with identical semantics — that CPU run is the test oracle. A construct that cannot
  run on the CPU has no place in the emitter.

- **Anything outside the lowered subset produces a clean compile error naming the
  construct** — never a silently wrong blob.

- **A shape constant does not travel into a kernel as a runtime value** (a size parameter, a
  selector field, anything a driver compiler would have to inline-and-fold to recover): the
  capability ships a specialization path beside it, or `MASTERPLAN.md` records that it cannot.

- **A test of emitted shape asserts the literal in the words or disassembly,** never the das
  source.

- **A kernel-model capability lands on both backends — here and `modules/dasMetal` — or the
  asymmetry is recorded** in the shared ledger in `modules/dasMetal/MASTERPLAN.md`.
