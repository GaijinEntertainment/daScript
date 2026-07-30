options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_visual_aids shared public

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_live
require live/live_commands
require live_host
require daslib/json public
require daslib/json_boost public
require strings
require math

// Module overview lives in handmade/module-imgui_visual_aids.rst.

def public rgba(r, g, b, a : int) : uint {
    //! Pack R/G/B/A bytes into ImGui's IM_COL32 word order. Useful when configuring the tunable color globals by channel rather than hex literal.
    return ((uint(a) << uint(IM_COL32_A_SHIFT))
          | (uint(r) << uint(IM_COL32_R_SHIFT))
          | (uint(g) << uint(IM_COL32_G_SHIFT))
          | (uint(b) << uint(IM_COL32_B_SHIFT)))
}

// ===== Tunable knobs (var public) =====

// Reskin the overlays by reassigning these globals from app init or at runtime.
// Per-call args on highlight()/mouse_trail()/narrate() override them.

// Manual highlight() — appearance when user code (or imgui_highlight curl)
// asks for a flash explicitly.
var public highlight_color          : uint  = rgba(255, 255, 0, 255)   // yellow
var public highlight_thickness      : float = 2.0f
var public highlight_padding        : float = 2.0f
var public highlight_rounding       : float = 2.0f
var public highlight_default_frames : int   = 60

// Auto-flash on remote command — when imgui_auto_highlight_on_command is
// true. Visually distinct from manual highlights so viewers can tell
// which actions were driven remotely vs. user-issued.
var public auto_highlight_color  : uint = rgba(0, 255, 128, 255)        // bright green
var public auto_highlight_frames : int  = 180                            // ~2s at 90fps

// Mouse trail
var public trail_max_dots   : int   = 64
var public trail_color      : uint  = rgba(255, 255, 255, 255)
var public trail_dot_radius : float = 4.0f
var public trail_fade       : float = 0.96f
var public trail_enabled    : bool  = false

// Cursor sprite — drawn at io.MousePos inside ImGui's foreground drawlist, so
// it shows up in glReadPixels-based recordings (the OS hardware cursor never
// hits the GL back buffer). Off by default; enable during APNG recording.
var public cursor_sprite_enabled            : bool  = false
var public cursor_sprite_color              : uint  = rgba(255, 64, 64, 64)   // ~25% alpha — see clicks/widgets through the indicator
var public cursor_sprite_outline_color      : uint  = rgba(255, 255, 255, 96) // ~38% — ring stays trackable while semi-transparent
var public cursor_sprite_radius             : float = 9.0f
var public cursor_sprite_outline_thickness  : float = 2.0f
var public cursor_sprite_inner_radius       : float = 1.5f

// Click flash — expanding ring at the cursor when imgui_boost_runtime's synth
// driver fires a press. Proof-of-click for recordings when the widget's own
// hover/active visuals don't fire (backend poll fights our synth io.MousePos).
var public click_flash_enabled       : bool  = true
var public click_flash_color         : uint  = rgba(255, 255, 0, 255)   // yellow
var public click_flash_start_radius  : float = 8.0f
var public click_flash_end_radius    : float = 36.0f
var public click_flash_thickness     : float = 2.5f
var public click_flash_duration_s    : float = 0.35f

// Narrate callout — post-it-note look: vivid yellow body, dark text, soft shadow.
var public narrate_bg_color       : uint  = rgba(255, 236, 130, 240)    // post-it yellow #FFEC82
var public narrate_fg_color       : uint  = rgba(30, 25, 10, 255)       // near-black warm #1E190A
var public narrate_border_color   : uint  = rgba(200, 160, 30, 255)     // darker amber #C8A01E
var public narrate_border_thickness : float = 1.5f
var public narrate_pad_x          : float = 12.0f
var public narrate_pad_y          : float = 8.0f
var public narrate_corner_radius  : float = 2.0f
var public narrate_default_frames : int   = 180
var public narrate_connector_thickness : float = 1.5f
var public narrate_shadow_color   : uint  = rgba(0, 0, 0, 90)
var public narrate_shadow_offset  : float2 = float2(3.0f, 4.0f)
var public narrate_max_width      : float = 480.0f   //! pixel width at which the callout wraps onto multiple lines; box height grows to fit

// Auto-highlight gate
var public imgui_auto_highlight_on_command : bool = false
    //! When true, every accepted imgui_* command auto-flashes its target with ``auto_highlight_color`` for ``auto_highlight_frames`` ticks.

