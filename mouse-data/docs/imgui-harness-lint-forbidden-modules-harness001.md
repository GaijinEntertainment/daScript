---
slug: imgui-harness-lint-forbidden-modules-harness001
title: What modules does imgui_harness_lint forbid (HARNESS001) and is imgui_app one of them?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

`imgui_harness_lint` is default-on for every file that does `require imgui/imgui_harness`. It blocks calls into exactly 5 modules:

```
glfw_boost
opengl_boost
glfw_live
opengl_live
imgui_live
```

Source: `D:/DASPKG/dasImgui/widgets/imgui_harness_lint.das:33-35` (`FORBIDDEN_MODULES` table).

**`imgui_app` is NOT in the list.** The harness imports `imgui_app` privately, so consumers still need their own `require imgui_app` to call `ImGui_ImplOpenGL3_RenderDrawData` (visibility), but no HARNESS001 fires — that's deliberate, because `imgui_app` is the bridge layer between ImGui and OpenGL that legitimate split-harness consumers need to drain DrawData. If `imgui_app` were forbidden, the split-harness recipe for custom 3D + ImGui overlay (separate card) would be uncodeable.

Error code: **50503**, prefix **HARNESS001**, severity `macro_error` (stops compile, not a warning).

Message:
```
HARNESS001: {mname}::{fname} is forbidden in files that require imgui_harness —
use the harness helpers (harness_init / harness_begin_frame / harness_new_frame /
harness_end_frame / harness_shutdown) instead. Per-file escape:
`options _allow_glfw_calls = true` (scaffolding only)
```

**Per-file opt-out:** `options _allow_glfw_calls = true`. Scaffolding-only — target end state is no opt-out, same pattern as PR #33's `_allow_imgui_legacy`. Use for the genuine custom 3D + ImGui overlay case; do NOT use to bypass when your code should be going through `harness_begin_frame` / `harness_end_frame`.

**Scoping:** lint walks `prog.getThisModule` only — transitively-required modules (including `imgui_harness` itself, which legitimately calls into the backends as the wrapper) are skipped. Macro-generated bodies and macro-synthesized call sites are filtered by `fileInfo` mismatch.

## Questions
- What modules does imgui_harness_lint forbid (HARNESS001) and is imgui_app one of them?
