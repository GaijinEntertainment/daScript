options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``wrap_tab_bar.apng`` against
//! ``examples/tutorial/wrap_tab_bar.das``. Tours the multi-row WRAPPING tab
//! bar: eight category headers wrap onto two rows in a narrow panel, and a
//! REAL synth click on a header (by name) selects that tab. Each stage clicks a
//! header UNDER its voice line and verifies the selected tab's body reveals
//! (only the active tab's body renders) - the durable proof the switch landed.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(
        b?["x"] ?? 0.0f,
        b?["y"] ?? 0.0f,
        b?["z"] ?? 0.0f,
        b?["w"] ?? 0.0f
    )
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/wrap_tab_bar.das",
                       "wrap_tab_bar.apng", 60) $(app) {
        let T_LAVA    = "WTB_WIN/WTB_BAR/lava"      // row 1, index 2
        let T_GRID    = "WTB_WIN/WTB_BAR/grid"      // row 2, index 6
        let T_PHYSICS = "WTB_WIN/WTB_BAR/physics"   // row 1, index 0
        let B_LAVA    = "WTB_WIN/WTB_BAR/WTB_B2"
        let B_GRID    = "WTB_WIN/WTB_BAR/WTB_B6"
        let B_PHYSICS = "WTB_WIN/WTB_BAR/WTB_B0"

        var snap = wait_for_widget(app, T_LAVA, 10.0f)
        if (snap == null) {
            panic("{T_LAVA} never registered - wrong app running?")
        }

        // Cursor onto the first target before the voice starts.
        move_to(app, widget_center(snap, T_LAVA), 500)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: wrapping + click-to-select ----
        click_render_voice(app, say_begin(app, "wrap_tab_bar - headers wrap, click to select", T_LAVA,
            [voice = "ImGui's native tab bar shrinks or scrolls its headers when they do not fit. This wrapping tab bar flows them onto as many rows as it needs - eight categories on two rows here. Click a header to select it; here is lava."]),
            T_LAVA, B_LAVA, true)

        // ---- Stage 2: select a header on the SECOND wrapped row ----
        move_to(app, widget_center(snap, T_GRID), 700)
        hold_content(app, SETTLE_MS)
        click_render_voice(app, say_begin(app, "select across rows", T_GRID,
            [voice = "Headers on the second row are real buttons too. Click grid on the lower row, and its panel takes over - only the active tab's body is drawn."]),
            T_GRID, B_GRID, true)

        // ---- Stage 3: back to the first tab ----
        move_to(app, widget_center(snap, T_PHYSICS), 700)
        hold_content(app, SETTLE_MS)
        click_render_voice(app, say_begin(app, "back to physics", T_PHYSICS,
            [voice = "Selection is just an index, so clicking physics brings the first tab back. The header strip is laid out from the previous frame, so keep the tab order stable."]),
            T_PHYSICS, B_PHYSICS, true)
    }
}
