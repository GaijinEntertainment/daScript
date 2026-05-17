---
slug: how-do-i-make-dasimgui-hdpi-aware-what-s-the-canonical-scale-plumbing
title: How do I make dasImgui HDPI-aware — what's the canonical scale plumbing?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Verified 2026-05-16 (dasImgui PR #42, branch `bbatkin/hdpi-theme-scaling`).

dasImgui prior to PR #42 had **zero DPI awareness**: theme constants hardcoded at 1x (`WindowPadding(8,8)`, `FramePadding(6,3)`, `ScrollbarSize=10`), font at 14px exactly. `glfwGetWindowContentScale` was already bound in dasGlfw but never queried.

**The plumbing (3 places):**

1. `widgets/imgui_live.das` — read scale once at init from the window, cache it, apply to fonts + style:
```das
var public live_imgui_content_scale : float = 1.0f

def public live_imgui_init(window : GLFWwindow?; glsl_version : string = "#version 330") {
    ...
    if (window != null) {
        var xs = 1.0f
        var ys = 1.0f
        glfwGetWindowContentScale(window, safe_addr(xs), safe_addr(ys))
        // Clamp: some backends report 0 if monitor unavailable.
        live_imgui_content_scale = max(max(xs, ys), 1.0f)
    }
    load_daslang_font(14.0f * live_imgui_content_scale)
    apply_daslang_theme()
    ScaleAllSizes(unsafe(GetStyle()), live_imgui_content_scale)
    ...
}
```

2. `widgets/imgui_harness.das` — `glfwWindowHint(int(GLFW_SCALE_TO_MONITOR), 1)` before `live_create_window`. On Windows GLFW scales window-creation size by monitor DPI; no-op on macOS/Linux (where logical-pixel sizing is intrinsic).

3. `widgets/imgui_theme_daslang.das` — leave theme constants at 1x; `ScaleAllSizes` runs AFTER `apply_daslang_theme`. Note: stock ImGui `ScaleAllSizes` does NOT scale border widths — hairline 1px borders on retina are the intended look.

**Required `require`s in imgui_live.das**: `daslib/safe_addr` (for `safe_addr(xs)`) and `math` (for `max`).

**Gotchas:**
- `glfwGetWindowContentScale` binds C `float*` as `float?` in das, NOT `float&`. Must pass `safe_addr(xs)`. There's a wrapper in `dasglfw/glfw_boost.das` for `glfwGetFramebufferSize(window, int&, int&)` but NO equivalent for content scale.
- Read once at init; DPI changes during monitor drag (`glfwSetWindowContentScaleCallback`) are deliberately out of scope — re-applying theme + rebuilding font atlas mid-frame is fiddly. Document the limitation.
- Font strategy: re-raster at `14 * scale` px (sharp), NOT `io.FontGlobalScale = scale` (bilinear-blurry).
- Headless / no-window path stays at 1.0 naturally.

**Per-platform behavior at runtime:**
- macOS retina: `glfwGetWindowContentScale` → 2.0, window opens at logical 800×600 = physical 1600×1200 implicitly.
- Windows DPI 200%: `GLFW_SCALE_TO_MONITOR` resizes window to physical 1600×1200, content scale → 2.0.
- Linux: depends on compositor (Wayland reports correctly; X11 often 1.0).

## Questions
- How do I make dasImgui HDPI-aware — what's the canonical scale plumbing?