// Keycap / modifier HUD — bottom-center keycap pops + modifier strip below.
// Pops a keycap per new synth key event (synth_last_codepoint/keycode); the strip
// uses IsKeyDown(ImGuiKey_*) so it reacts to both synth AND real hardware keys.
var public key_hud_enabled              : bool   = false
var public key_hud_show_modifiers       : bool   = true
var public key_hud_offset_y_from_bottom : float  = 80.0f
var public key_hud_keycap_color         : uint   = rgba(40, 40, 40, 230)
var public key_hud_keycap_border_color  : uint   = rgba(255, 255, 255, 200)
var public key_hud_text_color           : uint   = rgba(255, 255, 255, 255)
var public key_hud_keycap_w             : float  = 44.0f
var public key_hud_keycap_h             : float  = 36.0f
var public key_hud_keycap_spacing       : float  = 6.0f
var public key_hud_keycap_rounding      : float  = 6.0f
var public key_hud_keycap_border_thick  : float  = 1.5f
var public key_hud_keycap_fade_s        : float  = 0.7f
var public key_hud_mod_strip_y_gap      : float  = 8.0f
var public key_hud_mod_pill_w           : float  = 56.0f
var public key_hud_mod_pill_h           : float  = 20.0f
var public key_hud_mod_pill_spacing     : float  = 6.0f
var public key_hud_mod_pill_rounding    : float  = 10.0f
var public key_hud_mod_active_color     : uint   = rgba(255, 200, 80, 255)
var public key_hud_mod_idle_color       : uint   = rgba(80, 80, 80, 180)
var public key_hud_mod_active_text      : uint   = rgba(20, 20, 20, 255)
var public key_hud_mod_idle_text        : uint   = rgba(220, 220, 220, 255)
let g_keycap_trace_max = 6                                  // most-recent N keycaps still on screen
let g_keycap_trace_fallback_lifetime_s = 1.0f               // safety cap on a stuck trace

// Focus rectangle — halo around io.active_widget. Off by default; opt in via
// imgui_focus_rect for tutorial recordings. Ties the keycap HUD's output to the
// destination widget when typing into a text input.
var public focus_rect_enabled   : bool  = false
var public focus_rect_color     : uint  = rgba(80, 180, 255, 220)            // cyan
var public focus_rect_thickness : float = 2.0f
var public focus_rect_padding   : float = 3.0f
var public focus_rect_rounding  : float = 3.0f

// ===== Internal state =====

struct private HighlightSpec {
    target : string
    remaining_frames : int
    color : uint
}

struct private TrailDot {
    pos : float2
    age_frames : int
}

struct private NarrateSpec {
    text : string
    target : string            // anchor: connector points here, box placed beside it
    avoid : array<string>      // extra targets to keep the box clear of (union with target)
    remaining_frames : int
    remaining_s : float        // wall-clock budget in seconds; < 0 → fall back to the remaining_frames tick countdown
}

var private g_highlights : array<HighlightSpec>
var private g_trail : array<TrailDot>
var private g_narrates : array<NarrateSpec>

struct private KeycapTrace {
    glyph    : string         // rendered keycap text ("A", "Enter", "Tab", ...)
    t_pushed : float          // get_uptime() when added; used for fade-out
}

var private g_keycap_traces  : array<KeycapTrace>
var private g_last_seen_codepoint_t : float = -1.0    // mirror of synth_last_codepoint_t; only push a new trace when this advances
var private g_last_seen_keycode_t   : float = -1.0    // mirror of synth_last_keycode_t; pushed only when no char event represents it

// ===== User API =====

def public highlight(target : string;
                     frames : int = highlight_default_frames;
                     color : uint = highlight_color) {
    //! Flash a colored rect around the named widget's bbox for ``frames`` ticks. ``target`` is a registry path or ``"0x<hex_id>"``.
    g_highlights |> push(HighlightSpec(target = target, remaining_frames = frames, color = color))
}

def public mouse_trail(enabled : bool;
                       fade : float = -1.0f;
                       color : uint = 0u) {
    //! Toggle the fading mouse-trail overlay; non-default ``fade`` / ``color`` override the module globals. Disabling clears the trail buffer.
    trail_enabled = enabled
    if (fade >= 0.0f) {
        trail_fade = fade
    }
    if (color != 0u) {
        trail_color = color
    }
    if (!enabled) {
        g_trail |> clear()
    }
}

def public cursor_sprite(enabled : bool) {
    //! Toggle the in-frame cursor sprite. Drawn on the ForegroundDrawList so APNG recordings show a visible pointer even when the OS hardware cursor isn't captured.
    cursor_sprite_enabled = enabled
}

def public apply_synth_io_override() {
    //! Per-frame synth IO for hand-rolled (non-harness) update loops — call between the backend's
    //! ``*_NewFrame()`` and ImGui's ``NewFrame()``. Drains the full synth pipeline (``advance_coroutines()``
    //! then ``imgui_synth_tick()``), mirroring the harness so coroutine synth clicks work without it.
    advance_coroutines()
    imgui_synth_tick()
}

def public narrate(text : string;
                   target : string = "";
                   frames : int = narrate_default_frames;
                   duration_s : float = -1.0f) {
    //! Pop a callout box with ``text``. Lingers ``frames`` render ticks, or ``duration_s > 0``
    //! wall-clock seconds instead (needed when recording render fps != capture fps). A ``target``
    //! that resolves to a widget bbox anchors the box beside it with a connector; else it floats top-left.
    g_narrates |> emplace(NarrateSpec(text = text, target = target,
                                      remaining_frames = frames, remaining_s = duration_s))
}

