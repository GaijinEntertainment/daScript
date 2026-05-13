options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_visual_aids shared public

require imgui public
require imgui/imgui_boost_runtime public
require live/live_commands
require live/glfw_live
require glfw/glfw_boost
require live_host
require daslib/json public
require daslib/json_boost public
require strings
require math

//! Visual-aids overlays for dasImgui.
//!
//! Three primitives, all painted on `ImGui::GetForegroundDrawList()` via an
//! `end_of_frame()` hook installed at module init:
//!
//!   - `highlight(target, frames, color)` — flash a colored rect around a
//!     widget's bbox for N frames.
//!   - `mouse_trail(enabled, fade, color)` — toggle a fading dot trail
//!     following the cursor.
//!   - `narrate(text, target, frames)` — callout box with optional
//!     connector line to a named widget.
//!
//! All three are also exposed as `[live_command] imgui_*` for remote drive
//! by daslang-live / playwright. `imgui_auto_highlight_on_command`
//! (default false) opts in to flashing every command target automatically.
//!
//! Tunables (colors, frame counts, padding, thickness, etc.) live as
//! `var public` module globals — set them at init time or whenever to
//! reskin the overlays. Per-call args still override on a per-flash basis.

def public rgba(r, g, b, a : int) : uint {
    //! Pack a color into dasIMGUI's IM_COL32 byte order. Used by callers
    //! that prefer named channels over hex literals when configuring the
    //! tunable globals below.
    return ((uint(a) << uint(IM_COL32_A_SHIFT))
          | (uint(r) << uint(IM_COL32_R_SHIFT))
          | (uint(g) << uint(IM_COL32_G_SHIFT))
          | (uint(b) << uint(IM_COL32_B_SHIFT)))
}

// ===== Tunable knobs (var public) =====
//
// All visual settings live here as module globals. Reassign any of these
// from your app's init (or at runtime) to reskin the overlays. Per-call
// args on `highlight()` / `mouse_trail()` / `narrate()` still take
// precedence on a per-call basis.

// Manual highlight() — appearance when user code (or imgui_highlight curl)
// asks for a flash explicitly.
var public highlight_color          : uint  = rgba(255, 255, 0, 255)   //! yellow
var public highlight_thickness      : float = 2.0f
var public highlight_padding        : float = 2.0f
var public highlight_rounding       : float = 2.0f
var public highlight_default_frames : int   = 60

// Auto-flash on remote command — when imgui_auto_highlight_on_command is
// true. Visually distinct from manual highlights so viewers can tell
// which actions were driven remotely vs. user-issued.
var public auto_highlight_color  : uint = rgba(0, 255, 128, 255)        //! bright green
var public auto_highlight_frames : int  = 180                            //! ~2s at 90fps

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
var public cursor_sprite_color              : uint  = rgba(255, 64, 64, 255)
var public cursor_sprite_outline_color      : uint  = rgba(255, 255, 255, 255)
var public cursor_sprite_radius             : float = 9.0f
var public cursor_sprite_outline_thickness  : float = 2.0f
var public cursor_sprite_inner_radius       : float = 2.5f

// Click flash — expanding ring drawn at the cursor when glfw_live's synth
// driver fires a button press. ImGui's hit-test logic uses io.MousePos which
// the ImGui_ImplGlfw per-frame poll overwrites with the real OS cursor on
// focused windows, so the widget's own hover/active visuals don't fire for
// synth clicks. This overlay is the recording's proof-of-click.
var public click_flash_enabled       : bool  = true
var public click_flash_color         : uint  = rgba(255, 255, 0, 255)   //! yellow
var public click_flash_start_radius  : float = 8.0f
var public click_flash_end_radius    : float = 36.0f
var public click_flash_thickness     : float = 2.5f
var public click_flash_duration_s    : float = 0.35f

// Narrate callout
var public narrate_bg_color       : uint  = rgba(32, 32, 32, 204)
var public narrate_fg_color       : uint  = rgba(255, 255, 255, 255)
var public narrate_border_color   : uint  = rgba(255, 204, 51, 255)     //! amber
var public narrate_border_thickness : float = 1.5f
var public narrate_pad_x          : float = 8.0f
var public narrate_pad_y          : float = 4.0f
var public narrate_corner_radius  : float = 4.0f
var public narrate_default_frames : int   = 180
var public narrate_connector_thickness : float = 1.5f

