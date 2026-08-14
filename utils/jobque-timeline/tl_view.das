options gen2
options indenting = 4

module tl_view shared

require tl_model
require tl_lod
require imgui/imgui_harness
require math

// Canvas port of the lane-timeline HTML viewer: one row per lane, events colored by
// category with per-stage shading, gray barrier waits, ink publish ticks, marker row,
// overview strip with a viewport indicator. Custom item via the sanctioned internal rail
// (ItemSize + ItemAdd + ButtonBehavior); all drawing through the v2 drawlist scope.
// Draw runs are CACHED per (view, width, generation) — steady-state frames replay them
// without touching events; dense views build from the mip buckets with run merging.

let GUT = 74.0            // lane-label gutter px
let OV_H = 26.0           // overview strip height
let GRID_H = 14.0         // time-label row height
let UN_H = 14.0           // units (marker) row height — own band, below the time labels
let HDR_H = OV_H + GRID_H + UN_H
let SURFACE = 0x14171Bu   // canvas background 0xRRGGBB

struct public ViewUI {
    t0, t1 : double        // visible window, µs
    inited : bool
    hover_lane : int = -1
    hover_idx : int = -1
    interacted : bool      // this frame — drives view sync in main
    sel_active : bool      // shift+drag time-range selection (per-view)
    sel_dragging : bool
    sel_anchor : double
    sel_t0, sel_t1 : double
}

// one cached rectangle; x in px relative to the plot origin (overview: normalized 0..1)
struct public DrawRun {
    x0, x1 : float
    lane : int16
    kind : int8            // 0 row rect, 1 slim wait bar, 2 publish tick, 3 wake inset
    col : uint             // final 0xRRGGBB (alpha applied at replay)
}

struct public RenderCache {
    t0, t1 : double
    plot_w : float
    generation : int = -1
    bucket_mode : bool     // what the last rebuild used (exposed for describe/tests)
    runs : array<DrawRun>
    ov_generation : int = -1
    ov_rows : int = 1      // lanes fold into <= 22 overview rows
    ov_runs : array<DrawRun>
}

def public col32(rgb : uint; a : int) : uint {
    return rgba(int((rgb >> 16u) & 255u), int((rgb >> 8u) & 255u), int(rgb & 255u), a)
}

def private blend_rgb(a, b : uint; k : float) : uint {
    let ar = float((a >> 16u) & 255u)
    let ag = float((a >> 8u) & 255u)
    let ab = float(a & 255u)
    let br = float((b >> 16u) & 255u)
    let bg = float((b >> 8u) & 255u)
    let bb = float(b & 255u)
    return (uint(ar + (br - ar) * k) << 16u) | (uint(ag + (bg - ag) * k) << 8u) | uint(ab + (bb - ab) * k)
}

// earlier pipeline stages lighter, final stage full hue (HTML: k = 0.38*(1 - s/(n-1)))
def private stage_color(base : uint; stage, nstages : int) : uint {
    if (nstages <= 1) return base
    let k = 0.38 * (1.0 - float(stage) / float(nstages - 1))
    return blend_rgb(base, SURFACE, k)
}

def public fmt_us(v : double) : string {
    if (v < 0.0lf) return "-" + fmt_us(-v)
    if (v >= 10000.0lf) return "{int(v / 1000.0lf)}ms"
    if (v >= 1000.0lf) return "{float(v) / 1000.0f}ms"
    return "{int(v)}us"
}

def public clamp_view(var ui : ViewUI; span : double) {
    // perfetto-style: the window may pan up to one full window past either end of the
    // data — so share-timer keeps absolute alignment across files with unequal spans
    // (the shorter file's content just runs off-screen) instead of pinning at its edge
    let w = max(ui.t1 - ui.t0, 8.0lf)
    ui.t0 = clamp(ui.t0, -w, span)
    ui.t1 = ui.t0 + w
}

def public view_full(var ui : ViewUI; doc : TraceDoc) {
    ui.t0 = 0.0lf
    ui.t1 = max(doc.span, 8.0lf)
    ui.inited = true
}

def private kind_name(kind : int) : string {
    if (kind == EV_PUBLISH) return "publish"
    if (kind == EV_CHUNK) return "chunk"
    if (kind == EV_STAGE_WAIT) return "stage wait"
    if (kind == EV_WAKE) return "wake"
    return "fifo job"
}

