# tests/spirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasSpirv/ARCHITECTURE.md`, `modules/dasSpirv/ARCHITECTURE_COOPMAT.md`. Shared
emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one.

**A diff that adds a `*_words` fixture adds it to the roster in `test_census.das` - the
per-fixture `add_set` list the census sums.** A fixture outside the roster is outside the
census, so an opcode nothing declares can be emitted unnoticed.

**Removing a fixture from the `test_census.das` roster, or weakening either direction of the
census assert - every emitted opcode is in the declared set, every declared opcode is emitted -
is a defect.**

**A diff that adds or changes a fixture asserting a claim `validate_spirv` would check also
asserts that claim on the emitted words.** `validate_spirv` skips where the tool is absent or
predates an extension, so a spirv-val-only assertion can check nothing.

**A fixture cell that validates its words asserts the module version on `words[1]` in the same
cell.** `validate_spirv` checks the words against the target env it was given, not against the
version the fixture expects.

**A fixture cell that validates its words passes a `target_env` - directly or through the
file's own validate wrapper - that admits the version it asserts: `vulkan1.1` admits SPIR-V 1.3
and below, `vulkan1.2` up to 1.5, `vulkan1.3` up to 1.6.** `vulkan1.1` is the default.
