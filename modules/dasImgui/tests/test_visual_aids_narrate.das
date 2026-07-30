options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_visual_aids public
require math

//! Pure-function tests for the narrate post-it auto-fit algorithm.
//!
//! `compute_narrate_anchor` picks an anchor (top-left of the post-it) by
//! trying right → left → below → above of the target widget, falling back
//! to a viewport-clamped right placement if nothing fits cleanly.
//!
//! These tests don't need a running GL context — they exercise the placement
//! decision over a matrix of widget bboxes, viewport sizes, and HUD zones.

let SHADOW    = float2(3.0f, 4.0f)
let GAP       = 16.0f
let MARGIN    = 8.0f
let NO_AVOID  : array<float4>     // empty hard-avoid group — these geometry cases exercise other_bboxes only
let BOX_SMALL = float2(200.0f, 40.0f)
let BOX_WIDE  = float2(300.0f, 40.0f)
let VIEW_HD   = float2(1280.0f, 720.0f)
let VIEW_SD   = float2(800.0f, 600.0f)

def near(a, b : float; eps : float = 0.5f) : bool {
    return abs(a - b) < eps
}

def near2(a, b : float2; eps : float = 0.5f) : bool {
    return near(a.x, b.x, eps) && near(a.y, b.y, eps)
}

