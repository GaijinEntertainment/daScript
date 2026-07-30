options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// TUTORIAL: wrap_tab_bar / wrap_tab_item — a multi-row, WRAPPING tab bar.
//
//   wrap_tab_bar(WTB_BAR) {
//       wrap_tab_item("physics") { <body> }
//       wrap_tab_item("camera")  { <body> }
//       ...
//   }
//
// ImGui's native `tab_bar` shrinks/ellipsizes or scrolls its headers when they
// overflow the width; `wrap_tab_bar` instead wraps them onto as many rows as
// needed and shows only the active tab's body. The active tab is an INDEX
// (like `combo`) — `imgui_force_set {"value":N}` selects tab N. Because the
// body renders after the whole header strip, the strip is laid out from the
// labels seen on the PREVIOUS frame: a changed label set appears one frame late
// (never visible for a stable list). Keep tab declaration order stable.
//
// The narrow window below forces the headers to wrap across rows.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/wrap_tab_bar.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/wrap_tab_bar.das
//
// DRIVE (when running live) — select the "lava" tab (index 2):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"WTB_WIN/WTB_BAR","value":2}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    harness_init("wrap_tab_bar tutorial", 380, 520)
}

[export]
def update() {
    if (!harness_begin_frame()) {
        return
    }
    harness_new_frame()

    // deliberately narrow so the eight headers wrap onto multiple rows
    SetNextWindowSize(ImVec2(340.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(WTB_WIN, (text = "wrap_tab_bar tutorial", closable = false, flags = ImGuiWindowFlags.None)) {
        text(WTB_LEAD, (text = "Eight categories in a narrow panel - the native"))
        text(WTB_LEAD2, (text = "TabBar would ellipsize/scroll; wrap_tab_bar flows them."))
        separator()

        wrap_tab_bar(WTB_BAR) {
            wrap_tab_item("physics") {
                text(WTB_B0, (text = "physics - gravity, restitution, drag"))
            }
            wrap_tab_item("camera") {
                text(WTB_B1, (text = "camera - zoom, follow, lookahead"))
            }
            wrap_tab_item("lava") {
                text(WTB_B2, (text = "lava - speed, scale, warp, crust"))
            }
            wrap_tab_item("cinders") {
                text(WTB_B3, (text = "cinders - brightness, density, rise"))
            }
            wrap_tab_item("audio") {
                text(WTB_B4, (text = "audio - master gain, music, sfx"))
            }
            wrap_tab_item("lighting") {
                text(WTB_B5, (text = "lighting - ambient, noise, scale"))
            }
            wrap_tab_item("grid") {
                text(WTB_B6, (text = "grid - size, angle, snap step"))
            }
            wrap_tab_item("misc") {
                text(WTB_B7, (text = "misc - everything else"))
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
