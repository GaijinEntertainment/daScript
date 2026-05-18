---
slug: dasimgui-integration-tests-use-examples-features-recordings-use-examples-tutorial
title: dasImgui integration tests use examples/features/, recordings use examples/tutorial/
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

For a new dasImgui component, the **full triad** needs FOUR files, not three, because tests and recordings need different lifecycles:

| File | Lifecycle | Driven by |
|---|---|---|
| `examples/features/<name>.das` | `harness_init` / `harness_begin_frame` / `harness_new_frame` (require `imgui/imgui_harness`) | Integration tests via `with_imgui_app(feature_path)` |
| `examples/tutorial/<name>.das` | `live_create_window` / `live_imgui_init` / `live_begin_frame` (require `imgui_live` + GLFW + GL stack) | Recording drivers via `with_recording_app(tutorial_path, apng, secs)` |
| `tests/integration/test_<name>.das` | n/a | dastest; points at the `features/` file |
| `tests/integration/record_<name>.das` | n/a | record entry-point; points at the `tutorial/` file |

Plus the RST tutorial page (`doc/source/tutorials/<name>.rst`) doing `literalinclude` of the `tutorial/` file, and the APNG dropped at `doc/source/_static/tutorials/<name>.apng`.

**Don't** point an integration test at a `tutorial/` file. `with_imgui_app` spawns plain `daslang-live` which doesn't dispatch the `live_create_window` lifecycle — the host window never opens, `wait_for_widget("ROOT_WIN", 15s)` times out, and you get a CI failure that looks like "the widget never registers" but is actually "the app's `update()` never runs."

CI failure shape for the wrong-lifecycle case: `wait_for_widget` returns null after 15s with the failure message "<ROOT> registers (demo renders)" — no compile error, no panic, just silent timeout.

Bit me in dasImgui PR #47 (PR-A2 inputs.das sweep): I skipped `features/` and pointed tests at `tutorial/` directly. macOS CI surfaced 3 timeouts; fixup commit `2665617` added the feature files. Pattern confirmed against existing `test_with_indent.das` ← `features/with_indent.das` and similar.

## Questions
- dasImgui integration tests use examples/features/, recordings use examples/tutorial/
