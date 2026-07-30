options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _allow_imgui_legacy = true

module imgui_drawlist_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require math

//! Drawlist rail — scope wrappers + primitive functions for ImGui's three
//! drawlists (window / foreground / background). Same telemetry surface as
//! ``[widget]`` (kind + bbox land in ``g_registry`` under the leaf path key
//! ``<window>/<mod>:<line>:<col>``) but no per-call state global. Path keys
//! are mouse-cards / playwright targetable for click / hover assertions.
//!
//! Usage:
//!
//!   with_window_drawlist() $(var dl) {
//!       dl |> add_rect_filled(p0, p1, rgba(40u, 80u, 200u, 200u))
//!       dl |> add_line(a, b, rgba(255u, 255u, 0u, 255u), 2.0f)
//!   }
//!
//! Each ``add_*`` primitive is tagged ``[drawlist_prim]`` so the
//! ``DrawlistPrimCallMacro`` rewrites the call site, prepending a stable
//! ``<mod>:<line>:<col>`` path key as the first positional argument. The
//! primitive body forwards to the raw ``ImDrawList::Add*`` binding and then
//! calls ``drawlist_register`` to publish a lightweight ``WidgetEntry``
//! (kind + bbox, no hover/active/focus, no serializer/dispatcher).

// ===== Telemetry helper =====

def public drawlist_register(widget_ident : string; kind : string; bbox : float4) {
    //! Publish a drawlist primitive's per-frame snapshot. Path key is ``container_path/widget_ident``,
    //! where ``widget_ident`` is the ``<mod>:<line>:<col>`` synthesized by ``[drawlist_prim]``. No unique-render
    //! guard — decorative primitives; a loop hitting the same site N times is fine (last write wins on bbox).
    let path = widget_path_key(widget_ident)
    var entry = WidgetEntry(kind = kind, hex_id = 0u, bbox = bbox)
    g_registry[path] <- entry
}

// ===== Scope wrappers =====

def public with_window_drawlist(blk : block<(var dl : ImDrawList?) : void>) {
    //! Run ``blk`` with the current window's drawlist. Drawing is clipped
    //! to the window's content region and stacks under any subsequent
    //! widgets rendered in this window.
    var dl = GetWindowDrawList()
    invoke(blk, dl)
}

def public with_foreground_drawlist(blk : block<(var dl : ImDrawList?) : void>) {
    //! Run ``blk`` with the top-most foreground drawlist — paints over
    //! every window on this viewport. Use for HUD overlays, drag indicators,
    //! tooltips that must escape window clipping.
    var dl = GetForegroundDrawList()
    invoke(blk, dl)
}

def public with_background_drawlist(blk : block<(var dl : ImDrawList?) : void>) {
    //! Run ``blk`` with the bottom-most background drawlist — paints under
    //! every window on this viewport. Use for full-viewport backdrops
    //! (gradients, vignettes, debug grids).
    var dl = GetBackgroundDrawList()
    invoke(blk, dl)
}

// ===== Primitives =====

[drawlist_prim]
def public add_line(var dl : ImDrawList?; a : float2; b : float2; col : uint;
                    thickness : float = 1.0f) {
    //! Stroked line from ``a`` to ``b``. ``col`` is a packed ABGR ``uint``
    //! (use ``rgba(r,g,b,a)`` or ``GetColorU32(ImGuiCol.*)`` to build).
    *dl |> AddLine(a, b, col, thickness)
    drawlist_register(widget_ident, "add_line",
                      float4(min(a.x, b.x), min(a.y, b.y), max(a.x, b.x), max(a.y, b.y)))
}

[drawlist_prim]
def public add_rect(var dl : ImDrawList?; a : float2; b : float2; col : uint;
                    rounding : float = 0.0f;
                    flags : ImDrawFlags = ImDrawFlags.None;
                    thickness : float = 1.0f) {
    //! Stroked rectangle. ``a`` / ``b`` are opposite corners (screen space).
    //! ``rounding`` rounds corners; ``flags`` (``ImDrawFlags.RoundCornersTop`` etc.)
    //! pick which corners are rounded.
    *dl |> AddRect(a, b, col, rounding, flags, thickness)
    drawlist_register(widget_ident, "add_rect", float4(a.x, a.y, b.x, b.y))
}

[drawlist_prim]
def public add_rect_filled(var dl : ImDrawList?; a : float2; b : float2; col : uint;
                           rounding : float = 0.0f;
                           flags : ImDrawFlags = ImDrawFlags.None) {
    //! Filled rectangle. Same coords / rounding / flags as ``add_rect``;
    //! no stroke thickness because the interior is solid.
    *dl |> AddRectFilled(a, b, col, rounding, flags)
    drawlist_register(widget_ident, "add_rect_filled", float4(a.x, a.y, b.x, b.y))
}

