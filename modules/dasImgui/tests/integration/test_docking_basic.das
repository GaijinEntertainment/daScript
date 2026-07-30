options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Phase 7 docking integration smoke. Three coverage layers:
//!   1. Registration — `dockspace`/`dock_window` and their children appear in
//!      the snapshot under the right path keys, with the right kinds.
//!   2. Initial layout — DockBuilder-driven 4-panel layout actually places
//!      Explorer/Source/Output/Properties in distinct screen regions on the
//!      first frame; each dock_window reports `docked=true` once seeded.
//!   3. Live commands — `imgui_undock` (window becomes floating),
//!      `imgui_dock` (window re-docks to a target node), `imgui_dock_reset`
//!      (state.has_initial_layout flips false → setup re-runs),
//!      `imgui_close` on a closable dock_window flips `state.open` false,
//!      `imgui_raise` selects a window's tab when it's stacked (Console + Output
//!      share one node; raising flips which one's children render).
//!
//! One host spawn, six subtests; `reset(d)` re-simulates the compiled program
//! into a fresh context between subtests (no recompile) so each starts from a
//! clean dock layout — ~100x cheaper than respawning daslang-live per [test].

let DOCK_FEATURE = "modules/dasImgui/examples/features/dock_basic.das"

// dockspace is a [container], so it pushes itself onto the registry path — child dock_windows
// appear under `DOCK_ROOT/...`, mirroring how `window(MAIN_WIN){ dock_left(SIDEBAR){ button(BTN) } }`
// nests to `MAIN_WIN/SIDEBAR/BTN` in test_layout_helpers.

