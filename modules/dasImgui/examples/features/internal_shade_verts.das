options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require math

// =============================================================================
// FEATURE: internal_shade_verts — exercises the imgui_internal.h ShadeVerts*
//          vertex-shading family (#2, bound via bind_imgui.das). Each helper
//          rewrites vertices that were ALREADY emitted into the drawlist, over a
//          [start, end) index range. The range comes from length(dl.VtxBuffer)
//          read before and after the draw (the real VtxBuffer.Size; `length` is
//          allow-listed in imgui_lint as a host-agnostic ImVector accessor).
//          This is the "binding is usable" gate for family #2 — it calls the
//          real bound API at runtime, not just compiles it.
// SHOWS:   ShadeVertsLinearColorGradientKeepAlpha (recolor a quad to a gradient),
//          ShadeVertsLinearUV (UV remap — binding exercised; visually a no-op
//          without a custom texture), ShadeVertsTransformPos (rotate a triangle
//          in place around its centroid).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_shade_verts.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_shade_verts.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_shade_verts.das
// =============================================================================

[export]
def init() {
    harness_init("internal_shade_verts - imgui_internal.h ShadeVerts* family", 760, 470)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 430.0), ImGuiCond.Always)
    window(SHADE_WIN, (text = "internal_shade_verts", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        text("imgui_internal.h ShadeVerts* helpers - rewrite already-emitted vertices.")
        separator()

        let o = GetCursorScreenPos()
        with_window_drawlist() $(var dl) {
            // (1) Linear color gradient: emit a white quad, then recolor its
            //     vertices left-to-right from blue to magenta.
            let g0 = ImVec2(o.x + 20.0, o.y + 20.0)
            let g1 = ImVec2(o.x + 250.0, o.y + 92.0)
            let v0 = length(dl.VtxBuffer)
            dl |> add_rect_filled(g0, g1, rgba(255u, 255u, 255u, 255u))
            let v1 = length(dl.VtxBuffer)
            ShadeVertsLinearColorGradientKeepAlpha(dl, v0, v1, g0, g1,
                rgba(60u, 120u, 240u, 255u), rgba(230u, 70u, 200u, 255u))

            // (2) Linear UV remap over a second quad. No custom texture is bound,
            //     so this is visually a no-op, but it exercises the binding end to
            //     end (range + four UV corners + clamp flag).
            let u0 = ImVec2(o.x + 20.0, o.y + 112.0)
            let u1 = ImVec2(o.x + 250.0, o.y + 184.0)
            let w0 = length(dl.VtxBuffer)
            dl |> add_rect_filled(u0, u1, rgba(90u, 200u, 140u, 255u))
            let w1 = length(dl.VtxBuffer)
            ShadeVertsLinearUV(dl, w0, w1, u0, u1, ImVec2(0.0, 0.0), ImVec2(1.0, 1.0), true)

            // (3) Position transform: emit a triangle, then rotate its vertices
            //     ~25 degrees in place around its centroid.
            let c = ImVec2(o.x + 430.0, o.y + 100.0)
            let t0 = length(dl.VtxBuffer)
            dl |> add_triangle_filled(ImVec2(c.x, c.y - 52.0),
                                      ImVec2(c.x + 48.0, c.y + 32.0),
                                      ImVec2(c.x - 48.0, c.y + 32.0),
                                      rgba(240u, 190u, 70u, 255u))
            let t1 = length(dl.VtxBuffer)
            let ang = 0.43         // ~25 degrees, in radians
            ShadeVertsTransformPos(dl, t0, t1, c, cos(ang), sin(ang), c)
        }

        dummy(SPACER, ImVec2(700.0, 208.0))
        separator()
        text("Each shape's vertices were rewritten in place by a ShadeVerts* call.")
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
