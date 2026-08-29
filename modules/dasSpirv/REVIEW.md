# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Shared emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` - apply that
list with this one.

**A diff that adds an emitter capability - a name the emitter recognizes, an opcode it emits,
or a type it accepts, and a new cm2 tile shape counts - also adds its fixture under
`tests/spirv/` (repo root), in the same
change: a kernel in `tests/spirv/_spirv_common.das` that uses the capability, with a row in
`tests/spirv/_gen_golden.das` and its `_golden/<name>.txt`, or an assertion on the emitted
words.** A capability no fixture exercises is emitted by nothing the suite runs.

**A diff that adds a rejection path also adds its fixture under `tests/spirv/_fail_closed/`
(repo root) and asserts that fixture's error text in `tests/spirv/test_fail_closed.das`, in
the same change.** A rejection path is emitter code that refuses a construct with a compile
error.

**A diff that adds a capability a downstream consumer uses also adds a test there, in the
same change** - under `modules/dasVulkan/tests/integration/` or in
`modules/dasLLAMA/tests/test_vulkan_kernels.das` - that runs the kernel on a device.

**A device test's oracle is an independent CPU computation of the same result** - the kernel
body run on the CPU, the CPU twin the emitter's builtins mirror, or a plain CPU reference of the
same arithmetic - never an expectation re-spelled inline in the test.

**Weakening `tests/spirv/test_census.das` is a defect** - it holds every fixture opcode
declared and every declared opcode emitted, in both directions.

**A diff under `modules/dasSpirv` that edits a file under `modules/dasGlsl` or
`modules/dasOpenGL` is a defect** - dasSpirv copies dasGlsl's design, not its code.
