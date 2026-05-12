options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module glfw_live shared public

//! GLFW window lifecycle for daslang-live.
//!
//! Provides window creation, frame management, and automatic window
//! preservation across live reloads. The GLFW window pointer is stored
//! in the persistent byte store so it survives recompilation.
//!
//! Usage in your script:
//!   require live/glfw_live
//!   [export] def init() { live_create_window("My App", 1280, 720) }
//!   [export] def update() { if (live_begin_frame()) { /* render */ ; live_end_frame() } }
//!   [export] def shutdown() { live_destroy_window() }

require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_cache
require daslib/archive
require daslib/json
require daslib/json_boost
require daslib/utf8_utils
require live/live_commands
require live_host
require math
require strings

var public live_window : GLFWwindow?

def public live_create_window(title : string; width, height : int) : GLFWwindow? {
    //! Creates a GLFW window (or reuses the preserved one on reload).
    //! Call this from init().
    // Already have a window (restored from reload)
    return live_window if (live_window != null)
    if (glfwInit() == 0) {
        panic("glfw_live: can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    glfwWindowHint(int(GLFW_SAMPLES), 4)
    live_window = glfwCreateWindow(width, height, title, null, null)
    if (live_window == null) {
        panic("glfw_live: can't create window")
    }
    glfwMakeContextCurrent(live_window)
    glEnable(GL_MULTISAMPLE)
    init_opengl_cache()
    return live_window
}

def public live_destroy_window() {
    //! Destroys the GLFW window. Call this from shutdown() (but NOT during reload —
    //! the [before_reload] handler preserves the window automatically).
    if (live_window != null && !is_reload()) {
        glfwDestroyWindow(live_window)
        live_window = null
        glfwTerminate()
    }
}

def public live_begin_frame() : bool {
    //! Polls GLFW events and checks if the window should close.
    //! Returns false if the window was closed (signals exit).
    return false if (live_window == null)
    glfwPollEvents()
    if (glfwWindowShouldClose(live_window) != 0) {
        request_exit()
        return false
    }
    return true
}

def public live_end_frame() {
    //! Swaps buffers. Call after rendering.
    if (live_window != null) {
        glfwSwapBuffers(live_window)
    }
}

def public live_get_framebuffer_size(var width, height : int&) {
    //! Gets the current framebuffer size for glViewport.
    if (live_window != null) {
        glfwGetFramebufferSize(live_window, width, height)
    } else {
        width = 0
        height = 0
    }
}

// --- Window preservation across reloads ---

[before_reload]
def save_glfw_window() {
    if (live_window != null) {
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var ptr_val = unsafe(reinterpret<uint64>(live_window))
        arch |> serialize(ptr_val)
        let data <- ser->extractData()
        live_store_bytes("__glfw_window", data)
        unsafe { delete ser; }
    }
}

[after_reload]
def restore_glfw_window() {
    var data : array<uint8>
    if (live_load_bytes("__glfw_window", data) && !empty(data)) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        var ptr_val : uint64
        arch |> serialize(ptr_val)
        unsafe {
            live_window = reinterpret<GLFWwindow?>(ptr_val)
            delete ser
        }
        if (live_window != null) {
            glfwMakeContextCurrent(live_window)
        }
    }
}

// ============================================================================
// Synthetic mouse driver
// ============================================================================
//
// `mouse_pos` / `mouse_click` / `mouse_scroll` are one-shot synthetic events.
// `mouse_move_to` is an animated single-target tween over `duration_ms`.
// `mouse_play` runs a scripted event timeline; between consecutive `move`
// events the per-frame tick linearly interpolates and posts one cursor event
// per frame so anything reading mouse position (ImGui, the visual_aids trail,
// app-level handlers) sees smooth motion.

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
    action : int    = 0    // 1=press, 0=release (matches GLFW's GLFW_PRESS/GLFW_RELEASE)
}