def public narrate(text : string;
                   targets : array<string>;
                   frames : int = narrate_default_frames;
                   duration_s : float = -1.0f) {
    //! Multi-target callout: the box points at ``targets[0]`` and is placed to HARD-avoid the union
    //! of all ``targets``' bboxes, so a step spanning sibling controls (e.g. a +/- pair) never covers
    //! a related element. Empty list floats top-left.
    var avoid <- [for (k in range(1, length(targets))); targets[k]]
    g_narrates |> emplace(NarrateSpec(text = text,
                                      target = empty(targets) ? "" : targets[0],
                                      avoid <- avoid,
                                      remaining_frames = frames, remaining_s = duration_s))
}

// ===== Painters =====

def private paint_highlights {
    if (empty(g_highlights)) return
    var dl = GetForegroundDrawList()
    var i = length(g_highlights)
    while (i > 0) {
        i--
        let r = widget_rect(g_highlights[i].target)
        if (r._0) {
            let bb = r._1
            let pad = highlight_padding
            *dl |> AddRect(
                float2(bb.x - pad, bb.y - pad),
                float2(bb.z + pad, bb.w + pad),
                g_highlights[i].color,
                highlight_rounding,
                ImDrawFlags.None,
                highlight_thickness
            )
        }
        g_highlights[i].remaining_frames--
        if (g_highlights[i].remaining_frames <= 0) {
            g_highlights |> erase(i)
        }
    }
}

def private effective_cursor_pos() : float2 {
    // Synth cursor when active, else real ImGui cursor — backend poll may overwrite io.MousePos.
    let (active, sx, sy) = imgui_boost_runtime::get_synth_cursor()
    return float2(sx, sy) if (active)
    return GetMousePos()
}

def private paint_trail {
    if (!trail_enabled) return
    var dl = GetForegroundDrawList()
    let mouse = effective_cursor_pos()
    g_trail |> push(TrailDot(pos = mouse, age_frames = 0))
    while (length(g_trail) > trail_max_dots) {
        g_trail |> erase(0)
    }
    let base_rgb = trail_color & 0x00FFFFFFu
    var i = length(g_trail)
    while (i > 0) {
        i--
        let frac = 1.0f - float(g_trail[i].age_frames) / float(trail_max_dots)
        if (frac <= 0.0f) {
            g_trail |> erase(i)
            continue
        }
        let alpha_f = clamp(frac * 255.0f * trail_fade, 0.0f, 255.0f)
        let color = base_rgb | (uint(alpha_f) << 24u)
        *dl |> AddCircleFilled(g_trail[i].pos, trail_dot_radius * frac + 1.0f, color, 12)
        g_trail[i].age_frames++
    }
}

def private rect_intersects(c : float2; box_size : float2; b : float4) : bool {
    //! Half-open box-on-box: touching edges count as non-overlapping.
    let bx1 = c.x + box_size.x
    let by1 = c.y + box_size.y
    return !(bx1 <= b.x || c.x >= b.z || by1 <= b.y || c.y >= b.w)
}

def private clamp_anchor(c : float2; box_size : float2; viewport : float2;
                         edge_margin : float; shadow : float2; hud_zone_h : float) : float2 {
    // Slide the top-left into [edge_margin, viewport - edge_margin - box - shadow]; bottom edge pulled up by hud_zone_h when the key-HUD is visible.
    var r = c
    let max_x = viewport.x - edge_margin - box_size.x - shadow.x
    let max_y = viewport.y - edge_margin - box_size.y - shadow.y - hud_zone_h
    if (r.x > max_x) {
        r.x = max_x
    }
    if (r.x < edge_margin) {
        r.x = edge_margin
    }
    if (r.y > max_y) {
        r.y = max_y
    }
    if (r.y < edge_margin) {
        r.y = edge_margin
    }
    return r
}

def private score_anchor(c : float2; box_size : float2;
                         target_bbox : float4; avoid_bboxes : array<float4>;
                         other_bboxes : array<float4>) : int {
    // +1000 for overlapping the anchor target or a hard-avoid member (box must never cover them); +1 per incidental widget, so it prefers the least-crowded gap.
    var s = 0
    if (rect_intersects(c, box_size, target_bbox)) {
        s += 1000
    }
    for (b in avoid_bboxes) {
        if (rect_intersects(c, box_size, b)) {
            s += 1000
        }
    }
    for (b in other_bboxes) {
        s++ if (rect_intersects(c, box_size, b))
    }
    return s
}

