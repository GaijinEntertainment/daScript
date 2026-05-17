---
slug: port-v1-imgui-boost-example-to-boost-v2-checklist
title: What's the conversion checklist for porting a v1 dasImgui imgui_boost example to the boost-v2 harness + widget surface?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Mechanical substitutions for porting any v1 `imgui_boost` example onto current dasImgui (post PR #33 default-on lint, PR #38 harness, PR #39 daslang theme). Worked example: `examples/graphics/furier_opengl_imgui_example.das` — daslang PR #2695.

**1. Requires.** Drop `require imgui/imgui_boost` + the v1 import bundle. Replace with one line:
```daslang
require imgui/imgui_harness
require daslib/safe_addr   // if you'll use the edit_* rail
```
`imgui_harness` re-exports `imgui public` + all `imgui_boost_v2`/`imgui_widgets_builtin`/`imgui_containers_builtin`/`imgui_scope_builtin`/etc., the daslang theme module, and harness lifecycle. Backend modules (`glfw_boost`, `opengl_boost`, `imgui_app`) are required PRIVATELY — re-require explicitly only if you call them directly (custom GL case).

**2. Init.** Drop hand-rolled `glfwInit` / `glfwCreateWindow` / `CreateContext` / `StyleColorsDark` / `FontGlobalScale` shims. Use:
```daslang
[export] def init() { harness_init("Title", 1024, 768) }
[export] def shutdown() { harness_shutdown() }
```
Theme + JetBrains Mono auto-apply via `live_imgui_init` (PR #39, 2026-05-16). No explicit `apply_daslang_theme()` / `load_daslang_font()` call needed.

**3. Window container.** `Begin("X", p_open, flags) ... End()` → `window(IDENT, (text = "X", closable = false, flags = ImGuiWindowFlags.None)) { ... }`. `IDENT` is a daslang identifier the `[container]` macro auto-emits a `WindowState` global for.

**4. Widgets — caller-owned globals (preferred for ports).** Use the `[edit_widget]` rail with `safe_addr(global)`. No state-struct migration of your data.

| v1 raw | boost-v2 wrapper |
|---|---|
| `Checkbox("L", unsafe(addr(b)))` | `edit_checkbox(safe_addr(b), (id = "K", text = "L"))` |
| `InputFloat("L", unsafe(addr(f)), step)` | `edit_input_float(safe_addr(f), (id = "K", text = "L", step = step))` |
| Adjacent `InputFloat(field.x)` + `InputFloat(field.y)` | Collapse → `edit_input_float2(safe_addr(field), (id = "K", text = "L"))` (same for float3/float4) |

`id = "..."` is a required string-literal at every `[edit_widget]` call site (telemetry path key).

For struct-field access (no `safe_addr` allowed): `unsafe(addr(struct.field))`. But the cleaner move is to collapse adjacent field edits into a vector widget.

**5. Narrative widgets.** `Text("...")` → `text("...")` — Form 3 positional-string sugar; macro auto-generates the identifier from line info. `Separator()` → `separator(SEP_IDENT)` (no string-sugar, needs explicit ident — `[widget]` macro spelling).

**6. Drop `FontGlobalScale` / `StyleColorsDark` calls** — theme picks 14px JetBrains Mono. For a bigger font, call `load_daslang_font(18.0f)` BEFORE `harness_init`, not `FontGlobalScale`.

**7. Custom GL + ImGui hybrid.** See split-harness recipe in `imgui-harness-split-custom-opengl-overlay-recipe`. Summary: `_allow_glfw_calls = true` + explicit requires for `live/glfw_live` + `imgui_app` + `glfw/glfw_boost` + `opengl/opengl_boost`, replace `harness_end_frame` with manual `end_of_frame` / `Render` / `ImGui_ImplOpenGL3_RenderDrawData` / `live_end_frame`.

**Widget-rail decision (when both work):**
- `[edit_widget]` rail (`edit_*` family) — caller-owned `T?`, `{value}` payload, `id=` opt-in. Best for ports (your existing `var foo : T` stays).
- `[widget]` rail (`checkbox`/`input_float`/etc.) — auto-emitted state struct (`ToggleState`/`InputStateFloat`/etc.), full state surface (pending, changed, click count). Best for greenfield where state IS the data.

## Questions
- What's the conversion checklist for porting a v1 dasImgui imgui_boost example to the boost-v2 harness + widget surface?