var private timeline : array<MouseEvent>
var private play_start_t : float = 0.0
var private play_idx     : int = 0
var private playing      : bool = false
var private cursor_x     : float = 0.0
var private cursor_y     : float = 0.0
// Sticky: true once *any* synth event has fired (cursor move OR button).
// Overlays trust the last-known synth position for the entire session once
// flipped — without this, after the timeline drains `effective_cursor_pos`
// falls back to GetMousePos() (real OS cursor), and the recording's cursor
// sprite jumps off-screen whenever the user's physical mouse is elsewhere.
var private synth_cursor_owned : bool = false
// Cached waypoint for between-move interpolation. -1 = no prior move consumed.
var private last_move_x  : float = 0.0
var private last_move_y  : float = 0.0
var private last_move_t  : int = -1
// Last synthesized click (button + uptime-seconds). Consumed by visual aids
// (imgui_visual_aids.paint_click_flash) to draw a click indicator; the OS
// cursor isn't visible in the GL back buffer so a real click would otherwise
// leave no trace in APNG recordings.
var public synth_click_button : int = -1
var public synth_click_action : int = 0   // 1 = press, 0 = release
var public synth_click_t      : float = -1.0    //! get_uptime() at last click; -1 = no click yet
// Currently-synth-pressed buttons. Drained by mouse_stop so an interrupted
// drag/timeline doesn't leave ImGui believing a button is held.
var private synth_held_buttons : table<int>
// Synth events go through dasGLFW's chain dispatcher only — ImGui-GLFW (and
// any other chain listener) sees them via its installed callback. We never
// glfwSetCursorPos: the OS cursor lives outside the GL back buffer (the
// window manager paints it at display time), so warping it has no effect on
// glReadPixels-driven APNG recordings. The visible proof of motion in a
// recording is the visual_aids trail + cursor sprite, both drawn through
// ImGui's draw lists.

def private synth_cursor_pos(x, y : float) {
    cursor_x = x
    cursor_y = y
    synth_cursor_owned = true
    glfw_dispatch_cursor_pos(live_window, double(x), double(y))
}

def private synth_mouse_button(button, action : int) {
    glfw_post_mouse_button(live_window, button, action, 0)
    if (action == 1) {
        synth_held_buttons |> insert(button)
    } else {
        synth_held_buttons |> erase(button)
    }
    synth_click_button  = button
    synth_click_action  = action
    synth_click_t       = get_uptime()
    synth_cursor_owned  = true
}

def private release_held_buttons() {
    // No window = nothing to dispatch to; drop bookkeeping (matches
    // release_held_keys above).
    if (live_window == null) {
        synth_held_buttons |> clear()
        return
    }
    // Same snapshot-before-iterate pattern as release_held_keys —
    // synth_mouse_button erases from synth_held_buttons.
    var held : array<int>
    held |> reserve(length(synth_held_buttons))
    for (b in keys(synth_held_buttons)) {
        held |> push(b)
    }
    for (b in held) {
        synth_mouse_button(b, 0)
    }
}

def public get_synth_cursor() : tuple<bool; float; float> {
    //! (owned, x, y). When owned=true the synthetic driver has fired at least
    //! one event during this session — overlays (mouse_trail, cursor_sprite in
    //! imgui_visual_aids) should render against (x, y) instead of GetMousePos()
    //! since the per-frame ImGui_ImplGlfw polling will otherwise overwrite
    //! io.MousePos with the real OS cursor on focused windows.
    //!
    //! Sticky once flipped — without that, after a mouse_play timeline drains
    //! the cursor sprite would jump to GetMousePos() (the real OS cursor),
    //! and any APNG recording would lose the cursor whenever the user's
    //! physical mouse is over another window.
    return (synth_cursor_owned, cursor_x, cursor_y)
}

def private reset_timeline_state() {
    timeline |> clear()
    play_idx     = 0
    playing      = false
    last_move_x  = cursor_x
    last_move_y  = cursor_y
    last_move_t  = -1
}

struct MousePosArgs {
    x : float = 0.0
    y : float = 0.0
}

[live_command(description="Teleport synthetic cursor. Args: x, y")]
def mouse_pos(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<MousePosArgs>)
    synth_cursor_pos(args.x, args.y)
    return JV((x = args.x, y = args.y))
}

struct MouseClickArgs {
    @optional button : int    = 0
    action           : string = "press"
}

[live_command(description="Synthetic mouse button. Args: button (0/1/2), action (press|release)")]
def mouse_click(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<MouseClickArgs>)
    var act : int
    if (args.action == "press") {
        act = 1
    } elif (args.action == "release") {
        act = 0
    } else {
        return JV((error = "invalid action; expected \"press\" or \"release\"", action = args.action))
    }
    synth_mouse_button(args.button, act)
    return JV((button = args.button, action = args.action))
}

