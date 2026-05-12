---
slug: does-imgui-impl-glfw-newframe-overwrite-io-mousepos-every-frame
title: Does ImGui_ImplGlfw_NewFrame overwrite ImGuiIO.MousePos every frame? Can I assign io.MousePos directly to drive a synthetic cursor?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Short answer: yes, every frame. Direct `io.MousePos = X` assignment is clobbered.**

The GLFW backend pushes mouse position into imgui through `io.AddMousePosEvent` (the post-1.87 event-queue API, not direct field assignment), and it does so **every frame** when the window is focused:

- **`ImGui_ImplGlfw_CursorPosCallback`** ([imgui_impl_glfw.cpp:456](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp#L456)): fires on every GLFW cursor-pos event, calls `io.AddMousePosEvent(x, y)`.
- **`ImGui_ImplGlfw_UpdateMouseData`** ([imgui_impl_glfw.cpp:748](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp#L748)), called from `ImGui_ImplGlfw_NewFrame`: per-frame fallback that polls `glfwGetCursorPos` and posts `io.AddMousePosEvent` when window is focused. So even an idle real cursor gets re-posted every frame.

Both feed an event queue; imgui processes the queue inside `NewFrame()` and ends up writing `io.MousePos`. Any pre-NewFrame `io.MousePos = X` is overwritten during processing.

## What WOULD work, with caveats

- **`io.WantSetMousePos = true` + `io.MousePos = X`** ([imgui_impl_glfw.cpp:730](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp#L730)): the backend reads this and calls `glfwSetCursorPos` to **warp the real OS cursor**. Side effect: the user's hardware cursor jumps. Useful for nav-driven UIs, terrible for "drive a video recording without the user noticing".
- **`AddMousePosEvent(X, Y)` from script** after `NewFrame`: skipped — the event queue is drained during NewFrame, so a post-NewFrame add lands in NEXT frame's processing. And imgui's docs warn against mixing direct field writes with the event API.

## The pattern that avoids the race entirely

For overlays that consume the mouse position (mouse trail, debug overlays, cursor-following narrate boxes), **don't touch `io.MousePos`**. Add a module-level `g_synth_pos : float2` + `g_synth_active : bool`, and have the overlay read `g_synth_active ? g_synth_pos : GetMousePos()` at its draw call. No GLFW backend race, no hardware-cursor warp, works headlessly.

This is what `imgui_visual_aids` (Phase 5 follow-up) uses for synthetic-cursor recording — the trail's `paint_trail` reads the override at the same `end_of_frame` hook it already runs from.

Surfaced 2026-05-11 during Phase 5 recording follow-up planning, when curl-driven `record_*` produced trail-less videos because no real cursor motion was happening.

## Questions
- Does ImGui_ImplGlfw_NewFrame overwrite ImGuiIO.MousePos every frame in dasImgui / imgui?
- Can I assign io.MousePos directly to drive a synthetic cursor in dasImgui?
- How does the GLFW backend push mouse position into imgui (AddMousePosEvent vs direct field)?
- What's `io.WantSetMousePos` and when do I use it?
- Why does my pre-NewFrame io.MousePos override get reset?

## Questions
- Does ImGui_ImplGlfw_NewFrame overwrite ImGuiIO.MousePos every frame? Can I assign io.MousePos directly to drive a synthetic cursor?
