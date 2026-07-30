options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record with_id.apng against examples/tutorial/with_id.das.
//! Drives every site the human way and self-verifies — the recording doubles as a test:
//!   with_id scopes -> click the same Save button in scope A then B; one shared counter ticks 0,1,2
//!   nested chain   -> click the deep button (outer/inner/NESTED_BTN)
//!   id= per-call   -> click the hash-stable button
//!   path= per-call -> drag the slider whose driver path is pinned to "rps_stable"
//! Each click asserts click_count ticked; the drag asserts the value moved — a dud aborts at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/with_id.das",
                       "with_id.apng", 55) $(app) {
        let T_SAVE_A = "ID_WIN/section_a/SAVE_BTN"
        let T_SAVE_B = "ID_WIN/section_b/SAVE_BTN"
        let T_NESTED = "ID_WIN/outer/inner/NESTED_BTN"
        let T_HASHED = "ID_WIN/HASHED_BTN"
        let T_RPS    = "ID_WIN/rps_stable"

        var snap = wait_for_render(app, T_SAVE_A, 10.0f)
        if (snap == null) {
            panic("{T_SAVE_A} never rendered - wrong app running?")
        }

        // ---- Stage 1: with_id scopes — same widget, two render sites ----
        move_to(app, widget_click_point(snap, T_SAVE_A), 600)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "with_id - same widget, two scopes", [T_SAVE_A, T_SAVE_B],
            [voice = "with_id opens a scope, so the same widget can be drawn more than once without clashing. Here one Save button appears in scope A and again in scope B. Click each; they share a single counter, so it climbs to two."]),
            [T_SAVE_A, T_SAVE_B])

        // ---- Stage 2: nested with_id chain ----
        move_to(app, widget_click_point(snap, T_NESTED), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "with_id chains nest (outer/inner)", T_NESTED,
            [voice = "Scopes nest. Wrap one inside another and the paths simply join with slashes - here outer, then inner, then the button."]),
            [T_NESTED])

        // ---- Stage 3: id= per-call — stable ImGui hash ----
        move_to(app, widget_click_point(snap, T_HASHED), 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "id= - stable ImGui hash", T_HASHED,
            [voice = "You can also tag a single call. The id argument keeps a widget's internal hash stable even if you rename it later, so saved layouts and scripts keep working."]),
            [T_HASHED])

        // ---- Stage 4: path= per-call — real slider drag ----
        let bb = widget_bbox(snap, T_RPS)
        let cy = (bb.y + bb.w) * 0.5f
        move_to(app, (bb.x + 14.0f, cy), 800)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "path= - stable driver path", T_RPS,
            [voice = "The path argument does the same for the name external drivers and tools use to address the widget. Drag the speed slider; rename it in code and it still answers to this stable path."])
        drag_through_voice(app, d4, T_RPS, (bb.x + 0.65f * (bb.z - bb.x), cy))
    }
}
