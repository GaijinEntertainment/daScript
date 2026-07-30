options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_external_scalars — [edit_widget] rail, scalar overloads.
// SHOWS:   edit_slider_{float,int}, edit_drag_{float,int}, edit_input_{float,int},
//          edit_input_double — all take a caller-owned `T?` pointer. No state
//          struct, no auto-emit; payload is `{value: T}`. Caller picks
//          safe_addr(local) (when the pointee is a plain local/global) or
//          unsafe(addr(struct.field)) for struct-field access. Required
//          `id="…"` literal at the call site keys the telemetry path —
//          missing id fails at compile time.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/SLIDER_F","value":0.9}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/SLIDER_F".payload.value'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_external_scalars.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_external_scalars.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_external_scalars.das
// =============================================================================

var private g_slider_f : float = 0.5f
var private g_slider_i : int = 50
var private g_drag_f : float = 1.0f
var private g_drag_i : int = 7
var private g_input_f : float = 3.14f
var private g_input_i : int = 42
var private g_input_d : double = 6.28lf

[export]
def init() {
    harness_init("edit_external_scalars — boost v2 §3.7", 760, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_* scalars", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Each row reads/writes through safe_addr(global). imgui_force_set updates the pointee directly."))
        edit_slider_float(safe_addr(g_slider_f), (id = "SLIDER_F",
                                                  text = "slider float",
                                                  v_min = 0.0f, v_max = 1.0f))
        edit_slider_int(safe_addr(g_slider_i), (id = "SLIDER_I",
                                                text = "slider int",
                                                v_min = 0, v_max = 100))
        edit_drag_float(safe_addr(g_drag_f), (id = "DRAG_F",
                                              text = "drag float", speed = 0.1f))
        edit_drag_int(safe_addr(g_drag_i), (id = "DRAG_I",
                                            text = "drag int", speed = 1.0f))
        edit_input_float(safe_addr(g_input_f), (id = "INPUT_F",
                                                text = "input float", step = 0.1f))
        edit_input_int(safe_addr(g_input_i), (id = "INPUT_I",
                                              text = "input int", step = 1))
        edit_input_double(safe_addr(g_input_d), (id = "INPUT_D",
                                                 text = "input double", step = 0.01lf))
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