def private count_visible(doc : TraceDoc; t0, t1 : double) : int {
    var n = 0
    for (lane in doc.lanes) {
        n += lane_lower_bound(lane, t1) - lane_lower_bound(lane, t0 - lane.max_dur)
    }
    return n
}

def private rebuild_event_runs(doc : TraceDoc; var cache : RenderCache; px_per_us : double) {
    let vis_t0 = cache.t0
    let vis_t1 = cache.t1
    for (li in range(length(doc.lanes))) {
        let lane & = unsafe(doc.lanes[li])
        var i = lane_lower_bound(lane, vis_t0 - lane.max_dur)
        while (i < length(lane.ev)) {
            let e & = unsafe(lane.ev[i])
            if (e.t0 > vis_t1) {
                break
            }
            i++
            let te = e.t0 + double(e.dur)
            if (te < vis_t0) {
                continue
            }
            let x0 = max(float((e.t0 - vis_t0) * px_per_us), 0.0f)
            let kind = ev_kind(e.meta)
            if (kind == EV_CHUNK) {
                let x1 = max(x0 + 0.6f, float((te - vis_t0) * px_per_us))
                let cat & = unsafe(doc.cats[ev_cat(e.meta)])
                cache.runs |> push(DrawRun(x0 = x0, x1 = x1, lane = int16(li), kind = int8(0),
                    col = stage_color(cat.col, ev_stage(e.meta), ev_nstages(e.meta))))
            } elif (kind == EV_STAGE_WAIT) {
                let x1 = max(x0 + 0.6f, float((te - vis_t0) * px_per_us))
                cache.runs |> push(DrawRun(x0 = x0, x1 = x1, lane = int16(li), kind = int8(1), col = 0x3A3F46u))
            } elif (kind == EV_PUBLISH) {
                let x1 = max(x0 + 1.4f, float((te - vis_t0) * px_per_us))
                cache.runs |> push(DrawRun(x0 = x0, x1 = x1, lane = int16(li), kind = int8(2), col = 0xE8EAEDu))
            } elif (kind == EV_WAKE) {
                let x1 = max(x0 + 0.6f, float((te - vis_t0) * px_per_us))
                cache.runs |> push(DrawRun(x0 = x0, x1 = x1, lane = int16(li), kind = int8(3), col = 0xC39536u))
            } else {
                let x1 = max(x0 + 0.6f, float((te - vis_t0) * px_per_us))
                cache.runs |> push(DrawRun(x0 = x0, x1 = x1, lane = int16(li), kind = int8(0), col = 0x7A9AAEu))
            }
        }
    }
}

// quantized bucket color: dominant category blended toward surface by busy fraction.
// COARSE bands (4) — merge-friendliness beats gradient fidelity: adjacent noisy buckets
// must collapse into few runs or the replay cost eats the LOD win
def private bucket_col(doc : TraceDoc; dom : int; busy_q, wait_q : int) : uint {
    if (busy_q <= 0) {
        return wait_q > 0 ? 0x2C3138u : 0u   // 0 = skip
    }
    let base = dom >= 0 ? doc.cats[dom].col : 0xC39536u
    return blend_rgb(SURFACE, base, float(busy_q) / 3.0f)
}

def private rebuild_bucket_runs(doc : TraceDoc; var cache : RenderCache; span, px_per_us : double) {
    for (li in range(length(doc.lanes))) {
        let lane & = unsafe(doc.lanes[li])
        if (empty(lane.mips)) {
            continue
        }
        let lvl_i = pick_level(lane, span, px_per_us, 1.0lf)
        let lvl & = unsafe(lane.mips[lvl_i])
        let bw = span / double(lvl.nb)
        let b0 = clamp(int(cache.t0 / bw), 0, lvl.nb - 1)
        let b1 = clamp(int(cache.t1 / bw) + 1, 0, lvl.nb - 1)
        var run_col = 0u
        var run_x0 = 0.0f
        var run_x1 = 0.0f
        for (bi in range(b0, b1 + 1)) {
            let bk & = unsafe(lvl.b[bi])
            let busy_q = min(int(bk.busy * 4.0f), 3)
            let wait_q = bk.wait > 0.25f ? 1 : 0
            let col = bk.count == 0 ? 0u : bucket_col(doc, bk.dom, busy_q, wait_q)
            let x0 = max(float((double(bi) * bw - cache.t0) * px_per_us), 0.0f)
            let x1 = float((double(bi + 1) * bw - cache.t0) * px_per_us)
            if (col == run_col && run_col != 0u && x0 <= run_x1 + 0.01f) {
                run_x1 = x1   // extend the current run
                continue
            }
            if (run_col != 0u) {
                cache.runs |> push(DrawRun(x0 = run_x0, x1 = run_x1, lane = int16(li), kind = int8(0), col = run_col))
            }
            run_col = col
            run_x0 = x0
            run_x1 = x1
        }
        if (run_col != 0u) {
            cache.runs |> push(DrawRun(x0 = run_x0, x1 = run_x1, lane = int16(li), kind = int8(0), col = run_col))
        }
    }
}

