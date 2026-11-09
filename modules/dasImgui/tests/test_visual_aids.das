options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui/imgui_visual_aids public
require daslib/json public
require daslib/json_boost public
require math

//! Visual aids: live commands, focus rect, narrate placement.

let SHADOW = float2(3.0f, 4.0f)
let GAP = 16.0f
let MARGIN = 8.0f
let NO_AVOID : array<float4>
let BOX_SMALL = float2(200.0f, 40.0f)
let BOX_WIDE = float2(300.0f, 40.0f)
let BOX_TALL = float2(200.0f, 56.0f)
let VIEW_HD = float2(1280.0f, 720.0f)
let VIEW_SD = float2(800.0f, 600.0f)

def near(a, b : float; eps : float = 0.5f) : bool => abs(a - b) < eps

def near2(a, b : float2; eps : float = 0.5f) : bool => near(a.x, b.x, eps) && near(a.y, b.y, eps)

//! Half-open like rect_intersects: touching edges are clear.
def box_overlaps(anchor : float2; box_size : float2; b : float4) : bool {
    let bx1 = anchor.x + box_size.x
    let by1 = anchor.y + box_size.y
    return !(bx1 <= b.x || anchor.x >= b.z || by1 <= b.y || anchor.y >= b.w)
}

[test(description = "each of the seven visual-aid live commands answers ok=true, and each toggle-style one reports enabled=false when switched off", timeout = 1200)]
def test_visual_aids_dashboard_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/visual_aids_dashboard.das") $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/TARGET_BTN", 15.0f)
        t |> success(snap != null, "TARGET_BTN registers")
        if (snap == null) return

        for (ident in ["LEAD", "TARGET_BTN", "TARGET_CHECK", "TARGET_INPUT", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }

        let hl = post_command(d, "imgui_highlight",
                              JV((target = "MAIN_WIN/TARGET_BTN", frames = 60)))
        t |> equal(hl?["ok"] ?? false, true, "imgui_highlight ok=true")

        let mt = post_command(d, "imgui_mouse_trail", JV((enabled = true)))
        t |> equal(mt?["ok"] ?? false, true, "imgui_mouse_trail ok=true")

        let nr = post_command(d, "imgui_narrate",
                              JV((text = "stage smoke", target = "MAIN_WIN/TARGET_BTN",
                                  frames = 30)))
        t |> equal(nr?["ok"] ?? false, true, "imgui_narrate ok=true")

        let ah = post_command(d, "imgui_auto_highlight", JV((enabled = true)))
        t |> equal(ah?["ok"] ?? false, true, "imgui_auto_highlight ok=true")

        let cs = post_command(d, "imgui_cursor_sprite", JV((enabled = true)))
        t |> equal(cs?["ok"] ?? false, true, "imgui_cursor_sprite ok=true")

        let kh = post_command(d, "imgui_key_hud",
                              JV((enabled = true, show_modifiers = true)))
        t |> equal(kh?["ok"] ?? false, true, "imgui_key_hud ok=true")

        let fr = post_command(d, "imgui_focus_rect", JV((enabled = true)))
        t |> equal(fr?["ok"] ?? false, true, "imgui_focus_rect ok=true")

        let mt_off = post_command(d, "imgui_mouse_trail", JV((enabled = false)))
        t |> equal(mt_off?["enabled"] ?? true, false, "imgui_mouse_trail off")
        let cs_off = post_command(d, "imgui_cursor_sprite", JV((enabled = false)))
        t |> equal(cs_off?["enabled"] ?? true, false, "imgui_cursor_sprite off")
        let kh_off = post_command(d, "imgui_key_hud", JV((enabled = false)))
        t |> equal(kh_off?["enabled"] ?? true, false, "imgui_key_hud off")
        let fr_off = post_command(d, "imgui_focus_rect", JV((enabled = false)))
        t |> equal(fr_off?["enabled"] ?? true, false, "imgui_focus_rect off")
        let ah_off = post_command(d, "imgui_auto_highlight", JV((enabled = false)))
        t |> equal(ah_off?["enabled"] ?? true, false, "imgui_auto_highlight off")
    }
}

