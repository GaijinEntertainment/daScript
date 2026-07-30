options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: columns_demo — §4.6 `columns(N, ${a}, ${b}, …)` call_macro.
// SHOWS:   - `columns(3, ${col0}, ${col1}, ${col2})` expands to
//            columns_open(3); invoke(col0); next_col(); invoke(col1);
//            next_col(); invoke(col2); columns_close().
//          - Each column's children are addressed by the surrounding window's
//            path (columns is not a container — it doesn't push a path leaf).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/columns_demo.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/columns_demo.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/columns_demo.das
// =============================================================================

[export]
def init() {
    harness_init("§4.6 — columns demo", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(560.0f, 380.0f), ImGuiCond.FirstUseEver)
    window(COLS_WIN, (text = "Columns demo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Three columns:")
        columns(3,
                ${
                    text("Col 0")
                    button(C0_BTN, (text = "Col 0 btn"))
                },
                ${
                    text("Col 1")
                    button(C1_BTN, (text = "Col 1 btn"))
                },
                ${
                    text("Col 2")
                    button(C2_BTN, (text = "Col 2 btn"))
                })
        text("After columns block")
        button(AFTER_BTN, (text = "After"))
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