[test]
def test_anchor_centered_widget_picks_right(t : T?) {
    //! Case 1: widget in the middle of a roomy viewport — right wins on priority.
    let bb = float4(500.0f, 300.0f, 600.0f, 340.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    t |> success(near2(a, float2(616.0f, 300.0f)), "case 1 right: anchor=(bb.right+gap, bb.top)")
}

[test]
def test_anchor_right_edge_flips_to_left(t : T?) {
    //! Case 2: widget near the right edge — right candidate overflows, flip to left.
    let bb = float4(1000.0f, 300.0f, 1100.0f, 340.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    t |> success(near2(a, float2(784.0f, 300.0f)), "case 2 left: anchor=(bb.left-gap-box_w, bb.top)")
}

[test]
def test_anchor_wide_widget_picks_below(t : T?) {
    //! Case 3: widget too wide for right OR left to fit — drops to below.
    let bb = float4(100.0f, 250.0f, 700.0f, 300.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_WIDE, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    let expected_x = (100.0f + 700.0f) * 0.5f - BOX_WIDE.x * 0.5f  // = 250
    let expected_y = 300.0f + GAP                                   // = 316
    t |> success(near2(a, float2(expected_x, expected_y)), "case 3 below: horizontally centered, gap below bottom")
}

[test]
def test_anchor_bottom_right_picks_above(t : T?) {
    //! Case 4: wide widget at the bottom — right/left/below all fail, above wins.
    let bb = float4(40.0f, 540.0f, 760.0f, 580.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_WIDE, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    let expected_x = (40.0f + 760.0f) * 0.5f - BOX_WIDE.x * 0.5f    // = 250
    let expected_y = 540.0f - GAP - BOX_WIDE.y                      // = 484
    t |> success(near2(a, float2(expected_x, expected_y)), "case 4 above: horizontally centered, gap above top")
}

[test]
def test_anchor_top_left_widget_picks_right(t : T?) {
    //! Case 5: widget at top-left corner — right candidate fits, no flipping needed.
    let bb = float4(40.0f, 40.0f, 200.0f, 80.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    t |> success(near2(a, float2(216.0f, 40.0f)), "case 5 right: top-left widget, plenty of room")
}

[test]
def test_anchor_giant_widget_falls_back_to_clamped_right(t : T?) {
    //! Case 6: widget covers almost the whole viewport — no candidate fits,
    //! fallback clamps right placement into viewport bounds.
    let view = float2(400.0f, 300.0f)
    let bb = float4(10.0f, 10.0f, 390.0f, 290.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, view, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    let max_x = view.x - MARGIN - BOX_SMALL.x - SHADOW.x   // = 189
    t |> success(near2(a, float2(max_x, 10.0f)), "case 6 fallback: right candidate clamped to max_x")
}

[test]
def test_anchor_hud_zone_pulls_right_up(t : T?) {
    //! Case 7: widget near bottom with key_hud reserve active — every candidate
    //! gets pulled up by the clamp; right-of-widget at the clamped y has no
    //! target overlap (it sits above-right of the target) and wins by tiebreak.
    let bb = float4(500.0f, 580.0f, 700.0f, 620.0f)
    let hud_h = 110.0f
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, hud_h, no_others, NO_AVOID)
    let expected_x = 700.0f + GAP                                                                 // = 716
    let expected_y = VIEW_HD.y - MARGIN - BOX_SMALL.y - SHADOW.y - hud_h                          // = 558
    t |> success(near2(a, float2(expected_x, expected_y)), "case 7: HUD-zone clamp pulls right candidate up, no target overlap → wins")
}

[test]
def test_anchor_text_wider_than_viewport_clamps_to_margin(t : T?) {
    //! Case 8: text is wider than the viewport — every candidate fails the x-fit
    //! check, fallback clamps to edge_margin on the left.
    let view = float2(400.0f, 300.0f)
    let bb = float4(100.0f, 100.0f, 200.0f, 140.0f)
    let big = float2(500.0f, 40.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, big, view, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    t |> equal(a.x, MARGIN, "case 8 fallback: anchor.x clamps to edge_margin when text is too wide")
}

[test]
def test_floating_narrate_returns_corner(t : T?) {
    //! Floating case (no target): anchor is the top-left corner of the viewport.
    let no_others : array<float4>
    let a = compute_narrate_anchor(false, float4(0.0f, 0.0f, 0.0f, 0.0f),
                                   BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f, no_others, NO_AVOID)
    t |> success(near2(a, float2(MARGIN, MARGIN)), "no-target: anchor pinned to top-left corner")
}

[test]
def test_connector_start_picks_edge_facing_widget(t : T?) {
    //! Case 9: connector_start picks the post-it edge midpoint closest to the
    //! widget center. Four sub-cases cover each of the four edges.

    // Post-it to the right of widget → connector starts at left edge.
    let a_right = compute_connector_start(float2(716.0f, 300.0f), BOX_SMALL, float2(550.0f, 320.0f))
    t |> success(near2(a_right, float2(716.0f, 320.0f)), "connector: post-it on right starts at left-edge midpoint")

    // Post-it to the left of widget → connector starts at right edge.
    let a_left = compute_connector_start(float2(300.0f, 300.0f), BOX_SMALL, float2(620.0f, 320.0f))
    t |> success(near2(a_left, float2(500.0f, 320.0f)), "connector: post-it on left starts at right-edge midpoint")

    // Post-it above widget → connector starts at bottom edge.
    let a_above = compute_connector_start(float2(400.0f, 100.0f), BOX_SMALL, float2(500.0f, 300.0f))
    t |> success(near2(a_above, float2(500.0f, 140.0f)), "connector: post-it above starts at bottom-edge midpoint")

    // Post-it below widget → connector starts at top edge.
    let a_below = compute_connector_start(float2(400.0f, 400.0f), BOX_SMALL, float2(500.0f, 200.0f))
    t |> success(near2(a_below, float2(500.0f, 400.0f)), "connector: post-it below starts at top-edge midpoint")
}

[test]
def test_anchor_right_blocked_by_other_widget_flips_to_left(t : T?) {
    //! Case 10: right candidate fits the viewport but overlaps a sibling
    //! widget — flip to left, which fits and has no neighbor in the way.
    let bb = float4(500.0f, 300.0f, 600.0f, 340.0f)
    let others <- [float4(620.0f, 290.0f, 900.0f, 380.0f)]
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f, others, NO_AVOID)
    t |> success(near2(a, float2(284.0f, 300.0f)), "case 10: right blocked by sibling, falls to left")
}

[test]
def test_anchor_all_sides_blocked_falls_back_to_clamped_right(t : T?) {
    //! Case 11: every candidate (R/L/Below/Above) overlaps a sibling — fall
    //! back to clamped right placement (existing graceful-degradation path).
    let bb = float4(500.0f, 300.0f, 600.0f, 340.0f)
    let others <- [
        float4(610.0f, 290.0f, 900.0f, 380.0f),    // blocks right
        float4(100.0f, 290.0f, 490.0f, 380.0f),    // blocks left
        float4(380.0f, 350.0f, 720.0f, 500.0f),    // blocks below
        float4(380.0f, 100.0f, 720.0f, 290.0f)     // blocks above
    ]
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f, others, NO_AVOID)
    t |> success(near2(a, float2(616.0f, 300.0f)), "case 11: all sides blocked, fallback clamped right wins")
}