[drawlist_prim]
def public add_circle(var dl : ImDrawList?; center : float2; radius : float; col : uint;
                      num_segments : int = 0; thickness : float = 1.0f) {
    //! Stroked circle. ``num_segments=0`` (default) uses ImGui's adaptive
    //! ``CircleSegmentMaxError``-derived count — smooth at any radius.
    *dl |> AddCircle(center, radius, col, num_segments, thickness)
    drawlist_register(widget_ident, "add_circle",
                      float4(center.x - radius, center.y - radius,
                             center.x + radius, center.y + radius))
}

[drawlist_prim]
def public add_circle_filled(var dl : ImDrawList?; center : float2; radius : float; col : uint;
                             num_segments : int = 0) {
    //! Filled circle. ``num_segments=0`` uses ImGui's adaptive segment count.
    *dl |> AddCircleFilled(center, radius, col, num_segments)
    drawlist_register(widget_ident, "add_circle_filled",
                      float4(center.x - radius, center.y - radius,
                             center.x + radius, center.y + radius))
}

[drawlist_prim]
def public add_triangle(var dl : ImDrawList?; a : float2; b : float2; c : float2;
                        col : uint; thickness : float = 1.0f) {
    //! Stroked triangle through three vertices.
    *dl |> AddTriangle(a, b, c, col, thickness)
    let xs_min = min(min(a.x, b.x), c.x)
    let ys_min = min(min(a.y, b.y), c.y)
    let xs_max = max(max(a.x, b.x), c.x)
    let ys_max = max(max(a.y, b.y), c.y)
    drawlist_register(widget_ident, "add_triangle",
                      float4(xs_min, ys_min, xs_max, ys_max))
}

[drawlist_prim]
def public add_triangle_filled(var dl : ImDrawList?; a : float2; b : float2; c : float2;
                               col : uint) {
    //! Filled triangle through three vertices.
    *dl |> AddTriangleFilled(a, b, c, col)
    let xs_min = min(min(a.x, b.x), c.x)
    let ys_min = min(min(a.y, b.y), c.y)
    let xs_max = max(max(a.x, b.x), c.x)
    let ys_max = max(max(a.y, b.y), c.y)
    drawlist_register(widget_ident, "add_triangle_filled",
                      float4(xs_min, ys_min, xs_max, ys_max))
}

[drawlist_prim]
def public add_text(var dl : ImDrawList?; pos : float2; col : uint; text : string) {
    //! Render ``text`` at ``pos`` in the current ImGui font. ``col`` is the
    //! glyph color. Bbox is computed via ``CalcTextSize`` so playwright
    //! targeting on the text's path key resolves to its rendered rect.
    *dl |> AddText(pos, col, text)
    let sz = CalcTextSize(text)
    drawlist_register(widget_ident, "add_text",
                      float4(pos.x, pos.y, pos.x + sz.x, pos.y + sz.y))
}

[drawlist_prim]
def public add_text_clipped(var dl : ImDrawList?; pos : float2; col : uint;
                            text : string; clip_rect : float4) {
    //! Render ``text`` at ``pos`` with a per-call CPU-side clip rect — only glyphs intersecting
    //! ``clip_rect = (xmin, ymin, xmax, ymax)`` are rasterized. Current font/size; no clip-stack push or
    //! PushClipRect mutation. Equivalent to ``ImDrawList::AddText(font, size, pos, col, text, NULL, 0.0f, &clip_rect)``.
    var cr = clip_rect
    unsafe {
        *dl |> AddText(GetFont(), GetFontSize(), pos, col, text, 0.0f, addr(cr))
    }
    let sz = CalcTextSize(text)
    drawlist_register(widget_ident, "add_text_clipped",
                      float4(pos.x, pos.y, pos.x + sz.x, pos.y + sz.y))
}

// ===== Extra shape primitives =====

[drawlist_prim]
def public add_rect_filled_multi_color(var dl : ImDrawList?; a : float2; b : float2;
                                       col_ul : uint; col_ur : uint;
                                       col_br : uint; col_bl : uint) {
    //! Filled rectangle with a per-corner color gradient. Corners go
    //! upper-left / upper-right / bottom-right / bottom-left. ImGui
    //! interpolates linearly between them across the interior.
    *dl |> AddRectFilledMultiColor(a, b, col_ul, col_ur, col_br, col_bl)
    drawlist_register(widget_ident, "add_rect_filled_multi_color",
                      float4(a.x, a.y, b.x, b.y))
}

