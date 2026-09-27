# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_COOPMAT.md`. Shared emitter rules:
`modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one. A SPIR-V fixture - a file
that compiles a shader and asserts on its emitted words - answers to `tests/spirv/REVIEW.md`
(repo root), wherever the diff puts it.

**A diff under this folder that makes the emitter accept a das program it refused before, or
changes the words it emits for a program it already accepted, adds a fixture under
`tests/spirv/` (repo root) for each such difference, in the same change, that compiles a program
showing it and asserts on the emitted words.** Emitted
words no fixture asserts are produced by nothing the suite runs. The fixture forms are
`ARCHITECTURE.md#test-architecture`.

**A diff that declares a struct the emitter recognizes by name and lowers to a type whose storage
exists only on the device - a tile, tensor, layout, sampler, or image - adds a fixture under
`tests/spirv/` (repo root) that exercises the declaration and asserts on the emitted words, in the
same change.** A declaration no fixture drives is lowered by nothing the suite runs.

**A diff that adds emitter code refusing a construct a `.das` program can compile to also adds
its fixture under `tests/spirv/_fail_closed/` (repo root) and asserts that fixture's error text
in `tests/spirv/test_fail_closed.das`, in the same change.** A guard on a construct the front
end refuses first is reachable from no `.das` program, so no fixture can drive it.

**A diff that adds an emitter capability - a name the emitter recognizes, an opcode it emits,
or a type it accepts - leaves a device cell covering that capability after the change.** A
device cell is a test, in any module's tests, that runs a
kernel using the capability on a device. A fixture asserts words; only a device run shows the
words compute.

**A diff that adds a per-loop hint name to this emitter's accepted set leaves that name known to
`append_loop_hint_operand` in `modules/dasLLVM/daslib/llvm_jit.das` - lowered or accepted by
name - in the same change.** A kernel body also compiles for the CPU through the JIT, and the JIT
fails a hint name it does not know.

**A diff under this folder that adds or changes an emitter capability judges every device cell
it adds or changes for that capability - wherever the diff puts the cell - against a CPU result
computed independently of the emitter: the kernel body run on the CPU, a CPU body that returns
what the builtin's emitted form returns, or a plain CPU reference of the same arithmetic, never
an expectation re-spelled inline in the test.** An inline expectation is read off the emitter's
own output, so it passes whatever the emitter does.

**A diff that makes a file under this folder `require`, `#include`, or load by path any file
under `modules/dasGlsl` or `modules/dasOpenGL` is a defect - write dasSpirv's own implementation
under this folder instead.** dasSpirv copies dasGlsl's design, not its code.

**A diff that adds an emit into an emitted function's entry block puts every `OpVariable` it
emits ahead of every other instruction of that block, and every other instruction it emits there
after the block's last `OpVariable`.** SPIR-V requires every `OpVariable` of a block to
lead the block (`ARCHITECTURE_COOPMAT.md#direct-decode-call`), and CI runs no `spirv-val` to
catch an invalid module (`ARCHITECTURE.md#test-architecture`).

**A diff under `modules/dasSpirv` runs `tests/spirv` locally on a box that resolves `spirv-val`
and names the run in the PR.** CI resolves no `spirv-val`, so a module the validator rejects
reds nowhere but on that box. The run's command and what green means:
`skills/internal/tests_in_repo.md`, the emitter suite section.
