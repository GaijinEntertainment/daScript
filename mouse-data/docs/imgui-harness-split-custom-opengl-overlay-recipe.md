---
slug: imgui-harness-split-custom-opengl-overlay-recipe
title: How do I render custom OpenGL underneath an ImGui overlay when using imgui_harness? harness_end_frame does its own clear + ImGui draw call so I can't slot custom GL in.
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

`harness_end_frame` packs `end_of_frame` + `Render` + viewport/clear + `ImGui_ImplOpenGL3_RenderDrawData` + `live_end_frame` into one block. For a custom 3D scene under an ImGui controls window, you need GL between the clear and ImGui's draw drain — split the harness manually:

```daslang
options _allow_glfw_calls = true

require imgui/imgui_harness
require opengl/opengl_boost   // gl* + create_shader_program
require glfw/glfw_boost       // glfwGetTime
require live/glfw_live        // live_get_framebuffer_size, live_end_frame
require imgui_app             // ImGui_ImplOpenGL3_RenderDrawData
require daslib/safe_addr

[export] def init() {
    harness_init("My App", 1024, 1024)
    create_gl_objects()
}

[export] def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    my_widgets()            // boost-v2 window/edit_*/text calls

    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.85f, 0.85f, 0.90f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    my_custom_gl()          // your scene draws here

    end_of_frame()
    Render()
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())
    live_end_frame()
}

[export] def shutdown() { harness_shutdown() }
```

Why each piece:

- **`_allow_glfw_calls = true`** opts out of `imgui_harness_lint` (HARNESS001), which forbids direct calls into 5 windowed-backend modules (`glfw_boost`, `opengl_boost`, `glfw_live`, `opengl_live`, `imgui_live`). Scaffolding-only flag, same pattern as `_allow_imgui_legacy`.
- **Explicit backend requires** — `imgui_harness` imports the backend modules *privately*, so their symbols don't reach consumers transitively. Every `live_*`/`gl*`/`ImGui_Impl*` call you make directly needs its own `require`.
- **`imgui_app` needs the require but is NOT lint-blocked** — the harness still considers `ImGui_ImplOpenGL3_RenderDrawData` legitimate (you can't drain DrawData any other way), it's just a visibility issue.
- **`harness_init` / `harness_shutdown` still work** — they handle CreateContext, GLFW window, theme + JetBrains Mono via `live_imgui_init` (PR #39, 2026-05-16), and cleanup. No explicit `apply_daslang_theme()` call needed.

Working example: `examples/graphics/furier_opengl_imgui_example.das` (daslang PR #2695, merged 2026-05-17).

## Questions
- How do I render custom OpenGL underneath an ImGui overlay when using imgui_harness? harness_end_frame does its own clear + ImGui draw call so I can't slot custom GL in.
