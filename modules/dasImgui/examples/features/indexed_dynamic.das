options gen2
options _comment_hygiene = true

require imgui/imgui_harness
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
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/indexed_dynamic.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/indexed_dynamic.das
// =============================================================================

// Table starts EMPTY — no pre-populated keys. Each ADD button click inserts
// a fresh entry; REMOVE erases the highest key; BUMP_LAST nudges the value
// of the highest-indexed channel.
var private CHANNEL_VOL : table<int; SliderStateFloat>

[export]
def init() {
    harness_init("indexed_dynamic", 600, 500)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
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
    return if (!harness_begin_frame())
    harness_new_frame()

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
        same_line((spacing = 12.0f))
        if (button(REMOVE_CH, (text = "REMOVE channel"))) {
            let hi = find_max_key(CHANNEL_VOL)
            if (hi >= 0) {
                CHANNEL_VOL |> erase(hi)
            }
        }
        same_line((spacing = 12.0f))
        if (button(BUMP_LAST, (text = "BUMP last"))) {
            let hi = find_max_key(CHANNEL_VOL)
            if (hi >= 0) {
                let cur = CHANNEL_VOL[hi].value
                CHANNEL_VOL[hi].value = min(cur + 0.1f, 1.0f)
            }
        }

        text("channels: {length(CHANNEL_VOL)}")
        separator()

        // ----- Live render of every current key -----

        // Snapshot keys to an array first — `CHANNEL_VOL[k]` would trip "can't
        // insert to a locked table" if iterated through keys() directly, since
        // table[k] auto-inserts while the iterator holds the table lock.
        let live_keys <- [for (k in keys(CHANNEL_VOL)); k]
        for (k in live_keys) {
            slider_float(CHANNEL_VOL[k], (text = "ch{k}"))
        }
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
