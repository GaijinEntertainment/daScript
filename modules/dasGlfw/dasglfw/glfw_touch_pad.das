options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module glfw_touch_pad shared public

//! On-screen controls for a keyboard game on a touch device. The game declares its zones once a
//! frame, between ``pad_begin_frame`` and ``pad_end_frame``; a finger inside a zone holds that
//! zone's key in the virtual key table ``glfwGetKey`` reads, so the game's own key polling stays
//! as it is. Zones and buttons first, then ``pad_swipe`` / ``pad_tap`` / ``pad_axis_x``, which
//! act on the fingers no zone claimed.
//!
//! Coordinates are fractions of the window: ``(0, 0)`` top-left, ``(1, 1)`` bottom-right; a
//! radius is a fraction of the shorter side. The pad is on only on a touch device, or with
//! ``--touch-pad`` after the daslang ``--`` separator.

require glfw/glfw_boost
require live/glfw_live
require opengl/opengl_boost
require daslib/clargs
require daslib/safe_addr
require math
require daslib/math_boost

struct public PadTouch {
    //! One finger in window points; ``start`` is where it landed, moved forward by every swipe it fires.
    id : int
    pos : float2
    start : float2
    began : bool
    ended : bool
    claimed : bool
}

enum private PadShapeKind {
    disc
    rect
    arrow
}

struct private PadShape {
    kind : PadShapeKind
    pos : float2
    size : float2
    dir : float2
    color : float4
}

let private COLOR_IDLE = float4(1.0, 1.0, 1.0, 0.16)
let private COLOR_HELD = float4(1.0, 1.0, 1.0, 0.36)
let private COLOR_RING = float4(1.0, 1.0, 1.0, 0.28)
let private COLOR_SCRIM = float4(0.0, 0.0, 0.0, 0.72)
let private COLOR_ICON = float4(1.0, 1.0, 1.0, 0.85)
let private DPAD_DEAD = 0.28
//! 0.41 is tan 22.5 degrees: the band around the diagonal where both axes hold.
let private DPAD_DIAGONAL = 0.41

var private g_enabled : bool = false
var private g_parsed : bool = false
var private g_forced : bool = false
var private g_prefer_landscape : bool = false
var private g_win : float2 = float2(1.0, 1.0)
var private g_touches : array<PadTouch>
var private g_injected : array<PadTouch>
var private g_injecting : bool = false
var private g_shapes : array<PadShape>
var private g_keys_held : array<int>
var private g_frame_open : bool = false

def private ensure_parsed() {
    return if (g_parsed)
    g_parsed = true
    let args <- get_user_args()
    g_forced = (find_bool_flag_raw_value(args, "--touch-pad") |> unwrap_or("false")) == "true"
    g_enabled = g_forced || glfw_is_touch_device()
}

def public pad_enabled() : bool {
    //! True on a touch device or under ``--touch-pad``, where the left mouse button is one finger; every other pad call is a no-op otherwise.
    ensure_parsed()
    return g_enabled
}

def public pad_set_enabled(on : bool) {
    ensure_parsed()
    g_enabled = on
}

def public pad_prefer_landscape(want : bool = true) {
    //! The game plays sideways: a portrait window gets the rotate scrim instead of the controls.
    g_prefer_landscape = want
}

def public pad_rotate_text_y(h : float) : float {
    //! Where a line of text sits under the rotate scrim's frame, for a window ``h`` tall in the caller's units.
    return h * 0.5 + h * 0.20
}

def public pad_wants_rotate() : bool {
    //! True while the pad is on, the game prefers landscape, and the window is taller than wide.
    return pad_enabled() && g_prefer_landscape && g_win.y > g_win.x
}

def public pad_inject(touches : array<PadTouch>) {
    //! Fingers for the next ``pad_begin_frame`` instead of the device's, in window points; an empty list
    //! hands the device back. Tests.
    g_injecting = !empty(touches)
    delete g_injected
    g_injected := touches
}

