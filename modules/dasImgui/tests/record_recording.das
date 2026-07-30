options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``recording.apng`` against ``examples/tutorial/recording.das``.
//! This is the META tutorial - the driver IS the artifact that teaches recording.
//! It demonstrates the recipe every other recording uses (narrate caption + cursor
//! trail + scripted gesture + self-verify) on a tiny subject, and verifies each:
//!   1. scripted drag  - VOLUME.value must CHANGE (drag_through_voice).
//!   2. scripted click - SAVE_BTN must register (hold_through_voice).

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
    with_recording_app("examples/tutorial/recording.das",
                       "recording.apng", 35) $(app) {
        let T_VOLUME = "REC_WIN/VOLUME"
        let T_SAVE   = "REC_WIN/SAVE_BTN"

        var snap = wait_for_render(app, T_SAVE, 10.0f)
        if (snap == null) {
            panic("{T_SAVE} never rendered - wrong app running?")
        }
        let p_save = widget_center(snap, T_SAVE)
        let vol_bb = widget_bbox(snap, T_VOLUME)
        let vol_y  = (vol_bb.y + vol_bb.w) * 0.5f
        let vol_x0 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.15f
        let vol_x1 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.80f

        // Park the cursor at the slider's left and warm up the capture pipeline.
        move_to(app, (vol_x0, vol_y), 800)
        hold_content(app, 1800u)

        // ---- Beat 1: the recipe - narrate, trail, scripted drag ----
        drag_through_voice(app, say_begin(app, "narrate + scripted drag", T_VOLUME,
            [voice = "This is the recording recipe itself. A driver shows a caption box, moves the cursor along a trail, and scripts a real drag - here the Volume slider, and the snapshot confirms the value moved."]),
            T_VOLUME, (vol_x1, vol_y))

        // ---- Beat 2: scripted click + self-verify ----
        move_to(app, p_save, 600)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "scripted click + verify", T_SAVE,
            [voice = "Then it scripts a click and asserts the effect landed. Every recording you have seen is this recipe - narrate, move, act, verify - so a demo that quietly broke aborts instead of shipping."]),
            [T_SAVE])
    }
}
