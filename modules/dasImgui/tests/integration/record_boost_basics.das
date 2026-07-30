options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``boost_basics.apng`` against ``examples/tutorial/boost_basics.das``.
//! The smallest dasImgui v2 program - one slider, one button, both backed by daslang
//! globals. Two voiced, self-verifying beats:
//!   1. slider - drag Volume; VOLUME.value must CHANGE (drag_through_voice).
//!   2. button - click Bump twice; each click must register (hold_through_voice).

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/boost_basics.das",
                       "boost_basics.apng", 35) $(app) {
        let T_VOLUME = "BASICS_WIN/VOLUME"
        let T_BUMP   = "BASICS_WIN/BUMP_BTN"

        var snap = wait_for_render(app, T_BUMP, 10.0f)
        if (snap == null) {
            panic("{T_BUMP} never rendered - wrong app running?")
        }
        let p_bump = widget_center(snap, T_BUMP)
        let vol_bb = widget_bbox(snap, T_VOLUME)
        let vol_y  = (vol_bb.y + vol_bb.w) * 0.5f
        let vol_x0 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.12f
        let vol_x1 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.82f

        // Park the cursor at the slider's left and warm up the capture pipeline.
        move_to(app, (vol_x0, vol_y), 800)
        hold_content(app, 1800u)

        // ---- Beat 1: slider drives a global ----
        drag_through_voice(app, say_begin(app, "slider -> VOLUME.value", T_VOLUME,
            [voice = "A slider drives a daslang global. I drag Volume up, and VOLUME.value rises with it - the same global the text below reads."]),
            T_VOLUME, (vol_x1, vol_y))

        // ---- Beat 2: button increments a counter ----
        move_to(app, p_bump, 600)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "button -> click_count", T_BUMP,
            [voice = "A button increments a counter. I click Bump twice, and its click_count reads two - state the program reads straight back."]),
            [T_BUMP, T_BUMP])
    }
}
