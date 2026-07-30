options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for `dockspace_in_window` against
//! ``examples/features/dockspace_in_window.das``. Asserts:
//!   1. DS captures a non-zero dock_id after the first render.
//!   2. dock_id persists across frames (same id on a later snapshot).
//!   3. dock_window children render under the HOST/DS path.

let FEATURE = "modules/dasImgui/examples/features/dockspace_in_window.das"

[test]
def test_dockspace_in_window(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        t |> run("captures_dock_id") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "HOST", 15.0f)
            t |> success(snap0 != null, "HOST window renders")
            if (snap0 == null) return
            // dockspace_in_window publishes its kind under HOST/DS.
            var ds_snap = wait_for_widget(d, "HOST/DS", 15.0f)
            t |> success(ds_snap != null, "HOST/DS dockspace registers")
            if (ds_snap == null) return
            let ds_widget = find_widget(ds_snap, "HOST/DS")
            t |> equal(ds_widget?["kind"] ?? "", "dockspace_in_window",
                       "HOST/DS registers as kind=dockspace_in_window (not bare dockspace)")
            let dock_id_0 = ds_widget?["payload"]?["dock_id"] ?? 0u
            t |> success(dock_id_0 != 0u,
                         "DS.dock_id captured on first render (got 0x{dock_id_0})")
            // Wait a few frames; dock_id must stay stable.
            await_quiescent(d, 5.0f)
            var snap1 = snapshot(d)
            t |> success(snap1 != null, "stable-snapshot snap1 available")
            if (snap1 == null) return
            let dock_id_1 = find_widget(snap1, "HOST/DS")?["payload"]?["dock_id"] ?? 0u
            t |> equal(dock_id_1, dock_id_0,
                       "DS.dock_id stable across frames")
        }
        reset(d)

        t |> run("child_panels_render") <| @(t : T?) {
            // dock_window children sit under the dockspace path.
            var files_snap = wait_for_widget(d, "HOST/DS/FILES", 15.0f)
            t |> success(files_snap != null,
                         "FILES dock_window renders under HOST/DS")
            var output_snap = wait_for_widget(d, "HOST/DS/OUTPUT", 15.0f)
            t |> success(output_snap != null,
                         "OUTPUT dock_window renders under HOST/DS")
        }
    }
}