def private rebuild_overview(doc : TraceDoc; var cache : RenderCache) {
    // lanes fold into at most OV_ROWS visual rows (the strip is ~22px tall) — per (row,
    // bucket) the busiest lane's bucket wins, THEN runs merge per row. Bounds replay cost
    // by rows, not lanes.
    delete cache.ov_runs
    let nlanes = length(doc.lanes)
    let rows = clamp(nlanes, 1, 22)
    cache.ov_rows = rows
    let nb = MIP_BASE
    var row_busy : array<float>
    var row_dom : array<int>
    row_busy |> resize(rows * nb)
    row_dom |> resize(rows * nb)
    for (li in range(nlanes)) {
        let lane & = unsafe(doc.lanes[li])
        if (empty(lane.mips)) {
            continue
        }
        let row = li * rows / nlanes
        let lvl & = unsafe(lane.mips[0])
        for (bi in range(nb)) {
            let bk & = unsafe(lvl.b[bi])
            if (bk.count != 0 && bk.busy > row_busy[row * nb + bi]) {
                row_busy[row * nb + bi] = bk.busy
                row_dom[row * nb + bi] = bk.dom
            }
        }
    }
    for (row in range(rows)) {
        var run_col = 0u
        var run_x0 = 0.0f
        var run_x1 = 0.0f
        for (bi in range(nb)) {
            let busy = row_busy[row * nb + bi]
            let busy_q = min(int(busy * 4.0f), 3)
            let col = busy <= 0.0f ? 0u : bucket_col(doc, row_dom[row * nb + bi], max(busy_q, 1), 0)
            let x0 = float(bi) / float(nb)
            let x1 = float(bi + 1) / float(nb)
            if (col == run_col && run_col != 0u) {
                run_x1 = x1
                continue
            }
            if (run_col != 0u) {
                cache.ov_runs |> push(DrawRun(x0 = run_x0, x1 = run_x1, lane = int16(row), kind = int8(0), col = run_col))
            }
            run_col = col
            run_x0 = x0
            run_x1 = x1
        }
        if (run_col != 0u) {
            cache.ov_runs |> push(DrawRun(x0 = run_x0, x1 = run_x1, lane = int16(row), kind = int8(0), col = run_col))
        }
    }
    delete row_busy
    delete row_dom
    cache.ov_generation = doc.generation
}

// Per-frame canvas geometry, shared by the input, pick and draw phases.
struct private Geom {
    p : float2             // canvas origin, screen px
    bb : float4            // canvas bounds
    plot_x0, plot_w : float
    lanes_y0, row_h : float
    nlanes : int
    span : double
    px_per_us : double
}

// Wheel zoom-at-cursor, shift+drag range selection, plain drag pan, overview scrub,
// double-click reset, Escape clear — the only phase that moves the view.
def private handle_input(doc : TraceDoc; var ui : ViewUI; g : Geom;
                         hovered, held : bool; mx, my : float; m_t : double) {
    let io & = unsafe(GetIO())
    if (hovered && io.MouseWheel != 0.0f) {
        let factor = exp(-io.MouseWheel * 0.16f)
        var nw = (ui.t1 - ui.t0) * double(factor)
        nw = clamp(nw, 8.0lf, g.span * 2.0lf)
        let frac = clamp(double(mx - g.plot_x0) / double(g.plot_w), 0.0lf, 1.0lf)
        ui.t0 = m_t - nw * frac
        ui.t1 = ui.t0 + nw
        clamp_view(ui, g.span)
        ui.interacted = true
    }
    if (hovered && io.KeyShift && IsMouseClicked(ImGuiMouseButton.Left) && my >= g.lanes_y0) {
        ui.sel_dragging = true
        ui.sel_active = true
        ui.sel_anchor = m_t
        ui.sel_t0 = m_t
        ui.sel_t1 = m_t
    }
    if (ui.sel_dragging) {
        if (held) {
            ui.sel_t0 = min(ui.sel_anchor, m_t)
            ui.sel_t1 = max(ui.sel_anchor, m_t)
        } else {
            ui.sel_dragging = false
        }
    } elif (held && io.MouseDelta.x != 0.0f && !io.KeyShift) {
        if (my >= g.lanes_y0) {
            let dt = double(io.MouseDelta.x) / g.px_per_us
            ui.t0 -= dt
            ui.t1 -= dt
        }
        clamp_view(ui, g.span)
        ui.interacted = true
    }
    if (held && my < g.p.y + OV_H) {
        // overview scrub: center the view at the clicked time
        let frac = clamp(double(mx - g.plot_x0) / double(g.plot_w), 0.0lf, 1.0lf)
        let c = g.span * frac
        let vw = ui.t1 - ui.t0
        ui.t0 = c - vw * 0.5lf
        ui.t1 = ui.t0 + vw
        clamp_view(ui, g.span)
        ui.interacted = true
    }
    if (hovered && IsMouseDoubleClicked(ImGuiMouseButton.Left)) {
        view_full(ui, doc)
        ui.interacted = true
    }
    if (hovered && IsKeyPressed(ImGuiKey.Escape)) {
        ui.sel_active = false
        ui.sel_dragging = false
    }
}

