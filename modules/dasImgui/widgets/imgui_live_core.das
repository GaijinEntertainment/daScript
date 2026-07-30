options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_live_core shared public

// Animated synthetic-IO timelines (mouse + keyboard playback over frames). The
// immediate synth primitives, the per-frame tick, and user-control live in
// imgui_boost_runtime; this module registers its timeline advance with boost's
// synth-frame hook. Re-exported by imgui_live.

require imgui public
require imgui/imgui_boost_runtime public   // synth primitives + tick + hook registration (live requires boost, never the reverse)
require daslib/json public
require daslib/json_boost public
require daslib/utf8_utils
require live/live_commands
require live_host
require math
require strings

// ---------------------------------------------------------------------------
// Mouse driver
// ---------------------------------------------------------------------------

enum private MEKind {
    move
    button
    scroll
}

struct private MouseEvent {
    t_ms   : int    = 0
    kind   : MEKind = MEKind.move
    x      : float  = 0.0
    y      : float  = 0.0
    button : int    = 0
    action : int    = 0    // 1=press, 0=release
}

var private mouse_timeline   : array<MouseEvent>
var private mouse_play_start : float = 0.0
var private mouse_play_idx   : int   = 0
var private mouse_playing    : bool  = false
// Cached waypoint for between-move interpolation. -1 = no prior move consumed.
var private last_move_x : float = 0.0
var private last_move_y : float = 0.0
var private last_move_t : int   = -1
// Frame-paced playback: advance steps one t_ms-group per FRAME (t_ms = frame index) instead of
// wall-clock, so a drag delivers a per-frame cursor delta at any frame rate. A wall-clock drag
// collapses press+move into one frame on a slow/stalled frame (CI), landing a zero-delta gesture. Mirrors key_frame_paced.
var private mouse_frame_paced : bool = false
var private mouse_frame_no    : int  = 0

def private reset_mouse_timeline() {
    mouse_timeline |> clear()
    mouse_play_idx = 0
    mouse_playing  = false
    mouse_frame_paced = false
    mouse_frame_no    = 0
    let cur = get_synth_cursor()
    last_move_x = cur._1
    last_move_y = cur._2
    last_move_t = -1
}

struct MouseMoveToArgs {
    x : float = 0.0
    y : float = 0.0
    @optional duration_ms : int = 250
}

[live_command(description="Animated cursor move to (x,y) over duration_ms via ImGui bypass. Linear interpolation.")]
def imgui_mouse_move_to(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<MouseMoveToArgs>)
    reset_mouse_timeline()
    let cur = get_synth_cursor()
    mouse_timeline |> push(MouseEvent(t_ms = 0, kind = MEKind.move, x = cur._1, y = cur._2))
    mouse_timeline |> push(MouseEvent(t_ms = max(1, args.duration_ms), kind = MEKind.move, x = args.x, y = args.y))
    mouse_play_start = get_uptime()
    mouse_playing    = true
    return JV((duration_ms = args.duration_ms))
}

struct MouseClickAtArgs {
    x : float = 0.0
    y : float = 0.0
    @optional button    : int = 0
    @optional travel_ms : int = 250
}

[live_command(description="Atomic click: travel to (x,y) then press+release `button` (0=left,1=right,2=middle) as one self-contained timeline via ImGui bypass — no separate move/click round-trips to interleave with real input. Args: x, y, button?, travel_ms?")]
def imgui_mouse_click_at(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<MouseClickAtArgs>)
    let travel = max(1, args.travel_ms)
    reset_mouse_timeline()
    mouse_timeline |> reserve(5)
    let cur = get_synth_cursor()
    mouse_timeline |> push(MouseEvent(t_ms = 0,            kind = MEKind.move,   x = cur._1,           y = cur._2))
    mouse_timeline |> push(MouseEvent(t_ms = travel,       kind = MEKind.move,   x = args.x,           y = args.y))
    mouse_timeline |> push(MouseEvent(t_ms = travel + 200, kind = MEKind.button, button = args.button, action = 1))
    mouse_timeline |> push(MouseEvent(t_ms = travel + 300, kind = MEKind.button, button = args.button, action = 0))
    mouse_timeline |> push(MouseEvent(t_ms = travel + 500, kind = MEKind.move,   x = args.x,           y = args.y))
    mouse_play_start = get_uptime()
    mouse_playing    = true
    return JV((x = args.x, y = args.y, button = args.button, queued = length(mouse_timeline)))
}