// Auto-highlight gate
var public imgui_auto_highlight_on_command : bool = false
    //! When true, every accepted command in imgui_boost_runtime
    //! (imgui_click / imgui_set / imgui_focus / etc.) auto-flashes the
    //! target with `auto_highlight_color` for `auto_highlight_frames` ticks.

// Keycap / modifier HUD — bottom-center keycap pops + modifier-strip below.
// Reads `synth_last_codepoint` / `synth_last_keycode` from glfw_live to detect
// each new synth key event and pop a new keycap. Modifier strip lights up
// directly from `IsKeyDown(ImGuiKey_*)` so it reacts to BOTH synth events (via
// the chain → ImGui_ImplGlfw) AND real hardware keys.
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
let g_keycap_trace_max = 6                                  //! most-recent N keycaps still on screen
let g_keycap_trace_fallback_lifetime_s = 1.0f               //! safety cap on a stuck trace

// Focus rectangle — halo around io.active_widget. Off by default; opt in via
// imgui_focus_rect for tutorial recordings. Useful when typing into a text
// input — without this halo there's nothing on screen tying the keycap HUD's
// output to the destination widget.
var public focus_rect_enabled   : bool  = false
var public focus_rect_color     : uint  = rgba(80, 180, 255, 220)            //! cyan
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
    target : string
    remaining_frames : int
}

var private g_highlights : array<HighlightSpec>
var private g_trail : array<TrailDot>
var private g_narrates : array<NarrateSpec>

struct private KeycapTrace {
    glyph    : string         //! rendered keycap text ("A", "Enter", "Tab", ...)
    t_pushed : float          //! get_uptime() when added; used for fade-out
}

var private g_keycap_traces  : array<KeycapTrace>
var private g_last_seen_codepoint_t : float = -1.0    //! mirror of synth_last_codepoint_t; only push a new trace when this advances
var private g_last_seen_keycode_t   : float = -1.0    //! mirror of synth_last_keycode_t; pushed only when no char event represents it

// ===== User API =====

def public highlight(target : string;
                     frames : int = highlight_default_frames;
                     color : uint = highlight_color) {
    //! Flash a colored rect around the named widget's bbox for `frames`
    //! end_of_frame ticks. `target` is a registry path (`"Setup/SAVE_BTN"`)
    //! or hex_id (`"0x4f3a2b1c"`). Defaults pull from the module globals
    //! `highlight_default_frames` / `highlight_color` — override per call
    //! when you want a one-off color.
    g_highlights |> push(HighlightSpec(target = target, remaining_frames = frames, color = color))
}

def public mouse_trail(enabled : bool;
                       fade : float = -1.0f;
                       color : uint = 0u) {
    //! Toggle the fading mouse-trail overlay. Defaults pull from `trail_fade`
    //! and `trail_color`; passing non-default values overrides the module
    //! globals for the new on-state. Disabling clears the trail buffer.
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
    //! Toggle the in-frame cursor sprite. Drawn at io.MousePos via
    //! ForegroundDrawList, so APNG recordings show a visible cursor pointer
    //! even when synthetic motion isn't warping the (un-recordable) OS cursor.
    cursor_sprite_enabled = enabled
}

