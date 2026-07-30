options gen2
options _comment_hygiene = true

require imgui/imgui_harness

var SCROLLY_ROW : table<int; NarrativeState>
var LOG_ROW : table<int; NarrativeState>

// nolint:STYLE014 — file-header feature banner displaced below var decls
// =============================================================================
// FEATURE: scroll_state — Phase 1.2 scroll capture for window + child.
// SHOWS:   WindowState and ChildState now carry `scroll` and `scroll_max`
//          (float2). Each frame the container body reads GetScrollX/Y/MaxX/MaxY
//          inside Begin → invoke(blk) → End and stores them on state. The
//          standard serializer surfaces them through imgui_snapshot.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals."SCROLLY_WIN".payload.scroll      = [0, 0..max_y]
//                  .scroll_max  = [0, max_y]
//                  .size        = [400, 200]
// STANDALONE: daslang.exe modules/dasImgui/examples/features/scroll_state.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/scroll_state.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/scroll_state.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 1.2 — scroll state", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(420.0, 240.0), ImGuiCond.Always)
    window(SCROLLY_WIN, (text = "Scrolly window", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        text("Window scroll values appear in SCROLLY_WIN.payload.scroll")
        text("along with scroll_max. Forty rows below to make it scroll.")
        for (i in range(0, 40)) {
            text(SCROLLY_ROW[i],
                (text = "row {i}: lorem ipsum dolor sit amet consectetur adipiscing elit"))
        }
    }

    SetNextWindowPos(ImVec2(440.0, 0.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(360.0, 240.0), ImGuiCond.Always)
    window(NESTED_WIN, (text = "Child host", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        text("Below: a fixed-size child region with its own scroll state.")
        child(LOG_REGION, (text = "##log", size = float2(320.0, 140.0),
                           child_flags = ImGuiChildFlags.Borders,
                           window_flags = ImGuiWindowFlags.None)) {
            for (i in range(0, 25)) {
                text(LOG_ROW[i], (text = "log line {i}"))
            }
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
