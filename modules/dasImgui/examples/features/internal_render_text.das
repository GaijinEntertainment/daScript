options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: internal_render_text — the imgui_internal.h Render* TEXT helpers, the
//          last piece of family #1. RenderTextClipped / RenderTextClippedEx /
//          RenderTextEllipsis can't bind directly: they carry a non-defaulted
//          `const char* text_end` that must be NULL to mean "render the whole
//          string", but daslang's string->const char* cast turns a null/empty
//          string into "" (a pointer to '\0'), never nullptr — and these funcs
//          feed text_end straight to CalcTextSizeA, which over-reads past the
//          buffer on a non-null "" (verified: it crashes). So they are bound via
//          thin C++ wrappers in src/dasIMGUI.main.cpp that pin text_end (and the
//          text_size_if_known perf hint) to nullptr. This is the "binding is
//          usable" gate — it calls the real wrapped API at runtime.
// SHOWS:   RenderTextClipped (hard-clip text to a box, with alignment),
//          RenderTextClippedEx (the same into an explicit drawlist),
//          RenderTextEllipsis (truncate with a trailing ellipsis on overflow).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_render_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_render_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_render_text.das
// =============================================================================

[export]
def init() {
    harness_init("internal_render_text - imgui_internal.h Render* text helpers", 760, 540)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

def private box(var dl : ImDrawList?; x, y, w, h : float; edge : uint) {
    dl |> add_rect(ImVec2(x, y), ImVec2(x + w, y + h), edge, 0.0, ImDrawFlags.None, 2.0)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    let edge = rgba(120u, 180u, 240u, 255u)

    SetNextWindowSize(ImVec2(720.0, 500.0), ImGuiCond.Always)
    window(TEXT_WIN, (text = "internal_render_text", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("imgui_internal.h Render* text helpers - clip to a box / align / ellipsize.")
        separator()

        // RenderTextClipped - render text clipped to [pos_min, pos_max], aligned
        // within the box by the align fraction. The wrapper draws into the current
        // window drawlist (no explicit ImDrawList argument).
        text("RenderTextClipped: aligned within a box (left / center / right), and hard-clipped.")
        let o1 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            box(dl, o1.x + 12.0, o1.y + 8.0, 200.0, 28.0, edge)
            box(dl, o1.x + 232.0, o1.y + 8.0, 200.0, 28.0, edge)
            box(dl, o1.x + 452.0, o1.y + 8.0, 200.0, 28.0, edge)
        }
        RenderTextClipped(ImVec2(o1.x + 16.0, o1.y + 12.0), ImVec2(o1.x + 208.0, o1.y + 32.0), "left")
        RenderTextClipped(ImVec2(o1.x + 236.0, o1.y + 12.0), ImVec2(o1.x + 428.0, o1.y + 32.0),
                          "center", ImVec2(0.5, 0.0))
        RenderTextClipped(ImVec2(o1.x + 456.0, o1.y + 12.0), ImVec2(o1.x + 648.0, o1.y + 32.0),
                          "right", ImVec2(1.0, 0.0))
        dummy(SPACER1, ImVec2(700.0, 44.0))
        separator()

        // A box narrower than the text hard-clips the overflow at the box edge.
        text("RenderTextClipped with a too-narrow box: the overflow is hard-clipped (no ellipsis).")
        let o2 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            box(dl, o2.x + 12.0, o2.y + 8.0, 240.0, 28.0, edge)
        }
        RenderTextClipped(ImVec2(o2.x + 16.0, o2.y + 12.0), ImVec2(o2.x + 248.0, o2.y + 32.0),
                          "this string is wider than its clipping box")
        dummy(SPACER2, ImVec2(700.0, 44.0))
        separator()

        // RenderTextClippedEx - identical, but renders into an explicit ImDrawList
        // (here the window drawlist), so custom-widget code can target any list.
        text("RenderTextClippedEx: the same, but into an explicit ImDrawList argument.")
        let o3 = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            box(dl, o3.x + 12.0, o3.y + 8.0, 300.0, 28.0, edge)
            RenderTextClippedEx(dl, ImVec2(o3.x + 16.0, o3.y + 12.0), ImVec2(o3.x + 308.0, o3.y + 32.0),
                                "drawn via RenderTextClippedEx", ImVec2(0.5, 0.0))
        }
        dummy(SPACER3, ImVec2(700.0, 44.0))
        separator()

        // RenderTextEllipsis - clip to the box, but truncate with a trailing "..."
        // when the text overflows (clip_max_x / ellipsis_max_x mark the right edge).
        text("RenderTextEllipsis: overflow is truncated with a trailing ellipsis.")
        let o4 = GetCursorScreenPos()
        let ex = o4.x + 12.0 + 220.0
        with_window_drawlist() $(var dl) {
            box(dl, o4.x + 12.0, o4.y + 8.0, 220.0, 28.0, edge)
            RenderTextEllipsis(dl, ImVec2(o4.x + 16.0, o4.y + 12.0), ImVec2(ex, o4.y + 32.0),
                               ex, "this tab label is far too long to fit in its box")
        }
        dummy(SPACER4, ImVec2(700.0, 44.0))
        separator()
        text("All three are imgui_internal.h Render* text helpers (family #1), bound via C++ wrappers.")
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