struct MouseScrollArgs {
    x : float = 0.0
    y : float = 0.0
}

[live_command(description="Synthetic mouse scroll. Args: x, y (offsets)")]
def mouse_scroll(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<MouseScrollArgs>)
    glfw_post_scroll(live_window, double(args.x), double(args.y))
    return JV((x = args.x, y = args.y))
}

struct MouseMoveToArgs {
    x : float = 0.0
    y : float = 0.0
    @optional duration_ms : int = 250
}

[live_command(description="Animated cursor move to (x,y) over duration_ms. Linear interpolation.")]
def mouse_move_to(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<MouseMoveToArgs>)
    reset_timeline_state()
    timeline |> push(MouseEvent(t_ms = 0,                 kind = MEKind.move, x = cursor_x, y = cursor_y))
    timeline |> push(MouseEvent(t_ms = max(1, args.duration_ms), kind = MEKind.move, x = args.x,    y = args.y))
    play_start_t = get_uptime()
    playing      = true
    return JV((duration_ms = args.duration_ms))
}

struct MouseEventWire {
    @optional t_ms   : int    = 0
    kind             : string = "move"  // "move" | "button" | "scroll"
    @optional x      : float  = 0.0
    @optional y      : float  = 0.0
    @optional button : int    = 0
    @optional action : string = "press"
}

struct MousePlayArgs {
    events : array<MouseEventWire>
}

def public sort_play_events(var events : array<MouseEventWire>) {
    //! Sort a mouse_play timeline by ``t_ms`` ascending. mouse_play only walks
    //! the queue forward, so out-of-order input would silently drop earlier
    //! events; sort first. Equal-``t_ms`` ordering is unspecified (the
    //! underlying ``sort()`` is qsort, not stable).
    events |> sort() $(a, b : MouseEventWire) {
        return a.t_ms < b.t_ms
    }
}

[live_command(description="Play a scripted mouse timeline. Args: events array of (t_ms, kind, x, y, button, action)")]
def mouse_play(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    var args = from_JV(input, type<MousePlayArgs>)
    return JV((error = "no events")) if (empty(args.events))
    sort_play_events(args.events)
    reset_timeline_state()
    timeline |> reserve(length(args.events))
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
        timeline |> push(ev)
    }
    play_start_t = get_uptime()
    playing      = true
    return JV((queued = length(timeline)))
}

[live_command(description="Stop synthetic playback and clear the timeline.")]
def mouse_stop(input : JsonValue?) : JsonValue? {
    let was_playing = playing
    let released    = length(synth_held_buttons)
    release_held_buttons()
    reset_timeline_state()
    return JV((stopped = was_playing, released = released))
}

struct MouseStatusResult {
    playing       : bool
    elapsed_ms    : int
    cursor_x      : float
    cursor_y      : float
    queue_idx     : int
    queue_total   : int
    held_count    : int
    cursor_owned  : bool
}

[live_command(description="Synthetic-mouse playback status.")]
def mouse_status(input : JsonValue?) : JsonValue? {
    let elapsed = playing ? int((get_uptime() - play_start_t) * 1000.0) : 0
    return JV(MouseStatusResult(
        playing       = playing,
        elapsed_ms    = elapsed,
        cursor_x      = cursor_x,
        cursor_y      = cursor_y,
        queue_idx     = play_idx,
        queue_total   = length(timeline),
        held_count    = length(synth_held_buttons),
        cursor_owned  = synth_cursor_owned
    ))
}

[before_update]
def mouse_tick() {
    return if (!playing || live_window == null)
    let now_ms = int((get_uptime() - play_start_t) * 1000.0)
    // Fire every due event (button/scroll instantly; move snaps the cursor to the waypoint).
    while (play_idx < length(timeline)) {
        let ev = timeline[play_idx]
        break if (ev.t_ms > now_ms)
        if (ev.kind == MEKind.move) {
            synth_cursor_pos(ev.x, ev.y)
            last_move_x = ev.x
            last_move_y = ev.y
            last_move_t = ev.t_ms
        } elif (ev.kind == MEKind.button) {
            synth_mouse_button(ev.button, ev.action)
        } elif (ev.kind == MEKind.scroll) {
            glfw_post_scroll(live_window, double(ev.x), double(ev.y))
        }
        play_idx++
    }
    if (play_idx >= length(timeline)) {
        playing = false
        return
    }
    // Between waypoints: lerp from the last consumed move toward the next move.
    if (last_move_t >= 0) {
        var next_move_idx = play_idx
        while (next_move_idx < length(timeline) && timeline[next_move_idx].kind != MEKind.move) {
            next_move_idx++
        }
        if (next_move_idx < length(timeline)) {
            let nm = timeline[next_move_idx]
            let span = nm.t_ms - last_move_t
            if (span > 0) {
                var alpha = float(now_ms - last_move_t) / float(span)
                if (alpha < 0.0) { alpha = 0.0; }
                if (alpha > 1.0) { alpha = 1.0; }
                let x = last_move_x + (nm.x - last_move_x) * alpha
                let y = last_move_y + (nm.y - last_move_y) * alpha
                synth_cursor_pos(x, y)
            }
        }
    }
}

