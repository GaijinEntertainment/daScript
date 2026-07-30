options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_item_hittest — the imgui_internal.h item hit-test / rounding
//          geometry helpers a custom-widget author needs below the public widget
//          API. Cherry-picked into the v2 binding (bind_imgui.das
//          internal_allow_func): ItemHoverable, IsClippedEx,
//          CalcRoundingFlagsForRectInRect. All take an ImRect (aliased to float4
//          as (min.x, min.y, max.x, max.y)) + an ImGuiID(uint); they read the
//          current window implicitly, so no ImGuiWindow* and no v2 host state.
//          This is the "binding is usable" gate: it calls the real bound API at
//          runtime, not just compiles it.
// SHOWS:   ItemHoverable (hit-test a hand-drawn rect by its bb + id, recolour on
//          hover); IsClippedEx (cull test — a visible rect vs. one pushed far
//          offscreen); CalcRoundingFlagsForRectInRect (round only the corners of
//          an inner rect that sit away from the outer rect's corners, e.g. a tab
//          nested in a bar).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_item_hittest.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_item_hittest.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_item_hittest.das
// =============================================================================

def private box(var dl : ImDrawList?; x, y, w, h : float; edge : uint) {
    dl |> add_rect(ImVec2(x, y), ImVec2(x + w, y + h), edge, 0.0, ImDrawFlags.None, 2.0)
}

[export]
def init() {
    harness_init("internal_item_hittest - imgui_internal.h item hit-test / geometry", 760, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let edge = rgba(120u, 180u, 240u, 255u)
    let fill_idle = rgba(60u, 90u, 140u, 255u)
    let fill_hover = rgba(90u, 150u, 230u, 255u)

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    window(HIT_WIN, (text = "internal_item_hittest", closable = false,
                     flags = ImGuiWindowFlags.None)) {

        // ----- ItemHoverable: hit-test a hand-drawn rect by its bb + id -----
        text("ItemHoverable(bb, id): hit-test a custom rect (hover it to recolour).")
        let o1 = GetCursorScreenPos()
        let hb_min = ImVec2(o1.x + 12.0, o1.y + 8.0)
        let hb_max = ImVec2(o1.x + 220.0, o1.y + 44.0)
        // ImRect is float4 = (min.x, min.y, max.x, max.y). 0 = ImGuiItemFlags.None.
        let bb = float4(hb_min.x, hb_min.y, hb_max.x, hb_max.y)
        let hovered = ItemHoverable(bb, GetID("hover_rect"), 0)
        with_window_drawlist() $(var dl) {
            dl |> add_rect_filled(hb_min, hb_max, hovered ? fill_hover : fill_idle, 4.0, ImDrawFlags.None)
            box(dl, hb_min.x, hb_min.y, hb_max.x - hb_min.x, hb_max.y - hb_min.y, edge)
        }
        dummy(SP1, ImVec2(700.0, 48.0))
        text("  hovered = {hovered}")
        separator()

        // ----- IsClippedEx: cull test — visible rect vs. one pushed offscreen -----
        text("IsClippedEx(bb, id): is a rect outside the visible clip region (cull test)?")
        let o2 = GetCursorScreenPos()
        let vis_bb = float4(o2.x + 12.0, o2.y + 8.0, o2.x + 200.0, o2.y + 40.0)
        // Same width, but pushed far below the window — outside the clip rect.
        let off_bb = float4(o2.x + 12.0, o2.y + 4000.0, o2.x + 200.0, o2.y + 4032.0)
        let vis_clipped = IsClippedEx(vis_bb, GetID("vis_rect"))
        let off_clipped = IsClippedEx(off_bb, GetID("off_rect"))
        with_window_drawlist() $(var dl) {
            box(dl, o2.x + 12.0, o2.y + 8.0, 188.0, 32.0, edge)
        }
        dummy(SP2, ImVec2(700.0, 40.0))
        text("  visible rect clipped = {vis_clipped}    offscreen rect clipped = {off_clipped}")
        separator()

        // ----- CalcRoundingFlagsForRectInRect: round only the "free" corners -----
        text("CalcRoundingFlagsForRectInRect(r_in, r_outer, threshold): round only the")
        text("  inner-rect corners that sit away from the outer rect (e.g. a tab in a bar).")
        let o3 = GetCursorScreenPos()
        let outer_min = ImVec2(o3.x + 12.0, o3.y + 8.0)
        let outer_max = ImVec2(o3.x + 320.0, o3.y + 88.0)
        // Inner rect pinned to the outer's top-left corner; its bottom-right is "free".
        let inner_min = outer_min
        let inner_max = ImVec2(o3.x + 160.0, o3.y + 48.0)
        let r_outer = float4(outer_min.x, outer_min.y, outer_max.x, outer_max.y)
        let r_in = float4(inner_min.x, inner_min.y, inner_max.x, inner_max.y)
        let round_flags = CalcRoundingFlagsForRectInRect(r_in, r_outer, 16.0)
        with_window_drawlist() $(var dl) {
            dl |> add_rect(outer_min, outer_max, edge, 0.0, ImDrawFlags.None, 2.0)
            dl |> add_rect_filled(inner_min, inner_max, fill_idle, 12.0, round_flags)
        }
        dummy(SP3, ImVec2(700.0, 92.0))
        text("  computed ImDrawFlags = {int(round_flags)} (only corners away from the outer edges round)")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
