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
require imgui/imgui_scope_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: with_disabled — stateless scope-wrapper family.
//
// Four new wrappers landed alongside the v2 boost surface. They share the
// same shape: a top-level function that takes a single value + trailing
// block, pushes the corresponding ImGui scope on entry, pops on exit. None
// emit telemetry (no [widget] / [container] annotation, no idents).
//
//   with_disabled(disabled, blk)         — BeginDisabled / EndDisabled
//   with_font(font, blk)                 — PushFont / PopFont
//   with_button_repeat(repeat, blk)      — PushButtonRepeat / PopButtonRepeat
//   with_clip_rect(min, max, isect, blk) — PushClipRect / PopClipRect
//
// They subsume the raw Push/Pop pairs that v2's lint flags as invisible.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/with_disabled.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/with_disabled.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui with_disabled family", 760, 520)
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

    SetNextWindowPos(ImVec2(60.0, 60.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0, 440.0), ImGuiCond.Always)
    window(SCOPES_WIN, (text = "Scope wrappers", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        separator_text("with_disabled")

        text("Toggle ENABLED_MASTER; child widgets follow.")
        checkbox(ENABLED_MASTER, (text = "Enable child group"))

        // The whole block inside `with_disabled(...)` is rendered greyed-out
        // when the disabled flag is true; child clicks are inert.
        with_disabled(!ENABLED_MASTER.value) {
            if (button(CHILD_SAVE, (text = "Save"))) {
                print("save\n")
            }
            same_line(SL_CHILD)
            if (button(CHILD_LOAD, (text = "Load"))) {
                print("load\n")
            }
            slider_float(CHILD_VOL, (text = "Volume"))
        }
        text("CHILD_SAVE: clicked {CHILD_SAVE.click_count} times")

        separator_text("with_button_repeat")

        // Inside with_button_repeat(true), holding the button fires click
        // events repeatedly — useful for steppers.
        text("Hold the button - click_count keeps rising.")
        with_button_repeat(true) {
            if (button(STEP_UP, (text = "+"))) {
                print("step up: {STEP_UP.click_count}\n")
            }
        }

        text("STEP_UP.click_count = {STEP_UP.click_count}")

        separator_text("with_font")

        // The default font is the same as GetFont() — change to any loaded
        // font handle to switch scope-local typography.
        text("with_font(...) swaps the active font for one scope.")
        with_font(GetFont()) {
            text(FONT_DEMO, (text = "Inside the with_font scope."))
        }
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