def public compute_narrate_anchor(
                                  has_target   : bool;
                                  widget_bbox  : float4;
                                  box_size     : float2;
                                  viewport     : float2;
                                  edge_margin  : float;
                                  gap          : float;
                                  shadow       : float2;
                                  hud_zone_h   : float;
                                  other_bboxes : array<float4>;
                                  avoid_bboxes : array<float4>
                                  ) : float2 {
    //! Pick a post-it anchor (top-left) by scoring 4 viewport-clamped candidates (right/left/below/above):
    //! ``score_anchor`` adds 1 per incidental ``other_bboxes`` overlap and 1000 per anchor-target or ``avoid_bboxes`` overlap.
    //! Lowest score wins (ties by order right>left>below>above); a banner too big to place returns a bottom-left failsafe.
    return float2(edge_margin, edge_margin) if (!has_target)

    let max_w = viewport.x - 2.0f * edge_margin - shadow.x
    let max_h = viewport.y - 2.0f * edge_margin - shadow.y - hud_zone_h
    let failsafe = float2(edge_margin,
                          viewport.y - edge_margin - box_size.y - shadow.y - hud_zone_h)
    return failsafe if (box_size.x > max_w || box_size.y > max_h)

    let bb = widget_bbox
    let cx = (bb.x + bb.z) * 0.5f
    let c_right = clamp_anchor(float2(bb.z + gap, bb.y),
                               box_size, viewport, edge_margin, shadow, hud_zone_h)
    let c_left  = clamp_anchor(float2(bb.x - gap - box_size.x, bb.y),
                               box_size, viewport, edge_margin, shadow, hud_zone_h)
    let c_below = clamp_anchor(float2(cx - box_size.x * 0.5f, bb.w + gap),
                               box_size, viewport, edge_margin, shadow, hud_zone_h)
    let c_above = clamp_anchor(float2(cx - box_size.x * 0.5f, bb.y - gap - box_size.y),
                               box_size, viewport, edge_margin, shadow, hud_zone_h)
    let s_right = score_anchor(c_right, box_size, widget_bbox, avoid_bboxes, other_bboxes)
    let s_left  = score_anchor(c_left,  box_size, widget_bbox, avoid_bboxes, other_bboxes)
    let s_below = score_anchor(c_below, box_size, widget_bbox, avoid_bboxes, other_bboxes)
    let s_above = score_anchor(c_above, box_size, widget_bbox, avoid_bboxes, other_bboxes)

    // Tiebreak by iteration order: right > left > below > above.
    var best_anchor = c_right
    var best_score = s_right
    if (s_left < best_score) {
        best_score = s_left
        best_anchor = c_left
    }
    if (s_below < best_score) {
        best_score = s_below
        best_anchor = c_below
    }
    if (s_above < best_score) {
        best_score = s_above
        best_anchor = c_above
    }
    return best_anchor
}

def public compute_connector_start(
                                   anchor        : float2;
                                   box_size      : float2;
                                   widget_center : float2
                                   ) : float2 {
    //! Closest point on the post-it's boundary to ``widget_center`` (clamp-onto-rect). It's the edge
    //! midpoint when ``widget_center`` is directly outside one side; for offset/corner cases it walks to
    //! the nearest corner so the arrow points naturally at the target.
    let rect_min = anchor
    let rect_max = float2(anchor.x + box_size.x, anchor.y + box_size.y)
    return float2(clamp(widget_center.x, rect_min.x, rect_max.x),
                  clamp(widget_center.y, rect_min.y, rect_max.y))
}

def public compute_connector_end(
                                 target_bbox   : float4;
                                 banner_center : float2
                                 ) : float2 {
    //! Mirror of ``compute_connector_start`` on the target side: closest point on the target's
    //! bbox boundary to ``banner_center``. Same clamp-onto-rect trick — terminates the line on the
    //! target's edge (or corner) instead of inside it.
    return float2(clamp(banner_center.x, target_bbox.x, target_bbox.z),
                  clamp(banner_center.y, target_bbox.y, target_bbox.w))
}

def private narrate_hud_zone_h : float {
    // Bottom screen reserve so narrates don't collide with the key_hud keycap row + modifier strip when the HUD is enabled.
    return 0.0f if (!key_hud_enabled)
    return key_hud_offset_y_from_bottom + key_hud_mod_pill_h + key_hud_mod_strip_y_gap
}

