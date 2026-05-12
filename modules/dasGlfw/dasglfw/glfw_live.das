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
require live/live_commands
require live_host
require math

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
    glfw_dispatch_cursor_pos(live_window, double(x), double(y))
}

def private synth_mouse_button(button, action : int) {
    glfw_post_mouse_button(live_window, button, action, 0)
    synth_click_button = button
    synth_click_action = action
    synth_click_t      = get_uptime()
}

def public get_synth_cursor() : tuple<bool; float; float> {
    //! (active, x, y). When active=true the synthetic driver owns the cursor
    //! position — overlays (mouse_trail, cursor_sprite in imgui_visual_aids)
    //! should render against (x, y) instead of GetMousePos() since the
    //! per-frame ImGui_ImplGlfw polling will otherwise overwrite io.MousePos
    //! with the real OS cursor on focused windows. Active during mouse_play
    //! and mouse_move_to playback.
    return (playing, cursor_x, cursor_y)
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
    reset_timeline_state()
    return JV((stopped = was_playing))
}

struct MouseStatusResult {
    playing     : bool
    elapsed_ms  : int
    cursor_x    : float
    cursor_y    : float
    queue_idx   : int
    queue_total : int
}

[live_command(description="Synthetic-mouse playback status.")]
def mouse_status(input : JsonValue?) : JsonValue? {
    let elapsed = playing ? int((get_uptime() - play_start_t) * 1000.0) : 0
    return JV(MouseStatusResult(
        playing     = playing,
        elapsed_ms  = elapsed,
        cursor_x    = cursor_x,
        cursor_y    = cursor_y,
        queue_idx   = play_idx,
        queue_total = length(timeline)
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