struct MouseEventWire {
    @optional t_ms   : int    = 0
    kind             : string = "move"
    @optional x      : float  = 0.0
    @optional y      : float  = 0.0
    @optional button : int    = 0
    @optional action : string = "press"
}

struct MousePlayArgs {
    events : array<MouseEventWire>
    // When true, t_ms is read as a FRAME index (one event-group per frame) not wall-clock ms —
    // mirrors imgui_key_play's frame_paced. Keeps a drag robust on slow/stalled frames: each frame
    // advances one step, so press and the following moves never collapse into a zero-delta drag.
    @optional frame_paced : bool = false
}

def public sort_mouse_play_events(var events : array<MouseEventWire>) {
    //! Sort wire-format mouse events ascending by ``t_ms`` in place — call before feeding ``imgui_mouse_play`` so the timeline drains in chronological order.
    events |> sort() $(a, b : MouseEventWire) {
        return a.t_ms < b.t_ms
    }
}

[live_command(description="Play a scripted mouse timeline via ImGui bypass. Args: events array of (t_ms, kind, x, y, button, action)")]
def imgui_mouse_play(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<MousePlayArgs>)
    return JV((error = "no events")) if (empty(args.events))
    sort_mouse_play_events(args.events)
    reset_mouse_timeline()
    mouse_frame_paced = args.frame_paced   // after reset (which clears it); frame-index pacing for drags
    mouse_timeline |> reserve(length(args.events))
    for (w in args.events) {
        var ev = MouseEvent(t_ms = w.t_ms)
        if (w.kind == "move") {
            ev.kind = MEKind.move
            ev.x = w.x
            ev.y = w.y
        } elif (w.kind == "button") {
            ev.kind = MEKind.button
            ev.button = w.button
            if (w.action == "press") {
                ev.action = 1
            } elif (w.action == "release") {
                ev.action = 0
            } else {
                return JV((error = "invalid button action; expected \"press\" or \"release\"", action = w.action))
            }
        } elif (w.kind == "scroll") {
            ev.kind = MEKind.scroll
            ev.x = w.x
            ev.y = w.y
        } else {
            return JV((error = "unknown event kind", kind = w.kind))
        }
        mouse_timeline |> push(ev)
    }
    mouse_play_start = get_uptime()
    mouse_playing    = true
    return JV((queued = length(mouse_timeline)))
}

[live_command(description="Stop synthetic mouse playback (ImGui bypass) and release any held buttons.")]
def imgui_mouse_stop(_input : JsonValue?) : JsonValue? {
    let was_playing = mouse_playing
    var released = 0
    for (b in range(0, 5)) {
        if ((synth_buttons_mask & (1 << b)) != 0) {
            released++
        }
    }
    release_held_buttons()
    reset_mouse_timeline()
    return JV((stopped = was_playing, released = released))
}

struct MouseStatusResult {
    playing      : bool
    elapsed_ms   : int
    cursor_x     : float
    cursor_y     : float
    queue_idx    : int
    queue_total  : int
    held_count   : int
    cursor_owned : bool
}

[live_command(description="ImGui-bypass mouse playback status.")]
def imgui_mouse_status(_input : JsonValue?) : JsonValue? {
    let elapsed = mouse_playing ? int((get_uptime() - mouse_play_start) * 1000.0) : 0
    let cur = get_synth_cursor()
    var held = 0
    for (b in range(0, 5)) {
        if ((synth_buttons_mask & (1 << b)) != 0) {
            held++
        }
    }
    return JV(MouseStatusResult(
        playing      = mouse_playing,
        elapsed_ms   = elapsed,
        cursor_x     = cur._1,
        cursor_y     = cur._2,
        queue_idx    = mouse_play_idx,
        queue_total  = length(mouse_timeline),
        held_count   = held,
        cursor_owned = cur._0
    ))
}

