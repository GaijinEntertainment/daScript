options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_visual_aids public

//! Unit test for the multi-target narrative box: ``compute_narrate_anchor`` must keep the caption
//! box clear of the step's HARD-avoid group (the other "main" elements a multi-target narrate
//! spans), so a step over sibling controls — e.g. an arrow_button pair — never has its box cover a
//! related element. Pure geometry; no app/window needed.

def box_overlaps(anchor : float2; box_size : float2; b : float4) : bool {
    // Half-open box-on-box (touching edges = clear), mirroring the resolver's rect_intersects.
    let bx1 = anchor.x + box_size.x
    let by1 = anchor.y + box_size.y
    return !(bx1 <= b.x || anchor.x >= b.z || by1 <= b.y || anchor.y >= b.w)
}

let VIEWPORT = float2(800.0f, 600.0f)
let BOX      = float2(200.0f, 56.0f)
let EDGE     = 8.0f
let GAP      = 16.0f
let SHADOW   = float2(3.0f, 4.0f)

[test]
def test_anchor_clears_arrow_pair(t : T?) {
    // LEFT anchor, RIGHT sibling just to its right (the arrow_button pair).
    let left  = float4(300.0f, 200.0f, 330.0f, 230.0f)
    let right = float4(334.0f, 200.0f, 364.0f, 230.0f)
    var avoid <- [right]
    var other : array<float4>
    let anchor = compute_narrate_anchor(true, left, BOX, VIEWPORT, EDGE, GAP, SHADOW, 0.0f, other, avoid)
    t |> success(!box_overlaps(anchor, BOX, left),  "box clears the anchor arrow (LEFT)")
    t |> success(!box_overlaps(anchor, BOX, right), "box clears the sibling arrow (RIGHT)")
    delete avoid
    delete other
}

[test]
def test_anchor_clears_triple(t : T?) {
    // Three controls in a row (e.g. R/G/B color buttons): anchor R, hard-avoid G + B.
    let r = float4(300.0f, 200.0f, 330.0f, 230.0f)
    let g = float4(334.0f, 200.0f, 364.0f, 230.0f)
    let b = float4(368.0f, 200.0f, 398.0f, 230.0f)
    var avoid <- [g, b]
    var other : array<float4>
    let anchor = compute_narrate_anchor(true, r, BOX, VIEWPORT, EDGE, GAP, SHADOW, 0.0f, other, avoid)
    t |> success(!box_overlaps(anchor, BOX, r), "box clears anchor R")
    t |> success(!box_overlaps(anchor, BOX, g), "box clears sibling G")
    t |> success(!box_overlaps(anchor, BOX, b), "box clears sibling B")
    delete avoid
    delete other
}

[test]
def test_hard_avoid_beats_incidental_crowding(t : T?) {
    // Discriminating case: the sibling sits in the only otherwise-clear direction (right), while an
    // incidental widget crowds left/below/above (soft +1). Soft-only avoidance ties all four at +1
    // and the right tiebreak lands the box ON the sibling; hard-avoid (+1000) forces it off. FAILs pre-fix.
    let left  = float4(300.0f, 200.0f, 330.0f, 230.0f)
    let right = float4(334.0f, 200.0f, 364.0f, 230.0f)
    var avoid <- [right]
    var other <- [float4(50.0f, 190.0f, 290.0f, 260.0f),    // crowds the LEFT candidate
                  float4(200.0f, 240.0f, 340.0f, 310.0f),   // crowds the BELOW candidate
                  float4(200.0f, 120.0f, 340.0f, 190.0f)]   // crowds the ABOVE candidate
    let anchor = compute_narrate_anchor(true, left, BOX, VIEWPORT, EDGE, GAP, SHADOW, 0.0f, other, avoid)
    t |> success(!box_overlaps(anchor, BOX, right),
                 "hard-avoid keeps the box off the sibling even when every other direction is crowded")
    delete avoid
    delete other
}
