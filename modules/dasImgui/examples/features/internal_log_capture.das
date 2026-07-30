options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_log_capture — the imgui_internal.h logging/capture primitives
//          below the public Log* API. Cherry-picked into the v2 binding
//          (bind_imgui.das internal_allow_func): LogBegin (start a capture to a
//          chosen target) and LogSetNextTextDecoration (wrap the next captured
//          text); LogRenderedText (nullable ref_pos / text_end) is a manual C++
//          wrapper. The ImGuiLogType enum (None/TTY/File/Buffer/Clipboard) selects
//          the target. Surface = the with_log scope guard (imgui_scope_builtin.das,
//          LogBegin / LogFinish) plus the two raw calls. This is the "binding is
//          usable" gate: it drives the real bound API at runtime.
// SHOWS:   a small widget "report" captured to the system clipboard and read back
//          via the already-bound GetClipboardText — proving (1) real widget text is
//          captured, (2) LogSetNextTextDecoration prefixes a section header ("# "),
//          and (3) LogRenderedText injects a line that appears ONLY in the capture,
//          never in the on-screen report.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_log_capture.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_log_capture.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_log_capture.das
// =============================================================================

var g_decorate = true
var g_capture_count = 0
var g_last_clip = ""
var g_capture_requested = false

// The report submitted each frame. When `logging` is true it runs inside a
// with_log scope, so every widget's rendered text is captured to the target — plus
// a decorated section header and a log-only injected line.
def private render_report(logging : bool) {
    text("=== Daslang Widget Report ===")
    // single '#' (not '##') — imgui strips everything after '##', so readback would lose it.
    if (logging && g_decorate) {
        LogSetNextTextDecoration("# ", "")
    }
    text("Section: Fruits")
    text("  Apple, Banana, Cherry")
    if (logging && g_decorate) {
        LogSetNextTextDecoration("# ", "")
    }
    text("Section: Colours")
    text("  Red, Green, Blue")
    // log-only: never drawn in the window, only appended to the active capture.
    if (logging) {
        LogRenderedText("(injected via LogRenderedText - capture #{g_capture_count})")
    }
}

[export]
def init() {
    harness_init("internal_log_capture - LogBegin / LogRenderedText / decoration", 700, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.2
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 540.0), ImGuiCond.Always)
    window(LOG_WIN, (text = "internal_log_capture", closable = false,
                     flags = ImGuiWindowFlags.None)) {

        text("LogBegin / LogRenderedText / LogSetNextTextDecoration capture the rendered")
        text("text of widgets to a target. Here: the system clipboard, then read it back.")
        separator()

        if (button(BTN_CAP, (text = "Capture report to clipboard"))) {
            g_capture_requested = true
        }
        same_line()
        if (button(BTN_DEC, (text = g_decorate ? "header decoration: ON" : "header decoration: OFF"))) {
            g_decorate = !g_decorate
        }
        separator()

        // The report renders every frame; this frame, wrap it in a Clipboard-type log
        // when a capture was requested so each widget's text lands in the clipboard.
        let capture_now = g_capture_requested
        g_capture_requested = false
        if (capture_now) {
            g_capture_count++
            with_log(ImGuiLogFlags.OutputClipboard, -1) {
                render_report(true)
            }
            g_last_clip := GetClipboardText()
        } else {
            render_report(false)
        }

        separator()
        text("Captures so far: {g_capture_count}. Note the '# ' header decoration and the")
        text("injected line below — neither is part of the on-screen report above:")
        child(CLIP_CHILD, (text = "clip", size = float2(640.0f, 200.0f),
                           child_flags = ImGuiChildFlags.Borders,
                           window_flags = ImGuiWindowFlags.None)) {
            text(CLIP_VIEW, (text = g_last_clip))
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
