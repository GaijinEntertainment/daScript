# dasImgui Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that deletes or weakens a section or an assertion of `test_grammar_canary.das` to clear a
failure is a defect** - fix `tree-sitter-daslang/grammar.js` or the fold kinds in
`modules/dasImgui/text/imgui_text_language.das` (repo root) instead.

**Tests never hardcode machine-local host paths or host path separators.** Resolve
SDK resources through `get_das_root()` in daslang and use portable path APIs in
external drivers. HTTP URL paths and browser virtual-filesystem paths are not
host filesystem locations.