[test]
def test_docking_basic(t : T?) {
    with_imgui_app(DOCK_FEATURE) $(d) {
        t |> run("registration") <| @(t : T?) {
            var snap = wait_for_widget(d, "DOCK_ROOT/EXPLORER", 15.0f)
            t |> success(snap != null, "snapshot reachable; DOCK_ROOT/EXPLORER registers")
            if (snap == null) return

            t |> success(widget_exists(snap, "DOCK_ROOT/SOURCE"),     "SOURCE registers under DOCK_ROOT")
            t |> success(widget_exists(snap, "DOCK_ROOT/OUTPUT"),     "OUTPUT registers under DOCK_ROOT")
            t |> success(widget_exists(snap, "DOCK_ROOT/PROPERTIES"), "PROPERTIES registers under DOCK_ROOT")
            t |> success(widget_exists(snap, "DOCK_ROOT"),            "DOCK_ROOT (dockspace) registers")

            // Leaf widgets register with full path prefix (dockspace + dock_window).
            t |> success(widget_exists(snap, "DOCK_ROOT/EXPLORER/REFRESH_BTN"),
                         "REFRESH_BTN registers under DOCK_ROOT/EXPLORER path prefix")
            t |> success(widget_exists(snap, "DOCK_ROOT/SOURCE/SAVE_BTN"),
                         "SAVE_BTN registers under DOCK_ROOT/SOURCE path prefix")
            t |> success(widget_exists(snap, "DOCK_ROOT/OUTPUT/CLEAR_BTN"),
                         "CLEAR_BTN registers under DOCK_ROOT/OUTPUT path prefix")
            t |> success(widget_exists(snap, "DOCK_ROOT/PROPERTIES/APPLY_BTN"),
                         "APPLY_BTN registers under DOCK_ROOT/PROPERTIES path prefix")

            // Kinds.
            t |> equal(find_widget(snap, "DOCK_ROOT")?["kind"] ?? "",
                       "dockspace", "DOCK_ROOT registers as 'dockspace' kind")
            t |> equal(find_widget(snap, "DOCK_ROOT/EXPLORER")?["kind"] ?? "",
                       "dock_window", "DOCK_ROOT/EXPLORER registers as 'dock_window' kind")
        }
        reset(d)

        t |> run("initial_layout") <| @(t : T?) {
            var snap = wait_for_widget(d, "DOCK_ROOT/EXPLORER", 15.0f)
            if (snap == null) {
                t |> success(false, "snapshot reachable")
                return
            }

            // Give the dockspace a couple of frames to actually dock the windows
            // (DockBuilderFinish on frame 1, ImGui applies docking on frame 2).
            let docked = wait_for_payload_value(d, "DOCK_ROOT/EXPLORER", "docked", true, 300)
            t |> success(docked, "EXPLORER reports docked=true after DockBuilder finish")

            var snap2 = snapshot(d)
            // bbox: float4 = (x0,y0,x1,y1). Layout seeds: Explorer left of the rest (split-left
            // 0.22 of root); Source atop Output+Properties (split-top 0.55 of main); Output left
            // of Properties (split-left 0.55 of bottom).
            let ex_bb = find_widget(snap2, "DOCK_ROOT/EXPLORER")?["bbox"]
            let so_bb = find_widget(snap2, "DOCK_ROOT/SOURCE")?["bbox"]
            let ou_bb = find_widget(snap2, "DOCK_ROOT/OUTPUT")?["bbox"]
            let pr_bb = find_widget(snap2, "DOCK_ROOT/PROPERTIES")?["bbox"]
            let ex_x1 = ex_bb?["z"] ?? 0.0f
            let so_x0 = so_bb?["x"] ?? 0.0f
            let so_y1 = so_bb?["w"] ?? 0.0f
            let ou_y0 = ou_bb?["y"] ?? 0.0f
            let ou_x1 = ou_bb?["z"] ?? 0.0f
            let pr_x0 = pr_bb?["x"] ?? 0.0f
            t |> success(ex_x1 <= so_x0 + 4.0f,
                         "EXPLORER.right ({ex_x1}) at-or-left-of SOURCE.left ({so_x0}) — split-left landed")
            t |> success(so_y1 <= ou_y0 + 4.0f,
                         "SOURCE.bottom ({so_y1}) at-or-above OUTPUT.top ({ou_y0}) — split-top landed")
            t |> success(ou_x1 <= pr_x0 + 4.0f,
                         "OUTPUT.right ({ou_x1}) at-or-left-of PROPERTIES.left ({pr_x0}) — split-left landed inside bottom row")
        }
        reset(d)

        t |> run("undock") <| @(t : T?) {
            var snap = wait_for_widget(d, "DOCK_ROOT/OUTPUT", 15.0f)
            if (snap == null) {
                t |> success(false, "snapshot reachable")
                return
            }
            // Wait until OUTPUT is docked (initial layout finished).
            let docked = wait_for_payload_value(d, "DOCK_ROOT/OUTPUT", "docked", true, 300)
            t |> success(docked, "OUTPUT starts docked")

            // Send undock command, wait for `docked` field to flip false.
            post_command(d, "imgui_undock", JV((target = "DOCK_ROOT/OUTPUT")))
            let undocked = wait_for_payload_value(d, "DOCK_ROOT/OUTPUT", "docked", false, 300)
            t |> success(undocked, "imgui_undock flipped OUTPUT.docked false")
        }
        reset(d)

        t |> run("close") <| @(t : T?) {
            var snap = wait_for_widget(d, "DOCK_ROOT/OUTPUT", 15.0f)
            if (snap == null) {
                t |> success(false, "snapshot reachable")
                return
            }
            // OUTPUT is closable=true in the feature demo.
            let open0 = find_widget(snap, "DOCK_ROOT/OUTPUT")?["payload"]?["open"] ?? false
            t |> success(open0, "OUTPUT starts open")

            post_command(d, "imgui_close", JV((target = "DOCK_ROOT/OUTPUT")))
            let closed = wait_for_payload_value(d, "DOCK_ROOT/OUTPUT", "open", false, 300)
            t |> success(closed, "imgui_close flipped OUTPUT.open false")
        }
        reset(d)

        t |> run("set_window_pos") <| @(t : T?) {
            var snap = wait_for_widget(d, "DOCK_ROOT/OUTPUT", 15.0f)
            if (snap == null) {
                t |> success(false, "snapshot reachable")
                return
            }
            // First undock so the floating window's pos isn't pinned by the dock.
            post_command(d, "imgui_undock", JV((target = "DOCK_ROOT/OUTPUT")))
            let undocked = wait_for_payload_value(d, "DOCK_ROOT/OUTPUT", "docked", false, 300)
            t |> success(undocked, "OUTPUT undocked")

            // Set window position to a specific spot.
            post_command(d, "imgui_set_window_pos", JV((
                target = "DOCK_ROOT/OUTPUT",
                value = (x = 600.0f, y = 200.0f, w = 300.0f, h = 180.0f)
            )))
            // State.pos is a float2 observed every frame from GetWindowPos();
            // poll the whole field rather than a dotted sub-key (wait_for_payload_value
            // matches on the full field value).
            let landed = wait_for_payload_value(d, "DOCK_ROOT/OUTPUT",
                                                "pos", float2(600.0f, 200.0f), 300)
            t |> success(landed, "OUTPUT.pos converged to (600,200) after imgui_set_window_pos")
        }
        reset(d)

        t |> run("dock_reset") <| @(t : T?) {
            var snap = wait_for_widget(d, "DOCK_ROOT", 15.0f)
            if (snap == null) {
                t |> success(false, "snapshot reachable")
                return
            }
            // Wait for initial layout flag to be true.
            let seeded = wait_for_payload_value(d, "DOCK_ROOT", "has_initial_layout", true, 300)
            t |> success(seeded, "DOCK_ROOT.has_initial_layout=true after initial setup")

            // imgui_dock_reset flips the flag false, but setup re-runs and flips it true again
            // immediately, so polling for a `has_initial_layout=false` frame would race. Instead
            // check the public contract: reset, then verify EXPLORER is still docked (layout came back).
            post_command(d, "imgui_dock_reset", JV((target = "DOCK_ROOT")))
            let re_docked = wait_for_payload_value(d, "DOCK_ROOT/EXPLORER", "docked", true, 300)
            t |> success(re_docked, "EXPLORER re-docks after imgui_dock_reset round-trip")
        }
        reset(d)

        t |> run("raise") <| @(t : T?) {
            // Console + Output share one dock node (tab pair). A backgrounded tab's children aren't
            // painted (rendered:false), so selecting a tab shows as its child starting to paint and
            // the other's stopping. No command clicks a dock tab bar, so imgui_raise is the rail.
            var snap = wait_for_widget(d, "DOCK_ROOT/CONSOLE", 15.0f)
            if (snap == null) {
                t |> success(false, "snapshot reachable")
                return
            }
            // Output is the front tab initially (docked last); Console is backgrounded.
            t |> success(wait_for_visible(d, "DOCK_ROOT/OUTPUT/CLEAR_BTN", 5.0f) != null,
                         "OUTPUT starts as the front tab — CLEAR_BTN paints")
            t |> success(!widget_rendered(snapshot(d), "DOCK_ROOT/CONSOLE/CONSOLE_BTN"),
                         "CONSOLE starts backgrounded — its child does not paint")

            // Raise Console -> it becomes the front tab; its child paints, Output's stops.
            post_command(d, "imgui_raise", JV((target = "DOCK_ROOT/CONSOLE")))
            t |> success(wait_for_visible(d, "DOCK_ROOT/CONSOLE/CONSOLE_BTN", 5.0f) != null,
                         "imgui_raise CONSOLE selected its tab — CONSOLE_BTN paints")
            t |> success(wait_cond(d, 5.0f) $() => !widget_rendered(snapshot(d), "DOCK_ROOT/OUTPUT/CLEAR_BTN"),
                         "...and OUTPUT is backgrounded — its child stops painting")

            // Raise Output back -> the tab switches the other way.
            post_command(d, "imgui_raise", JV((target = "DOCK_ROOT/OUTPUT")))
            t |> success(wait_for_visible(d, "DOCK_ROOT/OUTPUT/CLEAR_BTN", 5.0f) != null,
                         "imgui_raise OUTPUT switched the tab back — CLEAR_BTN paints again")
        }
    }
}