// Hover pick on the lane under the cursor; a chunk event wins over anything else it overlaps.
def private pick_hover(doc : TraceDoc; var ui : ViewUI; g : Geom; hovered : bool; my : float; m_t : double) {
    ui.hover_lane = -1
    ui.hover_idx = -1
    if (!hovered || my < g.lanes_y0) return
    let hl = int((my - g.lanes_y0) / g.row_h)
    if (hl < 0 || hl >= g.nlanes) return
    let tol = 4.0lf / g.px_per_us
    var best = -1
    var bestChunk = false
    let lane & = unsafe(doc.lanes[hl])
    var i = lane_lower_bound(lane, m_t - max(lane.max_dur, tol))
    while (i < length(lane.ev)) {
        let e & = unsafe(lane.ev[i])
        if (e.t0 > m_t + tol) {
            break
        }
        if (m_t >= e.t0 - tol && m_t <= e.t0 + max(double(e.dur), tol)) {
            let isChunk = ev_kind(e.meta) == EV_CHUNK
            if (best < 0 || (isChunk && !bestChunk)) {
                best = i
                bestChunk = isChunk
            }
        }
        i++
    }
    ui.hover_lane = hl
    ui.hover_idx = best
}

def private refresh_caches(doc : TraceDoc; ui : ViewUI; var cache : RenderCache; g : Geom) {
    if (cache.generation != doc.generation || cache.t0 != ui.t0 || cache.t1 != ui.t1 || cache.plot_w != g.plot_w) {
        delete cache.runs
        cache.t0 = ui.t0
        cache.t1 = ui.t1
        cache.plot_w = g.plot_w
        cache.generation = doc.generation
        cache.bucket_mode = count_visible(doc, ui.t0, ui.t1) > EVENT_PATH_MAX
        if (cache.bucket_mode) {
            rebuild_bucket_runs(doc, cache, g.span, g.px_per_us)
        } else {
            rebuild_event_runs(doc, cache, g.px_per_us)
        }
    }
    if (cache.ov_generation != doc.generation) {
        rebuild_overview(doc, cache)
    }
}

// overview strip (cached normalized runs, lane-folded rows) + viewport rect
def private draw_overview(var dl : ImDrawList?; cache : RenderCache; ui : ViewUI; g : Geom) {
    let ov_y0 = g.p.y + 2.0f
    let ov_y1 = g.p.y + OV_H - 2.0f
    let ov_row = (ov_y1 - ov_y0) / float(max(cache.ov_rows, 1))
    for (r in cache.ov_runs) {
        let oy0 = ov_y0 + float(int(r.lane)) * ov_row
        dl |> add_rect_filled(float2(g.plot_x0 + r.x0 * g.plot_w, oy0),
            float2(max(g.plot_x0 + r.x1 * g.plot_w, g.plot_x0 + r.x0 * g.plot_w + 0.5f), oy0 + ov_row),
            col32(r.col, 150))
    }
    let vx0 = clamp(g.plot_x0 + float(ui.t0 / g.span) * g.plot_w, g.plot_x0, g.plot_x0 + g.plot_w)
    let vx1 = clamp(g.plot_x0 + float(ui.t1 / g.span) * g.plot_w, g.plot_x0, g.plot_x0 + g.plot_w)
    dl |> add_rect_filled(float2(vx0, ov_y0), float2(max(vx1, vx0 + 2.0f), ov_y1), col32(0xE8EAEDu, 36))
    dl |> add_line(float2(vx0, ov_y0), float2(vx0, ov_y1), col32(0xE8EAEDu, 160), 1.0)
    dl |> add_line(float2(vx1, ov_y0), float2(vx1, ov_y1), col32(0xE8EAEDu, 160), 1.0)
}

