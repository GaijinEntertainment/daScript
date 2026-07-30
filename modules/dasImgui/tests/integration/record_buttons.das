options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``buttons.apng`` against
//! ``examples/tutorial/buttons.das``. Tours six click triggers —
//! button, small_button, arrow_button (pair), invisible_button,
//! image_button (font atlas), tab_item_button inside a tab_bar.
//! Each stage speaks an ear-first ``voice`` line while the click fires
//! UNDER it (cursor already on target) so the ripple and the counter
//! tick are visible as Emma talks — the on-screen ``caption`` stays terse.

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
    with_recording_app("examples/tutorial/buttons.das",
                       "buttons.apng", 60) $(app) {
        let T_GO    = "BT_WIN/BT_GO"
        let T_SMALL = "BT_WIN/BT_SMALL"
        let T_BACK  = "BT_WIN/BT_BACK"
        let T_FWD   = "BT_WIN/BT_FWD"
        let T_HOT   = "BT_WIN/BT_HOT"
        let T_IMG   = "BT_WIN/BT_IMG"
        let T_HELP  = "BT_WIN/BT_BAR/BT_HELP"
        let T_ADD   = "BT_WIN/BT_BAR/BT_ADD"

        var snap = wait_for_widget(app, T_GO, 10.0f)
        if (snap == null) {
            panic("{T_GO} never registered — wrong app running?")
        }
        let p_go    = widget_center(snap, T_GO)
        let p_small = widget_center(snap, T_SMALL)
        let p_back  = widget_center(snap, T_BACK)
        let p_fwd   = widget_center(snap, T_FWD)
        let p_hot   = widget_center(snap, T_HOT)
        let p_img   = widget_center(snap, T_IMG)
        let p_help  = widget_center(snap, T_HELP)
        let p_add   = widget_center(snap, T_ADD)

        //! Initial cursor placement.
        move_to(app, p_go, 500)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: button — the workhorse ----
        hold_through_voice(app, say_begin(app, "button - the workhorse", T_GO,
            [voice = "Here is the everyday button. Each time you click it, its counter goes up by one. Watch the number climb."]),
            [T_GO, T_GO])

        // ---- Stage 2: small_button — no padding ----
        move_to(app, p_small, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "small_button - no padding", T_SMALL,
            [voice = "A small button is the same thing, just without the padding around the label. Handy for tight tool strips, like a quit or a reset."]),
            [T_SMALL])

        // ---- Stage 3: arrow_button — directional pair ----
        move_to(app, p_back, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "arrow_button - directional", [T_BACK, T_FWD],
            [voice = "Arrow buttons draw a little triangle. You pass a direction, and each one needs its own name. Here is back, and then forward."]),
            [T_BACK, T_FWD])

        // ---- Stage 4: invisible_button — hit area without glyph ----
        move_to(app, p_hot, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "invisible_button - hit area", T_HOT,
            [voice = "An invisible button is a clickable region that draws nothing at all. Lay one over your own artwork to make any shape respond to clicks."]),
            [T_HOT])

        // ---- Stage 5: image_button — font atlas as texture ----
        move_to(app, p_img, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "image_button - textured", T_IMG,
            [voice = "An image button wears a texture as its face. Here it is showing the font atlas, but a real app would load its own picture."]),
            [T_IMG])

        // ---- Stage 6: tab_item_button — inside tab_bar ----
        move_to(app, p_help, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "tab_item_button - tab styled", [T_HELP, T_ADD],
            [voice = "This one looks like a tab, and it must live inside a tab bar. A leading button anchors to the start, a trailing one to the end. Think of a help mark, or an add button."]),
            [T_HELP, T_ADD])
    }
}
