options gen2
options indenting = 4

module sfx_icons shared

require imgui public
require daslib/math

//! SFX Lab icon set — vector icons drawn directly onto the ImGui draw list,
//! in the same asset-free / data-first spirit as ``imgui_theme_daslang.das``.
//! No SVG, no texture atlas: every glyph is geometry on a logical 24x24 grid,
//! mapped into any target rect and tinted from the daslang amber accent.
//!
//! Usage (inside an imgui_harness update()):
//!     if (icon_button("play"))  { play_mix() }
//!     same_line()
//!     icon("waveform", 16.0f)            // draw-only, no button
//!
//! Destined for dasImgui as a general, growing icon set; depends only on imgui + math.

// ===== Accent (mirrors imgui_theme_daslang amber) =====
let private amber = float4(0.910f, 0.631f, 0.227f, 1.0f) // #e8a13a

// ===== Draw context =====
// One small value passed to every glyph. `s` is the box side in px; coords are
// authored on a 24-grid and scaled by s/24. `dl` is the window draw list pointer.
struct IconCtx {
    dl : ImDrawList?
    org : float2    // top-left of the glyph box, screen space
    s : float       // box side in px
    col : uint      // glyph color, ImU32
    th : float      // stroke thickness in px
}

def private rad(d : float) : float { return d * 3.14159265f / 180.0f }

def private P(ctx : IconCtx; gx, gy : float) : float2 {
    return float2(ctx.org.x + gx * ctx.s / 24.0f, ctx.org.y + gy * ctx.s / 24.0f)
}

// stroked line with round caps (matches the SVG stroke-linecap:round look)
def private L(var ctx : IconCtx; x1, y1, x2, y2 : float) {
    *ctx.dl |> AddLine(P(ctx, x1, y1), P(ctx, x2, y2), ctx.col, ctx.th)
    *ctx.dl |> AddCircleFilled(P(ctx, x1, y1), ctx.th * 0.5f, ctx.col, 0)
    *ctx.dl |> AddCircleFilled(P(ctx, x2, y2), ctx.th * 0.5f, ctx.col, 0)
}

def private TriF(var ctx : IconCtx; ax, ay, bx, by, cx, cy : float) {
    *ctx.dl |> AddTriangleFilled(P(ctx, ax, ay), P(ctx, bx, by), P(ctx, cx, cy), ctx.col)
}

def private RectF(var ctx : IconCtx; x, y, w, h, round : float) {
    *ctx.dl |> AddRectFilled(P(ctx, x, y), P(ctx, x + w, y + h), ctx.col, round * ctx.s / 24.0f, ImDrawFlags.None)
}

def private RectS(var ctx : IconCtx; x, y, w, h, round : float) {
    *ctx.dl |> AddRect(P(ctx, x, y), P(ctx, x + w, y + h), ctx.col, round * ctx.s / 24.0f, ImDrawFlags.None, ctx.th)
}

def private Disc(var ctx : IconCtx; cx, cy, rr : float) {
    *ctx.dl |> AddCircleFilled(P(ctx, cx, cy), rr * ctx.s / 24.0f, ctx.col, 0)
}

def private Ring(var ctx : IconCtx; cx, cy, rr : float) {
    *ctx.dl |> AddCircle(P(ctx, cx, cy), rr * ctx.s / 24.0f, ctx.col, 0, ctx.th)
}

// polyline arc, angles in degrees (y-down screen space), round-capped
def private Arc(var ctx : IconCtx; cx, cy, rr, d0, d1 : float; segs : int = 18) {
    var prev = P(ctx, cx + rr * cos(rad(d0)), cy + rr * sin(rad(d0)))
    *ctx.dl |> AddCircleFilled(prev, ctx.th * 0.5f, ctx.col, 0)
    for (i in range(1, segs + 1)) {
        let t = d0 + (d1 - d0) * float(i) / float(segs)
        let cur = P(ctx, cx + rr * cos(rad(t)), cy + rr * sin(rad(t)))
        *ctx.dl |> AddLine(prev, cur, ctx.col, ctx.th)
        prev = cur
    }
    *ctx.dl |> AddCircleFilled(prev, ctx.th * 0.5f, ctx.col, 0)
}