def private paint_narrates {
    if (empty(g_narrates)) return
    var dl = GetForegroundDrawList()
    let io & = unsafe(GetIO())
    let viewport = io.DisplaySize
    let edge_margin = 8.0f
    let gap = 16.0f
    let hud_zone_h = narrate_hud_zone_h()
    var i = length(g_narrates)
    while (i > 0) {
        i--
        // CalcTextSize shim (main.cpp:342) picks up FontGlobalScale/active font; wrap_width=narrate_max_width grows long text vertically instead of off-viewport.
        let text_size = CalcTextSize(g_narrates[i].text, false, narrate_max_width)
        let box_size = float2(text_size.x + narrate_pad_x * 2.0f,
                              text_size.y + narrate_pad_y * 2.0f)

        var have_target = false
        var widget_bbox = float4(0.0f, 0.0f, 0.0f, 0.0f)
        var widget_center = float2(0.0f, 0.0f)
        if (!empty(g_narrates[i].target)) {
            let r = widget_rect(g_narrates[i].target)
            if (r._0) {
                widget_bbox = r._1
                widget_center = float2((widget_bbox.x + widget_bbox.z) * 0.5f,
                                       (widget_bbox.y + widget_bbox.w) * 0.5f)
                have_target = true
            }
        }
        // Collect every registered widget bbox except the target's so the post-it doesn't land on a sibling (linear scan, ≤ 30 entries).
        var other_bboxes : array<float4>
        for (k, w in keys(imgui_boost_runtime::g_registry), values(imgui_boost_runtime::g_registry)) {
            continue if (have_target && k == g_narrates[i].target)
            other_bboxes |> push(w.bbox)
        }
        // Sibling targets are HARD-avoided (scored like the anchor). Plain loop, not a comprehension, so widget_rect(av) resolves once per target.
        var avoid_bboxes : array<float4>
        for (av in g_narrates[i].avoid) {
            let r = widget_rect(av)
            if (r._0) {
                avoid_bboxes |> push(r._1)
            }
        }
        let anchor = compute_narrate_anchor(have_target, widget_bbox, box_size,
                                            viewport, edge_margin, gap,
                                            narrate_shadow_offset, hud_zone_h,
                                            other_bboxes, avoid_bboxes)
        delete other_bboxes
        delete avoid_bboxes
        let box_end = float2(anchor.x + box_size.x, anchor.y + box_size.y)
        // Connector drawn first (post-it covers crossings); endpoints on the OUTSIDE edges of banner + target so the line never strikes through the target's label.
        if (have_target) {
            let banner_center = float2(anchor.x + box_size.x * 0.5f,
                                       anchor.y + box_size.y * 0.5f)
            let conn_start = compute_connector_start(anchor, box_size, widget_center)
            let conn_end = compute_connector_end(widget_bbox, banner_center)
            *dl |> AddLine(conn_start, conn_end, narrate_border_color, narrate_connector_thickness)
        }
        // Drop shadow first so the post-it sits on top of it.
        *dl |> AddRectFilled(anchor + narrate_shadow_offset, box_end + narrate_shadow_offset,
                             narrate_shadow_color, narrate_corner_radius, ImDrawFlags.None)
        *dl |> AddRectFilled(anchor, box_end, narrate_bg_color, narrate_corner_radius, ImDrawFlags.None)
        *dl |> AddRect(anchor, box_end, narrate_border_color, narrate_corner_radius, ImDrawFlags.None, narrate_border_thickness)
        // 8-arg AddText overload (…, wrap_width, cpu_fine_clip_rect=null) — passes wrap_width so rendered text matches the multi-line size computed above.
        *dl |> AddText(null, 0.0f,
                       float2(anchor.x + narrate_pad_x, anchor.y + narrate_pad_y),
                       narrate_fg_color, g_narrates[i].text, narrate_max_width, null)
        if (g_narrates[i].remaining_s >= 0.0f) {
            // Wall-clock countdown: sum io.DeltaTime for a true span — a tick budget drains at render fps and vanishes early when render fps != capture fps.
            g_narrates[i].remaining_s -= io.DeltaTime
            if (g_narrates[i].remaining_s <= 0.0f) {
                g_narrates |> erase(i)
            }
        } else {
            g_narrates[i].remaining_frames--
            if (g_narrates[i].remaining_frames <= 0) {
                g_narrates |> erase(i)
            }
        }
    }
}

def private paint_cursor_sprite {
    if (!cursor_sprite_enabled) return
    var dl = GetForegroundDrawList()
    let p = effective_cursor_pos()
    *dl |> AddCircleFilled(p, cursor_sprite_radius, cursor_sprite_color, 16)
    *dl |> AddCircle(p, cursor_sprite_radius, cursor_sprite_outline_color, 16, cursor_sprite_outline_thickness)
    *dl |> AddCircleFilled(p, cursor_sprite_inner_radius, cursor_sprite_outline_color, 12)
}

def private effective_synth_click() : tuple<int; float> {
    // (action, t) for last synth click — action: 1=press, 0=release.
    return (imgui_boost_runtime::synth_click_action, imgui_boost_runtime::synth_click_t)
}

def private paint_click_flash {
    // Anchor latched on press (release doesn't reset it) so the ring animates the full click_flash_duration_s even though press->release is ~1 frame.
    let t = effective_synth_click()._1
    return if (!click_flash_enabled || t < 0.0)
    let elapsed = get_uptime() - t
    return if (elapsed < 0.0 || elapsed > click_flash_duration_s)
    let alpha_f = (1.0f - elapsed / click_flash_duration_s) * 255.0f
    let radius = click_flash_start_radius + (click_flash_end_radius - click_flash_start_radius) * (elapsed / click_flash_duration_s)
    let color = (click_flash_color & 0x00FFFFFFu) | (uint(alpha_f) << 24u)
    var dl = GetForegroundDrawList()
    *dl |> AddCircle(effective_cursor_pos(), radius, color, 24, click_flash_thickness)
}

def private glyph_for_codepoint(cp : uint) : string {
    if (cp == uint('\n') || cp == uint('\r')) return "Enter"
    if (cp == uint('\t')) return "Tab"
    if (cp == uint(' ')) return "Space"
    if (cp == 8u) return "Bksp"
    if (cp == 27u) return "Esc"
    return "?" if (cp < 0x20u || cp >= 0x7Fu)
    return build_string() $(writer) {
        writer |> write_char(int(cp))
    }
}