def private advance_mouse_timeline() {
    return if (!mouse_playing)
    // Frame-paced gestures step one t_ms-group per frame (others wall-clock); the lerp below interpolates by (now_ms - last_move_t)/span, so a frame-index now_ms advances one cursor delta per frame.
    var now_ms = 0
    if (mouse_frame_paced) {
        now_ms = mouse_frame_no
        mouse_frame_no++
    } else {
        now_ms = int((get_uptime() - mouse_play_start) * 1000.0)
    }
    while (mouse_play_idx < length(mouse_timeline)) {
        let ev = mouse_timeline[mouse_play_idx]
        break if (ev.t_ms > now_ms)
        if (ev.kind == MEKind.move) {
            synth_mouse_move(ev.x, ev.y)
            last_move_x = ev.x
            last_move_y = ev.y
            last_move_t = ev.t_ms
        } elif (ev.kind == MEKind.button) {
            synth_mouse_button(ev.button, ev.action)
        } elif (ev.kind == MEKind.scroll) {
            synth_mouse_wheel(ev.x, ev.y)
        }
        mouse_play_idx++
    }
    if (mouse_play_idx >= length(mouse_timeline)) {
        mouse_playing = false
        return
    }
    // Between waypoints: lerp from the last consumed move toward the next.
    if (last_move_t >= 0) {
        var next_move_idx = mouse_play_idx
        while (next_move_idx < length(mouse_timeline) && mouse_timeline[next_move_idx].kind != MEKind.move) {
            next_move_idx++
        }
        if (next_move_idx < length(mouse_timeline)) {
            let nm = mouse_timeline[next_move_idx]
            let span = nm.t_ms - last_move_t
            if (span > 0) {
                var alpha = float(now_ms - last_move_t) / float(span)
                if (alpha < 0.0) { alpha = 0.0; }
                if (alpha > 1.0) { alpha = 1.0; }
                let x = last_move_x + (nm.x - last_move_x) * alpha
                let y = last_move_y + (nm.y - last_move_y) * alpha
                synth_mouse_move(x, y)
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Keyboard driver
// ---------------------------------------------------------------------------

// State is keyed by ImGuiKey (int), NOT GLFW_KEY_*. ASCII translation tables
// resolve characters to ImGuiKey values directly — io.AddKeyEvent receives
// ImGuiKey, no GLFW indirection.

enum private KEKind {
    press
    release
    char_
}

struct private KeyEvent {
    t_ms      : int    = 0
    kind      : KEKind = KEKind.press
    key       : int    = 0    // ImGuiKey value (cast to int for storage)
    codepoint : uint   = 0u
}

var private key_timeline    : array<KeyEvent>
var private key_play_start  : float = 0.0
var private key_play_idx    : int   = 0
var private key_playing     : bool  = false
// Chords advance one event-group per FRAME (t_ms = frame index), not wall-clock, so a discrete tap
// is held exactly one frame and fires once at any frame rate. A wall-clock hold spans a variable
// frame count, which double-fired non-idempotent shortcuts (paste) on fast POSIX frames. Text keeps wall-clock.
var private key_frame_paced : bool  = false
var private key_frame_no    : int   = 0

def private reset_key_timeline() {
    key_timeline |> clear()
    key_play_idx    = 0
    key_playing     = false
    key_frame_paced = false
    key_frame_no    = 0
}

// --- ASCII translation tables (resolve to ImGuiKey values) ---

def private mod_name_to_key(name : string) : int {
    if (name == "Ctrl" || name == "Control") return int(ImGuiKey.LeftCtrl)
    if (name == "Shift") return int(ImGuiKey.LeftShift)
    if (name == "Alt") return int(ImGuiKey.LeftAlt)
    if (name == "Super" || name == "Cmd" || name == "Meta") return int(ImGuiKey.LeftSuper)
    return 0
}

def private needs_shift_ascii(c : int) : bool {
    if (c >= 'A' && c <= 'Z') return true   // nolint:PERF014
    return (c == '~' || c == '!' || c == '@' || c == '#'
         || c == '$' || c == '%' || c == '^' || c == '&'
         || c == '*' || c == '(' || c == ')' || c == '_'
         || c == '+' || c == '{' || c == '}' || c == '|'
         || c == ':' || c == '"' || c == '<' || c == '>'
         || c == '?')
}

def private base_key_ascii(c : int) : int {
    // ImGuiKey (int) for the base key producing `c` on US layout. `.0` reads as a float literal in daslang so digit enums don't parse — anchor the digit base on `ImGuiKey.A - 10`.
    let key_a = int(ImGuiKey.A)
    let key_0 = key_a - 10
    if (c >= 'A' && c <= 'Z') return key_a + (c - 'A')   // nolint:PERF014
    if (c >= 'a' && c <= 'z') return key_a + (c - 'a')   // nolint:PERF014
    if (c >= '0' && c <= '9') return key_0 + (c - '0')   // nolint:PERF014
    // shifted digit row: ! @ # $ % ^ & * ( ) → keys 1 2 3 4 5 6 7 8 9 0
    if (c == '!') return key_0 + 1
    if (c == '@') return key_0 + 2
    if (c == '#') return key_0 + 3
    if (c == '$') return key_0 + 4
    if (c == '%') return key_0 + 5
    if (c == '^') return key_0 + 6
    if (c == '&') return key_0 + 7
    if (c == '*') return key_0 + 8
    if (c == '(') return key_0 + 9
    if (c == ')') return key_0
    // punctuation
    if (c == ' ') return int(ImGuiKey.Space)
    if (c == '\n') return int(ImGuiKey.Enter)
    if (c == '\t') return int(ImGuiKey.Tab)
    if (c == ',' || c == '<') return int(ImGuiKey.Comma)
    if (c == '.' || c == '>') return int(ImGuiKey.Period)
    if (c == '/' || c == '?') return int(ImGuiKey.Slash)
    if (c == ';' || c == ':') return int(ImGuiKey.Semicolon)
    if (c == '\'' || c == '"') return int(ImGuiKey.Apostrophe)
    if (c == '-' || c == '_') return int(ImGuiKey.Minus)
    if (c == '=' || c == '+') return int(ImGuiKey.Equal)
    if (c == '[' || c == '{') return int(ImGuiKey.LeftBracket)
    if (c == ']' || c == '}') return int(ImGuiKey.RightBracket)
    if (c == '\\' || c == '|') return int(ImGuiKey.Backslash)
    if (c == '`' || c == '~') return int(ImGuiKey.GraveAccent)
    return 0
}

struct KeyTypeArgs {
    text                       : string = ""
    @optional total_duration_s : float  = 0.0
    @optional per_char_ms      : int    = 60
}

[live_command(description="Stream text as synthetic key+char events via ImGui bypass. Args: text, total_duration_s? | per_char_ms?")]
def imgui_key_type(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<KeyTypeArgs>)
    return JV((error = "empty text")) if (empty(args.text))
    let cps <- utf8_decode(args.text)
    let n_chars = length(cps)
    var per_char_ms = max(1, args.per_char_ms)
    if (args.total_duration_s > 0.0f) {
        per_char_ms = max(1, int(args.total_duration_s * 1000.0f) / max(1, n_chars))
    }
    reset_key_timeline()
    var t = 0
    key_timeline |> reserve(n_chars * 5)
    let shift_key = int(ImGuiKey.LeftShift)
    for (cp in cps) {
        if (cp < 0x80u) {
            let c     = int(cp)
            let k     = base_key_ascii(c)
            let shift = needs_shift_ascii(c)
            if (shift) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press, key = shift_key))
            }
            if (k != 0) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press, key = k))
            }
            // Real backends emit a Char event only for printable codepoints; control chars arrive as
            // a key alone (Enter -> '\n', Tab -> focus). Emitting the char too double-inserts in
            // InputTextMultiline (the Enter key AND the 0x0A char each add a newline).
            if (cp >= 0x20u) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.char_, codepoint = cp))
            }
            if (k != 0) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, key = k))
            }
            if (shift) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, key = shift_key))
            }
        } else {
            key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.char_, codepoint = cp))
        }
        t += per_char_ms
    }
    key_play_start = get_uptime()
    key_playing    = true
    return JV((queued = length(key_timeline), per_char_ms = per_char_ms, chars = n_chars))
}

