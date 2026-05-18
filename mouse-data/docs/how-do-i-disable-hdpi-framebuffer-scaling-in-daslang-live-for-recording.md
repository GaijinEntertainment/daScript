---
slug: how-do-i-disable-hdpi-framebuffer-scaling-in-daslang-live-for-recording
title: How do I disable HDPI framebuffer scaling in daslang-live (for APNG recording at logical pixel count)?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Verified 2026-05-17 (daslang PR #2704, dasImgui PR #44).

Pass `--no-hdpi-framebuffer` to `daslang-live` after the `--`
separator. It tells `glfw_live` (in `modules/dasGlfw/dasglfw/glfw_live.das`)
to set two GLFW window hints before `glfwCreateWindow`:

  - `GLFW_COCOA_RETINA_FRAMEBUFFER = 0` (macOS: disable retina backing)
  - `GLFW_SCALE_TO_MONITOR = 0` (Windows: don't auto-scale by monitor DPI)

Result: framebuffer matches the requested logical size 1:1. A
640×320-logical window stays at 640×320 in the framebuffer, instead
of 1280×640 on retina mac or 960×480 on a 150%-scaled Windows
monitor.

**Use case:** APNG recording tools (dasImgui's `with_recording_app`
spawn path) pass both:

```
daslang-live <feature.das> -- --imgui-content-scale=1.0 --no-hdpi-framebuffer
```

`--imgui-content-scale=1.0` is the ImGui-style-side companion
(`live_imgui_init` reads it and styles ImGui at 1x instead of querying
GLFW's content-scale). Without these flags, `glReadPixels`-based
capture quadruples on retina and the APNG balloons in size.

**Linux is a no-op:** X11 doesn't apply scaling at the GLFW level;
Wayland reports framebuffer == window for normal windows. The flag is
harmless to pass everywhere.

**Tutorial users running `daslang-live` directly pass neither flag and
keep native HDPI** — this is recording-only.

**Verified on Windows (PR #44):** `record_boost_basics` produces a
640×320 / ~7.5 MB APNG matching the retina-mac reference within
encoder noise. `GLFW_SCALE_TO_MONITOR = 0` correctly engages.

**Inverse plumbing — keep HDPI by default:** see
`how-do-i-make-dasimgui-hdpi-aware-what-s-the-canonical-scale-plumbing`
(PR #42). dasImgui's `live_imgui_init` reads
`glfwGetWindowContentScale` and scales fonts + style automatically.
`--no-hdpi-framebuffer` short-circuits this by reporting scale = 1.0
even on retina.

**Source:**
- daslang PR #2704: `modules/dasGlfw/dasglfw/glfw_live.das`
- dasImgui PR #44 helper invocation:
  `widgets/imgui_playwright.das` `with_recording_app` argv build
- ImGui-side `--imgui-content-scale=1.0` parser: `widgets/imgui_live.das`

## Questions
- How do I disable HDPI framebuffer scaling in daslang-live (for APNG recording at logical pixel count)?