def private glyph_for_imgui_key(kc : int) : string {
    if (kc == int(ImGuiKey.Enter)) return "Enter"
    if (kc == int(ImGuiKey.Tab)) return "Tab"
    if (kc == int(ImGuiKey.Backspace)) return "Bksp"
    if (kc == int(ImGuiKey.Escape)) return "Esc"
    if (kc == int(ImGuiKey.Space)) return "Space"
    if (kc == int(ImGuiKey.LeftArrow)) return "<-"
    if (kc == int(ImGuiKey.RightArrow)) return "->"
    if (kc == int(ImGuiKey.UpArrow)) return "Up"
    if (kc == int(ImGuiKey.DownArrow)) return "Down"
    if (kc == int(ImGuiKey.Delete)) return "Del"
    if (kc == int(ImGuiKey.Insert)) return "Ins"
    if (kc == int(ImGuiKey.Home)) return "Home"
    if (kc == int(ImGuiKey.End)) return "End"
    if (kc == int(ImGuiKey.PageUp)) return "PgUp"
    if (kc == int(ImGuiKey.PageDown)) return "PgDn"
    if (kc == int(ImGuiKey.LeftShift) || kc == int(ImGuiKey.RightShift)) return "Shift"
    if (kc == int(ImGuiKey.LeftCtrl)  || kc == int(ImGuiKey.RightCtrl)) return "Ctrl"
    if (kc == int(ImGuiKey.LeftAlt)   || kc == int(ImGuiKey.RightAlt)) return "Alt"
    if (kc == int(ImGuiKey.LeftSuper) || kc == int(ImGuiKey.RightSuper)) return "Cmd"
    // Letter keys arrive as raw keycodes from a chord (no codepoint), so map them here too — else Ctrl+D pops only the "Ctrl" keycap.
    if (kc >= int(ImGuiKey.A) && kc <= int(ImGuiKey.Z))
        return build_string() $(writer) { writer |> write_char('A' + (kc - int(ImGuiKey.A))) }
    return ""    // unknown — caller skips the trace
}

