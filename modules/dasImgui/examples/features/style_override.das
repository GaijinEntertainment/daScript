options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: style_override — §4.7 `with_style((key, val), ...)` { ... }.
// SHOWS:   - Color (ImGuiCol) and var (ImGuiStyleVar) tuples mixed in one call.
//          - Nested `with_style` blocks (inner additions stack on top of outer).
//          - Plain widgets after a `with_style` block see baseline style again
//            (proves pop count matches push count — no style leak).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"STYLED_BTN"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/style_override.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/style_override.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/style_override.das
// =============================================================================

[export]
def init() {
    harness_init("§4.7 — with_style", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(560.0f, 400.0f), ImGuiCond.FirstUseEver)
    window(STYLE_WIN, (text = "with_style demo", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        text("Mixed color + var, single block:")
        with_style((ImGuiCol.Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)),
                   (ImGuiStyleVar.FrameRounding, 8.0f)) {
            button(STYLED_BTN, (text = "Red rounded"))
        }

        separator()
        text("Nested with_style — inner stacks on outer:")
        with_style((ImGuiCol.Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f))) {
            with_style((ImGuiCol.Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f))) {
                button(NESTED_STYLED_BTN, (text = "Yellow text + blue button"))
            }
        }

        separator()
        text("After with_style — baseline style restored:")
        // If push/pop counts were off, this button would render with stale
        // color/rounding — and ImGui's style stack auto-asserts unbalanced pops
        // at EndFrame, crashing the process. A normal-looking button = balanced.
        button(UNSTYLED_BTN, (text = "Plain button"))
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