// ============================================================================
// Synthetic keyboard driver
// ============================================================================
//
// Mirrors the mouse driver: one-shot helpers (`key_press`/`key_release`/`key_char`),
// chord sugar (`key_chord`), text streaming (`key_type`), and scripted timeline
// playback (`key_play`). Synth events go through dasGLFW's key/char chain
// dispatchers — ImGui_ImplGlfw_KeyCallback / _CharCallback receive them via the
// chain, so widgets see them as real input. No `apply_synth_io_override` is
// needed for keys: ImGui_ImplGlfw_NewFrame does not poll glfwGetKey per frame
// (key state is event-driven), so a single chain dispatch suffices.

enum private KEKind {
    press
    release
    char_
}

struct private KeyEvent {
    t_ms      : int    = 0
    kind      : KEKind = KEKind.press
    keycode   : int    = 0
    scancode  : int    = 0
    mods      : int    = 0
    codepoint : uint   = 0u
}

var private key_timeline     : array<KeyEvent>
var private key_play_start_t : float = 0.0
var private key_play_idx     : int   = 0
var private key_playing      : bool  = false

// Public state read by visual_aids for the keycap HUD.
var public synth_held_keys           : table<int>       //! set of currently-held GLFW keycodes
var public synth_last_keycode        : int   = 0        //! last GLFW keycode synth-pressed; 0 = none
var public synth_last_keycode_t      : float = -1.0     //! get_uptime() at last keycode press; -1 = none
var public synth_last_codepoint      : uint  = 0u       //! last Unicode codepoint synth-typed; 0 = none
var public synth_last_codepoint_t    : float = -1.0     //! get_uptime() at last codepoint event; -1 = none

def private synth_key_press(keycode, scancode, mods : int) {
    glfw_post_key(live_window, keycode, scancode, 1, mods)
    synth_held_keys |> insert(keycode)
    synth_last_keycode   = keycode
    synth_last_keycode_t = get_uptime()
}

def private synth_key_release(keycode, scancode, mods : int) {
    glfw_post_key(live_window, keycode, scancode, 0, mods)
    synth_held_keys |> erase(keycode)
}

def private synth_char(codepoint : uint) {
    glfw_post_char(live_window, codepoint)
    synth_last_codepoint   = codepoint
    synth_last_codepoint_t = get_uptime()
}

def public get_synth_keys() : tuple<bool; uint; float> {
    //! (active, last_codepoint, last_codepoint_t). Visual aids read this each
    //! frame to detect "a new codepoint just landed" (push a keycap into the
    //! HUD when last_codepoint_t advances). Modifier-strip lights from
    //! `IsKeyDown(ImGuiKey_*)` direct — ImGui already reflects synth events
    //! via the chain, no separate plumbing needed.
    return (key_playing, synth_last_codepoint, synth_last_codepoint_t)
}

def private reset_key_timeline_state() {
    key_timeline |> clear()
    key_play_idx = 0
    key_playing  = false
}

def private release_held_keys() {
    // No window = nothing to dispatch to; drop bookkeeping and return so
    // key_stop stays safe to call after live_destroy_window() (or before
    // live_create_window()) without passing a null GLFWwindow* into C++.
    if (live_window == null) {
        synth_held_keys |> clear()
        return
    }
    // Snapshot first — synth_key_release erases from synth_held_keys, which
    // would invalidate iteration. Same pattern as the `for (k in keys(t))` /
    // mutate-t fix in feedback_for_keys_table_locked.md.
    var held : array<int>
    held |> reserve(length(synth_held_keys))
    for (kc in keys(synth_held_keys)) {
        held |> push(kc)
    }
    for (kc in held) {
        synth_key_release(kc, 0, 0)
    }
}

