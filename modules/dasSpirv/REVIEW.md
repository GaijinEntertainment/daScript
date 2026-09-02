# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Shared emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` - apply that
list with this one. A SPIR-V fixture - a file that compiles a shader and asserts on its emitted
words - answers to `tests/spirv/REVIEW.md` (repo root), wherever the diff puts it.

**A diff that changes what the emitter emits for any das program - a program it used to reject
now compiles, or a program's emitted words change - also adds, in the same change, a fixture
under `tests/spirv/` (repo root) that exercises the change and asserts on the emitted words.**
Emitted words no fixture asserts are produced by nothing the suite runs. The fixture forms are
`ARCHITECTURE.md` section 4.

**A diff that adds a rejection path also adds its fixture under `tests/spirv/_fail_closed/`
(repo root) and asserts that fixture's error text in `tests/spirv/test_fail_closed.das`, in
the same change.** A rejection path is emitter code that refuses a construct with a compile
error.

**A diff that adds an emitter capability - a name the emitter recognizes, an opcode it emits,
or a type it accepts - that a downstream consumer uses also adds, in the same change, a test in
that consumer's device suite - `modules/dasVulkan/tests/integration/` or
`modules/dasLLAMA/tests/test_vulkan_kernels.das` - that runs a kernel using it on a device
against an independent CPU computation of the same result: the kernel body run on the CPU, the
CPU body that returns what the builtin's emitted form returns, or a plain CPU reference of the
same arithmetic, never an expectation re-spelled inline in the test.** A fixture asserts words;
only a device run shows the words compute.

**A diff under `modules/dasSpirv` that edits a file under `modules/dasGlsl` or
`modules/dasOpenGL` is a defect** - dasSpirv copies dasGlsl's design, not its code.