def private paint_key_hud {
    return if (!key_hud_enabled)

    // 1) Pull new synth events into traces. Codepoint takes priority (a typed char already includes the glyph); modifier-only keystrokes fall to the keycode branch.
    let now = get_uptime()
    let cp_t = imgui_boost_runtime::synth_last_codepoint_t
    let cp   = imgui_boost_runtime::synth_last_codepoint
    let kc_t = imgui_boost_runtime::synth_last_keycode_t
    let kc   = imgui_boost_runtime::synth_last_keycode
    if (cp_t > g_last_seen_codepoint_t && cp != 0u) {
        g_keycap_traces |> push(KeycapTrace(glyph = glyph_for_codepoint(cp), t_pushed = now))
        g_last_seen_codepoint_t = cp_t
        // Mark this keycode "represented by char" so a key press at the same instant doesn't double-push.
        g_last_seen_keycode_t = kc_t
    } elif (kc_t > g_last_seen_keycode_t && kc != 0) {
        let g = glyph_for_imgui_key(kc)
        if (!empty(g)) {
            g_keycap_traces |> push(KeycapTrace(glyph = g, t_pushed = now))
        }
        g_last_seen_keycode_t = kc_t
    }

    // 2) Drop traces older than fade duration.
    var i = length(g_keycap_traces)
    while (i > 0) {
        i--
        if (now - g_keycap_traces[i].t_pushed > key_hud_keycap_fade_s) {
            g_keycap_traces |> erase(i)
        }
    }
    // Cap to the most-recent N (keeps a runaway script from filling the row).
    while (length(g_keycap_traces) > g_keycap_trace_max) {
        g_keycap_traces |> erase(0)
    }

    // 3) Layout bottom-center. ImGui's main viewport gives the window size in pixels — use it for the anchor.
    let viewport = GetMainViewport()
    let vp_size = viewport.Size
    let vp_pos  = viewport.Pos
    let n = length(g_keycap_traces)
    var dl = GetForegroundDrawList()

    if (n > 0) {
        // Pre-measure each glyph and grow the per-cap width past the default so "Space"/"Enter"/"Shift" don't overflow the keycap box.
        var cap_widths : array<float>
        cap_widths |> resize(n)
        var row_w = 0.0f
        for (j in range(n)) {
            let label_size = CalcTextSize(g_keycap_traces[j].glyph, false, -1.0f)
            let w = max(key_hud_keycap_w, label_size.x + key_hud_keycap_w * 0.4f)
            cap_widths[j] = w
            row_w += w
            if (j > 0) {
                row_w += key_hud_keycap_spacing
            }
        }
        let row_y = vp_pos.y + vp_size.y - key_hud_offset_y_from_bottom - key_hud_keycap_h
        var x = vp_pos.x + (vp_size.x - row_w) * 0.5f
        for (j in range(n)) {
            let trace = g_keycap_traces[j]
            let elapsed = now - trace.t_pushed
            let fade = clamp(1.0f - elapsed / key_hud_keycap_fade_s, 0.0f, 1.0f)
            let bg     = (key_hud_keycap_color        & 0x00FFFFFFu) | (uint(float(key_hud_keycap_color        >> 24u) * fade) << 24u)
            let border = (key_hud_keycap_border_color & 0x00FFFFFFu) | (uint(float(key_hud_keycap_border_color >> 24u) * fade) << 24u)
            let text   = (key_hud_text_color          & 0x00FFFFFFu) | (uint(float(key_hud_text_color          >> 24u) * fade) << 24u)
            let cap_w = cap_widths[j]
            let p0 = float2(x, row_y)
            let p1 = float2(x + cap_w, row_y + key_hud_keycap_h)
            *dl |> AddRectFilled(p0, p1, bg, key_hud_keycap_rounding, ImDrawFlags.None)
            *dl |> AddRect(p0, p1, border, key_hud_keycap_rounding, ImDrawFlags.None, key_hud_keycap_border_thick)
            let label_size = CalcTextSize(trace.glyph, false, -1.0f)
            let tx = x + (cap_w - label_size.x) * 0.5f
            let ty = row_y + (key_hud_keycap_h - label_size.y) * 0.5f
            *dl |> AddText(float2(tx, ty), text, trace.glyph)
            x += cap_w + key_hud_keycap_spacing
        }
    }

    // 4) Modifier strip — pulls direct from ImGui's live key state so it works for both synth events and real hardware keys.
    if (key_hud_show_modifiers) {
        let names = ["Ctrl", "Shift", "Alt", "Cmd"]
        let active = [
            IsKeyDown(ImGuiKey.LeftCtrl)  || IsKeyDown(ImGuiKey.RightCtrl),
            IsKeyDown(ImGuiKey.LeftShift) || IsKeyDown(ImGuiKey.RightShift),
            IsKeyDown(ImGuiKey.LeftAlt)   || IsKeyDown(ImGuiKey.RightAlt),
            IsKeyDown(ImGuiKey.LeftSuper) || IsKeyDown(ImGuiKey.RightSuper)
        ]
        let strip_n = 4
        let strip_w = float(strip_n) * key_hud_mod_pill_w + float(strip_n - 1) * key_hud_mod_pill_spacing
        let strip_y = vp_pos.y + vp_size.y - key_hud_offset_y_from_bottom + key_hud_mod_strip_y_gap
        var sx = vp_pos.x + (vp_size.x - strip_w) * 0.5f
        for (k in range(strip_n)) {
            let bg   = active[k] ? key_hud_mod_active_color : key_hud_mod_idle_color
            let text = active[k] ? key_hud_mod_active_text  : key_hud_mod_idle_text
            let p0 = float2(sx, strip_y)
            let p1 = float2(sx + key_hud_mod_pill_w, strip_y + key_hud_mod_pill_h)
            *dl |> AddRectFilled(p0, p1, bg, key_hud_mod_pill_rounding, ImDrawFlags.None)
            let label_size = CalcTextSize(names[k], false, -1.0f)
            let tx = sx + (key_hud_mod_pill_w - label_size.x) * 0.5f
            let ty = strip_y + (key_hud_mod_pill_h - label_size.y) * 0.5f
            *dl |> AddText(float2(tx, ty), text, names[k])
            sx += key_hud_mod_pill_w + key_hud_mod_pill_spacing
        }
    }
}

def private paint_focus_rect {
    return if (!focus_rect_enabled)
    let path = active_widget_path()
    return if (empty(path))
    let r = widget_rect(path)
    return if (!r._0)
    let bb = r._1
    let pad = focus_rect_padding
    var dl = GetForegroundDrawList()
    *dl |> AddRect(
        float2(bb.x - pad, bb.y - pad),
        float2(bb.z + pad, bb.w + pad),
        focus_rect_color,
        focus_rect_rounding,
        ImDrawFlags.None,
        focus_rect_thickness
    )
}

def private paint_all {
    paint_highlights()
    paint_trail()
    // Paint the focus rect before the narrative bar so the later-drawn caption stays readable over the halo, not under it.
    paint_focus_rect()
    paint_narrates()
    paint_cursor_sprite()
    paint_click_flash()
    paint_key_hud()
}

def private on_command(_command : string; target : string) {
    if (imgui_auto_highlight_on_command && !empty(target)) {
        highlight(target, auto_highlight_frames, auto_highlight_color)
    }
}

[init]
def visual_aids_init {
    //! ``[init]`` — registers the foreground-drawlist paint hook and the post-command auto-highlight hook. Runs once at module load.
    register_end_of_frame_hook("imgui_visual_aids", @@ paint_all)
    register_post_command_hook("imgui_visual_aids", @@ on_command)
}

// ===== Live commands =====

struct HighlightArgs {
    target : string
    @optional frames : int = -1
    @optional color : uint = 0u
}