def public apply_synth_io_override() {
    //! Re-assert synthetic input state into ImGui's IO between
    //! `ImGui_ImplGlfw_NewFrame()` and `NewFrame()`. Same call-site as
    //! `advance_coroutines()` in `imgui_boost_runtime.das:677-682`. Covers
    //! two ImGui_ImplGlfw race conditions where the impl's per-callback
    //! polling overrides our synth state:
    //!
    //! 1. **Mouse pos**: `ImGui_ImplGlfw_UpdateMouseData` polls
    //!    `glfwGetCursorPos` every frame and posts `AddMousePosEvent(real)`
    //!    AFTER our synth chain dispatch — real cursor wins last-write-wins
    //!    and widget hit-tests miss. Re-queue synth pos as the last event.
    //!
    //! 2. **Modifier keys** (Ctrl / Shift / Alt / Super): each call into
    //!    `ImGui_ImplGlfw_KeyCallback` runs `UpdateKeyModifiers` which polls
    //!    `glfwGetKey(LEFT_CONTROL/RIGHT_CONTROL/...)` and posts
    //!    `AddKeyEvent(ImGuiMod_*, false)` because the physical key isn't
    //!    held. This nukes our synth `ImGuiMod_Ctrl=true` immediately after
    //!    we set it — `key_chord({Ctrl,A})` then misses select-all because
    //!    `io.KeyCtrl` drains as false. Re-queue per-modifier `true` as the
    //!    last event, gated on the actual `synth_held_keys` state.
    let (owned, sx, sy) = get_synth_cursor()
    if (owned) {
        var io & = unsafe(GetIO())
        unsafe(io.AddMousePosEvent(sx, sy))
    }
    if (!empty(synth_held_keys)) {
        var io & = unsafe(GetIO())
        if (synth_held_keys |> key_exists(GLFW_KEY_LEFT_CONTROL)
                || synth_held_keys |> key_exists(GLFW_KEY_RIGHT_CONTROL)) {
            unsafe(io.AddKeyEvent(ImGuiKey.ImGuiMod_Ctrl, true))
        }
        if (synth_held_keys |> key_exists(GLFW_KEY_LEFT_SHIFT)
                || synth_held_keys |> key_exists(GLFW_KEY_RIGHT_SHIFT)) {
            unsafe(io.AddKeyEvent(ImGuiKey.ImGuiMod_Shift, true))
        }
        if (synth_held_keys |> key_exists(GLFW_KEY_LEFT_ALT)
                || synth_held_keys |> key_exists(GLFW_KEY_RIGHT_ALT)) {
            unsafe(io.AddKeyEvent(ImGuiKey.ImGuiMod_Alt, true))
        }
        if (synth_held_keys |> key_exists(GLFW_KEY_LEFT_SUPER)
                || synth_held_keys |> key_exists(GLFW_KEY_RIGHT_SUPER)) {
            unsafe(io.AddKeyEvent(ImGuiKey.ImGuiMod_Super, true))
        }
    }
}