[drawlist_prim]
def public add_ellipse(var dl : ImDrawList?; center : float2; radii : float2; col : uint;
                       rot : float = 0.0f; num_segments : int = 0;
                       thickness : float = 1.0f) {
    //! Stroked ellipse. ``radii`` = (rx, ry) half-axes. ``rot`` rotates the
    //! ellipse about ``center`` (radians). ``num_segments=0`` uses ImGui's
    //! adaptive segment count.
    *dl |> AddEllipse(center, radii, col, rot, num_segments, thickness)
    drawlist_register(widget_ident, "add_ellipse",
                      float4(center.x - radii.x, center.y - radii.y,
                             center.x + radii.x, center.y + radii.y))
}

[drawlist_prim]
def public add_ellipse_filled(var dl : ImDrawList?; center : float2; radii : float2; col : uint;
                              rot : float = 0.0f; num_segments : int = 0) {
    //! Filled ellipse. Same shape semantics as ``add_ellipse``.
    *dl |> AddEllipseFilled(center, radii, col, rot, num_segments)
    drawlist_register(widget_ident, "add_ellipse_filled",
                      float4(center.x - radii.x, center.y - radii.y,
                             center.x + radii.x, center.y + radii.y))
}

[drawlist_prim]
def public add_ngon(var dl : ImDrawList?; center : float2; radius : float; col : uint;
                    num_segments : int; thickness : float = 1.0f) {
    //! Stroked regular N-gon (``num_segments`` sides). Unlike ``add_circle``,
    //! ``num_segments`` is required — there is no adaptive default.
    *dl |> AddNgon(center, radius, col, num_segments, thickness)
    drawlist_register(widget_ident, "add_ngon",
                      float4(center.x - radius, center.y - radius,
                             center.x + radius, center.y + radius))
}

[drawlist_prim]
def public add_ngon_filled(var dl : ImDrawList?; center : float2; radius : float; col : uint;
                           num_segments : int) {
    //! Filled regular N-gon (``num_segments`` sides).
    *dl |> AddNgonFilled(center, radius, col, num_segments)
    drawlist_register(widget_ident, "add_ngon_filled",
                      float4(center.x - radius, center.y - radius,
                             center.x + radius, center.y + radius))
}

[drawlist_prim]
def public add_polyline(var dl : ImDrawList?; points : array<float2>; col : uint;
                        flags : ImDrawFlags = ImDrawFlags.None;
                        thickness : float = 1.0f) {
    //! Stroked polyline through ``points``. ``flags`` (typically ``ImDrawFlags.Closed`` for closed
    //! polygons) controls open / closed shape. Pass ``points`` as an ``array<float2>``; the wrapper
    //! hands the underlying pointer to ImGui.
    let n = length(points)
    return if (n < 2)
    unsafe {
        let ptr = addr(points[0])
        *dl |> AddPolyline(reinterpret<ImVec2?>(ptr), n, col, flags, thickness)
    }
    var xs_min = points[0].x
    var ys_min = points[0].y
    var xs_max = points[0].x
    var ys_max = points[0].y
    for (p in points) {
        xs_min = min(xs_min, p.x)
        ys_min = min(ys_min, p.y)
        xs_max = max(xs_max, p.x)
        ys_max = max(ys_max, p.y)
    }
    drawlist_register(widget_ident, "add_polyline",
                      float4(xs_min, ys_min, xs_max, ys_max))
}

[drawlist_prim]
def public add_bezier_cubic(var dl : ImDrawList?; p1 : float2; p2 : float2;
                            p3 : float2; p4 : float2; col : uint;
                            thickness : float = 1.0f; num_segments : int = 0) {
    //! Cubic bezier curve through ``p1`` → ``p4`` with control points
    //! ``p2`` / ``p3``. ``num_segments=0`` uses ImGui's adaptive count.
    *dl |> AddBezierCubic(p1, p2, p3, p4, col, thickness, num_segments)
    let xs_min = min(min(p1.x, p2.x), min(p3.x, p4.x))
    let ys_min = min(min(p1.y, p2.y), min(p3.y, p4.y))
    let xs_max = max(max(p1.x, p2.x), max(p3.x, p4.x))
    let ys_max = max(max(p1.y, p2.y), max(p3.y, p4.y))
    drawlist_register(widget_ident, "add_bezier_cubic",
                      float4(xs_min, ys_min, xs_max, ys_max))
}

[drawlist_prim]
def public add_bezier_quadratic(var dl : ImDrawList?; p1 : float2; p2 : float2;
                                p3 : float2; col : uint;
                                thickness : float = 1.0f; num_segments : int = 0) {
    //! Quadratic bezier curve through ``p1`` → ``p3`` with one control
    //! point ``p2``. ``num_segments=0`` uses ImGui's adaptive count.
    *dl |> AddBezierQuadratic(p1, p2, p3, col, thickness, num_segments)
    let xs_min = min(min(p1.x, p2.x), p3.x)
    let ys_min = min(min(p1.y, p2.y), p3.y)
    let xs_max = max(max(p1.x, p2.x), p3.x)
    let ys_max = max(max(p1.y, p2.y), p3.y)
    drawlist_register(widget_ident, "add_bezier_quadratic",
                      float4(xs_min, ys_min, xs_max, ys_max))
}

