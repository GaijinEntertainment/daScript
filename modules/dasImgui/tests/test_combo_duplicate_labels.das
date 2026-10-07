options gen2
options _allow_imgui_legacy = true
require dastest/testing_boost public
require imgui/imgui_boost_v2
require imgui/imgui_boost_runtime
require imgui/imgui_containers_builtin
require imgui/imgui_widgets_builtin
require daslib/safe_addr

[test]
def duplicate_combo_rows_select_independently(t : T?) {
    for (mode in range(4)) {
        let editable = mode == 1
        let getter_mode = mode >= 2
        let omit_row = mode == 3
        let context = CreateContext(null)
        var io & = unsafe(GetIO())
        io.DisplaySize = float2(640, 480)
        io.BackendFlags |= ImGuiBackendFlags.RendererHasTextures
        var selected = 0
        var point = float2(0)
        let items = ["Fence", "Fence", "Fence"]
        for (frame in range(12)) {
            if (frame == 2 || frame == 7) { io |> AddMousePosEvent(point.x, point.y); io |> AddMouseButtonEvent(0, true) }
            if (frame == 3 || frame == 8) { io |> AddMouseButtonEvent(0, false) }
            NewFrame(); imgui_boost_runtime::begin_frame()
            window(COMBO_TEST, (text = "Repeated rows", closable = false, flags = ImGuiWindowFlags.None)) {
                if (editable) { edit_combo(safe_addr(selected), (id = "EDIT", text = "Model", items = items)) }
                elif (getter_mode) {
                    combo_getter(GETTER, (text = "Model", items_count = omit_row ? 4 : 3,
                        getter = @(idx : int; var result : string&) : bool {
                            if (omit_row && idx == 1) { return false }
                            result = "Fence"
                            return true
                        }))
                    selected = GETTER.value
                }
                else { combo(CHOICE, (text = "Model", items = items)); selected = CHOICE.value }
            }
            let id = editable ? "COMBO_TEST/EDIT" : (getter_mode ? "COMBO_TEST/GETTER" : "COMBO_TEST/CHOICE")
            if (frame == 1 && key_exists(g_registry, id)) {
                let box = g_registry[id].bbox; point = (box.xy + box.zw) * .5
            }
            if (frame == 6) {
                t |> success(key_exists(g_registry, id) && length(g_registry[id].sub_bboxes) == 3, "three same-label rows are visible")
                if (key_exists(g_registry, id) && length(g_registry[id].sub_bboxes) == 3) {
                    let box = g_registry[id].sub_bboxes[2]; point = (box.xy + box.zw) * .5
                }
            }
            EndFrame()
        }
        t |> equal(selected, omit_row ? 3 : 2, "clicking third identical label selects its source index, mode={mode}")
        clear(g_registry); clear(g_widgets); DestroyContext(context)
    }
}