def public pad_set_window_size(w, h : float) {
    //! Window size for a windowless test; a real frame reads it from the GLFW window.
    g_win = float2(max(w, 1.0), max(h, 1.0))
}

def public pad_key_held(key : int) : bool {
    //! Whether a zone declared this frame holds ``key``.
    return find_index(g_keys_held, key) >= 0
}

def public pad_touches() : array<PadTouch> {
    return <- clone_to_move(g_touches)
}

def private hold_key(key : int) {
    return if (key < 0 || pad_key_held(key))
    g_keys_held |> push(key)
    glfw_set_virtual_key(key, true)
}

def private carried_start(id : int; pos : float2; began : bool) : float2 {
    return pos if (began)
    for (old in g_touches) {
        return old.start if (old.id == id && !old.ended)
    }
    return pos
}

def private merge_device_touches() {
    var fresh : array<PadTouch>
    let count = glfw_touch_count()
    fresh |> reserve(count)
    for (i in range(count)) {
        let id = glfw_touch_id(i)
        let pos = float2(glfw_touch_x(i), glfw_touch_y(i))
        let began = glfw_touch_began(i)
        fresh |> push(PadTouch(id = id, pos = pos, start = carried_start(id, pos, began), began = began, ended = glfw_touch_ended(i)))
    }
    if (count == 0 && g_forced && live_window != null) {
        merge_mouse_finger(fresh)
    }
    delete g_touches
    g_touches <- fresh
}

