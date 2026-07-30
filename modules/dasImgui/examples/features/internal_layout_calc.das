options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_layout_calc — exercises the imgui_internal.h item / layout
//          helper family (#4, bound via bind_imgui.das internal_allow_func). These
//          help custom-widget code size and position items the same way the built-in
//          widgets do. Args/results are already-bound public types (ImVec2, float);
//          ItemSize's rect overload takes ImRect, now aliased to float4 (glue in
//          cb_dasIMGUI.h), so no internal struct is pulled in as a real type. One-shot
//          calls, raw on the ALLOWED_IMGUI allow-list (like families 1-2), not behind
//          a scope guard. This is the "binding is usable" gate for family #4 — it
//          calls the real bound API at runtime, not just compiles it.
// SHOWS:   CalcItemSize (resolve a requested size against defaults — three cases
//          drawn at their resolved pixel size), CalcWrapWidthForPos (the wrap
//          width behind PushTextWrapPos, drawn as a measured span),
//          GetContentRegionMaxAbs (the content region's absolute far corner,
//          marked with a crosshair), and ItemSize (reserve layout space for a
//          custom-drawn box so the next widget flows directly below it).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_layout_calc.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_layout_calc.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_layout_calc.das
// =============================================================================

[export]
def init() {
    harness_init("internal_layout_calc - imgui_internal.h layout helpers", 760, 690)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let ink   = rgba(232u, 232u, 240u, 255u)
    let fill  = rgba(48u, 96u, 168u, 220u)
    let edge  = rgba(120u, 180u, 240u, 255u)
    let span  = rgba(90u, 220u, 130u, 255u)
    let mark  = rgba(240u, 180u, 70u, 255u)

    SetNextWindowSize(ImVec2(720.0, 650.0), ImGuiCond.Always)
    window(LAYOUT_WIN, (text = "internal_layout_calc", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        text("imgui_internal.h layout-geometry calculators - pure sizing/position math.")
        separator()

        // CalcItemSize - resolve a requested size against (default_w, default_h).
        // A zero component means "use the default"; the rect drawn is the result.
        text("CalcItemSize(size, default_w=220, default_h=40): a zero picks the default.")
        let dW = 220.0
        let dH = 40.0
        let o1 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            var x = o1.x + 12.0
            let y = o1.y + 8.0
            let a = CalcItemSize(ImVec2(0.0, 0.0), dW, dH)       // both default
            dl |> add_rect_filled(ImVec2(x, y), ImVec2(x + a.x, y + a.y), fill)
            dl |> add_rect(ImVec2(x, y), ImVec2(x + a.x, y + a.y), edge, 0.0, ImDrawFlags.None, 2.0)
            dl |> add_text(ImVec2(x + 8.0, y + 12.0), ink, "(0,0) -> {int(a.x)}x{int(a.y)}")
            x += a.x + 22.0
            let b = CalcItemSize(ImVec2(120.0, 0.0), dW, dH)     // explicit width
            dl |> add_rect_filled(ImVec2(x, y), ImVec2(x + b.x, y + b.y), fill)
            dl |> add_rect(ImVec2(x, y), ImVec2(x + b.x, y + b.y), edge, 0.0, ImDrawFlags.None, 2.0)
            dl |> add_text(ImVec2(x + 8.0, y + 12.0), ink, "(120,0) -> {int(b.x)}x{int(b.y)}")
            x += b.x + 22.0
            let c = CalcItemSize(ImVec2(0.0, 70.0), dW, dH)      // explicit height
            dl |> add_rect_filled(ImVec2(x, y), ImVec2(x + c.x, y + c.y), fill)
            dl |> add_rect(ImVec2(x, y), ImVec2(x + c.x, y + c.y), edge, 0.0, ImDrawFlags.None, 2.0)
            dl |> add_text(ImVec2(x + 8.0, y + 12.0), ink, "(0,70) -> {int(c.x)}x{int(c.y)}")
        }
        dummy(SPACER1, ImVec2(700.0, 92.0))
        separator()

        // CalcWrapWidthForPos - wrap width from a position to the content's right
        // edge (wrap_pos_x=0). Drawn as a measured span with end caps.
        text("CalcWrapWidthForPos(pos, wrap_pos_x=0): width from pos to the content edge.")
        let o2 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            let px = o2.x + 12.0
            let py = o2.y + 26.0
            let w = CalcWrapWidthForPos(ImVec2(px, py), 0.0)
            dl |> add_line(ImVec2(px, py), ImVec2(px + w, py), span, 2.0)
            dl |> add_line(ImVec2(px, py - 8.0), ImVec2(px, py + 8.0), span, 2.0)
            dl |> add_line(ImVec2(px + w, py - 8.0), ImVec2(px + w, py + 8.0), span, 2.0)
            dl |> add_text(ImVec2(px + 8.0, py - 22.0), ink, "wrap width = {int(w)} px")
        }
        dummy(SPACER2, ImVec2(700.0, 56.0))
        separator()

        // Absolute screen coord of the content region's far (bottom-right) corner;
        // marked with a crosshair + ring. cursor-screen-pos + content-region-avail
        // gives the far corner.
        text("content far corner (cursor + GetContentRegionAvail): absolute screen coord.")
        let o3 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            let m = o3 + GetContentRegionAvail()
            let cx = m.x - 8.0
            let cy = m.y - 8.0
            dl |> add_line(ImVec2(cx - 12.0, cy), ImVec2(cx + 12.0, cy), mark, 2.0)
            dl |> add_line(ImVec2(cx, cy - 12.0), ImVec2(cx, cy + 12.0), mark, 2.0)
            dl |> add_circle(ImVec2(cx, cy), 10.0, mark, 0, 2.0)
            dl |> add_text(ImVec2(o3.x + 12.0, o3.y + 22.0), ink, "far corner = ({int(m.x)}, {int(m.y)})")
        }
        dummy(SPACER3, ImVec2(700.0, 64.0))
        separator()

        // ItemSize - reserve layout space for a custom-drawn item (advances the
        // cursor without registering an item, unlike dummy). The text below flows
        // directly under the reserved box.
        text("ItemSize reserves layout space for a custom-drawn box:")
        let o4 = GetCursorScreenPos()
        let box = ImVec2(300.0, 34.0)
        with_window_drawlist() $(var dl) {
            dl |> add_rect_filled(ImVec2(o4.x, o4.y), ImVec2(o4.x + box.x, o4.y + box.y), fill)
            dl |> add_rect(ImVec2(o4.x, o4.y), ImVec2(o4.x + box.x, o4.y + box.y), edge,
                           0.0, ImDrawFlags.None, 2.0)
            dl |> add_text(ImVec2(o4.x + 8.0, o4.y + 9.0), ink, "custom box - footprint reserved by ItemSize")
        }
        ItemSize(box)
        text("this line flows directly below the ItemSize-reserved box.")
        separator()
        text("All four readouts come from imgui_internal.h item/layout helpers (family #4).")
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
