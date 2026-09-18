options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_touch shared public

//! Touch input for a dasImgui app: one finger stays the mouse the backend already makes of it;
//! two fingers scroll the hovered window. The gesture step is pure (a touch list in, ImGui events
//! out) so it runs identically under GLFW, headless, and a test that injects fingers.

require imgui public
require live/live_commands
require live_host public
require daslib/json
require daslib/json_boost
require math

struct public TouchPoint {
    //! One finger, in ImGui display coordinates. ``began``/``ended`` hold for exactly one frame.
    id : int
    pos : float2
    began : bool
    ended : bool
}

struct public TouchScrollState {
    //! Two-finger scroll state carried across frames: how many fingers were down, the centroid last
    //! seen while two or more were, and the velocity that keeps scrolling for a few frames after they lift.
    two_down : bool
    count : int
    last : float2
    velocity : float2
}

let public TOUCH_WHEEL_X_PER_Y = 0.4     // ImGui scrolls two font heights per horizontal wheel unit against five vertical

struct public TouchGesture {
    //! What one frame of fingers asks of ImGui: drop the mouse button (the second finger landed on
    //! a one-finger drag), hover the centroid on that frame so the scroll lands on the window under
    //! the fingers, and/or scroll by this many wheel units.
    release_button : bool
    wheel : float2
    has_hover : bool
    hover : float2
}

let public TOUCH_FLICK_DECAY = 0.9
let public TOUCH_FLICK_STOP_PX = 0.5
let public TOUCH_BODY_FONT_PX = 17.0f
let public DESKTOP_BODY_FONT_PX = 14.0f

def public body_font_px() : float {
    //! The unscaled body-text size this device reads: a finger reads a bigger face than a cursor does.
    return imgui_touch_device ? TOUCH_BODY_FONT_PX : DESKTOP_BODY_FONT_PX
}

var public imgui_touch_device : bool = false
var public imgui_touch_injected : array<TouchPoint>
var private g_touch_injected_ever : bool = false

[arch(at="../ARCHITECTURE.md#touch")]
def public touch_scroll_step(var st : TouchScrollState; touches : array<TouchPoint>; step_px : float) : TouchGesture {
    //! One frame of the two-finger scroll machine. ``step_px`` is the pixel distance one wheel unit
    //! scrolls, so content follows the fingers 1:1.
    var g : TouchGesture
    var down = 0
    var sum = float2(0.0, 0.0)
    for (t in touches) {
        if (!t.ended) {
            down++
            sum += t.pos
        }
    }
    if (down >= 2) {
        let centroid = sum / float(down)
        if (!st.two_down) {
            st.two_down = true
            st.last = centroid
            st.velocity = float2(0.0, 0.0)
            g.release_button = true
            g.has_hover = true          // once: a hover every frame would defer every wheel a frame behind it
            g.hover = centroid
        } elif (down != st.count) {
            st.last = centroid          // a finger landed or lifted: the centroid jumped, the content must not
            st.velocity = float2(0.0, 0.0)
        } else {
            let delta = centroid - st.last
            st.last = centroid
            st.velocity = delta
            g.wheel = float2(delta.x / (step_px * TOUCH_WHEEL_X_PER_Y), delta.y / step_px)
        }
        st.count = down
        return g
    }
    st.two_down = false
    st.count = 0
    if (length(st.velocity) > TOUCH_FLICK_STOP_PX) {
        g.wheel = float2(st.velocity.x / (step_px * TOUCH_WHEEL_X_PER_Y), st.velocity.y / step_px)
        st.velocity *= TOUCH_FLICK_DECAY
    } else {
        st.velocity = float2(0.0, 0.0)
    }
    return g
}

def public touch_gesture_apply(g : TouchGesture) {
    //! Feed a gesture into the current ImGui context's event queue. The release also clears the
    //! queue - every event of that frame, so a press that landed with the second finger never becomes a click.
    var io & = unsafe(GetIO())
    if (g.release_button) {
        io |> ClearEventsQueue()
        io |> AddMouseButtonEvent(0, false)
    }
    if (g.has_hover) {
        io |> AddMousePosEvent(g.hover.x, g.hover.y)
    }
    if (g.wheel.x != 0.0 || g.wheel.y != 0.0) {
        io |> AddMouseWheelEvent(g.wheel.x, g.wheel.y)
    }
}

def public touch_scroll_step_px(fallback_font_px : float) : float {
    //! ImGui scrolls five font heights per wheel unit. Read between frames the font size is the unscaled
    //! base, so the style's scales are applied here; before the first frame it reads 0.
    let font = GetFontSize()
    let style & = unsafe(GetStyle())
    return 5.0 * (font > 0.0 ? font : fallback_font_px) * style.FontScaleMain * style.FontScaleDpi
}

def public apply_touch_style(scale : float) {
    //! Finger-sized hit targets on top of the daslang theme: extra hit padding, a scrollbar a thumb
    //! can grab, and windows that move only by their title bar so a body drag never drags the window.
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.IsTouchScreen
    io.ConfigWindowsMoveFromTitleBarOnly = true
    var style & = unsafe(GetStyle())
    style.TouchExtraPadding = float2(4.0 * scale, 4.0 * scale)
    style.ScrollbarSize = max(style.ScrollbarSize, 16.0 * scale)
    style.GrabMinSize = max(style.GrabMinSize, 22.0 * scale)
    style.FramePadding = float2(max(style.FramePadding.x, 8.0 * scale), max(style.FramePadding.y, 6.0 * scale))
    style.ItemSpacing = float2(style.ItemSpacing.x, max(style.ItemSpacing.y, 8.0 * scale))
}

def public touch_take_injected() : array<TouchPoint> {
    //! The injected fingers as this frame sees them. The list then commits like the device poll does:
    //! ``began`` clears and an ended finger leaves, so a headless host running thousands of frames
    //! between two commands keeps the fingers down until the next injection replaces them.
    var out := imgui_touch_injected
    var keep <- [for (f in imgui_touch_injected); TouchPoint(id = f.id, pos = f.pos); where !f.ended]
    delete imgui_touch_injected
    imgui_touch_injected <- keep
    return <- out
}

def public touch_injection_seen() : bool {
    return g_touch_injected_ever
}

struct TouchInjectFinger {
    id : int
    x : float
    y : float
    @optional began : bool = false
    @optional ended : bool = false
}

struct TouchInjectArgs {
    touches : array<TouchInjectFinger>
}

[live_command(description = "Inject fingers for the next frame (ImGui display coords). Args: touches [\{id, x, y, began, ended\}]")]
def imgui_touch_inject(input : JsonValue?) : JsonValue? {
    //! Live-command: the fingers the harness reads from now on instead of the device's, until the
    //! next injection. ``began``/``ended`` are one-shot.
    var args <- from_JV(input, type<TouchInjectArgs>)
    g_touch_injected_ever = true
    delete imgui_touch_injected
    imgui_touch_injected <- [for (f in args.touches); TouchPoint(id = f.id, pos = float2(f.x, f.y), began = f.began, ended = f.ended)]
    return JV((count = length(imgui_touch_injected)))
}
