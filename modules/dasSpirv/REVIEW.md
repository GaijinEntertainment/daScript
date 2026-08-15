# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist. Shared
emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` — apply that list with this one.**
Architecture doc: `MASTERPLAN.md`.

- **A new emitter capability ships its word-level tests in `tests/spirv/`:** a `_golden/`
  disassembly or equivalence fixture, and presence in the opcode census.

- **A new rejection path ships its `_fail_closed/` fixture,** with its error needle asserted
  in `tests/spirv/test_fail_closed.das`.

- **A capability that serves a downstream consumer lands with a behavioral arm there** —
  dasVulkan `tests/integration` or the dasLLAMA vulkan kernel suite — where the kernel runs
  on a device against its CPU oracle.

- **The opcode census is two-directional:** every opcode a fixture emits is declared, and
  every declared opcode is emitted by some fixture.