// ============================================================
// Glyphs — coordinates ported 1:1 from the HTML reference sheet
// ============================================================

// --- Transport ---
def private ic_play(var ctx : IconCtx) { TriF(ctx, 8.0f, 5.0f, 19.0f, 12.0f, 8.0f, 19.0f) }
def private ic_pause(var ctx : IconCtx) { RectF(ctx, 7.0f, 5.0f, 3.5f, 14.0f, 1.0f); RectF(ctx, 13.5f, 5.0f, 3.5f, 14.0f, 1.0f) }
def private ic_stop(var ctx : IconCtx) { RectF(ctx, 6.0f, 6.0f, 12.0f, 12.0f, 1.5f) }
def private ic_record(var ctx : IconCtx) { Disc(ctx, 12.0f, 12.0f, 6.0f) }
def private ic_prev(var ctx : IconCtx) { TriF(ctx, 18.0f, 6.0f, 9.0f, 12.0f, 18.0f, 18.0f); RectF(ctx, 6.0f, 6.0f, 2.5f, 12.0f, 0.5f) }
def private ic_next(var ctx : IconCtx) { TriF(ctx, 6.0f, 6.0f, 15.0f, 12.0f, 6.0f, 18.0f); RectF(ctx, 15.5f, 6.0f, 2.5f, 12.0f, 0.5f) }
def private ic_rewind(var ctx : IconCtx) { TriF(ctx, 11.0f, 6.0f, 4.0f, 12.0f, 11.0f, 18.0f); TriF(ctx, 20.0f, 6.0f, 13.0f, 12.0f, 20.0f, 18.0f) }
def private ic_forward(var ctx : IconCtx) { TriF(ctx, 4.0f, 6.0f, 11.0f, 12.0f, 4.0f, 18.0f); TriF(ctx, 13.0f, 6.0f, 20.0f, 12.0f, 13.0f, 18.0f) }
def private ic_loop(var ctx : IconCtx) {
    Arc(ctx, 12.0f, 12.0f, 7.0f, 200.0f, 340.0f)
    L(ctx, 18.6f, 9.6f, 16.6f, 8.6f); L(ctx, 18.6f, 9.6f, 17.6f, 11.8f)
    Arc(ctx, 12.0f, 12.0f, 7.0f, 20.0f, 160.0f)
    L(ctx, 5.4f, 14.4f, 7.4f, 15.4f); L(ctx, 5.4f, 14.4f, 6.4f, 12.2f)
}

// --- Edit ---
def private ic_cut(var ctx : IconCtx) {
    Ring(ctx, 6.0f, 7.0f, 2.5f); Ring(ctx, 6.0f, 17.0f, 2.5f)
    L(ctx, 8.1f, 8.3f, 20.0f, 17.0f); L(ctx, 8.1f, 15.7f, 20.0f, 7.0f); L(ctx, 8.1f, 8.3f, 13.0f, 11.7f)
}
def private ic_add(var ctx : IconCtx) { L(ctx, 12.0f, 5.0f, 12.0f, 19.0f); L(ctx, 5.0f, 12.0f, 19.0f, 12.0f) }
def private ic_duplicate(var ctx : IconCtx) {
    RectS(ctx, 8.0f, 8.0f, 11.0f, 11.0f, 2.0f)
    L(ctx, 5.0f, 16.0f, 5.0f, 5.0f); L(ctx, 5.0f, 5.0f, 16.0f, 5.0f)
}
def private ic_delete(var ctx : IconCtx) {
    L(ctx, 5.0f, 7.0f, 19.0f, 7.0f)
    L(ctx, 9.0f, 7.0f, 9.0f, 4.5f); L(ctx, 9.0f, 4.5f, 15.0f, 4.5f); L(ctx, 15.0f, 4.5f, 15.0f, 7.0f)
    L(ctx, 7.0f, 7.0f, 8.0f, 20.0f); L(ctx, 17.0f, 7.0f, 16.0f, 20.0f); L(ctx, 8.0f, 20.0f, 16.0f, 20.0f)
}
def private ic_undo(var ctx : IconCtx) {
    L(ctx, 9.0f, 7.0f, 4.0f, 12.0f); L(ctx, 4.0f, 12.0f, 9.0f, 17.0f)
    L(ctx, 4.0f, 12.0f, 13.0f, 12.0f); Arc(ctx, 13.0f, 18.0f, 6.0f, 270.0f, 360.0f)
}
def private ic_redo(var ctx : IconCtx) {
    L(ctx, 15.0f, 7.0f, 20.0f, 12.0f); L(ctx, 20.0f, 12.0f, 15.0f, 17.0f)
    L(ctx, 20.0f, 12.0f, 11.0f, 12.0f); Arc(ctx, 11.0f, 18.0f, 6.0f, 180.0f, 270.0f)
}
def private ic_zoom_in(var ctx : IconCtx) {
    Ring(ctx, 10.5f, 10.5f, 6.5f); L(ctx, 15.5f, 15.5f, 20.5f, 20.5f)
    L(ctx, 10.5f, 7.5f, 10.5f, 13.5f); L(ctx, 7.5f, 10.5f, 13.5f, 10.5f)
}
def private ic_zoom_out(var ctx : IconCtx) {
    Ring(ctx, 10.5f, 10.5f, 6.5f); L(ctx, 15.5f, 15.5f, 20.5f, 20.5f)
    L(ctx, 7.5f, 10.5f, 13.5f, 10.5f)
}