def public narrate(text : string;
                   target : string = "";
                   frames : int = narrate_default_frames) {
    //! Pop a callout box with `text` on the foreground for N frames.
    //! Colors / padding / corner-radius come from the `narrate_*` globals.
    //! When `target` resolves to a widget bbox the box anchors next to it
    //! with a connector line; otherwise it floats top-left.
    g_narrates |> push(NarrateSpec(text = text, target = target, remaining_frames = frames))
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
    //! Returns the synthetic cursor pos when glfw_live's mouse driver is
    //! playing, else the real ImGui cursor. Decouples overlay rendering from
    //! io.MousePos, which the per-frame ImGui_ImplGlfw polling overwrites
    //! with the real OS cursor on focused windows.
    let (active, sx, sy) = get_synth_cursor()
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

def private paint_narrates {
    if (empty(g_narrates)) return
    var dl = GetForegroundDrawList()
    var i = length(g_narrates)
    while (i > 0) {
        i--
        var anchor = float2(40.0f, 40.0f)
        var have_target = false
        var widget_center = float2(0.0f, 0.0f)
        if (!empty(g_narrates[i].target)) {
            let r = widget_rect(g_narrates[i].target)
            if (r._0) {
                let bb = r._1
                anchor = float2(bb.z + 16.0f, bb.y)
                widget_center = float2((bb.x + bb.z) * 0.5f, (bb.y + bb.w) * 0.5f)
                have_target = true
            }
        }
        // Actual text size via the CalcTextSize shim (main.cpp:342) — picks up
        // FontGlobalScale and any active font, so the box fits the rendered text.
        let text_size = CalcTextSize(g_narrates[i].text, false, -1.0f)
        let box_end = float2(anchor.x + text_size.x + narrate_pad_x * 2.0f,
                             anchor.y + text_size.y + narrate_pad_y * 2.0f)
        *dl |> AddRectFilled(anchor, box_end, narrate_bg_color, narrate_corner_radius, ImDrawFlags.None)
        *dl |> AddRect(anchor, box_end, narrate_border_color, narrate_corner_radius, ImDrawFlags.None, narrate_border_thickness)
        *dl |> AddText(float2(anchor.x + narrate_pad_x, anchor.y + narrate_pad_y), narrate_fg_color, g_narrates[i].text)
        if (have_target) {
            let connector_start = float2(anchor.x, (anchor.y + box_end.y) * 0.5f)
            *dl |> AddLine(connector_start, widget_center, narrate_border_color, narrate_connector_thickness)
        }
        g_narrates[i].remaining_frames--
        if (g_narrates[i].remaining_frames <= 0) {
            g_narrates |> erase(i)
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

def private paint_click_flash {
    // Only fire on press, not release; skip if disabled or never clicked.
    return if (!click_flash_enabled || synth_click_t < 0.0 || synth_click_action != 1)
    let elapsed = float(get_uptime()) - synth_click_t
    return if (elapsed < 0.0 || elapsed > click_flash_duration_s)
    let alpha_f = (1.0f - elapsed / click_flash_duration_s) * 255.0f
    let radius = click_flash_start_radius + (click_flash_end_radius - click_flash_start_radius) * (elapsed / click_flash_duration_s)
    let color = (click_flash_color & 0x00FFFFFFu) | (uint(alpha_f) << 24u)
    var dl = GetForegroundDrawList()
    *dl |> AddCircle(effective_cursor_pos(), radius, color, 24, click_flash_thickness)
}

def private glyph_for_codepoint(cp : uint) : string {
    //! Map a Unicode codepoint to a 1-4 char keycap label. Special-cases
    //! whitespace + common control chars to their named glyphs ("Enter",
    //! "Space", "Tab", "Bksp", "Esc"); other printable ASCII passes through
    //! as the literal character. Non-ASCII falls back to "?" — the keycap
    //! is for the recording, not for rich i18n; viewers reading captions
    //! get the textual content elsewhere.
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

def private glyph_for_keycode(kc : int) : string {
    //! Map a GLFW keycode to a keycap label for keys that don't produce a
    //! char event (function keys, arrows, modifiers pressed alone). Only
    //! covers the bits we use in tutorials today — extend on demand.
    if (kc == GLFW_KEY_ENTER) return "Enter"
    if (kc == GLFW_KEY_TAB) return "Tab"
    if (kc == GLFW_KEY_BACKSPACE) return "Bksp"
    if (kc == GLFW_KEY_ESCAPE) return "Esc"
    if (kc == GLFW_KEY_SPACE) return "Space"
    if (kc == GLFW_KEY_LEFT) return "<-"
    if (kc == GLFW_KEY_RIGHT) return "->"
    if (kc == GLFW_KEY_UP) return "Up"
    if (kc == GLFW_KEY_DOWN) return "Down"
    if (kc == GLFW_KEY_LEFT_SHIFT || kc == GLFW_KEY_RIGHT_SHIFT) return "Shift"
    if (kc == GLFW_KEY_LEFT_CONTROL || kc == GLFW_KEY_RIGHT_CONTROL) return "Ctrl"
    if (kc == GLFW_KEY_LEFT_ALT || kc == GLFW_KEY_RIGHT_ALT) return "Alt"
    if (kc == GLFW_KEY_LEFT_SUPER || kc == GLFW_KEY_RIGHT_SUPER) return "Cmd"
    return ""    // unknown — caller skips the trace
}

def private paint_key_hud {
    return if (!key_hud_enabled)

    // 1) Pull new synth events into traces. Codepoint takes priority: a
    //    typed char already includes the visible glyph, so the keycap
    //    matches what the user sees in the text input. Modifier-only
    //    keystrokes (no char event) push via the keycode branch.
    let now = float(get_uptime())
    if (synth_last_codepoint_t > g_last_seen_codepoint_t && synth_last_codepoint != 0u) {
        g_keycap_traces |> push(KeycapTrace(glyph = glyph_for_codepoint(synth_last_codepoint), t_pushed = now))
        g_last_seen_codepoint_t = synth_last_codepoint_t
        // Mark this keycode as "represented by char"; if a key press at the
        // same instant fires next, don't double-push.
        g_last_seen_keycode_t = synth_last_keycode_t
    } elif (synth_last_keycode_t > g_last_seen_keycode_t && synth_last_keycode != 0) {
        let g = glyph_for_keycode(synth_last_keycode)
        if (!empty(g)) {
            g_keycap_traces |> push(KeycapTrace(glyph = g, t_pushed = now))
        }
        g_last_seen_keycode_t = synth_last_keycode_t
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

    // 3) Layout bottom-center. ImGui's main viewport gives the window size in
    //    pixels — use it for the anchor.
    let viewport = GetMainViewport()
    let vp_size = viewport.Size
    let vp_pos  = viewport.Pos
    let n = length(g_keycap_traces)
    var dl = GetForegroundDrawList()

    if (n > 0) {
        let row_w = float(n) * key_hud_keycap_w + float(max(0, n - 1)) * key_hud_keycap_spacing
        let row_y = vp_pos.y + vp_size.y - key_hud_offset_y_from_bottom - key_hud_keycap_h
        var x = vp_pos.x + (vp_size.x - row_w) * 0.5f
        for (j in range(n)) {
            let trace = g_keycap_traces[j]
            let elapsed = now - trace.t_pushed
            let fade = clamp(1.0f - elapsed / key_hud_keycap_fade_s, 0.0f, 1.0f)
            let bg     = (key_hud_keycap_color        & 0x00FFFFFFu) | (uint(float(key_hud_keycap_color        >> 24u) * fade) << 24u)
            let border = (key_hud_keycap_border_color & 0x00FFFFFFu) | (uint(float(key_hud_keycap_border_color >> 24u) * fade) << 24u)
            let text   = (key_hud_text_color          & 0x00FFFFFFu) | (uint(float(key_hud_text_color          >> 24u) * fade) << 24u)
            let p0 = float2(x, row_y)
            let p1 = float2(x + key_hud_keycap_w, row_y + key_hud_keycap_h)
            *dl |> AddRectFilled(p0, p1, bg, key_hud_keycap_rounding, ImDrawFlags.None)
            *dl |> AddRect(p0, p1, border, key_hud_keycap_rounding, ImDrawFlags.None, key_hud_keycap_border_thick)
            let label_size = CalcTextSize(trace.glyph, false, -1.0f)
            let tx = x + (key_hud_keycap_w - label_size.x) * 0.5f
            let ty = row_y + (key_hud_keycap_h - label_size.y) * 0.5f
            *dl |> AddText(float2(tx, ty), text, trace.glyph)
            x += key_hud_keycap_w + key_hud_keycap_spacing
        }
    }

    // 4) Modifier strip — pulls direct from ImGui's live key state so it
    //    works for synth (chain → impl_glfw → io) AND real keys.
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
    paint_narrates()
    paint_focus_rect()
    paint_cursor_sprite()
    paint_click_flash()
    paint_key_hud()
}

def private on_command(command : string; target : string) {
    //! Post-command hook: auto-flash the touched widget when
    //! `imgui_auto_highlight_on_command` is set. Uses the distinct
    //! `auto_highlight_color` / `auto_highlight_frames` knobs so viewers
    //! can tell command-driven flashes from manual `highlight()` ones.
    if (imgui_auto_highlight_on_command && !empty(target)) {
        highlight(target, auto_highlight_frames, auto_highlight_color)
    }
}

[init]
def visual_aids_init {
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
    let args = from_JV(input, type<MouseTrailArgs>)
    mouse_trail(args.enabled, args.fade, args.color)
    return JV((ok = true, enabled = args.enabled))
}

struct NarrateArgs {
    text : string
    @optional target : string = ""
    @optional frames : int = -1
}

[live_command(description="Show a callout box with text for N frames; optional target widget for connector line.")]
def imgui_narrate(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<NarrateArgs>)
    let frames = args.frames > 0 ? args.frames : narrate_default_frames
    narrate(args.text, args.target, frames)
    return JV((ok = true))
}

struct AutoHighlightArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle the imgui_auto_highlight_on_command flag — when on, every accepted command auto-flashes its target.")]
def imgui_auto_highlight(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<AutoHighlightArgs>)
    imgui_auto_highlight_on_command = args.enabled
    return JV((ok = true, enabled = args.enabled))
}

struct CursorSpriteArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle the in-frame cursor sprite. Enable before record_start so the recording shows a visible cursor pointer.")]
def imgui_cursor_sprite(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CursorSpriteArgs>)
    cursor_sprite(args.enabled)
    return JV((ok = true, enabled = args.enabled))
}

struct KeyHudArgs {
    @optional enabled         : bool = true
    @optional show_modifiers  : bool = true
}

[live_command(description="Toggle the keycap + modifier-strip HUD. Pops a keycap at bottom-center for every synth key event and lights up Ctrl/Shift/Alt/Cmd pills while held.")]
def imgui_key_hud(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<KeyHudArgs>)
    key_hud_enabled = args.enabled
    key_hud_show_modifiers = args.show_modifiers
    return JV((ok = true, enabled = args.enabled, show_modifiers = args.show_modifiers))
}

struct FocusRectArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle the focus rectangle around io.active_widget. Shows where typing lands; opt-in for tutorial recordings.")]
def imgui_focus_rect(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<FocusRectArgs>)
    focus_rect_enabled = args.enabled
    return JV((ok = true, enabled = args.enabled))
}