[test(description = "imgui_focus_rect toggles on and off, and a focused input exposes the non-empty bbox the focus overlay draws around", timeout = 1200)]
def test_visual_aids_focus_rect(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/glfw_synth_keys.das") $(d) {
        t |> run("toggle") <| @(t : T?) {
            var snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
            t |> success(snap0 != null, "NAME_INPUT renders")
            if (snap0 == null) return

            let on = post_command(d, "imgui_focus_rect", JV((enabled = true)))
            t |> equal(on?["ok"] ?? false, true, "imgui_focus_rect ok=true on toggle on")
            t |> equal(on?["enabled"] ?? false, true, "imgui_focus_rect reports enabled=true")

            let off = post_command(d, "imgui_focus_rect", JV((enabled = false)))
            t |> equal(off?["enabled"] ?? true, false,
                       "imgui_focus_rect reports enabled=false on toggle off")
        }
        reset(d)

        //! Gate on focus: InputText's ActiveID is not 1:1 with focus.
        t |> run("resolves_active_widget") <| @(t : T?) {
            var snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
            if (snap0 == null) {
                t |> success(false, "NAME_INPUT renders")
                return
            }

            focus(d, "MAIN_WIN/NAME_INPUT")
            let focused = wait_until(d, 180) $(var snap) {
                return snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
            }
            t |> success(focused != null,
                         "NAME_INPUT receives keyboard focus after focus dispatch")

            let snap = post_command(d, "imgui_snapshot", null)
            let bb = snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["bbox"]
            t |> success(bb != null, "NAME_INPUT.bbox populated in snapshot")
            let x = bb?["x"] ?? 0.0f
            let z = bb?["z"] ?? 0.0f
            t |> success(z > x,
                         "NAME_INPUT.bbox has non-zero width (overlay has something to draw)")
        }
    }
}

[test(description = "the narrate-placement grid registers its checkbox and all 20 indexed buttons, and a click on a second button counts there without firing the first again", timeout = 1200)]
def test_narrate_placement_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/narrate_placement.das") $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/SHOW_NARRATION", 15.0f)
        t |> success(snap != null, "SHOW_NARRATION registers")
        if (snap == null) return

        let cb = find_widget(snap, "MAIN_WIN/SHOW_NARRATION")
        t |> equal(cb?["kind"] ?? "", "checkbox",
                   "SHOW_NARRATION.kind == 'checkbox'")

        for (idx in range(20)) {
            t |> success(widget_exists(snap, "MAIN_WIN/GRID_BTN[{idx}]"),
                         "GRID_BTN[{idx}] registers")
        }

        click(d, "MAIN_WIN/GRID_BTN[0]")
        t |> success(wait_for_int_value(d, "MAIN_WIN/GRID_BTN[0]", "click_count", 1, 300),
                     "GRID_BTN[0].click_count == 1 after click")

        click(d, "MAIN_WIN/GRID_BTN[7]")
        t |> success(wait_for_int_value(d, "MAIN_WIN/GRID_BTN[7]", "click_count", 1, 300),
                     "GRID_BTN[7].click_count == 1 after retarget")
        var snap_after = snapshot(d)
        t |> equal(widget_payload_field(snap_after, "MAIN_WIN/GRID_BTN[0]", "click_count") ?? -1,
                   1,
                   "GRID_BTN[0].click_count still 1 (no extra fire on retarget)")
    }
}