// --- Visualize ---
def private ic_waveform(var ctx : IconCtx) {
    L(ctx, 4.0f, 10.0f, 4.0f, 14.0f); L(ctx, 7.0f, 7.0f, 7.0f, 17.0f); L(ctx, 10.0f, 3.5f, 10.0f, 20.5f)
    L(ctx, 13.0f, 8.0f, 13.0f, 16.0f); L(ctx, 16.0f, 5.5f, 16.0f, 18.5f); L(ctx, 19.0f, 10.0f, 19.0f, 14.0f)
}
def private ic_spectrum(var ctx : IconCtx) {
    L(ctx, 4.0f, 20.0f, 4.0f, 13.0f); L(ctx, 8.0f, 20.0f, 8.0f, 6.0f); L(ctx, 12.0f, 20.0f, 12.0f, 11.0f)
    L(ctx, 16.0f, 20.0f, 16.0f, 4.0f); L(ctx, 20.0f, 20.0f, 20.0f, 15.0f)
}
def private ic_envelope(var ctx : IconCtx) {
    L(ctx, 3.0f, 20.0f, 7.0f, 5.0f); L(ctx, 7.0f, 5.0f, 11.0f, 12.0f); L(ctx, 11.0f, 12.0f, 17.0f, 12.0f); L(ctx, 17.0f, 12.0f, 21.0f, 20.0f)
}
def private ic_filter(var ctx : IconCtx) {
    L(ctx, 3.0f, 8.0f, 11.0f, 8.0f); L(ctx, 11.0f, 8.0f, 16.0f, 11.0f); L(ctx, 16.0f, 11.0f, 18.0f, 15.0f); L(ctx, 18.0f, 15.0f, 18.5f, 19.0f)
}
def private ic_eq(var ctx : IconCtx) {
    L(ctx, 6.0f, 4.0f, 6.0f, 20.0f); L(ctx, 12.0f, 4.0f, 12.0f, 20.0f); L(ctx, 18.0f, 4.0f, 18.0f, 20.0f)
    RectF(ctx, 3.5f, 8.0f, 5.0f, 3.2f, 1.0f); RectF(ctx, 9.5f, 13.0f, 5.0f, 3.2f, 1.0f); RectF(ctx, 15.5f, 6.0f, 5.0f, 3.2f, 1.0f)
}
def private ic_grid(var ctx : IconCtx) {
    RectS(ctx, 4.0f, 4.0f, 16.0f, 16.0f, 1.5f)
    L(ctx, 4.0f, 9.3f, 20.0f, 9.3f); L(ctx, 4.0f, 14.6f, 20.0f, 14.6f)
    L(ctx, 9.3f, 4.0f, 9.3f, 20.0f); L(ctx, 14.6f, 4.0f, 14.6f, 20.0f)
}

