# tests/spirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `modules/dasSpirv/ARCHITECTURE.md`. Shared emitter rules: `modules/REVIEW_SHADER_EMITTERS.md`
- apply that list with this one.

**Weakening `test_census.das` is a defect** - it holds every fixture opcode declared and every
declared opcode emitted.

**A diff that adds or changes a fixture asserting a claim `validate_spirv` would check also
asserts that claim on the emitted words.** `validate_spirv` skips where the tool is absent or
predates an extension, so a spirv-val-only assertion can check nothing.

**A fixture cell that validates its words asserts the module version on `words[1]` in the same
cell, and passes a `target_env` - directly or through the file's `validate` wrapper - that
admits that version: `vulkan1.2` for 1.4 and 1.5, `vulkan1.3` for 1.6.** The default,
`vulkan1.1`, admits SPIR-V 1.3 at most.