struct KeyChordArgs {
    mods : array<string>
    key  : string = ""
}

[live_command(description="Synthetic chord via ImGui bypass. Args: mods (\"Ctrl\"|\"Shift\"|\"Alt\"|\"Super\"), key (single ASCII char)")]
def imgui_key_chord(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<KeyChordArgs>)
    return JV((error = "missing key")) if (empty(args.key))
    var target_key = 0
    if (length(args.key) == 1) {
        target_key = base_key_ascii(first_character(args.key))
    }
    return JV((error = "unsupported chord key", key = args.key)) if (target_key == 0)
    // Validate mod names up front.
    for (m in args.mods) {
        return JV((error = "unknown modifier", modifier = m)) if (mod_name_to_key(m) == 0)
    }
    reset_key_timeline()
    key_frame_paced = true
    key_timeline |> reserve(length(args.mods) + 2)
    // Frame-paced edges (t_ms = frame index): mods down (frame 0), target key down (frame 1), held
    // CHORD_HOLD_FRAMES, target up, mods up one frame later. Fixed frame count (not wall-clock) so a
    // fast/stalled frame can't stretch it into a repeat (wall-clock double-pasted on POSIX). synth_key_press auto-pairs the ImGuiMod_* alias for Shortcut() routing.
    let CHORD_HOLD_FRAMES = 2
    var t = 0
    for (m in args.mods) {
        key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press, key = mod_name_to_key(m)))
    }
    t++
    key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press, key = target_key))
    t += CHORD_HOLD_FRAMES
    key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, key = target_key))
    t++
    var i = length(args.mods) - 1
    while (i >= 0) {
        key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, key = mod_name_to_key(args.mods[i])))
        i--
    }
    key_play_start = get_uptime()
    key_playing    = true
    return JV((queued = length(key_timeline), frame_paced = true))
}

