options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_scope_builtin    // with_button_repeat

// =============================================================================
// FEATURE: internal_button_ex — the imgui_internal.h *Ex buttons that take a
//          behavior-flags argument. Cherry-picked into the v2 binding
//          (bind_imgui.das internal_allow_func): ButtonEx, ArrowButtonEx. Their
//          ImGuiButtonFlags arg's USEFUL values (Repeat, PressedOnClick, …) live
//          in the internal ImGuiButtonFlagsPrivate_ enum — bound here and combined
//          with the public ImGuiButtonFlags via the cross-enum `|` rail in
//          imgui_enums.das (both halves are one int-backed flag set). This is the
//          "binding is usable" gate: it calls the real bound API at runtime.
// SHOWS:   ButtonEx with Repeat (held fires repeatedly), PressedOnClick (fires on
//          mouse-down not release), MouseButtonRight (responds only to right-click),
//          and two private flags combined; ArrowButtonEx in all four directions
//          with Repeat. The cross-enum `|` yields the single ImGuiButtonFlags the
//          *Ex functions accept; a lone private flag uses `None | Private.X`.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_button_ex.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_button_ex.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_button_ex.das
// =============================================================================

var g_plain = 0
var g_repeat = 0
var g_press = 0
var g_right = 0
var g_combo = 0
var g_arrow = "(none)"

[export]
def init() {
    harness_init("internal_button_ex - imgui_internal.h *Ex buttons + behavior flags", 760, 540)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let auto_sz = float2(0.0, 0.0)

    SetNextWindowSize(ImVec2(720.0, 500.0), ImGuiCond.Always)
    window(BTN_WIN, (text = "internal_button_ex", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        text("ButtonEx(label, size, flags): public ImGuiButtonFlags + internal")
        text("  ImGuiButtonFlagsPrivate, combined via the imgui_enums.das cross-enum `|`.")
        separator()

        // Plain ButtonEx — no behavior flags (same as the public button).
        if (ButtonEx("Plain", auto_sz, ImGuiButtonFlags.None)) {
            g_plain++
        }
        same_line()
        text("clicks: {g_plain}")

        // with_button_repeat pushes the ButtonRepeat item flag.
        with_button_repeat(true) {
            if (ButtonEx("Hold to repeat", auto_sz, ImGuiButtonFlags.None)) {
                g_repeat++
            }
        }
        same_line()
        text("repeat fires: {g_repeat}")

        // PressedOnClick (private) — returns true on mouse-DOWN, not on release.
        if (ButtonEx("Fire on press", auto_sz, ImGuiButtonFlags.None | ImGuiButtonFlagsPrivate.PressedOnClick)) {
            g_press++
        }
        same_line()
        text("press fires: {g_press}")

        // MouseButtonRight (public) — responds only to the right mouse button.
        if (ButtonEx("Right-click me", auto_sz, ImGuiButtonFlags.MouseButtonRight)) {
            g_right++
        }
        same_line()
        text("right-clicks: {g_right}")

        // Repeat (item flag, via with_button_repeat) + PressedOnClick (ButtonFlag).
        with_button_repeat(true) {
            if (ButtonEx("Repeat + on-press", auto_sz,
                         ImGuiButtonFlags.None | ImGuiButtonFlagsPrivate.PressedOnClick)) {
                g_combo++
            }
        }
        same_line()
        text("combo fires: {g_combo}")
        separator()

        // ArrowButtonEx in all four directions, repeat via with_button_repeat
        text("ArrowButtonEx(str_id, dir, size, flags): directional repeat buttons.")
        let asz = float2(28.0, 28.0)
        let rep = ImGuiButtonFlags.None
        with_button_repeat(true) {
            if (ArrowButtonEx("a_left", ImGuiDir.Left, asz, rep)) {
                g_arrow = "Left"
            }
            same_line()
            if (ArrowButtonEx("a_right", ImGuiDir.Right, asz, rep)) {
                g_arrow = "Right"
            }
            same_line()
            if (ArrowButtonEx("a_up", ImGuiDir.Up, asz, rep)) {
                g_arrow = "Up"
            }
            same_line()
            if (ArrowButtonEx("a_down", ImGuiDir.Down, asz, rep)) {
                g_arrow = "Down"
            }
        }
        same_line()
        text("last arrow: {g_arrow}")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
