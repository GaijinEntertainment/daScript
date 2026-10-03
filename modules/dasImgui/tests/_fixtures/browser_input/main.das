options gen2
options gc
require imgui/imgui_harness
require imgui_app
require daslib/fio
require daslib/json_boost
var input_enabled = true

[export]
def init() {
    harness_init("Browser text input", 800, 400)
}
[export]
def update() {
    if (fexist("/input-reinit")) { remove("/input-reinit"); harness_shutdown(); init(); input_enabled = true }
    let enabled = !fexist("/input-disable")
    if (enabled != input_enabled) { imgui_set_real_input_callbacks(enabled); input_enabled = enabled }
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowPos(float2(10, 10), ImGuiCond.Always)
    SetNextWindowSize(float2(760, 330), ImGuiCond.Always)
    window(INPUT_WINDOW, (text = "Browser text input", closable = false, flags = ImGuiWindowFlags.None)) {
        input_text(SINGLE, (text = "Single line"))
        let single_active = IsItemActive()
        input_text_multiline(MULTI, (text = "Multiple lines", size = float2(540, 150)))
        var state = JV((single = SINGLE.value, multi = MULTI.value, active = GetIO().WantTextInput, enabled = input_enabled, active_field = single_active ? "single" : IsItemActive() ? "multi" : ""))
        fwrite("/input-state.json", write_json_compact(state)); delete_json(state)
    }
    harness_end_frame()
}
[export]
def shutdown(){harness_shutdown()}
