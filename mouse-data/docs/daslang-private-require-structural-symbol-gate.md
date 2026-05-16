---
slug: daslang-private-require-structural-symbol-gate
title: How do I make a daslang wrapper module enforce "users cannot call symbols from the backend module" structurally — without relying on a lint or a runtime check?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Pattern: private `require` for the backend.**

```das
module my_wrapper shared public

require imgui_backend_safe public   // re-exported to consumers
require imgui_backend_glfw           // PRIVATE — symbols stay inside this module
```

A `require X` (no `public`) is **private to the requiring module**. Symbols from X are visible inside `my_wrapper.das` but do NOT transitively re-export to anything that does `require my_wrapper`. Consumers literally cannot resolve `glfwInit` / `ImGui_ImplGlfw_*` etc. — those names aren't in their scope.

**Why this matters:** lints catch use AFTER it happens; private-require prevents the symbol from existing in user scope at all. A user who tries to call a forbidden symbol gets the standard "function not found" diagnostic instead of a lint error.

**A user who explicitly `require imgui_backend_glfw` themselves bypasses the structural guarantee** — and that's where a complementary lint catches the explicit-require attempt. Defense in depth: structure first, lint second.

**Application:** dasImgui's `imgui_harness` (PR landing 2026-05-15) privately requires `imgui_app`, `glfw/glfw_boost`, `opengl/opengl_boost`, `live/glfw_live`, `live/opengl_live`, `imgui/imgui_live`, `imgui/imgui_visual_aids`. Consumers `require imgui/imgui_harness` and the GLFW/GL function names are not in their scope.

**Source:** `modules/dasImgui/widgets/imgui_harness.das`. Same pattern works for any backend-abstraction wrapper.

## Questions
- How do I make a daslang wrapper module enforce "users cannot call symbols from the backend module" structurally — without relying on a lint or a runtime check?