struct KeyEventWire {
    @optional t_ms      : int    = 0
    kind                : string = "press"   // "press" | "release" | "char"
    @optional key       : int    = 0
    @optional codepoint : uint   = 0u
}

struct KeyPlayArgs {
    events : array<KeyEventWire>
    // When true, t_ms is a FRAME index (one event-group per frame), not wall-clock ms — same pacing
    // imgui_key_chord uses so a discrete tap fires once at any frame rate (a wall-clock hold crosses
    // ImGui's key-repeat threshold on a slow frame). Use for non-ASCII taps (Tab/Enter/arrows) chord can't express.
    @optional frame_paced : bool = false
}

def public sort_key_play_events(var events : array<KeyEventWire>) {
    //! Sort wire-format key events ascending by ``t_ms`` in place — call before feeding ``imgui_key_play`` so the timeline drains in chronological order.
    events |> sort() $(a, b : KeyEventWire) {
        return a.t_ms < b.t_ms
    }
}

[live_command(description="Play a scripted key/char timeline via ImGui bypass. Args: events array of (t_ms, kind, key, codepoint); optional frame_paced (t_ms as frame index for repeat-proof discrete taps)")]
def imgui_key_play(input : JsonValue?) : JsonValue? {
    var args = from_JV(input, type<KeyPlayArgs>)
    return JV((error = "no events")) if (empty(args.events))
    sort_key_play_events(args.events)
    reset_key_timeline()
    key_frame_paced = args.frame_paced   // after reset (which clears it); frame-index pacing for discrete taps
    key_timeline |> reserve(length(args.events))
    for (w in args.events) {
        var ev = KeyEvent(t_ms = w.t_ms, key = w.key, codepoint = w.codepoint)
        if (w.kind == "press") {
            ev.kind = KEKind.press
        } elif (w.kind == "release") {
            ev.kind = KEKind.release
        } elif (w.kind == "char") {
            ev.kind = KEKind.char_
        } else {
            return JV((error = "unknown event kind", kind = w.kind))
        }
        key_timeline |> push(ev)
    }
    key_play_start = get_uptime()
    key_playing    = true
    return JV((queued = length(key_timeline)))
}

