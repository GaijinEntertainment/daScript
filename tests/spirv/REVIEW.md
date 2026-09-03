# tests/spirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `modules/dasSpirv/ARCHITECTURE.md`. Shared emitter rules: `modules/REVIEW_SHADER_EMITTERS.md`
(repo root) - apply that list with this one. The emitter's own checklist,
`modules/dasSpirv/REVIEW.md`, binds a diff that touches the emitter with this one.

**Weakening `test_census.das` is a defect** - it holds every fixture opcode declared and every
declared opcode emitted.

**A diff that adds or changes a fixture asserting a claim `validate_spirv` would check also
asserts that claim on the emitted words.** `validate_spirv` skips where the tool is absent or
predates an extension, so a spirv-val-only assertion can check nothing.
