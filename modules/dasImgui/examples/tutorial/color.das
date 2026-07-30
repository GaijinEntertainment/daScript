options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: color — four shapes of color editing + one click trigger.
//
//   color_edit3(IDENT, (text = ".."))         — inline RGB editor.
//   color_edit4(IDENT, (text = ".."))         — inline RGBA (adds alpha row).
//   color_picker3(IDENT, (text = ".."))       — popout wheel + RGB sliders.
//   color_picker4(IDENT, (text = "..",        — same, plus optional ref swatch.
//                         has_ref_col = true,
//                         ref_col = float4(...)))
//   color_button(IDENT, "##desc", col)        — click-trigger swatch; caller
//                                                owns `col`. Use to surface a
//                                                style-palette row or to open
//                                                a custom picker popup.
//
// State types:
//   - color_edit3 / color_picker3 → ColorState3 (value : float3)
//   - color_edit4 / color_picker4 → ColorState4 (value : float4 incl. alpha)
//   - color_button                → ClickState  (color is caller-owned)
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/color.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/color.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"CO_WIN/CO_EDIT3","value":[1.0,0.5,0.0]}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"CO_WIN/CO_EDIT4","value":[0.2,0.4,0.9,0.6]}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"CO_WIN/CO_SWATCH"}}' \
//        localhost:9090/command
// =============================================================================

let SWATCH_COLOR : float4 = float4(0.85f, 0.20f, 0.20f, 1.0f)
let REF_COLOR    : float4 = float4(0.10f, 0.55f, 0.95f, 1.0f)

[export]
def init() {
    live_create_window("dasImgui color tutorial", 800, 700)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(760.0f, 660.0f), ImGuiCond.Always)
    window(CO_WIN, (text = "color tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Four edit shapes + one click trigger.")
        text(CO_HINT, (text = "edit_* is inline; picker_* is the open square + hue bar; color_button is a swatch."))
        separator()

        // ---- Stage 1: color_edit3 — inline RGB ----
        color_edit3(CO_EDIT3, (text = "tint (RGB)"))
        text("CO_EDIT3.value = ({CO_EDIT3.value.x:.2f}, {CO_EDIT3.value.y:.2f}, {CO_EDIT3.value.z:.2f})")
        spacing()

        // ---- Stage 2: color_edit4 — inline RGBA ----
        color_edit4(CO_EDIT4, (text = "tint (RGBA)"))
        text("CO_EDIT4.value.a = {CO_EDIT4.value.w:.2f}  // 4-form adds alpha")
        spacing()

        // ---- Stage 3: color_picker4 — popout wheel + ref swatch ----
        color_picker4(CO_PICKER, (text = "picker",
                                  flags = ImGuiColorEditFlags.None,
                                  ref_col = REF_COLOR,
                                  has_ref_col = true))
        text("CO_PICKER.value = ({CO_PICKER.value.x:.2f}, {CO_PICKER.value.y:.2f}, {CO_PICKER.value.z:.2f}, {CO_PICKER.value.w:.2f})")
        spacing()

        // ---- Stage 4: color_button — caller-owned color, click trigger ----
        text("Swatch (click triggers - value is caller-owned, not widget state):")
        if (color_button(CO_SWATCH, (desc_id = "##swatch", col = SWATCH_COLOR,
                                     size = float2(96.0f, 48.0f)))) {
            // Hook: open a custom picker popup, copy to clipboard, etc.
        }
        text("CO_SWATCH clicks = {CO_SWATCH.click_count}")
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
