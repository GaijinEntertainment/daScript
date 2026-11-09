# examples/games Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff that changes a game's on-screen controls, its prompts, or its canvas size runs the
playground spec suite under `site/tests/playground/` (repo root).**

**A diff that leaves `REVIEW.das` (beside this file) checking less than it did - a check deleted,
its walk narrowed, a finding it used to report gone - is a defect.**

**A `.das` in a game's folder that declares a module declares `module <name> public`, never
`shared`** - the game tree recompiles on a live reload, and a shared module in it keeps running
the old code.

**A diff that adds or edits a `[vertex_program]` or `[fragment_program]` function in a game,
or any `def` such a function reaches directly or through another `def`, keeps that code inside
GLSL ES 3.00** - the same file runs in the playground on WebGL2.

**A colour attachment a diff creates or retypes in a format other than `GL_RGBA8` checks
framebuffer completeness at creation and falls back to `GL_RGBA8` when the framebuffer is
incomplete** - WebGL2 does not guarantee a float colour attachment.

**A `.das` a diff adds to a game with a playground copy is named in the sample's `files` list
(`web/examples/ui/samples/data.json`) in the same change** - a staged file no list names never
mounts, and the sample fails to load with `missing prerequisite`.

**A diff that adds a `[test]` to a game puts it in the game's existing category file for its
topic or in the game's `uncategorized.das`, never in a new file, with a `description`, and with
`timeout = 1200` when the test spawns a child or waits on a peer** - the shape rules are
`tests/REVIEW.md` (repo root) and `tests/ARCHITECTURE.md`; `REVIEW.das` (beside this file)
applies them tests-only, since each game's sources sit beside its tests, and `TESTS.md` (beside
this file) is the generated index (`daslang utils/internal/test-audit/main.das -- --root
examples/games --tests-only --index examples/games/TESTS.md --readme`; the `check_tests_index`
target verifies it). A game with a playground copy carries the same test files in both.