struct KeyPressArgs {
    keycode           : int = 0
    @optional scancode : int = 0
    @optional mods    : int = 0
}

[live_command(description="Synthetic key press. Args: keycode (GLFW_KEY_*), scancode?, mods?")]
def key_press(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<KeyPressArgs>)
    return JV((error = "missing keycode")) if (args.keycode == 0)
    synth_key_press(args.keycode, args.scancode, args.mods)
    return JV((keycode = args.keycode, action = "press"))
}

[live_command(description="Synthetic key release. Args: keycode (GLFW_KEY_*), scancode?, mods?")]
def key_release(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<KeyPressArgs>)
    return JV((error = "missing keycode")) if (args.keycode == 0)
    synth_key_release(args.keycode, args.scancode, args.mods)
    return JV((keycode = args.keycode, action = "release"))
}

struct KeyCharArgs {
    codepoint : uint = 0u
}

[live_command(description="Synthetic Unicode char (no paired key event). Args: codepoint")]
def key_char(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<KeyCharArgs>)
    return JV((error = "missing codepoint")) if (args.codepoint == 0u)
    synth_char(args.codepoint)
    return JV((codepoint = args.codepoint))
}

// --- ASCII translation tables for key_type / key_chord ---

def private mod_name_to_keycode(name : string) : int {
    if (name == "Ctrl" || name == "Control") return GLFW_KEY_LEFT_CONTROL
    if (name == "Shift") return GLFW_KEY_LEFT_SHIFT
    if (name == "Alt") return GLFW_KEY_LEFT_ALT
    if (name == "Super" || name == "Cmd" || name == "Meta") return GLFW_KEY_LEFT_SUPER
    return 0
}

def private mod_name_to_bit(name : string) : int {
    if (name == "Ctrl" || name == "Control") return int(GLFW_MOD_CONTROL)
    if (name == "Shift") return int(GLFW_MOD_SHIFT)
    if (name == "Alt") return int(GLFW_MOD_ALT)
    if (name == "Super" || name == "Cmd" || name == "Meta") return int(GLFW_MOD_SUPER)
    return 0
}

// Letters and digits map directly; printable punctuation is a small fixed table.
// Capitals + shifted punctuation get an explicit Shift press/release around the
// char in key_type. Everything else falls through as a codepoint-only `char_`
// event with `keycode == 0` (text-input widgets still receive it via
// CharCallback).
def private needs_shift_ascii(c : int) : bool {
    if (c >= int('A') && c <= int('Z')) return true   // nolint:PERF014 (uppercase-only, is_alpha would also match lowercase)
    // shifted punctuation on a US layout. Listed as char literals (not a single
    // string) so the parser doesn't treat `{` / `}` as interpolation delimiters.
    return (c == int('~') || c == int('!') || c == int('@') || c == int('#')
         || c == int('$') || c == int('%') || c == int('^') || c == int('&')
         || c == int('*') || c == int('(') || c == int(')') || c == int('_')
         || c == int('+') || c == int('{') || c == int('}') || c == int('|')
         || c == int(':') || c == int('"') || c == int('<') || c == int('>')
         || c == int('?'))
}

