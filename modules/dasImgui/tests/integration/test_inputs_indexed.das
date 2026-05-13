options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for indexed-form widgets per API_REWORK §4.4. Drives
//! `examples/features/inputs_indexed.das` — int-keyed CHANNEL_VOL and
//! string-keyed TRACK_VOL — and asserts the snapshot's globals contain
//! distinct path keys for each subscript, that imgui_set lands per-key
//! without bleeding across keys, and that the dispatch surface treats
//! "MIX_WIN/CHANNEL_VOL[3]" as a normal target string.

let INDEXED_FEATURE = "modules/dasImgui/examples/features/inputs_indexed.das"

[test]
def test_inputs_indexed_smoke(t : T?) {
    with_imgui_app(INDEXED_FEATURE) $(d) {
        // First frame: the wrapper's first-touch path runs for each rendered
        // key. Wait for the int-keyed [0] entry — proves the macro emitted
        // the indexed wrapper and it registered the per-key path.
        var snap0 = wait_for_widget(d, "MIX_WIN/CHANNEL_VOL[0]", 15.0f)
        t |> success(snap0 != null, "MIX_WIN/CHANNEL_VOL[0] registered after first frame")
        return if (snap0 == null)

        // ----- Path key shape: int-keyed -----
        for (i in range(4)) {
            var entry = find_widget(snap0, "MIX_WIN/CHANNEL_VOL[{i}]")
            t |> success(entry != null, "MIX_WIN/CHANNEL_VOL[{i}] in snapshot")
            t |> equal(entry?["kind"] ?? "", "slider_float", "[{i}].kind == 'slider_float'")
            t |> success(entry?["bbox"] != null, "[{i}].bbox present")
            t |> success(entry?["hex_id"] != null, "[{i}].hex_id present (distinct per key)")
        }

        // ----- Path key shape: string-keyed -----
        for (name in ["bass", "lead", "drum", "vocals"]) {
            var entry = find_widget(snap0, "MIX_WIN/TRACK_VOL[{name}]")
            t |> success(entry != null, "MIX_WIN/TRACK_VOL[{name}] in snapshot")
            t |> equal(entry?["kind"] ?? "", "slider_float", "[{name}].kind == 'slider_float'")
        }

        // ----- imgui_set lands on the right key (cross-key isolation) -----
        set_value(d, "MIX_WIN/CHANNEL_VOL[2]", JV(0.123f))
        t |> success(wait_for_payload_value(d, "MIX_WIN/CHANNEL_VOL[2]", "value", 0.123f, 300),
                     "imgui_set MIX_WIN/CHANNEL_VOL[2]=0.123 lands")
        var snap_after_set = snapshot(d)
        // Other keys must be untouched.
        let v0 = widget_payload_field(snap_after_set, "MIX_WIN/CHANNEL_VOL[0]", "value")
        t |> equal(v0 ?? -1.0f, 0.5f, "MIX_WIN/CHANNEL_VOL[0].value still 0.5 (init default)")
        let v3 = widget_payload_field(snap_after_set, "MIX_WIN/CHANNEL_VOL[3]", "value")
        t |> equal(v3 ?? -1.0f, 0.5f, "MIX_WIN/CHANNEL_VOL[3].value still 0.5 (init default)")

        // ----- String-keyed isolation -----
        set_value(d, "MIX_WIN/TRACK_VOL[lead]", JV(0.95f))
        t |> success(wait_for_payload_value(d, "MIX_WIN/TRACK_VOL[lead]", "value", 0.95f, 300),
                     "imgui_set MIX_WIN/TRACK_VOL[lead]=0.95 lands")
        var snap_after_str = snapshot(d)
        let bass_v = widget_payload_field(snap_after_str, "MIX_WIN/TRACK_VOL[bass]", "value")
        t |> equal(bass_v ?? -1.0f, 0.7f, "MIX_WIN/TRACK_VOL[bass].value still 0.7 (init default)")
    }
}
