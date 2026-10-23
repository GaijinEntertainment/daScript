# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_COOPMAT.md`. Shared emitter rules:
`modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one. A file holding a SPIR-V
fixture - a function that compiles a shader and returns its emitted words - answers to
`tests/spirv/REVIEW.md` (repo root), wherever the diff puts it.

**A diff under this folder that makes the emitter lower a das program it did not lower before -
a construct it refused, or a name or struct the diff declares - or emit different words for a
program it already lowered, adds in the same change one fixture per such difference under
`tests/spirv/` (repo root) that asserts on the emitted words.** Words no fixture asserts can
change with every test still green. The fixture forms are `ARCHITECTURE.md#test-architecture`.

**A diff that adds emitter code refusing a construct the front end accepts also adds its
fixture under `tests/spirv/_fail_closed/` (repo root) and asserts that fixture's error text
in `tests/spirv/test_fail_closed.das`, in the same change.** A guard on a construct the front
end refuses first is reachable from no `.das` program, so no fixture can drive it.

**A diff that adds an emitter capability - a name the emitter recognizes, an opcode it emits,
or a type it accepts - leaves a device cell covering that capability after the change.** A
device cell is a test, in any module's tests, that runs a kernel using the capability on a
device. A fixture asserts words; only a device run shows the words compute the right result.

**A diff that teaches this emitter a new loop hint - a name in the bracketed annotation on a
`for` or `while`, like `unroll_full` - makes `append_loop_hint_operand` in
`modules/dasLLVM/daslib/llvm_jit.das` lower or accept that name, in the same change.** A kernel
body also compiles for the CPU through the JIT, and the JIT fails a hint name it does not know.

**A device cell - a test, in any module, that runs a kernel on a device - that a diff under this
folder adds or changes checks its result against a CPU result computed without the emitter: the
kernel body run on the CPU, a CPU body returning what the builtin's emitted form returns, or a
plain CPU reference of the same arithmetic, never a literal written into the test.** A literal
is read off the emitter's own output, so it passes whatever the emitter does.

**Weakening `REVIEW.das`'s check that no file under this folder `require`s, `#include`s, or
loads by path a file under `modules/dasGlsl` or `modules/dasOpenGL` is a defect.** dasSpirv
copies dasGlsl's design, not its code.

**A diff that adds emitter code writing instructions into an emitted SPIR-V function's first
block puts every `OpVariable` it writes ahead of every other instruction of that block, and
every other instruction it writes there after the block's last `OpVariable`.** SPIR-V requires a
function's `OpVariable`s to open its first block (`ARCHITECTURE_COOPMAT.md#direct-decode-call`),
and CI runs no `spirv-val` to catch an invalid module (`ARCHITECTURE.md#test-architecture`).

**A diff under this folder runs `tests/spirv` (repo root) on a machine where `spirv-val` is
found, through `VULKAN_SDK` or `PATH`, gets every cell passing with no cell skipping its
`spirv-val` check, and states that run in the PR body.** CI has no `spirv-val`, so a module the
validator rejects fails only on that machine.
