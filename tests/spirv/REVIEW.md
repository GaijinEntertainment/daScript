# tests/spirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasSpirv/ARCHITECTURE.md`, `modules/dasSpirv/ARCHITECTURE_COOPMAT.md`. Shared
emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one.

**A `*_words` function a `[test]` asserts on has an `add_set` row in `test_census.das` - the
per-function list the opcode census sums: a diff that adds such a function adds its row in the
same change, and a diff that removes a row removes the function with it.** A function with no
row is outside the census, so an opcode nothing declares can be emitted unnoticed.

**Weakening either direction of the census assert - every emitted opcode is in the declared
set, every declared opcode is emitted - is a defect.**

**A diff that adds or changes a `[test]` that validates a `*_words` function's words - through
`validate_spirv` directly or through the file's own validate wrapper - also asserts on the words,
with `op_has_operand(words, SpvOp.Capability, ...)`, every capability the emitted module must
declare other than `Shader`.** `validate_spirv` skips where the tool is absent
or predates an extension, so a spirv-val-only assertion can check nothing.

**A diff that adds or changes a `[test]` that validates a `*_words` function's words asserts the
module version on `words[1]` - directly or through the file's own validate wrapper.**
`validate_spirv` checks the words against the target env it was given, not against the version
the function is expected to emit.

**A diff that adds or changes a `[test]` that validates a `*_words` function's words passes a
`target_env` - directly or through the file's own validate wrapper - that admits the module
version the function emits: `vulkan1.1`, the default, admits SPIR-V 1.3 and below; `vulkan1.2`
up to 1.5; `vulkan1.3` up to 1.6.**

**A diff that adds a `*_words` function instantiating the same set of marker structs - empty CPU
stand-ins, with no storage, that name a device-side tile - as an existing `*_words` function
deletes the existing one, or changes it to instantiate a set some kernel under
`modules/dasLLAMA/dasllama/` still instantiates, in the same change.** Two functions over one set
let one keep a set no kernel instantiates any more and still report green.

**A diff that adds or changes a `*_words` function, or a `[test]` asserting on one, runs
`daslang dastest/dastest.das -- --test tests/spirv --isolated-mode` from the repo root on a box
that resolves `spirv-val`, green - every `[test]` passes and none prints `spirv-val not found
locally` - and names the run in the PR.** CI resolves no `spirv-val`, so a module the validator
rejects reds nowhere but on that box.
