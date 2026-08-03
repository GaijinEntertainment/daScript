# Code review rules — modules/dasImgui

Folder-scoped review rules. When a PR touches files under this folder, the
code review lists and enforces these (see `skills/make_pr.md` — CODEREVIEW.md
discovery runs after the master rebase).

* New functionality ships with tests — same PR, no follow-up promises.
* Tests go under `modules/dasImgui/tests`.
* No tests go under `tests/dasImgui` — that folder must not exist.
* Before a PR: run the suite —
  `daslang dastest/dastest.das -- --test modules/dasImgui/tests --headless --isolated-mode --isolated-mode-threads 4 --exclude glfw_synth --exclude key_hud --exclude embedded_terminal`
  (also available as `preflight --only imgui`; nightly_imgui.yml is the suite's
  only CI lane, so the local run is the pre-push gate).
* On local **Windows** runs, additionally pass the high-POST excludes from
  `modules/dasImgui/CLAUDE.md` (libhv's 16-POST-per-subprocess stall):
  `inputs_drag inputs_numeric inputs_slider indexed_dynamic inputs_color inputs_choice inputs_text`.
* Tests are kept multiplatform — no hardcoded platform-specific paths,
  separators, or OS-only assumptions; resolve locations via `get_das_root()` /
  module-relative helpers, never absolute or machine-local paths.

## Grammar canary — tree-sitter-daslang drift contract

* Any change to `tree-sitter-daslang/grammar.js` requires: regenerating
  `parser.c`, rebuilding ALL THREE consumers (the tree_sitter_daslang shared
  module/DLL, `daslang`, `daslang-live`), and a green
  `tests/test_grammar_canary.das`. A grammar error region silently SWALLOWS
  every fold/outline after it (the bare-named-args gap hid exactly this way) —
  "it still parses" is not evidence.
* New syntax (`src/parser/ds2_parser.ypp` or `grammar.js`) lands with a new
  canary section in `test_grammar_canary.das` in the same PR — the canary only
  protects syntax it pins.
* A red canary names the section that broke. Never ship around it by deleting
  or loosening sections.
