options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: inputs_indexed — table-keyed slider widgets per API_REWORK §4.4.
// SHOWS:   `slider_float(CHANNEL_VOL[i], ...)` (int-keyed),
//          `slider_float(TRACK_VOL[name], ...)` (string-keyed).
// DRIVE:   curl -s -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # snapshot.globals has "MIX_WIN/CHANNEL_VOL[3]" and "MIX_WIN/TRACK_VOL[lead]"
//          curl -s -X POST -d '{"name":"imgui_force_set","args":{
//               "target":"MIX_WIN/CHANNEL_VOL[3]","value":0.7}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_indexed.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_indexed.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_indexed.das
// =============================================================================

// Tables are user-declared; the [widget] macro detects subscript form and
// emits a per-(kind, IDENT) wrapper that registers each rendered key into
// g_widgets on first touch. Bounds/initial value are populated imperatively
// in init() — per-call init=/bounds= sugar is not available on indexed form
// (V1 limitation; see plan §H).
var private CHANNEL_VOL : table<int; SliderStateFloat>
var private TRACK_VOL : table<string; SliderStateFloat>

let TRACK_NAMES : array<string> <- ["bass", "lead", "drum", "vocals"]

[export]
def init() {
    harness_init("inputs_indexed", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
    // Pre-populate so first-frame snapshot already has the right bounds + value.
    for (i in range(4)) {
        CHANNEL_VOL[i].bounds = (0.0f, 1.0f)
        CHANNEL_VOL[i].value = 0.5f
    }
    for (name in TRACK_NAMES) {
        TRACK_VOL[name].bounds = (0.0f, 1.0f)
        TRACK_VOL[name].value = 0.7f
    }
}

[export]
def update() {
    return if (!harness_begin_frame())
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    // Use the boost `window` container so indexed leaves get the path
    // prefix MIX_WIN/CHANNEL_VOL[i] in g_widgets — proves container_path
    // composition works for indexed leaves.
    window(MIX_WIN, (text = "MIX_WIN", closable = false, flags = ImGuiWindowFlags.None)) {
        text("Int-keyed channels (CHANNEL_VOL[0..3]):")
        separator()
        for (i in range(4)) {
            // Each [i] resolves to a distinct path component CHANNEL_VOL[<i>]
            // — separate g_widgets entry, separate dispatcher, etc.
            slider_float(CHANNEL_VOL[i], (text = "ch{i}"))
        }

        spacing()
        text("String-keyed tracks (TRACK_VOL[name]):")
        separator()
        for (name in TRACK_NAMES) {
            let _slid = slider_float(TRACK_VOL[name], (text = name))
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
