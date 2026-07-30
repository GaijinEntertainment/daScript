options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: text_filter + passes_filter — incl/excl text filter rail.
// SHOWS:   - `text_filter(FILT)` — boost wrapper over ImGuiTextFilter::Draw.
//            Renders an inline InputText editor; user types comma-separated
//            include tokens (`foo,bar`) or exclude tokens (`-baz`). Returns
//            true on the frame the filter expression changes.
//          - `passes_filter(FILT, line)` — predicate evaluated against the
//            current filter state. Empty filter = match-all.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/text_filter.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/text_filter.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/text_filter.das
// =============================================================================

var private LOG_ROW : table<int; NarrativeState>

let private LOG_LINES <- [
    "INFO  bootstrap done",
    "DEBUG cache warmed",
    "WARN  retry attempt 1/3",
    "ERROR connection timeout",
    "INFO  request 200 ok",
    "WARN  retry attempt 2/3",
    "ERROR connection refused",
    "INFO  shutdown clean",
    "DEBUG cache flushed",
    "INFO  goodbye"
]

[export]
def init() {
    harness_init("text_filter + passes_filter", 520, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 380.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "text_filter", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Try: 'INFO' (incl) / '-DEBUG' (excl) / 'WARN,ERROR' (multi-incl)."))
        text_filter(FILT, (text = "Filter (incl,-excl)", width = 220.0f))

        separator(SEP)

        for (i in range(length(LOG_LINES))) {
            if (passes_filter(FILT, LOG_LINES[i])) {
                text(LOG_ROW[i], (text = "{LOG_LINES[i]}"))
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