def private base_keycode_ascii(c : int) : int {
    // letters — distinguishing case to compute the right keycode offset
    if (c >= int('A') && c <= int('Z')) return GLFW_KEY_A + (c - int('A'))   // nolint:PERF014 (need uppercase-only offset)
    if (c >= int('a') && c <= int('z')) return GLFW_KEY_A + (c - int('a'))   // nolint:PERF014 (need lowercase-only offset)
    // digits 0-9 → GLFW_KEY_0..9
    if (c >= int('0') && c <= int('9')) return GLFW_KEY_0 + (c - int('0'))   // nolint:PERF014 (need digit-only offset)
    // shifted digit row: ! @ # $ % ^ & * ( )  → keys 1 2 3 4 5 6 7 8 9 0
    if (c == int('!')) return GLFW_KEY_1
    if (c == int('@')) return GLFW_KEY_2
    if (c == int('#')) return GLFW_KEY_3
    if (c == int('$')) return GLFW_KEY_4
    if (c == int('%')) return GLFW_KEY_5
    if (c == int('^')) return GLFW_KEY_6
    if (c == int('&')) return GLFW_KEY_7
    if (c == int('*')) return GLFW_KEY_8
    if (c == int('(')) return GLFW_KEY_9
    if (c == int(')')) return GLFW_KEY_0
    // punctuation
    if (c == int(' ')) return GLFW_KEY_SPACE
    if (c == int('\n')) return GLFW_KEY_ENTER
    if (c == int('\t')) return GLFW_KEY_TAB
    if (c == int(',') || c == int('<')) return GLFW_KEY_COMMA
    if (c == int('.') || c == int('>')) return GLFW_KEY_PERIOD
    if (c == int('/') || c == int('?')) return GLFW_KEY_SLASH
    if (c == int(';') || c == int(':')) return GLFW_KEY_SEMICOLON
    if (c == int('\'') || c == int('"')) return GLFW_KEY_APOSTROPHE
    if (c == int('-') || c == int('_')) return GLFW_KEY_MINUS
    if (c == int('=') || c == int('+')) return GLFW_KEY_EQUAL
    if (c == int('[') || c == int('{')) return GLFW_KEY_LEFT_BRACKET
    if (c == int(']') || c == int('}')) return GLFW_KEY_RIGHT_BRACKET
    if (c == int('\\') || c == int('|')) return GLFW_KEY_BACKSLASH
    if (c == int('`') || c == int('~')) return GLFW_KEY_GRAVE_ACCENT
    return 0
}

struct KeyTypeArgs {
    text                       : string = ""
    @optional total_duration_s : float  = 0.0
    @optional per_char_ms      : int    = 60
}

[live_command(description="Stream text as synthetic key+char events. Args: text (UTF-8; non-ASCII emits char-only events), total_duration_s? (preferred) | per_char_ms? (default 60)")]
def key_type(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<KeyTypeArgs>)
    return JV((error = "empty text")) if (empty(args.text))
    // Decode UTF-8 to codepoints so pacing + ASCII auto-shift work per character,
    // not per byte. Non-ASCII codepoints emit a char-only event (no key press) —
    // mirrors how GLFW dispatches IME / dead-key composition results to the
    // char callback without a paired key event.
    let cps <- utf8_decode(args.text)
    let n_chars = length(cps)
    var per_char_ms = max(1, args.per_char_ms)
    if (args.total_duration_s > 0.0f) {
        per_char_ms = max(1, int(args.total_duration_s * 1000.0f) / max(1, n_chars))
    }
    reset_key_timeline_state()
    var t = 0
    // Worst case: 5 events per char (Shift down, key down, char, key up, Shift up).
    key_timeline |> reserve(n_chars * 5)
    for (cp in cps) {
        if (cp < 0x80u) {
            let c     = int(cp)
            let kc    = base_keycode_ascii(c)
            let shift = needs_shift_ascii(c)
            let mods  = shift ? int(GLFW_MOD_SHIFT) : 0
            if (shift) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press,   keycode = GLFW_KEY_LEFT_SHIFT))
            }
            if (kc != 0) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press,   keycode = kc, mods = mods))
            }
            key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.char_,       codepoint = cp))
            if (kc != 0) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, keycode = kc, mods = mods))
            }
            if (shift) {
                key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, keycode = GLFW_KEY_LEFT_SHIFT))
            }
        } else {
            key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.char_, codepoint = cp))
        }
        t += per_char_ms
    }
    key_play_start_t = get_uptime()
    key_playing      = true
    return JV((queued = length(key_timeline), per_char_ms = per_char_ms, chars = n_chars))
}

struct KeyChordArgs {
    mods : array<string>
    key  : string = ""
}

