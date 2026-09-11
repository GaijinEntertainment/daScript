# dasSpirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_COOPMAT.md`. Shared emitter rules:
`modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one. A SPIR-V fixture - a file
that compiles a shader and asserts on its emitted words - answers to `tests/spirv/REVIEW.md`
(repo root), wherever the diff puts it.

**A diff that changes what the emitter emits for any das program - a program it used to reject
now compiles, or a program's emitted words change - also adds, in the same change, a fixture
under `tests/spirv/` (repo root) for each such change that exercises it and asserts on the
emitted words.** Emitted words no fixture asserts are produced by nothing the suite runs. The
fixture forms are `ARCHITECTURE.md` section 4.

**A diff that declares a struct the emitter recognizes by name and lowers to a device-side type,
or a builtin overload over one, adds a fixture under `tests/spirv/` (repo root) that exercises
the declaration and asserts on the emitted words, in the same change.** A declaration no fixture
drives is lowered by nothing the suite runs.

**A diff that adds emitter code refusing a construct a `.das` program can compile to also adds
its fixture under `tests/spirv/_fail_closed/` (repo root) and asserts that fixture's error text
in `tests/spirv/test_fail_closed.das`, in the same change.** A guard on a construct the front
end refuses first is reachable from no `.das` program, so no fixture can drive it.

**A diff that adds an emitter capability - a name the emitter recognizes, an opcode it emits,
or a type it accepts - that a downstream consumer uses leaves a device cell covering that
capability after the change.** A device cell runs a kernel using the capability on a device, in
`modules/dasVulkan/tests/integration/` or `modules/dasLLAMA/tests/test_vulkan_kernels.das`. A
fixture asserts words; only a device run shows the words compute.

**A diff under this folder that adds or changes a device cell for a capability of this emitter -
wherever the diff puts that cell - judges the cell's result against a CPU result computed
independently of the emitter: the kernel body run on the CPU, a CPU body that returns what the
builtin's emitted form returns, or a plain CPU reference of the same arithmetic, never an
expectation re-spelled inline in the test.** An inline expectation is read off the emitter's own
output, so it passes whatever the emitter does.

**A diff under `modules/dasSpirv` that edits a file under `modules/dasGlsl` or
`modules/dasOpenGL` is a defect** - dasSpirv copies dasGlsl's design, not its code.

**A diff that adds an `OpVariable` emit to an emitted function's entry block puts it ahead of
every non-`OpVariable` instruction of that block, and an instruction it emits into that block
goes after the block's last `OpVariable`.** SPIR-V requires every `OpVariable` of a block to
lead the block (`ARCHITECTURE_COOPMAT.md` section 3.5), and CI runs no `spirv-val` to catch an
invalid module (`ARCHITECTURE.md` section 4).

**A diff under `modules/dasSpirv` runs `tests/spirv` locally on a box that resolves `spirv-val`
and names the run in the PR.** CI resolves no `spirv-val`, so a module the validator rejects
reds nowhere but on that box. The run's command and what green means:
`skills/internal/tests_in_repo.md`, the emitter suite section.