[test(description = "a widget centered in a roomy viewport gets its narrate box on the right, one gap past its right edge and level with its top")]
def test_anchor_centered_widget_picks_right(t : T?) {
    let bb = float4(500.0f, 300.0f, 600.0f, 340.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    t |> success(near2(a, float2(616.0f, 300.0f)), "right: anchor=(bb.right+gap, bb.top)")
}

[test(description = "a widget near the right edge flips its narrate box to the left once the right candidate overflows the viewport")]
def test_anchor_right_edge_flips_to_left(t : T?) {
    let bb = float4(1000.0f, 300.0f, 1100.0f, 340.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    t |> success(near2(a, float2(784.0f, 300.0f)), "left: anchor=(bb.left-gap-box_w, bb.top)")
}

[test(description = "a widget too wide for either side gets its narrate box below it, horizontally centered one gap under its bottom")]
def test_anchor_wide_widget_picks_below(t : T?) {
    let bb = float4(100.0f, 250.0f, 700.0f, 300.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_WIDE, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    let expected_x = (100.0f + 700.0f) * 0.5f - BOX_WIDE.x * 0.5f
    let expected_y = 300.0f + GAP
    t |> success(near2(a, float2(expected_x, expected_y)),
                 "below: horizontally centered, gap below bottom")
}

[test(description = "a wide widget at the viewport bottom gets its narrate box above it once right, left and below all fail")]
def test_anchor_bottom_right_picks_above(t : T?) {
    let bb = float4(40.0f, 540.0f, 760.0f, 580.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_WIDE, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    let expected_x = (40.0f + 760.0f) * 0.5f - BOX_WIDE.x * 0.5f
    let expected_y = 540.0f - GAP - BOX_WIDE.y
    t |> success(near2(a, float2(expected_x, expected_y)),
                 "above: horizontally centered, gap above top")
}

[test(description = "a widget in the top-left corner keeps its narrate box on the right with no flip")]
def test_anchor_top_left_widget_picks_right(t : T?) {
    let bb = float4(40.0f, 40.0f, 200.0f, 80.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    t |> success(near2(a, float2(216.0f, 40.0f)), "right: top-left widget, plenty of room")
}

[test(description = "a widget covering nearly the whole viewport falls back to the right placement clamped inside the viewport")]
def test_anchor_giant_widget_falls_back_to_clamped_right(t : T?) {
    let view = float2(400.0f, 300.0f)
    let bb = float4(10.0f, 10.0f, 390.0f, 290.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, view, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    let max_x = view.x - MARGIN - BOX_SMALL.x - SHADOW.x
    t |> success(near2(a, float2(max_x, 10.0f)), "fallback: right candidate clamped to max_x")
}

[test(description = "the key-HUD reserve clamps every candidate upward, and the right candidate, now clear of the target, wins the tiebreak")]
def test_anchor_hud_zone_pulls_right_up(t : T?) {
    let bb = float4(500.0f, 580.0f, 700.0f, 620.0f)
    let hud_h = 110.0f
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, hud_h,
                                   no_others, NO_AVOID)
    let expected_x = 700.0f + GAP
    let expected_y = VIEW_HD.y - MARGIN - BOX_SMALL.y - SHADOW.y - hud_h
    t |> success(near2(a, float2(expected_x, expected_y)),
                 "HUD-zone clamp pulls right candidate up, no target overlap -> wins")
}

[test(description = "a narrate box wider than the viewport falls back to an x clamped to the edge margin")]
def test_anchor_text_wider_than_viewport_clamps_to_margin(t : T?) {
    let view = float2(400.0f, 300.0f)
    let bb = float4(100.0f, 100.0f, 200.0f, 140.0f)
    let big = float2(500.0f, 40.0f)
    let no_others : array<float4>
    let a = compute_narrate_anchor(true, bb, big, view, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    t |> equal(a.x, MARGIN, "fallback: anchor.x clamps to edge_margin when text is too wide")
}

[test(description = "a narrate with no target anchors at the viewport's top-left corner, one margin in")]
def test_floating_narrate_returns_corner(t : T?) {
    let no_others : array<float4>
    let a = compute_narrate_anchor(false, float4(0.0f, 0.0f, 0.0f, 0.0f),
                                   BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f,
                                   no_others, NO_AVOID)
    t |> success(near2(a, float2(MARGIN, MARGIN)), "no-target: anchor pinned to top-left corner")
}

[test(description = "the connector starts at the midpoint of the narrate box edge that faces the widget, on each of the four sides")]
def test_connector_start_picks_edge_facing_widget(t : T?) {
    let a_right = compute_connector_start(float2(716.0f, 300.0f), BOX_SMALL,
                                          float2(550.0f, 320.0f))
    t |> success(near2(a_right, float2(716.0f, 320.0f)),
                 "connector: post-it on right starts at left-edge midpoint")

    let a_left = compute_connector_start(float2(300.0f, 300.0f), BOX_SMALL,
                                         float2(620.0f, 320.0f))
    t |> success(near2(a_left, float2(500.0f, 320.0f)),
                 "connector: post-it on left starts at right-edge midpoint")

    let a_above = compute_connector_start(float2(400.0f, 100.0f), BOX_SMALL,
                                          float2(500.0f, 300.0f))
    t |> success(near2(a_above, float2(500.0f, 140.0f)),
                 "connector: post-it above starts at bottom-edge midpoint")

    let a_below = compute_connector_start(float2(400.0f, 400.0f), BOX_SMALL,
                                          float2(500.0f, 200.0f))
    t |> success(near2(a_below, float2(500.0f, 400.0f)),
                 "connector: post-it below starts at top-edge midpoint")
}

[test(description = "a right candidate that overlaps a sibling widget flips the narrate box to the clear left side")]
def test_anchor_right_blocked_by_other_widget_flips_to_left(t : T?) {
    let bb = float4(500.0f, 300.0f, 600.0f, 340.0f)
    let others <- [float4(620.0f, 290.0f, 900.0f, 380.0f)]
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f,
                                   others, NO_AVOID)
    t |> success(near2(a, float2(284.0f, 300.0f)), "right blocked by sibling, falls to left")
}

[test(description = "with a sibling blocking every side the narrate box falls back to the clamped right placement")]
def test_anchor_all_sides_blocked_falls_back_to_clamped_right(t : T?) {
    let bb = float4(500.0f, 300.0f, 600.0f, 340.0f)
    let others <- [
        float4(610.0f, 290.0f, 900.0f, 380.0f),
        float4(100.0f, 290.0f, 490.0f, 380.0f),
        float4(380.0f, 350.0f, 720.0f, 500.0f),
        float4(380.0f, 100.0f, 720.0f, 290.0f)
    ]
    let a = compute_narrate_anchor(true, bb, BOX_SMALL, VIEW_HD, MARGIN, GAP, SHADOW, 0.0f,
                                   others, NO_AVOID)
    t |> success(near2(a, float2(616.0f, 300.0f)),
                 "all sides blocked, fallback clamped right wins")
}

[test(description = "the narrate box clears both arrows of an arrow_button pair when the sibling arrow is in the hard-avoid group")]
def test_anchor_clears_arrow_pair(t : T?) {
    let left = float4(300.0f, 200.0f, 330.0f, 230.0f)
    let right = float4(334.0f, 200.0f, 364.0f, 230.0f)
    var avoid <- [right]
    var other : array<float4>
    let anchor = compute_narrate_anchor(true, left, BOX_TALL, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f,
                                        other, avoid)
    t |> success(!box_overlaps(anchor, BOX_TALL, left), "box clears the anchor arrow (LEFT)")
    t |> success(!box_overlaps(anchor, BOX_TALL, right), "box clears the sibling arrow (RIGHT)")
    delete avoid
    delete other
}

[test(description = "the narrate box clears all three controls of a row when the two siblings are in the hard-avoid group")]
def test_anchor_clears_triple(t : T?) {
    let r = float4(300.0f, 200.0f, 330.0f, 230.0f)
    let g = float4(334.0f, 200.0f, 364.0f, 230.0f)
    let b = float4(368.0f, 200.0f, 398.0f, 230.0f)
    var avoid <- [g, b]
    var other : array<float4>
    let anchor = compute_narrate_anchor(true, r, BOX_TALL, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f,
                                        other, avoid)
    t |> success(!box_overlaps(anchor, BOX_TALL, r), "box clears anchor R")
    t |> success(!box_overlaps(anchor, BOX_TALL, g), "box clears sibling G")
    t |> success(!box_overlaps(anchor, BOX_TALL, b), "box clears sibling B")
    delete avoid
    delete other
}

//! Soft-only avoidance would tie and land on the sibling.
[test(description = "a hard-avoid sibling outweighs incidental crowding, so the box stays off the sibling even when every other side is crowded")]
def test_hard_avoid_beats_incidental_crowding(t : T?) {
    let left = float4(300.0f, 200.0f, 330.0f, 230.0f)
    let right = float4(334.0f, 200.0f, 364.0f, 230.0f)
    var avoid <- [right]
    var other <- [float4(50.0f, 190.0f, 290.0f, 260.0f),
                  float4(200.0f, 240.0f, 340.0f, 310.0f),
                  float4(200.0f, 120.0f, 340.0f, 190.0f)]
    let anchor = compute_narrate_anchor(true, left, BOX_TALL, VIEW_SD, MARGIN, GAP, SHADOW, 0.0f,
                                        other, avoid)
    t |> success(!box_overlaps(anchor, BOX_TALL, right),
                 "hard-avoid keeps the box off the sibling when every other side is crowded")
    delete avoid
    delete other
}
