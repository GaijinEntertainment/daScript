options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: list_clipper — virtual-list culling rail.
// SHOWS:   `list_clipper(items_count) $(start, end_) { ... }` — block fires
//          once per visible chunk with `[start, end_)` row indices. Render
//          exactly those rows inside the block; rows outside the visible
//          region are SKIPPED entirely (no widget submission). The current
//          scrolling region (this demo's `child(SCROLL_AREA, ...)`) must
//          be scrollable so the clipper has something to cull.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/list_clipper.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/list_clipper.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/list_clipper.das
// =============================================================================

var private CLIPPED_ROW : table<int; NarrativeState>
let private ITEM_COUNT = 1000

[export]
def init() {
    harness_init("list_clipper — 1000-row virtual list", 480, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(440.0, 440.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "list_clipper", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "1000-row virtual list — only the visible chunk is rendered each frame."))

        child(SCROLL_AREA, (text = "scroll", size = float2(400.0f, 360.0f),
                            child_flags = ImGuiChildFlags.Borders,
                            window_flags = ImGuiWindowFlags.None)) {
            list_clipper(ITEM_COUNT) $(start; end_) {
                for (i in range(start, end_)) {
                    text(CLIPPED_ROW[i], (text = "row {i}"))
                }
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