// --- Mix ---
def private ic_speaker_body(var ctx : IconCtx) {
    RectF(ctx, 4.0f, 9.0f, 4.0f, 6.0f, 0.0f)
    TriF(ctx, 8.0f, 9.0f, 8.0f, 15.0f, 13.0f, 19.0f)
    TriF(ctx, 8.0f, 9.0f, 13.0f, 19.0f, 13.0f, 5.0f)
}
def private ic_volume(var ctx : IconCtx) {
    ic_speaker_body(ctx)
    Arc(ctx, 13.0f, 12.0f, 3.5f, -55.0f, 55.0f); Arc(ctx, 13.0f, 12.0f, 7.0f, -55.0f, 55.0f)
}
def private ic_mute(var ctx : IconCtx) {
    ic_speaker_body(ctx)
    L(ctx, 16.0f, 9.5f, 21.0f, 14.5f); L(ctx, 21.0f, 9.5f, 16.0f, 14.5f)
}
def private ic_fader(var ctx : IconCtx) { L(ctx, 3.0f, 12.0f, 21.0f, 12.0f); RectF(ctx, 13.0f, 7.5f, 4.0f, 9.0f, 1.5f) }
def private ic_sliders(var ctx : IconCtx) {
    L(ctx, 3.0f, 8.0f, 21.0f, 8.0f); L(ctx, 3.0f, 16.0f, 21.0f, 16.0f)
    Ring(ctx, 9.0f, 8.0f, 2.6f); Ring(ctx, 15.0f, 16.0f, 2.6f)
}
def private ic_pan(var ctx : IconCtx) {
    L(ctx, 3.0f, 12.0f, 21.0f, 12.0f); L(ctx, 12.0f, 5.0f, 12.0f, 19.0f)
    L(ctx, 6.0f, 9.0f, 3.0f, 12.0f); L(ctx, 3.0f, 12.0f, 6.0f, 15.0f)
    L(ctx, 18.0f, 9.0f, 21.0f, 12.0f); L(ctx, 21.0f, 12.0f, 18.0f, 15.0f)
}
def private ic_metronome(var ctx : IconCtx) {
    L(ctx, 9.0f, 4.0f, 15.0f, 4.0f); L(ctx, 15.0f, 4.0f, 18.0f, 20.0f); L(ctx, 18.0f, 20.0f, 6.0f, 20.0f); L(ctx, 6.0f, 20.0f, 9.0f, 4.0f)
    L(ctx, 12.0f, 18.0f, 16.0f, 6.0f)
}
def private ic_mic(var ctx : IconCtx) {
    RectS(ctx, 9.0f, 3.0f, 6.0f, 11.0f, 3.0f)
    Arc(ctx, 12.0f, 11.0f, 6.0f, 20.0f, 160.0f)
    L(ctx, 12.0f, 17.0f, 12.0f, 21.0f); L(ctx, 8.5f, 21.0f, 15.5f, 21.0f)
}
def private ic_headphones(var ctx : IconCtx) {
    Arc(ctx, 12.0f, 12.0f, 7.0f, 180.0f, 360.0f)
    RectF(ctx, 3.0f, 13.0f, 4.0f, 7.0f, 1.5f); RectF(ctx, 17.0f, 13.0f, 4.0f, 7.0f, 1.5f)
}

