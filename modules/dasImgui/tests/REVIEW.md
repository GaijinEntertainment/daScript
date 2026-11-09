# dasImgui Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that adds a `[test]` here puts it in the existing category file for its topic or in
`uncategorized.das`, never in a new file, with a `description`, and with `timeout = 1200` when the
test spawns a `daslang-live` host (every `with_imgui_app` test does)** - the shape rules are
`tests/REVIEW.md` (repo root) and `tests/ARCHITECTURE.md`; `REVIEW.das` (beside this file) applies
them tests-only, since the `record_*.das` drivers, the lint smokes and the feature apps a test
spawns are bare scripts beside the suites. `TESTS.md` (beside this file) is the generated index
(`daslang utils/internal/test-audit/main.das -- --root modules/dasImgui/tests --tests-only --index
modules/dasImgui/tests/TESTS.md --readme`; the `check_tests_index` target verifies it). `README.md`
beside this file is prose, not the index. A merged file's name never carries a substring the CI
lane excludes (`glfw_synth`, `embedded_terminal` - `../CMakeLists.txt` and the preflight `imgui`
gate) unless every test in it needs that exclusion.

**A diff that deletes or weakens a `CANARY_SECTIONS` entry or an assertion of the grammar canary
test in `test_text_tree_sitter.das` to clear a failure is a defect** - fix `tree-sitter-daslang/grammar.js` or the fold kinds in
`modules/dasImgui/text/imgui_text_language.das` (repo root) instead.

**Tests never hardcode machine-local host paths or host path separators.** Resolve
SDK resources through `get_das_root()` in daslang and use portable path APIs in
external drivers. HTTP URL paths and browser virtual-filesystem paths are not
host filesystem locations.
