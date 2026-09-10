# tests/spirv Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasSpirv/ARCHITECTURE.md`, `modules/dasSpirv/ARCHITECTURE_COOPMAT.md`. Shared
emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` - apply that list with this one.

**A diff that adds a `*_words` fixture adds it to the roster in `test_census.das` - the
per-fixture `add_set` list the census sums.** A fixture outside the roster is outside the
census, so an opcode nothing declares can be emitted unnoticed.

**Removing a fixture from the `test_census.das` roster while its `*_words` function stays in the
suite, or weakening either direction of the census assert - every emitted opcode is in the
declared set, every declared opcode is emitted - is a defect.**

**A diff that adds or changes a fixture asserting a claim `validate_spirv` would check also
asserts that claim on the emitted words.** `validate_spirv` skips where the tool is absent or
predates an extension, so a spirv-val-only assertion can check nothing.

**A diff that adds or changes a fixture cell that validates its words asserts the module version
on `words[1]` in the same cell.** `validate_spirv` checks the words against the target env it was
given, not against the version the fixture expects.

**A diff that adds or changes a fixture cell that validates its words passes a `target_env` -
directly or through the file's own validate wrapper - that admits the version it asserts:
`vulkan1.1` admits SPIR-V 1.3 and below, `vulkan1.2` up to 1.5, `vulkan1.3` up to 1.6.**
`vulkan1.1` is the default.

**A diff that adds a `*_words` fixture covering an engine kernel - the shipped kernel whose
shapes the fixture mirrors, named in the comment above the fixture's kernel - that an existing
`*_words` fixture already covers deletes that existing fixture, or reshapes it onto the shapes
that engine kernel declares, in the same change.** Two fixtures over one engine kernel let one
keep shapes that kernel no longer declares and still report green.

**A diff that adds or changes a fixture runs `tests/spirv` locally on a box that resolves
`spirv-val` and names the run in the PR.** CI resolves no `spirv-val`, so a module the
validator rejects reds nowhere but on that box. The run's command and what green means:
`skills/internal/tests_in_repo.md`, the emitter suite section.