[live_command(description="Flash a colored rect around the named widget's bbox for N frames.")]
def imgui_highlight(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` wrapper around :ref:`highlight <function-imgui_visual_aids_highlight_string_int_uint>`. Args: ``target``, ``frames``, ``color``.
    let args = from_JV(input, type<HighlightArgs>)
    let frames = args.frames > 0 ? args.frames : highlight_default_frames
    let color = args.color != 0u ? args.color : highlight_color
    highlight(args.target, frames, color)
    return JV((ok = true, target = args.target, frames = frames))
}

struct MouseTrailArgs {
    @optional enabled : bool = true
    @optional fade : float = -1.0f
    @optional color : uint = 0u
}

[live_command(description="Toggle the fading mouse-trail overlay. Args: enabled, fade, color.")]
def imgui_mouse_trail(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` wrapper around :ref:`mouse_trail <function-imgui_visual_aids_mouse_trail_bool_float_uint>`. Args: ``enabled``, ``fade``, ``color``.
    let args = from_JV(input, type<MouseTrailArgs>)
    mouse_trail(args.enabled, args.fade, args.color)
    return JV((ok = true, enabled = args.enabled))
}

struct NarrateArgs {
    text : string
    @optional target : string = ""
    @optional targets : array<string>
    @optional frames : int = -1
    @optional duration_s : float = -1.0f
}

[live_command(description="Show a callout box with text; lingers N frames, or duration_s wall-clock seconds if given. Optional target widget for connector line.")]
def imgui_narrate(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` wrapper around :ref:`narrate <function-imgui_visual_aids_narrate_string_string_int_float>`.
    //! Args: ``text``, ``target`` (single anchor) OR ``targets`` (list — first is the anchor, all
    //! hard-avoided), ``frames``, ``duration_s`` (wall-clock seconds; wins over ``frames``).
    let args = from_JV(input, type<NarrateArgs>)
    let frames = args.frames > 0 ? args.frames : narrate_default_frames
    if (!empty(args.targets)) {
        narrate(args.text, args.targets, frames, args.duration_s)
    } else {
        narrate(args.text, args.target, frames, args.duration_s)
    }
    return JV((ok = true))
}

struct AutoHighlightArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle the imgui_auto_highlight_on_command flag — when on, every accepted command auto-flashes its target.")]
def imgui_auto_highlight(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` toggle for the ``imgui_auto_highlight_on_command`` flag. Arg: ``enabled``.
    let args = from_JV(input, type<AutoHighlightArgs>)
    imgui_auto_highlight_on_command = args.enabled
    return JV((ok = true, enabled = args.enabled))
}

struct CursorSpriteArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle the in-frame cursor sprite. Enable before record_start so the recording shows a visible cursor pointer.")]
def imgui_cursor_sprite(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` wrapper around :ref:`cursor_sprite <function-imgui_visual_aids_cursor_sprite_bool>`. Arg: ``enabled``.
    let args = from_JV(input, type<CursorSpriteArgs>)
    cursor_sprite(args.enabled)
    return JV((ok = true, enabled = args.enabled))
}

struct KeyHudArgs {
    @optional enabled         : bool  = true
    @optional show_modifiers  : bool  = true
    @optional keycap_fade_s   : float = 0.7f   // how long each keycap lingers before fading out
}

[live_command(description="Toggle the keycap + modifier-strip HUD. Pops a keycap at bottom-center for every synth key event and lights up Ctrl/Shift/Alt/Cmd pills while held. keycap_fade_s sets how long each keycap lingers (default 0.7).")]
def imgui_key_hud(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` toggle for the bottom-center keycap HUD + Ctrl/Shift/Alt/Cmd modifier strip. Args: ``enabled``, ``show_modifiers``, ``keycap_fade_s`` (seconds a keycap lingers - bump it so a chord stays readable while a voice line names it).
    let args = from_JV(input, type<KeyHudArgs>)
    key_hud_enabled = args.enabled
    key_hud_show_modifiers = args.show_modifiers
    key_hud_keycap_fade_s = args.keycap_fade_s
    return JV((ok = true, enabled = args.enabled, show_modifiers = args.show_modifiers, keycap_fade_s = args.keycap_fade_s))
}

struct FocusRectArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle the focus rectangle around io.active_widget. Shows where typing lands; opt-in for tutorial recordings.")]
def imgui_focus_rect(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` toggle for the focus rectangle around ``io.active_widget`` — shows where typing lands. Arg: ``enabled``.
    let args = from_JV(input, type<FocusRectArgs>)
    focus_rect_enabled = args.enabled
    return JV((ok = true, enabled = args.enabled))
}

struct MemDebugArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle per-frame heap/string-heap GC-pressure logging. Enabling resets the baseline; a periodic line prints while on and a final delta line at teardown. Arg: enabled.")]
def imgui_memory_debug(input : JsonValue?) : JsonValue? {
    //! ``[live_command]`` toggle for :ref:`imgui_debug_memory <function-imgui_boost_runtime_imgui_debug_memory_bool>` — per-frame GC-pressure logging. Arg: ``enabled``.
    let args = from_JV(input, type<MemDebugArgs>)
    imgui_boost_runtime::imgui_debug_memory(args.enabled)
    return JV((ok = true, enabled = args.enabled))
}
