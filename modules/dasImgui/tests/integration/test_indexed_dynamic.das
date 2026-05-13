options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Dynamic-table smoke for indexed widgets. Drives
//! `examples/features/indexed_dynamic.das` through an ADD/BUMP/REMOVE
//! sequence and verifies (a) snapshot entries appear when a key is added,
//! (b) imgui_set lands per-key without bleeding across keys, (c) entries
//! vanish when a key is erased (validates §G expiration), (d) re-adding
//! the same key restores a fresh entry. Telemetry runs in process via the
//! standard playwright transport — no special fixture rigging.

let DYN_FEATURE = "modules/dasImgui/examples/features/indexed_dynamic.das"

[test]
def test_indexed_dynamic_add_remove(t : T?) {
    with_imgui_app(DYN_FEATURE) $(d) {
        // Wait for the window's controls to register.
        var snap0 = wait_for_widget(d, "DYN_WIN/ADD_CH", 15.0f)
        t |> success(snap0 != null, "DYN_WIN/ADD_CH registered after first frame")
        return if (snap0 == null)
        // Empty table: no CHANNEL_VOL[*] entries.
        t |> success(!widget_exists(snap0, "DYN_WIN/CHANNEL_VOL[0]"),
                     "CHANNEL_VOL empty initially (no [0] entry)")

        // ADD → snapshot grows by one.
        click(d, "DYN_WIN/ADD_CH")
        t |> success(wait_for_widget(d, "DYN_WIN/CHANNEL_VOL[0]", 5.0f) != null,
                     "ADD inserts CHANNEL_VOL[0]")

        // imgui_set lands on the new entry; other keys (none) unaffected.
        set_value(d, "DYN_WIN/CHANNEL_VOL[0]", JV(0.42f))
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[0]", "value", 0.42f, 300),
                     "imgui_set CHANNEL_VOL[0]=0.42 lands")

        // ADD again → [1] appears alongside [0].
        click(d, "DYN_WIN/ADD_CH")
        t |> success(wait_for_widget(d, "DYN_WIN/CHANNEL_VOL[1]", 5.0f) != null,
                     "second ADD inserts CHANNEL_VOL[1]")
        var snap_two = snapshot(d)
        t |> success(widget_exists(snap_two, "DYN_WIN/CHANNEL_VOL[0]"),
                     "[0] still present after second ADD")
        t |> equal(widget_payload_field(snap_two, "DYN_WIN/CHANNEL_VOL[0]", "value") ?? -1.0f,
                   0.42f, "[0].value still 0.42 (cross-key isolation)")

        // imgui_set on [1] doesn't perturb [0].
        set_value(d, "DYN_WIN/CHANNEL_VOL[1]", JV(0.91f))
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[1]", "value", 0.91f, 300),
                     "imgui_set CHANNEL_VOL[1]=0.91 lands")
        var snap_after_set1 = snapshot(d)
        t |> equal(widget_payload_field(snap_after_set1, "DYN_WIN/CHANNEL_VOL[0]", "value") ?? -1.0f,
                   0.42f, "[0].value unchanged after setting [1]")

        // REMOVE highest key → [1] vanishes, [0] stays.
        click(d, "DYN_WIN/REMOVE_CH")
        // §G expiration: indexed entry expires when last_seen_frame < g_frame-1.
        // Wait through a couple of frames for the snapshot to drop it.
        let dropped = wait_until(d, 300) $(var snap : JsonValue?) : bool {
            return !widget_exists(snap, "DYN_WIN/CHANNEL_VOL[1]")
        }
        t |> success(dropped != null, "REMOVE drops CHANNEL_VOL[1] from snapshot")
        var snap_after_remove = snapshot(d)
        t |> success(widget_exists(snap_after_remove, "DYN_WIN/CHANNEL_VOL[0]"),
                     "[0] still present after REMOVE of [1]")
        t |> equal(widget_payload_field(snap_after_remove, "DYN_WIN/CHANNEL_VOL[0]", "value") ?? -1.0f,
                   0.42f, "[0].value still 0.42 after REMOVE")

        // BUMP_LAST raises [0].value (only [0] remains).
        click(d, "DYN_WIN/BUMP_LAST")
        t |> success(wait_for_payload_value(d, "DYN_WIN/CHANNEL_VOL[0]", "value", 0.52f, 300),
                     "BUMP_LAST nudges [0].value by 0.1 (0.42 -> 0.52)")

        // ADD again → key 1 re-appears with fresh defaults (value=0.5).
        click(d, "DYN_WIN/ADD_CH")
        t |> success(wait_for_widget(d, "DYN_WIN/CHANNEL_VOL[1]", 5.0f) != null,
                     "re-ADD restores CHANNEL_VOL[1]")
        var snap_re = snapshot(d)
        t |> equal(widget_payload_field(snap_re, "DYN_WIN/CHANNEL_VOL[1]", "value") ?? -1.0f,
                   0.5f, "re-added [1].value is fresh default 0.5, not the prior 0.91")
    }
}
