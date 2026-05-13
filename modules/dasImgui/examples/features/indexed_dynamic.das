options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_containers_builtin
require imgui/imgui_widgets_builtin
require math

// =============================================================================
// FEATURE: indexed_dynamic — table-keyed sliders with runtime add/remove.
// SHOWS:   Snapshot tracks the live table contents per-frame; entries appear
//          when a key is inserted and vanish when erased (via §G expiration).
// DRIVE:   curl -s -X POST -d '{"name":"imgui_click","args":{
//                "target":"DYN_WIN/ADD_CH"}}' localhost:9090/command
//          # -> next snapshot has DYN_WIN/CHANNEL_VOL[0]
//          curl -s -X POST -d '{"name":"imgui_click","args":{
//                "target":"DYN_WIN/REMOVE_CH"}}' localhost:9090/command
//          # -> next snapshot drops the highest-keyed CHANNEL_VOL entry
//          curl -s -X POST -d '{"name":"imgui_click","args":{
//                "target":"DYN_WIN/BUMP_LAST"}}' localhost:9090/command
//          # -> last channel's .value increments by 0.1
// STANDALONE: daslang.exe modules/dasImgui/examples/features/indexed_dynamic.das
// LIVE:       daslang-live modules/dasImgui/examples/features/indexed_dynamic.das
// =============================================================================

// Table starts EMPTY — no pre-populated keys. Each ADD button click inserts
// a fresh entry; REMOVE erases the highest key; BUMP_LAST nudges the value
// of the highest-indexed channel.
var private CHANNEL_VOL : table<int; SliderStateFloat>

[export]
def init() {
    live_create_window("indexed_dynamic", 600, 500)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

def find_max_key(t : table<int; SliderStateFloat>) : int {
    var hi = -1
    for (k in keys(t)) {
        if (k > hi) {
            hi = k
        }
    }
    return hi
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(520.0, 440.0), ImGuiCond.Always)
    window(DYN_WIN, (text = "indexed_dynamic", closable = false, flags = ImGuiWindowFlags.None)) {
        // ----- Controls -----
        if (button(ADD_CH, (text = "ADD channel"))) {
            let next_key = find_max_key(CHANNEL_VOL) + 1
            unsafe {
                CHANNEL_VOL[next_key].bounds = (0.0f, 1.0f)
                CHANNEL_VOL[next_key].value = 0.5f
            }
        }
        SameLine(0.0f, 12.0f)
        if (button(REMOVE_CH, (text = "REMOVE channel"))) {
            let hi = find_max_key(CHANNEL_VOL)
            if (hi >= 0) {
                CHANNEL_VOL |> erase(hi)
            }
        }
        SameLine(0.0f, 12.0f)
        if (button(BUMP_LAST, (text = "BUMP last"))) {
            let hi = find_max_key(CHANNEL_VOL)
            if (hi >= 0) {
                let cur = CHANNEL_VOL[hi].value
                CHANNEL_VOL[hi].value = min(cur + 0.1f, 1.0f)
            }
        }

        Text("channels: {length(CHANNEL_VOL)}")
        Separator()

        // ----- Live render of every current key -----
        // Snapshot keys to an array first — the wrapper's `CHANNEL_VOL[k]`
        // access would trip "can't insert to a locked table" if iterated
        // directly through `keys(...)`, since table[k] auto-inserts and the
        // iterator holds the table lock.
        var live_keys : array<int>
        for (k in keys(CHANNEL_VOL)) {
            live_keys |> push(k)
        }
        for (k in live_keys) {
            slider_float(CHANNEL_VOL[k], (text = "ch{k}"))
        }
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

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
