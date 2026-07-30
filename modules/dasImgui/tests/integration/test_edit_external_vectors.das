options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_* vector rail (10b + 10c input vecs).

let FEATURE = "modules/dasImgui/examples/features/edit_external_vectors.das"

[test]
def test_edit_external_vectors_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/SF2", 15.0f)
        t |> success(snap != null, "SF2 renders")
        if (snap == null) return

        for (ident in ["SF2", "SF3", "SF4", "SI2", "SI3", "SI4",
                       "IF2", "IF3", "IF4", "II2", "II3", "II4",
                       "DF2", "DF3", "DF4", "DI2", "DI3", "DI4"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on a float3 — write {x,y,z} and verify the snapshot
        // reports back the same components.
        force_set_value(d, "MAIN_WIN/SF3", JV((x = 0.7f, y = 0.8f, z = 0.9f)))
        // Gate on the written component VALUES, not just field presence: SF3 already
        // exposes a non-null float3 before the write, so a presence-only predicate
        // passes on the pre-write snapshot and the round-trip isn't actually proven.
        let after = wait_until(d, 300) $(var s) {
            let xv = widget_payload_field(s, "MAIN_WIN/SF3", "value")
            let x = xv?["x"] ?? 0.0lf
            let y = xv?["y"] ?? 0.0lf
            let z = xv?["z"] ?? 0.0lf
            return (x > 0.69lf && x < 0.71lf &&
                    y > 0.79lf && y < 0.81lf &&
                    z > 0.89lf && z < 0.91lf)
        }
        t |> success(after != null, "edit_slider_float3 write round-trips to (0.7, 0.8, 0.9)")
    }
}
