# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist. Shared
emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one.**
Architecture doc: `ARCHITECTURE.md`.

- **A diff that adds an emitter capability also adds its tests under `tests/spirv/` (repo
  root), in the same change:** a `_golden/` disassembly or equivalence fixture, and presence
  in the opcode census.

- **A diff that adds a rejection path also adds its fixture under `tests/spirv/_fail_closed/`
  (repo root) and asserts that fixture's error text in `tests/spirv/test_fail_closed.das`, in
  the same change.** A rejection path is emitter code that refuses a construct with a compile
  error.

- **A diff that adds a capability a downstream consumer uses also adds a test there, in the
  same change** - in dasVulkan `tests/integration` or the dasLLAMA vulkan kernel suite - that
  runs the kernel on a device and compares the result against the same computation run on the
  CPU.

- **A diff that leaves a fixture emitting an opcode the opcode census does not declare, or
  leaves the census declaring an opcode no fixture emits, is a defect.**

- **A diff under `modules/dasSpirv` that edits a file under `modules/dasGlsl` or
  `modules/dasOpenGL` is a defect** - dasSpirv copies dasGlsl's design, not its code.
