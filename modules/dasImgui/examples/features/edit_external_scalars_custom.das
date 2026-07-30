options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_external_scalars_custom — edit_*_scalar generic-T overloads.
// SHOWS:   edit_drag_scalar / edit_slider_scalar / edit_input_scalar — the
//          `auto(T)` versions that accept any scalar type (int8, uint16,
//          int64, double, uint, ...). Sibling demo to edit_external_scalars
//          which covers only the float/int typed variants. Demonstrates that
//          a single rail call site adapts to whatever T the pointee is.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/DRAG_I8","value":42}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_external_scalars_custom.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_external_scalars_custom.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_external_scalars_custom.das
// =============================================================================

var private g_drag_i8 : int8 = int8(0)
var private g_drag_u16 : uint16 = uint16(100)
var private g_drag_i64 : int64 = 1000l
var private g_slider_double : double = 0.5lf
var private g_slider_u8 : uint8 = uint8(128)
var private g_input_uint : uint = 0u
var private g_input_double : double = 1.5lf

[export]
def init() {
    harness_init("edit_external_scalars_custom — boost v2 §3.7", 760, 520)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 460.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_*_scalar — generic T", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD_DRAG, (text = "edit_drag_scalar across int8, uint16, int64."))
        edit_drag_scalar(safe_addr(g_drag_i8), (id = "DRAG_I8",
                                                text = "drag int8",
                                                speed = 0.5f,
                                                v_min = int8(-100), v_max = int8(100)))
        edit_drag_scalar(safe_addr(g_drag_u16), (id = "DRAG_U16",
                                                 text = "drag uint16",
                                                 speed = 1.0f,
                                                 v_min = uint16(0), v_max = uint16(10000)))
        edit_drag_scalar(safe_addr(g_drag_i64), (id = "DRAG_I64",
                                                 text = "drag int64",
                                                 speed = 10.0f,
                                                 v_min = -1000000l, v_max = 1000000l))
        separator(SEP_SLIDER)
        text(LEAD_SLIDER, (text = "edit_slider_scalar across double and uint8."))
        edit_slider_scalar(safe_addr(g_slider_double), (id = "SLIDER_D",
                                                        text = "slider double",
                                                        v_min = 0.0lf, v_max = 1.0lf,
                                                        format = "%.4f"))
        edit_slider_scalar(safe_addr(g_slider_u8), (id = "SLIDER_U8",
                                                   text = "slider uint8",
                                                   v_min = uint8(0), v_max = uint8(255)))
        separator(SEP_INPUT)
        text(LEAD_INPUT, (text = "edit_input_scalar across uint and double."))
        edit_input_scalar(safe_addr(g_input_uint), (id = "INPUT_U",
                                                    text = "input uint",
                                                    step = 1u))
        edit_input_scalar(safe_addr(g_input_double), (id = "INPUT_D2",
                                                      text = "input double",
                                                      step = 0.01lf))
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