// On a desktop under --touch-pad the left mouse button is finger 0, so a pad mapping can be tried without a phone.
def private merge_mouse_finger(var fresh : array<PadTouch>) {
    let down = glfwGetMouseButton(live_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS
    var was_down = false
    for (old in g_touches) {
        was_down = was_down || (old.id == 0 && !old.ended)
    }
    return if (!down && !was_down)
    var mx, my : double
    glfwGetCursorPos(live_window, safe_addr(mx), safe_addr(my))
    let pos = float2(float(mx), float(my))
    fresh |> push(PadTouch(id = 0, pos = pos, start = carried_start(0, pos, !was_down), began = !was_down, ended = !down))
}

def private merge_injected_touches() {
    var fresh : array<PadTouch>
    fresh |> reserve(length(g_injected))
    for (t in g_injected) {
        var start = t.pos
        for (old in g_touches) {
            if (old.id == t.id && !old.ended) {
                start = old.start
                break
            }
        }
        fresh |> push(PadTouch(id = t.id, pos = t.pos, start = t.began ? t.pos : start, began = t.began, ended = t.ended))
    }
    delete g_touches
    g_touches <- fresh
    var keep <- [for (t in g_injected); PadTouch(id = t.id, pos = t.pos); where !t.ended]
    delete g_injected
    g_injected <- keep
}

[arch(at="../ARCHITECTURE.md#touch-pad")]
def public pad_begin_frame() {
    //! Releases last frame's keys and reads this frame's fingers. Call right after ``live_begin_frame``.
    glfw_clear_virtual_keys()      // the table itself, not only the mirror: a reload empties the mirror and keeps the table
    g_keys_held |> clear()
    g_shapes |> clear()
    g_frame_open = pad_enabled()
    return if (!g_frame_open)
    if (live_window != null) {
        var w = 0
        var h = 0
        glfwGetWindowSize(live_window, safe_addr(w), safe_addr(h))
        if (w > 0 && h > 0) {
            g_win = float2(float(w), float(h))
        }
    }
    if (g_injecting) {
        merge_injected_touches()
    } else {
        merge_device_touches()
    }
}

def private to_px(fx, fy : float) : float2 {
    return float2(fx * g_win.x, fy * g_win.y)
}

def private radius_px(r : float) : float {
    return r * min(g_win.x, g_win.y)
}

def private finger_down(t : PadTouch) : bool {
    //! A finger that ended this frame is a release, unless it also began this frame - a same-frame tap.
    return !t.claimed && (!t.ended || t.began)
}

def private finger_in_disc(t : PadTouch; c : float2; r : float) : bool {
    return finger_down(t) && length(t.pos - c) <= r
}

def public pad_button(fx, fy, r : float; key : int; alt_key : int = -1) : bool {
    //! A round button: any finger inside holds ``key`` (and ``alt_key``). Returns whether it is held.
    return false if (!g_frame_open || pad_wants_rotate())
    let c = to_px(fx, fy)
    let rp = radius_px(r)
    var held = false
    for (t in g_touches) {
        if (finger_in_disc(t, c, rp)) {
            t.claimed = true
            held = true
        }
    }
    if (held) {
        hold_key(key)
        hold_key(alt_key)
    }
    g_shapes |> push(PadShape(kind = PadShapeKind.disc, pos = c, size = float2(rp, rp), color = held ? COLOR_HELD : COLOR_IDLE))
    g_shapes |> push(PadShape(kind = PadShapeKind.disc, pos = c, size = float2(rp * 0.82, rp * 0.82), color = float4(0.0, 0.0, 0.0, 0.25)))
    return held
}

def public pad_zone(fx, fy, fw, fh : float; key : int; alt_key : int = -1) : bool {
    //! An unmarked rectangle: any finger inside holds ``key``. Drawn faintly only while held.
    return false if (!g_frame_open || pad_wants_rotate())
    let p = to_px(fx, fy)
    let s = to_px(fw, fh)
    var held = false
    for (t in g_touches) {
        if (finger_down(t) && t.pos.x >= p.x && t.pos.x <= p.x + s.x && t.pos.y >= p.y && t.pos.y <= p.y + s.y) {
            t.claimed = true
            held = true
        }
    }
    if (held) {
        hold_key(key)
        hold_key(alt_key)
        g_shapes |> push(PadShape(kind = PadShapeKind.rect, pos = p + s * 0.5, size = s * 0.5, color = float4(1.0, 1.0, 1.0, 0.08)))
    }
    return held
}

def public pad_prompt(action : string; key : string = "SPACE") : string {
    //! The words a prompt shows for ``action``: ``TAP TO ...`` with the pad on, ``PRESS <key> TO ...`` without.
    return pad_enabled() ? "TAP TO {action}" : "PRESS {key} TO {action}"
}

def public pad_rotate_caption() : string {
    //! The words a game writes under the rotate scrim.
    return "TURN YOUR PHONE SIDEWAYS"
}

def private hold_dir(dir : int2; left, right, up, down : int) {
    if (dir.x < 0) hold_key(left)
    if (dir.x > 0) hold_key(right)
    if (dir.y < 0) hold_key(up)
    if (dir.y > 0) hold_key(down)
}

def public pad_dpad(fx, fy, r : float; left, right, up, down : int; diagonal : bool = false) : int2 {
    //! Four directions around a center: the finger's offset picks one (the dominant axis), a small
    //! dead zone in the middle picks none; with ``diagonal`` an offset near the 45 degree line
    //! holds both axes. Returns the direction held, ``int2(0, 0)`` for none.
    return int2(0, 0) if (!g_frame_open || pad_wants_rotate())
    let c = to_px(fx, fy)
    let rp = radius_px(r)
    var dir = int2(0, 0)
    for (t in g_touches) {
        if (finger_in_disc(t, c, rp * 1.25)) {
            t.claimed = true
            let d = t.pos - c
            continue if (length(d) < rp * DPAD_DEAD)
            let both = diagonal && min(abs(d.x), abs(d.y)) > max(abs(d.x), abs(d.y)) * DPAD_DIAGONAL
            let x = (both || abs(d.x) >= abs(d.y)) ? (d.x < 0.0 ? -1 : 1) : 0
            let y = (both || abs(d.y) > abs(d.x)) ? (d.y < 0.0 ? -1 : 1) : 0
            dir = int2(x, y)
        }
    }
    hold_dir(dir, left, right, up, down)
    g_shapes |> push(PadShape(kind = PadShapeKind.disc, pos = c, size = float2(rp, rp), color = float4(1.0, 1.0, 1.0, 0.10)))
    let arm = rp * 0.62
    let head = rp * 0.22
    g_shapes |> push(PadShape(kind = PadShapeKind.arrow, pos = c + float2(-arm, 0.0), size = float2(head, head), dir = float2(-1.0, 0.0), color = dir.x < 0 ? COLOR_HELD : COLOR_RING))
    g_shapes |> push(PadShape(kind = PadShapeKind.arrow, pos = c + float2(arm, 0.0), size = float2(head, head), dir = float2(1.0, 0.0), color = dir.x > 0 ? COLOR_HELD : COLOR_RING))
    g_shapes |> push(PadShape(kind = PadShapeKind.arrow, pos = c + float2(0.0, -arm), size = float2(head, head), dir = float2(0.0, -1.0), color = dir.y < 0 ? COLOR_HELD : COLOR_RING))
    g_shapes |> push(PadShape(kind = PadShapeKind.arrow, pos = c + float2(0.0, arm), size = float2(head, head), dir = float2(0.0, 1.0), color = dir.y > 0 ? COLOR_HELD : COLOR_RING))
    return dir
}

def public pad_swipe(left, right, up, down : int; min_px : float = 24.0) : int2 {
    //! A free finger that travelled ``min_px`` from where it landed pulses the direction's key for
    //! this frame and starts measuring again from there. Returns the direction, ``int2(0, 0)`` for none.
    return int2(0, 0) if (!g_frame_open || pad_wants_rotate())
    var dir = int2(0, 0)
    for (t in g_touches) {
        continue if (t.claimed)
        let d = t.pos - t.start
        continue if (length(d) < min_px)
        dir = (abs(d.x) >= abs(d.y) ? int2(d.x < 0.0 ? -1 : 1, 0) : int2(0, d.y < 0.0 ? -1 : 1))
        t.start = t.pos
        t.claimed = true
    }
    hold_dir(dir, left, right, up, down)
    return dir
}

def public pad_tap(key : int) : bool {
    //! A free finger that landed this frame presses ``key`` for this frame.
    return false if (!g_frame_open || pad_wants_rotate())
    var tapped = false
    for (t in g_touches) {
        if (!t.claimed && t.began) {
            t.claimed = true
            tapped = true
        }
    }
    if (tapped) hold_key(key)
    return tapped
}

def public pad_axis_x() : float {
    //! The first free finger's x as a fraction of the window width, or -1 with no finger down.
    return -1.0 if (!g_frame_open || pad_wants_rotate())
    for (t in g_touches) {
        if (finger_down(t)) {
            t.claimed = true
            return t.pos.x / g_win.x
        }
    }
    return -1.0
}

var private @in @location pad_v_position : float2
var private @in @location pad_v_color : float4
var private @uniform pad_v_mvp : float4x4
var private @inout pad_f_color : float4
var private @out pad_f_FragColor : float4

[vertex_program(name = "PAD_VERTEX_SRC")]
def private pad_vs_main {
    pad_f_color = pad_v_color
    gl_Position = pad_v_mvp * float4(pad_v_position, 0.0, 1.0)
}

[fragment_program(name = "PAD_FRAGMENT_SRC")]
def private pad_fs_main {
    pad_f_FragColor = pad_f_color
}

[vertex_buffer]
struct private PadVertex {
    xy : float2
    rgba : float4
}

var private g_program : uint = 0u
var private g_vao : uint = 0u
var private g_vbo : uint = 0u
var private g_verts : array<PadVertex>

def private ensure_gl_objects() {
    return if (g_program != 0u)
    g_program = create_shader_program(@@pad_vs_main, @@pad_fs_main)
    glGenVertexArrays(1, safe_addr(g_vao))
    glBindVertexArray(g_vao)
    glGenBuffers(1, safe_addr(g_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo)
    glBindBuffer(GL_ARRAY_BUFFER, 0u)
    glBindVertexArray(0u)
}

def private emit_tri(a, b, c : float2; color : float4) {
    g_verts |> push(PadVertex(xy = a, rgba = color))
    g_verts |> push(PadVertex(xy = b, rgba = color))
    g_verts |> push(PadVertex(xy = c, rgba = color))
}

def private emit_shape(s : PadShape) {
    if (s.kind == PadShapeKind.rect) {
        let a = s.pos - s.size
        let b = s.pos + s.size
        emit_tri(a, float2(b.x, a.y), b, s.color)
        emit_tri(a, b, float2(a.x, b.y), s.color)
    } elif (s.kind == PadShapeKind.arrow) {
        let side = float2(-s.dir.y, s.dir.x)
        let tip = s.pos + s.dir * s.size.x
        let base = s.pos - s.dir * s.size.x * 0.6
        emit_tri(tip, base + side * s.size.y, base - side * s.size.y, s.color)
    } else {
        let n = 28
        for (i in range(n)) {
            let a0 = float(i) * 2.0 * PI / float(n)
            let a1 = float(i + 1) * 2.0 * PI / float(n)
            emit_tri(s.pos, s.pos + float2(cos(a0), sin(a0)) * s.size.x, s.pos + float2(cos(a1), sin(a1)) * s.size.x, s.color)
        }
    }
}

def private emit_rotate_scrim() {
    emit_shape(PadShape(kind = PadShapeKind.rect, pos = g_win * 0.5, size = g_win * 0.5, color = COLOR_SCRIM))
    let c = g_win * 0.5
    let side = min(g_win.x, g_win.y)
    let half = float2(side * 0.22, side * 0.13)
    let edge = side * 0.012
    emit_shape(PadShape(kind = PadShapeKind.rect, pos = c - float2(0.0, half.y), size = float2(half.x, edge), color = COLOR_ICON))
    emit_shape(PadShape(kind = PadShapeKind.rect, pos = c + float2(0.0, half.y), size = float2(half.x, edge), color = COLOR_ICON))
    emit_shape(PadShape(kind = PadShapeKind.rect, pos = c - float2(half.x, 0.0), size = float2(edge, half.y), color = COLOR_ICON))
    emit_shape(PadShape(kind = PadShapeKind.rect, pos = c + float2(half.x, 0.0), size = float2(edge, half.y), color = COLOR_ICON))
}

def public pad_end_frame() {
    //! Draws the declared shapes (or the rotate scrim) over the frame. Call before ``live_end_frame``.
    return if (!g_frame_open)
    g_frame_open = false
    return if (live_window == null)
    g_verts |> clear()
    if (pad_wants_rotate()) {
        emit_rotate_scrim()
    } else {
        for (s in g_shapes) {
            emit_shape(s)
        }
    }
    return if (empty(g_verts))
    ensure_gl_objects()
    var fbw = 0
    var fbh = 0
    live_get_framebuffer_size(fbw, fbh)
    return if (fbw <= 0 || fbh <= 0)
    glBindVertexArray(g_vao)
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo)
    glBufferData(GL_ARRAY_BUFFER, g_verts, GL_DYNAMIC_DRAW)
    bind_vertex_buffer(null, type<PadVertex>)
    glBindBuffer(GL_ARRAY_BUFFER, 0u)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glUseProgram(g_program)
    pad_v_mvp = ortho_rh(0.0, g_win.x, g_win.y, 0.0, -1.0, 1.0)
    pad_vs_main_bind_uniform(g_program)
    glDrawArrays(GL_TRIANGLES, 0, length(g_verts))
    glUseProgram(0u)
    glDisable(GL_BLEND)
    glBindVertexArray(0u)
    bind_ffp()
}