[live_command(description="Synthetic chord. Args: mods (array of \"Ctrl\"|\"Shift\"|\"Alt\"|\"Super\"|\"Cmd\"|\"Meta\"), key (single ASCII character)")]
def key_chord(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    let args = from_JV(input, type<KeyChordArgs>)
    return JV((error = "missing key")) if (empty(args.key))
    // resolve target keycode: prefer a single ASCII char (uppercase letter / digit / symbol).
    var target_kc = 0
    if (length(args.key) == 1) {
        target_kc = base_keycode_ascii(int(first_character(args.key)))
    }
    return JV((error = "unsupported chord key", key = args.key)) if (target_kc == 0)
    var mod_bits = 0
    for (m in args.mods) {
        let b = mod_name_to_bit(m)
        return JV((error = "unknown modifier", modifier = m)) if (b == 0)
        mod_bits |= b
    }
    reset_key_timeline_state()
    // press(mod)*N + press(key) + release(key) + release(mod)*N
    key_timeline |> reserve(length(args.mods) * 2 + 2)
    var t = 0
    // press modifiers in array order
    for (m in args.mods) {
        let mkc = mod_name_to_keycode(m)
        key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press, keycode = mkc))
        t += 8     // tight stamp: ImGui sees these as a combo
    }
    key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.press,   keycode = target_kc, mods = mod_bits))
    t += 16
    key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, keycode = target_kc, mods = mod_bits))
    t += 8
    // release modifiers in reverse order
    var i = length(args.mods) - 1
    while (i >= 0) {
        let mkc = mod_name_to_keycode(args.mods[i])
        key_timeline |> push(KeyEvent(t_ms = t, kind = KEKind.release, keycode = mkc))
        t += 8
        i--
    }
    key_play_start_t = get_uptime()
    key_playing      = true
    return JV((queued = length(key_timeline)))
}

struct KeyEventWire {
    @optional t_ms      : int    = 0
    kind                : string = "press"   // "press" | "release" | "char"
    @optional keycode   : int    = 0
    @optional scancode  : int    = 0
    @optional mods      : int    = 0
    @optional codepoint : uint   = 0u
}

struct KeyPlayArgs {
    events : array<KeyEventWire>
}

def public sort_key_play_events(var events : array<KeyEventWire>) {
    //! Sort a key_play timeline by ``t_ms`` ascending. Same rationale as
    //! ``sort_play_events`` for mouse: the queue is walked forward, so
    //! out-of-order input would silently drop earlier events.
    events |> sort() $(a, b : KeyEventWire) {
        return a.t_ms < b.t_ms
    }
}

[live_command(description="Play a scripted key/char timeline. Args: events array of (t_ms, kind, keycode, scancode, mods, codepoint)")]
def key_play(input : JsonValue?) : JsonValue? {
    return JV((error = "no window")) if (live_window == null)
    var args = from_JV(input, type<KeyPlayArgs>)
    return JV((error = "no events")) if (empty(args.events))
    sort_key_play_events(args.events)
    reset_key_timeline_state()
    key_timeline |> reserve(length(args.events))
    for (w in args.events) {
        var ev = KeyEvent(t_ms = w.t_ms, keycode = w.keycode, scancode = w.scancode, mods = w.mods, codepoint = w.codepoint)
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
    key_play_start_t = get_uptime()
    key_playing      = true
    return JV((queued = length(key_timeline)))
}

[live_command(description="Stop synthetic key playback and clear the timeline. Releases any keys still synth-held (e.g. modifiers from an interrupted key_chord) so ImGui doesn't inherit stuck modifier state.")]
def key_stop(input : JsonValue?) : JsonValue? {
    let was_playing = key_playing
    let released    = length(synth_held_keys)
    release_held_keys()
    reset_key_timeline_state()
    return JV((stopped = was_playing, released = released))
}

struct KeyStatusResult {
    playing       : bool
    elapsed_ms    : int
    queue_idx     : int
    queue_total   : int
    held_count    : int
    last_keycode  : int
    last_codepoint : uint
}

[live_command(description="Synthetic-keyboard playback status.")]
def key_status(input : JsonValue?) : JsonValue? {
    let elapsed = key_playing ? int((get_uptime() - key_play_start_t) * 1000.0) : 0
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

[before_update]
def key_tick() {
    return if (!key_playing || live_window == null)
    let now_ms = int((get_uptime() - key_play_start_t) * 1000.0)
    while (key_play_idx < length(key_timeline)) {
        let ev = key_timeline[key_play_idx]
        break if (ev.t_ms > now_ms)
        if (ev.kind == KEKind.press) {
            synth_key_press(ev.keycode, ev.scancode, ev.mods)
        } elif (ev.kind == KEKind.release) {
            synth_key_release(ev.keycode, ev.scancode, ev.mods)
        } elif (ev.kind == KEKind.char_) {
            synth_char(ev.codepoint)
        }
        key_play_idx++
    }
    if (key_play_idx >= length(key_timeline)) {
        key_playing = false
    }
}
