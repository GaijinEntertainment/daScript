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
require imgui/imgui_enums
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: item_flags — the imgui 1.92 item-flag stack via with_item_flag.
//
// 1.92 split the private item flags into their own ImGuiItemFlagsPrivate enum,
// and replaced PushButtonRepeat / PushTabStop with the unified
// PushItemFlag(flag, enabled). The boost layer wraps that as with_item_flag,
// and imgui/imgui_enums supplies cross-enum `|` operators so a private flag
// combines with a public one in a single push:
//
//   with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.MixedValue, true) { ... }
//   with_item_flag(ImGuiItemFlagsPrivate.ReadOnly | ImGuiItemFlags.NoNav, true) { ... }
//
// (The dedicated guards with_disabled / with_button_repeat / with_tab_stop
// cover the three public flags that have their own wrappers.)
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/item_flags.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/item_flags.das
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui item_flags tutorial", 820, 560)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.3
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(40.0f, 40.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(740.0f, 480.0f), ImGuiCond.Always)
    window(IF_WIN, (text = "item_flags", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("with_item_flag pushes an ImGuiItemFlags for the wrapped block.")
        separator()

        // MixedValue — draws a checkbox as the indeterminate tri-state dash,
        // independent of its real checked state.
        text("MixedValue (tri-state dash):")
        checkbox(CB_PLAIN, (text = "plain checkbox"))
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.MixedValue, true) {
            checkbox(CB_MIXED, (text = "same checkbox, drawn as a dash"))
        }
        separator()

        // ReadOnly | NoNav — a private flag and a public flag combined in one
        // push via the cross-enum `|` from imgui/imgui_enums.
        text("ReadOnly | NoNav (private + public flag, one push):")
        slider_float(SL_RW, (text = "editable"))
        with_item_flag(ImGuiItemFlagsPrivate.ReadOnly | ImGuiItemFlags.NoNav, true) {
            slider_float(SL_RO, (text = "read-only, skipped by nav"))
        }
        separator()

        // AllowOverlap — the wide button yields hover to a later widget drawn
        // on top of it instead of swallowing the overlap.
        text("AllowOverlap (small button sits on top of the wide one):")
        let p = GetCursorScreenPos()
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.AllowOverlap, true) {
            button(OVL_UNDER, (text = "wide button underneath", size = float2(320.0f, 36.0f)))
        }
        SetCursorScreenPos(p)
        small_button(OVL_OVER, (text = "on top"))
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
