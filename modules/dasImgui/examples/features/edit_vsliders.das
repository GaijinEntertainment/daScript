options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_vsliders — vertical-slider variants of edit_slider_{float,int}.
// SHOWS:   edit_vslider_float + edit_vslider_int. Same external-pointer shape
//          as their horizontal siblings, but the cursor moves vertically and
//          the per-call `size` parameter sets the column's pixel dimensions
//          (typical 40x200 for an audio-mixer style fader). Use `same_line`
//          between calls to lay multiple vsliders side-by-side.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/VSLIDE_F1","value":0.8}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_vsliders.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_vsliders.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_vsliders.das
// =============================================================================

var private g_vol_f1 : float = 0.6f
var private g_vol_f2 : float = 0.3f
var private g_vol_f3 : float = 0.85f
var private g_count_i1 : int = 30
var private g_count_i2 : int = 75

[export]
def init() {
    harness_init("edit_vsliders — boost v2 §3.7", 640, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(600.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_vslider_*", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Three float channels + two int channels, all vertical."))
        edit_vslider_float(safe_addr(g_vol_f1), (id = "VSLIDE_F1",
                                                 text = "ch1",
                                                 size = float2(40.0f, 220.0f),
                                                 v_min = 0.0f, v_max = 1.0f))
        same_line(SL1)
        edit_vslider_float(safe_addr(g_vol_f2), (id = "VSLIDE_F2",
                                                 text = "ch2",
                                                 size = float2(40.0f, 220.0f),
                                                 v_min = 0.0f, v_max = 1.0f))
        same_line(SL2)
        edit_vslider_float(safe_addr(g_vol_f3), (id = "VSLIDE_F3",
                                                 text = "ch3",
                                                 size = float2(40.0f, 220.0f),
                                                 v_min = 0.0f, v_max = 1.0f))
        same_line(SL3)
        edit_vslider_int(safe_addr(g_count_i1), (id = "VSLIDE_I1",
                                                 text = "n1",
                                                 size = float2(40.0f, 220.0f),
                                                 v_min = 0, v_max = 100))
        same_line(SL4)
        edit_vslider_int(safe_addr(g_count_i2), (id = "VSLIDE_I2",
                                                 text = "n2",
                                                 size = float2(40.0f, 220.0f),
                                                 v_min = 0, v_max = 100))
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
