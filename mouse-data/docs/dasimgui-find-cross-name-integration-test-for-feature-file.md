---
slug: dasimgui-find-cross-name-integration-test-for-feature-file
title: How do I find which integration test drives a given feature/example .das file (when the test name doesn't match)?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

## The problem

In `modules/dasImgui/tests/integration/`, most tests are paired same-name with `examples/features/`: `test_foo.das` drives `foo.das`. But some tests drive a **different** feature file:

- `test_snapshot.das` drives `examples/features/triggers.das`
- `test_visual_aids_focus_rect.das` drives `examples/features/glfw_synth_keys.das`
- `test_visual_aids_key_hud.das` drives `examples/features/glfw_synth_keys.das`

Same-name heuristic (or a delegated agent that's been told "rename matching tests") misses these. Result: a refactor lands, the same-name tests get updated, the cross-name tests fail at run-time with stale widget paths.

## The lookup

Grep the test files for `let FEATURE = "modules/dasImgui/examples/features/<filename>.das"` (or `with_imgui_app(FEATURE) $(...)` — both reach the same anchor):

    Grep "let FEATURE" modules/dasImgui/tests/integration/

That lists every test's target feature file in one shot. Use it to build a feature → test(s) reverse index before a path-rename refactor.

## When this bites

Any refactor that changes widget identifier strings — most commonly the `MAIN_WIN/` path-prefix migration when wrapping things in `window(NAME, ...)`. If you renamed widget paths in `glfw_synth_keys.das` and only updated `test_glfw_synth_keys.das`, the two `test_visual_aids_*` tests silently break on the next run.

## Discovery

Bit me 2026-05-13 during the raw-`Begin/End` → `window(NAME, ...)` migration. Fixed retrospectively by `replace_all`'ing the bare widget names to prefixed names in the three cross-name tests after `dastest` surfaced the failures.

## Questions
- How do I find which integration test drives a given feature/example .das file (when the test name doesn't match)?