[live_command(description="Stop synthetic key playback (ImGui bypass) and release any held keys.")]
def imgui_key_stop(_input : JsonValue?) : JsonValue? {
    let was_playing = key_playing
    let released    = length(synth_held_keys)
    release_held_keys()
    reset_key_timeline()
    return JV((stopped = was_playing, released = released))
}

struct KeyStatusResult {
    playing        : bool  //! true while a recorded key timeline is being replayed.
    elapsed_ms     : int   //! milliseconds since timeline playback began (0 when idle).
    queue_idx      : int   //! index of the next pending event in the timeline.
    queue_total    : int   //! total number of events in the active timeline.
    held_count     : int   //! number of synthetic keys currently held down.
    last_keycode   : int   //! ImGuiKey value (renamed from "key" so visual_aids can read uniformly)
    last_codepoint : uint  //! most recent codepoint pushed via AddInputCharacter.
}

[live_command(description="ImGui-bypass keyboard playback status.")]
def imgui_key_status(_input : JsonValue?) : JsonValue? {
    let elapsed = key_playing ? int((get_uptime() - key_play_start) * 1000.0) : 0
    return JV(KeyStatusResult(
        playing        = key_playing,
        elapsed_ms     = elapsed,
        queue_idx      = key_play_idx,
        queue_total    = length(key_timeline),
        held_count     = length(synth_held_keys),
        last_keycode   = synth_last_keycode,
        last_codepoint = synth_last_codepoint
    ))
}

def private advance_key_timeline() {
    return if (!key_playing)
    // Frame-paced chords step one t_ms-group per frame; text playback uses wall-clock.
    var threshold = 0
    if (key_frame_paced) {
        threshold = key_frame_no
        key_frame_no++
    } else {
        threshold = int((get_uptime() - key_play_start) * 1000.0)
    }
    while (key_play_idx < length(key_timeline)) {
        let ev = key_timeline[key_play_idx]
        break if (ev.t_ms > threshold)
        if (ev.kind == KEKind.press) {
            synth_key_press(ev.key)
        } elif (ev.kind == KEKind.release) {
            synth_key_release(ev.key)
        } elif (ev.kind == KEKind.char_) {
            synth_char(ev.codepoint)
        }
        key_play_idx++
    }
    if (key_play_idx >= length(key_timeline)) {
        key_playing = false
    }
}

// ---------------------------------------------------------------------------
// Hook registration — plug the timeline playback into boost's per-frame tick.
// ---------------------------------------------------------------------------

// The immediate primitives + the tick + user-control live in imgui_boost_runtime.
// boost's imgui_synth_tick invokes our advance each frame and our stop on
// set_user_control(true). advance_*_timeline call boost's synth_* primitives.

def private advance_synth_timelines() {
    advance_mouse_timeline()
    advance_key_timeline()
}

def private stop_synth_timelines() {
    reset_mouse_timeline()
    reset_key_timeline()
}

[init]
def private register_timeline_hooks() {
    // [init] — register this module's per-frame timeline advance + stop-on-handback with boost's synth tick.
    register_synth_frame_hook("imgui_live_core", @@ advance_synth_timelines)
    register_synth_stop_hook("imgui_live_core", @@ stop_synth_timelines)
}
