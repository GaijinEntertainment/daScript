options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math

//! §4.6 layout helpers (split_h / split_v / dock_left) integration smoke.
//! Three layers of coverage:
//!   1. Path & kind registration (split_*/dock_left appear under the window,
//!      all leaves resolve through their split panes).
//!   2. L3 `imgui_force_set` reaches the SliderStateFloat-shaped state and the
//!      layout actually moves next frame (left button shifts when SPLIT_MAIN
//!      goes from 0.3 → 0.7).
//!   3. Synth-mouse drag of the handle updates state.value (proves the
//!      InvisibleButton + IO.MouseDelta wiring works).

let LAYOUT_FEATURE = "modules/dasImgui/examples/features/layout_helpers.das"

[test]
def test_layout_helpers(t : T?) {
    with_imgui_app(LAYOUT_FEATURE) $(d) {
        t |> run("smoke") <| @(t : T?) {
            // All four leaves register through their containers / splits / dock.
            var snap0 = wait_for_widget(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/LEFT_BTN", 15.0f)
            t |> success(snap0 != null,
                         "deeply-nested LEFT_BTN registers under window/main/split_v/split_h")
            if (snap0 == null) return

            t |> success(widget_exists(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN"),
                         "RIGHT_BTN registers in split_h right pane")
            t |> success(widget_exists(snap0, "LAYOUT_WIN/SPLIT_VERT/BOTTOM_BTN"),
                         "BOTTOM_BTN registers in split_v bottom pane")
            t |> success(widget_exists(snap0, "LAYOUT_WIN/SIDEBAR/SIDEBAR_BTN"),
                         "SIDEBAR_BTN registers inside dock_left sidebar")

            // Layout widgets register as their own kinds.
            t |> equal(find_widget(snap0, "LAYOUT_WIN/SIDEBAR")?["kind"] ?? "",
                       "dock_left", "dock_left registers as 'dock_left' kind")
            t |> equal(find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT")?["kind"] ?? "",
                       "split_v", "split_v registers as 'split_v' kind")
            t |> equal(find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["kind"] ?? "",
                       "split_h", "split_h registers as 'split_h' kind")
        }
        reset(d)

        t |> run("geometry") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN", 15.0f)
            t |> success(snap0 != null, "snapshot reachable")
            if (snap0 == null) return

            // bbox is float4 serialized as {x,y,z,w} = (x0, y0, x1, y1).
            let lb = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/LEFT_BTN")?["bbox"]
            let rb = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]
            let lb_x1 = lb?["z"] ?? 0.0f
            let rb_x0 = rb?["x"] ?? 0.0f
            t |> success(lb_x1 < rb_x0,
                         "split_h: LEFT_BTN.right ({lb_x1}) < RIGHT_BTN.left ({rb_x0})")

            // SIDEBAR_BTN must be to the left of the main pane buttons (dock_left).
            let sb = find_widget(snap0, "LAYOUT_WIN/SIDEBAR/SIDEBAR_BTN")?["bbox"]
            let sb_x1 = sb?["z"] ?? 0.0f
            let lb_x0 = lb?["x"] ?? 0.0f
            t |> success(sb_x1 < lb_x0,
                         "dock_left: SIDEBAR_BTN.right ({sb_x1}) < LEFT_BTN.left ({lb_x0})")

            // split_v: BOTTOM_BTN must be below split_h's row.
            let bb = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/BOTTOM_BTN")?["bbox"]
            let bb_y0 = bb?["y"] ?? 0.0f
            let lb_y1 = lb?["w"] ?? 0.0f
            t |> success(bb_y0 > lb_y1,
                         "split_v: BOTTOM_BTN.top ({bb_y0}) > LEFT_BTN.bottom ({lb_y1})")

            // Fit (regression guard for the dock_left -> split_v line-height overflow): the bottom
            // pane must render INSIDE the content region. dock_left's handle spans full content
            // height, so its bbox bottom (bbox.w) is the region floor the bottom pane must stay above.
            let region_floor = find_widget(snap0, "LAYOUT_WIN/SIDEBAR/HANDLE")?["bbox"]?["w"] ?? 0.0f
            let bb_y1 = bb?["w"] ?? 0.0f
            t |> success(region_floor > 0.0f && bb_y1 <= region_floor + 1.0f,
                         "split_v fits: BOTTOM_BTN.bottom ({bb_y1}) within content floor ({region_floor})")
        }
        reset(d)

        t |> run("set_value") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/LEFT_BTN", 15.0f)
            if (snap0 == null) {
                t |> success(false, "snapshot reachable")
                return
            }

            // Baseline: state.value reflects init=0.3.
            let v0 = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["payload"]?["value"] ?? 0.0f
            t |> success(abs(v0 - 0.3f) < 1.0e-4f,
                         "SPLIT_MAIN.value baseline ({v0}) matches init=0.3")

            // L3 drive: push split to 0.7, verify state converges next frame.
            force_set_value(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN", JV(0.7f))
            let landed = wait_for_payload_value(d,
                            "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN",
                            "value", 0.7f, 300)
            t |> success(landed, "SPLIT_MAIN.value -> 0.7 after imgui_force_set")

            // Geometry follows: RIGHT_BTN sits at the right pane's start, so growing the left
            // fraction 0.3 → 0.7 shifts the pane boundary (and RIGHT_BTN.left) rightward. LEFT_BTN
            // tracks the left-aligned button, not the pane edge, so it barely moves.
            var snap_lt = snapshot(d)
            let rb_x0_orig = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f
            let rb_x0_now = find_widget(snap_lt, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f
            t |> success(rb_x0_now > rb_x0_orig + 10.0f,
                         "L3 force_set_value: RIGHT_BTN.left shifted right ({rb_x0_orig} -> {rb_x0_now})")
        }
        reset(d)

        t |> run("drag") <| @(t : T?) {
            // The split handle now surfaces its geometry as a child alias
            // <split>/HANDLE, so a synth-mouse drag targets it directly — the
            // L1 coverage gap that used to force imgui_force_set.
            let HANDLE = "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/HANDLE"
            var snap0 = wait_for_widget(d, HANDLE, 15.0f)
            t |> success(snap0 != null, "split_h HANDLE surfaces its bbox")
            if (snap0 == null) return

            t |> equal(find_widget(snap0, HANDLE)?["kind"] ?? "",
                       "splitter_handle", "HANDLE registers as 'splitter_handle' kind")

            // Non-degenerate vertical strip between the two panes.
            let hb = find_widget(snap0, HANDLE)?["bbox"]
            let hx0 = hb?["x"] ?? 0.0f
            let hx1 = hb?["z"] ?? 0.0f
            let hy0 = hb?["y"] ?? 0.0f
            let hy1 = hb?["w"] ?? 0.0f
            t |> success(hx1 > hx0 && hy1 > hy0 + 50.0f,
                         "HANDLE bbox is a tall vertical strip ([{hx0},{hx1}] x [{hy0},{hy1}])")

            let v0 = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["payload"]?["value"] ?? 0.0f
            let rb0 = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f

            // Real synth drag of the handle to the right. split_h width is
            // pixel-driven, so the left fraction must rise — no force_set.
            let resp = drag(d, HANDLE, 80.0f, 0.0f, 8)
            t |> equal(resp?["ok"] ?? false, true, "drag(HANDLE, +80) hit the handle")

            // Gate the drag play to completion before polling value/geometry,
            // so the snapshot below reads a fully-landed gesture.
            t |> success(wait_for_mouse_idle(d, 10.0f), "drag drains before value/geometry read")

            let moved = wait_until(d, 300) $(snap) => (find_widget(snap, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["payload"]?["value"] ?? 0.0f) > v0 + 0.02f
            t |> success(moved != null, "synth drag raised SPLIT_MAIN.value above baseline {v0}")

            var snap1 = snapshot(d)
            let rb1 = find_widget(snap1, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f
            t |> success(rb1 > rb0 + 5.0f,
                         "synth drag shifted RIGHT_BTN.left right ({rb0} -> {rb1})")
        }
    }
}
