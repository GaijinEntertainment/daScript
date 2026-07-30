options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_external_vectors — [edit_widget] vector overloads (boost §3.7).
// SHOWS:   edit_slider_*2/3/4, edit_drag_*2/3/4, edit_input_*2/3/4 for both
//          float and int. Each Tn? pointer is reinterpret-cast to T? at the
//          ImGui call (Slider* / Drag* / Input* family take a single T*).
//          Payload shape: {value: {x, y[, z[, w]]}}; imgui_force_set takes the
//          same shape and writes through the full vector.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/SF3","value":{"x":0.5,"y":0.6,"z":0.7}}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_external_vectors.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_external_vectors.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_external_vectors.das
// =============================================================================

var private g_sf2 : float2 = float2(0.1f, 0.2f)
var private g_sf3 : float3 = float3(0.1f, 0.2f, 0.3f)
var private g_sf4 : float4 = float4(0.1f, 0.2f, 0.3f, 0.4f)
var private g_si2 : int2 = int2(1, 2)
var private g_si3 : int3 = int3(1, 2, 3)
var private g_si4 : int4 = int4(1, 2, 3, 4)
var private g_if2 : float2 = float2(1.0f, 2.0f)
var private g_if3 : float3 = float3(1.0f, 2.0f, 3.0f)
var private g_if4 : float4 = float4(1.0f, 2.0f, 3.0f, 4.0f)
var private g_ii2 : int2 = int2(11, 22)
var private g_ii3 : int3 = int3(11, 22, 33)
var private g_ii4 : int4 = int4(11, 22, 33, 44)
var private g_df2 : float2 = float2(1.0f, 2.0f)
var private g_df3 : float3 = float3(1.0f, 2.0f, 3.0f)
var private g_df4 : float4 = float4(1.0f, 2.0f, 3.0f, 4.0f)
var private g_di2 : int2 = int2(10, 20)
var private g_di3 : int3 = int3(10, 20, 30)
var private g_di4 : int4 = int4(10, 20, 30, 40)

[export]
def init() {
    harness_init("edit_external_vectors — boost v2 §3.7", 800, 760)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(760.0, 700.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_* vectors", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        separator_text(SEC_SLIDER, (text = "slider"))
        edit_slider_float2(safe_addr(g_sf2), (id = "SF2", text = "float2"))
        edit_slider_float3(safe_addr(g_sf3), (id = "SF3", text = "float3"))
        edit_slider_float4(safe_addr(g_sf4), (id = "SF4", text = "float4"))
        edit_slider_int2(safe_addr(g_si2), (id = "SI2", text = "int2"))
        edit_slider_int3(safe_addr(g_si3), (id = "SI3", text = "int3"))
        edit_slider_int4(safe_addr(g_si4), (id = "SI4", text = "int4"))

        separator_text(SEC_INPUT, (text = "input"))
        edit_input_float2(safe_addr(g_if2), (id = "IF2", text = "float2"))
        edit_input_float3(safe_addr(g_if3), (id = "IF3", text = "float3"))
        edit_input_float4(safe_addr(g_if4), (id = "IF4", text = "float4"))
        edit_input_int2(safe_addr(g_ii2), (id = "II2", text = "int2"))
        edit_input_int3(safe_addr(g_ii3), (id = "II3", text = "int3"))
        edit_input_int4(safe_addr(g_ii4), (id = "II4", text = "int4"))

        separator_text(SEC_DRAG, (text = "drag"))
        edit_drag_float2(safe_addr(g_df2), (id = "DF2", text = "float2"))
        edit_drag_float3(safe_addr(g_df3), (id = "DF3", text = "float3"))
        edit_drag_float4(safe_addr(g_df4), (id = "DF4", text = "float4"))
        edit_drag_int2(safe_addr(g_di2), (id = "DI2", text = "int2"))
        edit_drag_int3(safe_addr(g_di3), (id = "DI3", text = "int3"))
        edit_drag_int4(safe_addr(g_di4), (id = "DI4", text = "int4"))
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