// --- Organize ---
def private ic_layers(var ctx : IconCtx) {
    L(ctx, 12.0f, 3.0f, 21.0f, 8.0f); L(ctx, 21.0f, 8.0f, 12.0f, 13.0f); L(ctx, 12.0f, 13.0f, 3.0f, 8.0f); L(ctx, 3.0f, 8.0f, 12.0f, 3.0f)
    L(ctx, 3.0f, 13.0f, 12.0f, 18.0f); L(ctx, 12.0f, 18.0f, 21.0f, 13.0f)
}
def private ic_folder(var ctx : IconCtx) {
    RectS(ctx, 3.0f, 8.0f, 18.0f, 10.0f, 1.5f)
    L(ctx, 3.0f, 8.0f, 8.0f, 8.0f); L(ctx, 8.0f, 8.0f, 10.0f, 6.0f); L(ctx, 10.0f, 6.0f, 14.0f, 6.0f); L(ctx, 14.0f, 6.0f, 14.0f, 8.0f)
}
def private ic_save(var ctx : IconCtx) {
    RectS(ctx, 5.0f, 4.0f, 14.0f, 16.0f, 1.0f)
    L(ctx, 8.0f, 4.0f, 8.0f, 9.0f); L(ctx, 15.0f, 4.0f, 15.0f, 9.0f); L(ctx, 8.0f, 9.0f, 15.0f, 9.0f)
    RectS(ctx, 8.0f, 13.0f, 8.0f, 6.0f, 0.5f)
}
def private ic_tray(var ctx : IconCtx) {
    L(ctx, 5.0f, 13.0f, 5.0f, 18.0f); L(ctx, 5.0f, 18.0f, 19.0f, 18.0f); L(ctx, 19.0f, 18.0f, 19.0f, 13.0f)
}
def private ic_export(var ctx : IconCtx) {
    L(ctx, 12.0f, 14.0f, 12.0f, 4.0f); L(ctx, 8.0f, 8.0f, 12.0f, 4.0f); L(ctx, 12.0f, 4.0f, 16.0f, 8.0f); ic_tray(ctx)
}
def private ic_import(var ctx : IconCtx) {
    L(ctx, 12.0f, 4.0f, 12.0f, 14.0f); L(ctx, 8.0f, 10.0f, 12.0f, 14.0f); L(ctx, 12.0f, 14.0f, 16.0f, 10.0f); ic_tray(ctx)
}
def private ic_visible(var ctx : IconCtx) {
    Arc(ctx, 12.0f, 18.0f, 8.5f, 215.0f, 325.0f); Arc(ctx, 12.0f, 6.0f, 8.5f, 35.0f, 145.0f)
    Ring(ctx, 12.0f, 12.0f, 3.0f)
}
def private ic_lock(var ctx : IconCtx) {
    RectS(ctx, 5.0f, 11.0f, 14.0f, 9.0f, 2.0f)
    L(ctx, 8.0f, 11.0f, 8.0f, 8.0f); L(ctx, 16.0f, 8.0f, 16.0f, 11.0f); Arc(ctx, 12.0f, 8.0f, 4.0f, 180.0f, 360.0f)
}
def private ic_unlock(var ctx : IconCtx) {
    RectS(ctx, 5.0f, 11.0f, 14.0f, 9.0f, 2.0f)
    L(ctx, 8.0f, 11.0f, 8.0f, 7.5f); Arc(ctx, 11.5f, 7.5f, 4.0f, 180.0f, 300.0f)
}
def private ic_marker(var ctx : IconCtx) {
    L(ctx, 6.0f, 3.0f, 6.0f, 21.0f)
    TriF(ctx, 6.0f, 4.0f, 17.0f, 4.0f, 14.0f, 8.0f); TriF(ctx, 6.0f, 4.0f, 14.0f, 8.0f, 17.0f, 12.0f); TriF(ctx, 6.0f, 4.0f, 17.0f, 12.0f, 6.0f, 12.0f)
}
def private ic_snap(var ctx : IconCtx) {
    L(ctx, 6.0f, 4.0f, 6.0f, 10.0f); L(ctx, 18.0f, 4.0f, 18.0f, 10.0f)
    Arc(ctx, 12.0f, 10.0f, 6.0f, 0.0f, 180.0f)
    RectF(ctx, 5.5f, 4.0f, 5.0f, 3.5f, 0.0f); RectF(ctx, 13.5f, 4.0f, 5.0f, 3.5f, 0.0f)
}