// ===== Path-building primitives =====

// Operate on the drawlist's internal path stack (path_clear → path_line_to / path_arc_to → path_stroke).
// No per-call registry entry — the final ``path_stroke`` / ``path_fill_convex`` / ``path_fill_concave`` renders.

def public path_clear(var dl : ImDrawList?) {
    //! Clear the drawlist's path stack — discards any pending path_line_to /
    //! path_arc_to / path_bezier_* points without rendering them.
    *dl |> PathClear()
}

def public path_line_to(var dl : ImDrawList?; pos : float2) {
    //! Append a straight-line segment to the path, ending at ``pos``.
    *dl |> PathLineTo(pos)
}

def public path_arc_to(var dl : ImDrawList?; center : float2; radius : float;
                       a_min : float; a_max : float; num_segments : int = 0) {
    //! Append a circular arc to the path. Angles in radians. ``num_segments=0``
    //! uses ImGui's adaptive count.
    *dl |> PathArcTo(center, radius, a_min, a_max, num_segments)
}

def public path_bezier_quadratic_curve_to(var dl : ImDrawList?; p2 : float2;
                                          p3 : float2; num_segments : int = 0) {
    //! Append a quadratic bezier curve from the path's current endpoint to
    //! ``p3``, with control point ``p2``. ``num_segments=0`` uses ImGui's
    //! adaptive count.
    *dl |> PathBezierQuadraticCurveTo(p2, p3, num_segments)
}

def public path_fill_convex(var dl : ImDrawList?; col : uint) {
    //! Close the current path and fill it (assumed convex — faster). For
    //! concave polygons use ``path_fill_concave``.
    *dl |> PathFillConvex(col)
}

def public path_fill_concave(var dl : ImDrawList?; col : uint) {
    //! Close the current path and fill it. Handles concave shapes correctly
    //! at the cost of an internal triangulation step.
    *dl |> PathFillConcave(col)
}

def public path_stroke(var dl : ImDrawList?; col : uint;
                       flags : ImDrawFlags = ImDrawFlags.None;
                       thickness : float = 1.0f) {
    //! Stroke the current path. ``flags`` (``ImDrawFlags.Closed`` for closed
    //! shapes) controls whether the last vertex joins back to the first.
    *dl |> PathStroke(col, flags, thickness)
}

// ===== Channel-splitter primitives =====

// Split the drawlist into N z-ordered layers, draw onto each, then merge — so a later draw can land
// UNDER an earlier one (typical: a card background painted after the contents but rendered behind them).
// Sequence: channels_split(n) → channels_set_current(i) → draw → channels_merge().

def public channels_split(var dl : ImDrawList?; count : int) {
    //! Split the drawlist into ``count`` channels. Subsequent draws land on
    //! the current channel (default 0). Pair with ``channels_merge``.
    *dl |> ChannelsSplit(count)
}

def public channels_set_current(var dl : ImDrawList?; idx : int) {
    //! Set the active channel index (0-based). Subsequent draws on ``dl``
    //! go to that channel until the next ``channels_set_current`` or
    //! ``channels_merge``.
    *dl |> ChannelsSetCurrent(idx)
}

def public channels_merge(var dl : ImDrawList?) {
    //! Merge all channels back into the drawlist's main draw stream. Channel
    //! 0 renders first, then channel 1 on top, etc. Pair with
    //! ``channels_split``.
    *dl |> ChannelsMerge()
}

// (with_clip_rect lives in imgui_scope_builtin.das — the scope rail —
// since it gates ImGui widgets as well as drawlist primitives.)

// ===== Per-drawlist clip-rect scope =====

def public with_drawlist_clip_rect(var dl : ImDrawList?; clip_rect_min : float2;
                                   clip_rect_max : float2;
                                   intersect_with_current : bool;
                                   blk : block) {
    //! Run ``blk`` with ``dl->PushClipRect(min, max, intersect)`` on THIS drawlist's clip stack;
    //! matching ``dl->PopClipRect()`` on return. Only affects subsequent primitives on ``dl`` (ImGui
    //! hit-testing + other drawlists unaffected). For the ImGui-wide variant use ``with_clip_rect`` (imgui_scope_builtin).
    *dl |> PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current)
    invoke(blk)
    *dl |> PopClipRect()
}