// time grid: 1-2-5 ladder targeting ~90px
def private draw_time_grid(var dl : ImDrawList?; ui : ViewUI; g : Geom) {
    let target_us = 90.0lf / g.px_per_us
    var step = 1.0lf
    while (step < target_us) {
        if (step * 2.0lf >= target_us) {
            step *= 2.0lf
            break
        }
        if (step * 5.0lf >= target_us) {
            step *= 5.0lf
            break
        }
        step *= 10.0lf
    }
    var gt = floor(ui.t0 / step) * step
    while (gt <= ui.t1) {
        let gx = g.plot_x0 + float((gt - ui.t0) * g.px_per_us)
        if (gx >= g.plot_x0) {
            dl |> add_line(float2(gx, g.lanes_y0), float2(gx, g.bb.w), col32(0x2A2F36u, 255), 1.0)
            dl |> add_text(float2(gx + 3.0f, g.p.y + OV_H), col32(0x8A9099u, 255), fmt_us(gt))
        }
        gt += step
    }
}

// marker (unit) row — its own band below the time labels
def private draw_markers(var dl : ImDrawList?; doc : TraceDoc; ui : ViewUI; g : Geom) {
    let un_y = g.p.y + OV_H + GRID_H
    for (mk in doc.marker_kinds) {
        for (i in range(length(mk.ts))) {
            let t = mk.ts[i]
            if (t < ui.t0 || t > ui.t1) {
                continue
            }
            let x = g.plot_x0 + float((t - ui.t0) * g.px_per_us)
            dl |> add_line(float2(x, un_y), float2(x, g.bb.w), col32(0xE8EAEDu, 70), 1.0)
            dl |> add_text(float2(x + 2.0f, un_y), col32(0xC9CDD2u, 255), "{mk.name} {mk.args[i]}")
        }
    }
}

def private draw_lane_labels(var dl : ImDrawList?; doc : TraceDoc; g : Geom) {
    for (li in range(g.nlanes)) {
        let y0 = g.lanes_y0 + float(li) * g.row_h
        dl |> add_line(float2(g.p.x, y0), float2(g.bb.z, y0), col32(0x20242Au, 255), 1.0)
        if (g.row_h >= 9.0f) {
            dl |> add_text(float2(g.p.x + 4.0f, y0 + max((g.row_h - 14.0f) * 0.5f, 0.0f)), col32(0x8A9099u, 255), doc.lanes[li].name)
        }
    }
}

def private draw_runs(var dl : ImDrawList?; cache : RenderCache; g : Geom) {
    for (r in cache.runs) {
        let y0 = g.lanes_y0 + float(int(r.lane)) * g.row_h
        let y1 = y0 + g.row_h - 1.0f
        let rx0 = g.plot_x0 + r.x0
        let rx1 = g.plot_x0 + r.x1
        if (int(r.kind) == 1) {
            let cy = (y0 + y1) * 0.5f
            dl |> add_rect_filled(float2(rx0, cy - g.row_h * 0.16f), float2(rx1, cy + g.row_h * 0.16f), col32(r.col, 255))
        } elif (int(r.kind) == 3) {
            dl |> add_rect_filled(float2(rx0, y0 + g.row_h * 0.3f), float2(rx1, y1 - g.row_h * 0.3f), col32(r.col, 120))
        } else {
            dl |> add_rect_filled(float2(rx0, y0 + 1.0f), float2(rx1, y1), col32(r.col, 255))
        }
    }
}

def private draw_selection(var dl : ImDrawList?; ui : ViewUI; g : Geom) {
    if (!ui.sel_active) return
    let sx0 = g.plot_x0 + float((ui.sel_t0 - ui.t0) * g.px_per_us)
    let sx1 = g.plot_x0 + float((ui.sel_t1 - ui.t0) * g.px_per_us)
    dl |> add_rect_filled(float2(sx0, g.lanes_y0), float2(max(sx1, sx0 + 1.0f), g.bb.w), col32(0xE8EAEDu, 22))
    dl |> add_line(float2(sx0, g.lanes_y0), float2(sx0, g.bb.w), col32(0xE8EAEDu, 180), 1.0)
    dl |> add_line(float2(sx1, g.lanes_y0), float2(sx1, g.bb.w), col32(0xE8EAEDu, 180), 1.0)
}