// ===== Dispatch =====
def draw_icon(name : string; var ctx : IconCtx) {
    if (name == "play") { ic_play(ctx) }
    elif (name == "pause") { ic_pause(ctx) }
    elif (name == "stop") { ic_stop(ctx) }
    elif (name == "record") { ic_record(ctx) }
    elif (name == "loop") { ic_loop(ctx) }
    elif (name == "prev") { ic_prev(ctx) }
    elif (name == "next") { ic_next(ctx) }
    elif (name == "rewind") { ic_rewind(ctx) }
    elif (name == "forward") { ic_forward(ctx) }
    elif (name == "cut") { ic_cut(ctx) }
    elif (name == "add") { ic_add(ctx) }
    elif (name == "duplicate") { ic_duplicate(ctx) }
    elif (name == "delete") { ic_delete(ctx) }
    elif (name == "undo") { ic_undo(ctx) }
    elif (name == "redo") { ic_redo(ctx) }
    elif (name == "zoom-in") { ic_zoom_in(ctx) }
    elif (name == "zoom-out") { ic_zoom_out(ctx) }
    elif (name == "waveform") { ic_waveform(ctx) }
    elif (name == "spectrum") { ic_spectrum(ctx) }
    elif (name == "envelope") { ic_envelope(ctx) }
    elif (name == "filter") { ic_filter(ctx) }
    elif (name == "eq") { ic_eq(ctx) }
    elif (name == "grid") { ic_grid(ctx) }
    elif (name == "volume") { ic_volume(ctx) }
    elif (name == "mute") { ic_mute(ctx) }
    elif (name == "fader") { ic_fader(ctx) }
    elif (name == "sliders") { ic_sliders(ctx) }
    elif (name == "pan") { ic_pan(ctx) }
    elif (name == "metronome") { ic_metronome(ctx) }
    elif (name == "mic") { ic_mic(ctx) }
    elif (name == "headphones") { ic_headphones(ctx) }
    elif (name == "layers") { ic_layers(ctx) }
    elif (name == "folder") { ic_folder(ctx) }
    elif (name == "save") { ic_save(ctx) }
    elif (name == "export") { ic_export(ctx) }
    elif (name == "import") { ic_import(ctx) }
    elif (name == "visible") { ic_visible(ctx) }
    elif (name == "lock") { ic_lock(ctx) }
    elif (name == "unlock") { ic_unlock(ctx) }
    elif (name == "marker") { ic_marker(ctx) }
    elif (name == "snap") { ic_snap(ctx) }
}

// ===== Public API =====

def public icon(name : string; size : float = 20.0f; weight : float = 1.0f) {
    //! Draw a glyph at the cursor (no interaction) and advance the cursor.
    let p0 = GetCursorScreenPos()
    var ctx = IconCtx(dl = GetWindowDrawList(), org = p0, s = size,
        col = GetColorU32(amber), th = (2.0f / 24.0f) * size * weight)
    draw_icon(name, ctx)
    Dummy(float2(size, size))
}

def public icon_button(id : string; glyph : string; size : float = 20.0f; weight : float = 1.0f) : bool {
    //! Square icon button that matches the daslang theme: chrome reads ImGuiCol.Button*,
    //! glyph is the amber accent (Text when pressed). `id` is the unique ImGui id — use
    //! distinct ids for repeated glyphs (e.g. per list row); `glyph` is the icon name.
    let style & = GetStyle()
    let pad = style.FramePadding
    let full = float2(size + pad.x * 2.0f, size + pad.y * 2.0f)
    let p0 = GetCursorScreenPos()
    let pressed = InvisibleButton(id, full)
    let hovered = IsItemHovered()
    let active = IsItemActive()
    var dl = GetWindowDrawList()

    var bg = GetColorU32(ImGuiCol.Button)
    if (active) {
        bg = GetColorU32(ImGuiCol.ButtonActive)
    } elif (hovered) {
        bg = GetColorU32(ImGuiCol.ButtonHovered)
    }
    *dl |> AddRectFilled(p0, p0 + full, bg, style.FrameRounding, ImDrawFlags.None)

    var ctx = IconCtx(dl = dl, org = p0 + pad, s = size,
        col = active ? GetColorU32(ImGuiCol.Text) : GetColorU32(amber),
        th = (2.0f / 24.0f) * size * weight)
    draw_icon(glyph, ctx)
    return pressed
}

def public icon_button(name : string; size : float = 20.0f; weight : float = 1.0f) : bool {
    //! Convenience: id and glyph are both `name`. Use the (id, glyph) form for repeated icons.
    return icon_button(name, name, size, weight)
}
