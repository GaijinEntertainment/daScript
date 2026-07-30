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
* Tests are kept multiplatform — no hardcoded platform-specific paths,
  separators, or OS-only assumptions; resolve locations via `get_das_root()` /
  module-relative helpers, never absolute or machine-local paths.