def private draw_hover_outline(var dl : ImDrawList?; doc : TraceDoc; ui : ViewUI; g : Geom) {
    if (ui.hover_lane < 0 || ui.hover_idx < 0) return
    let e & = unsafe(doc.lanes[ui.hover_lane].ev[ui.hover_idx])
    let y0 = g.lanes_y0 + float(ui.hover_lane) * g.row_h
    let x0 = g.plot_x0 + float((e.t0 - ui.t0) * g.px_per_us)
    let x1 = max(x0 + 1.0f, g.plot_x0 + float((e.t0 + double(e.dur) - ui.t0) * g.px_per_us))
    dl |> add_rect(float2(x0 - 1.0f, y0), float2(x1 + 1.0f, y0 + g.row_h - 1.0f), col32(0xE8EAEDu, 255), 0.0, ImDrawFlags.None, 1.0)
}

def private draw_hover_tooltip(doc : TraceDoc; ui : ViewUI) {
    if (ui.hover_lane < 0 || ui.hover_idx < 0) return
    let e & = unsafe(doc.lanes[ui.hover_lane].ev[ui.hover_idx])
    let cat & = unsafe(doc.cats[ev_cat(e.meta)])
    tooltip(TL_TIP) {
        text("{kind_name(ev_kind(e.meta))} — {cat.name}")
        text("stage {ev_stage(e.meta)}/{ev_nstages(e.meta)}  chunk {e.arg}  chain {e.chain}")
        text("t {fmt_us(e.t0)}  dur {fmt_us(double(e.dur))}")
    }
}

//! Draw one file's timeline canvas filling the current window's remaining space.
//! Handles wheel zoom-at-cursor, drag pan, shift+drag selection, double-click reset,
//! overview scrub, hover pick. Replays cached draw runs when the view is unchanged.
def public draw_timeline(doc : TraceDoc; var ui : ViewUI; var cache : RenderCache; id : string) {
    if (!ui.inited) {
        view_full(ui, doc)
    }
    ui.interacted = false
    let avail = GetContentRegionAvail()
    let w = max(avail.x, 120.0f)
    let h = max(avail.y, 120.0f)
    let p = GetCursorScreenPos()
    let bb = float4(p.x, p.y, p.x + w, p.y + h)
    ItemSize(bb)
    if (!ItemAdd(bb, GetID(id))) {
        return
    }
    var hovered = false
    var held = false
    ButtonBehavior(bb, GetID(id), hovered, held, ImGuiButtonFlags.MouseButtonLeft)

    let nlanes = max(length(doc.lanes), 1)
    let plot_w = max(w - GUT, 10.0f)
    var g = Geom(p = p, bb = bb,
                 plot_x0 = p.x + GUT, plot_w = plot_w,
                 lanes_y0 = p.y + HDR_H,
                 row_h = max((h - HDR_H) / float(nlanes), 3.0f),
                 nlanes = nlanes,
                 span = max(doc.span, 8.0lf),
                 px_per_us = double(plot_w) / (ui.t1 - ui.t0))

    let io & = unsafe(GetIO())
    let mx = io.MousePos.x
    let my = io.MousePos.y
    let m_t = ui.t0 + double(mx - g.plot_x0) / g.px_per_us
    handle_input(doc, ui, g, hovered, held, mx, my, m_t)
    g.px_per_us = double(g.plot_w) / (ui.t1 - ui.t0)

    pick_hover(doc, ui, g, hovered, my, m_t)
    refresh_caches(doc, ui, cache, g)

    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(float2(bb.x, bb.y), float2(bb.z, bb.w), col32(SURFACE, 255))
        with_drawlist_clip_rect(dl, float2(bb.x, bb.y), float2(bb.z, bb.w), true) $ {
            draw_overview(dl, cache, ui, g)
            draw_time_grid(dl, ui, g)
            draw_markers(dl, doc, ui, g)
            draw_lane_labels(dl, doc, g)
            draw_runs(dl, cache, g)
            draw_selection(dl, ui, g)
            draw_hover_outline(dl, doc, ui, g)
        }
    }

    draw_hover_tooltip(doc, ui)
}
