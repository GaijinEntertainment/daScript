options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module opengl_live shared public

//! OpenGL live utilities for daslang-live.
//!
//! Provides live commands for OpenGL applications:
//!   screenshot — captures the current framebuffer to a PNG file

require opengl/opengl_boost
require live/glfw_live
require math
require stbimage
require daslib/json
require daslib/json_boost
require live/live_commands
require live_host

// ============================================================================
// Render statistics
// ============================================================================

//! Per-frame render statistics. Reset at the beginning of each frame
//! by calling ``gl_stats_reset()``, then query with the ``gl_stats`` live command.
struct GlStats {
    draw_calls : int
    triangles : int
    lines : int
    points : int
    vertices : int  // total vertices submitted
}

var gl_render_stats : GlStats

//! Reset render statistics. Call at the beginning of each frame (e.g. in ``update()``).
def gl_stats_reset() {
    gl_render_stats = GlStats()
}

//! Record a draw call. Call after each ``glDrawArrays`` or ``glDrawElements``.
def gl_stats_draw(mode, count : int) {
    gl_render_stats.draw_calls++
    gl_render_stats.vertices += count
    if (mode == int(GL_TRIANGLES)) {
        gl_render_stats.triangles += count / 3
    } elif (mode == int(GL_TRIANGLE_FAN)) {
        gl_render_stats.triangles += count - 2
    } elif (mode == int(GL_TRIANGLE_STRIP)) {
        gl_render_stats.triangles += count - 2
    } elif (mode == int(GL_LINES)) {
        gl_render_stats.lines += count / 2
    } elif (mode == int(GL_LINE_LOOP)) {
        gl_render_stats.lines += count
    } elif (mode == int(GL_LINE_STRIP)) {
        gl_render_stats.lines += count - 1
    } elif (mode == int(GL_POINTS)) {
        gl_render_stats.points += count
    }
}

struct GlStatsResult {
    draw_calls : int
    triangles : int
    lines : int
    points : int
    vertices : int
    fps : float
}

[live_command(description="OpenGL render statistics for the last frame")]
def gl_stats(input : JsonValue?) : JsonValue? {
    return JV(GlStatsResult(
        draw_calls = gl_render_stats.draw_calls,
        triangles = gl_render_stats.triangles,
        lines = gl_render_stats.lines,
        points = gl_render_stats.points,
        vertices = gl_render_stats.vertices,
        fps = 1.0 / get_dt()
    ))
}

// ============================================================================
// Screenshot
// ============================================================================

struct ScreenshotArgs {
    file : string = "screenshot.png"
}

struct ScreenshotResult {
    saved : string
    width : int
    height : int
}

[live_command(description="Save framebuffer to PNG. Args: file")]
def screenshot(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<ScreenshotArgs>)
    var fname = args.file
    if (empty(fname)) { fname = "screenshot.png"; }
    var w, h : int
    live_get_framebuffer_size(w, h)
    return JV("no framebuffer") if (w <= 0 || h <= 0)
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    unsafe {
        glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, addr(pixels[0]))
    }
    // Flip vertically (OpenGL origin is bottom-left)
    let row_bytes = w * 4
    var temp : array<uint8>
    temp |> resize(row_bytes)
    for (y in range(h / 2)) {
        let top = y * row_bytes
        let bot = (h - 1 - y) * row_bytes
        unsafe {
            memcpy(addr(temp[0]), addr(pixels[top]), row_bytes)
            memcpy(addr(pixels[top]), addr(pixels[bot]), row_bytes)
            memcpy(addr(pixels[bot]), addr(temp[0]), row_bytes)
        }
    }
    unsafe {
        stbi_write_png(fname, w, h, 4, addr(pixels[0]), w * 4)
    }
    return JV(ScreenshotResult(saved = fname, width = w, height = h))
}

// ============================================================================
// APNG video recording
// ============================================================================
//
// Three live commands wrap a streaming APNG writer (dasStbImage's
// stbi_apng_begin/frame/end). Capture runs from a [before_update] hook
// at an fps throttle, with a max_seconds safety auto-stop. Encode + file I/O
// happen on a worker thread inside the C++ writer — the render loop only
// pays the cost of glReadPixels + a memcpy into the bounded queue.

struct RecorderState {
    writer            : void?    // StbiApngWriter* opaque handle
    fps               : float
    frame_interval_s  : float
    next_capture_t    : float
    max_seconds       : float
    start_t           : float
    frames_written    : int
    width             : int
    height            : int
    file              : string
}

var private recorder : RecorderState
var private recorder_active : bool = false

struct RecordStartArgs {
    @optional file        : string = "record.apng"
    @optional fps         : float  = 30.0f
    @optional max_seconds : float  = 60.0f   // 0 = no cap
}

struct RecordStartResult {
    file        : string
    fps         : float
    width       : int
    height      : int
    max_seconds : float
}

[live_command(description="Begin APNG recording. Args: file, fps, max_seconds.")]
def record_start(input : JsonValue?) : JsonValue? {
    return JV((error = "already recording")) if (recorder_active)
    let args = from_JV(input, type<RecordStartArgs>)
    var w, h : int
    live_get_framebuffer_size(w, h)
    return JV((error = "no framebuffer")) if (w <= 0 || h <= 0)
    var writer_ptr : void?
    unsafe {
        writer_ptr = stbi_apng_begin(args.file, w, h, 4)
    }
    return JV((error = "could not open file", file = args.file)) if (writer_ptr == null)
    let effective_fps = max(args.fps, 1.0f)
    recorder.writer           = writer_ptr
    recorder.fps              = effective_fps
    recorder.frame_interval_s = 1.0f / effective_fps
    recorder.max_seconds      = args.max_seconds
    recorder.start_t          = get_uptime()
    recorder.next_capture_t   = recorder.start_t
    recorder.frames_written   = 0
    recorder.width            = w
    recorder.height           = h
    recorder.file             = args.file
    recorder_active           = true
    return JV(RecordStartResult(
        file        = args.file,
        fps         = effective_fps,
        width       = w,
        height      = h,
        max_seconds = args.max_seconds
    ))
}

struct RecordStopResult {
    saved      : string
    frames     : int
    dropped    : int
    duration_s : float
    ok         : bool
}

[live_command(description="Stop APNG recording. Returns saved path + frame count.")]
def record_stop(input : JsonValue?) : JsonValue? {
    return JV((error = "not recording")) if (!recorder_active)
    var dropped : int
    var ok      : int
    unsafe {
        dropped = stbi_apng_dropped(recorder.writer)
        ok      = stbi_apng_end(recorder.writer)
    }
    let elapsed = get_uptime() - recorder.start_t
    let saved   = recorder.file
    let frames  = recorder.frames_written
    recorder.writer = null
    recorder_active = false
    return JV(RecordStopResult(
        saved      = saved,
        frames     = frames,
        dropped    = dropped,
        duration_s = elapsed,
        ok         = ok != 0
    ))
}

struct RecordStatusResult {
    active        : bool
    file          : string
    frames        : int
    elapsed_s     : float
    fps           : float
    width         : int
    height        : int
    dropped       : int
}

[live_command(description="Recording status — active state, frame count, elapsed.")]
def record_status(input : JsonValue?) : JsonValue? {
    var dropped = 0
    if (recorder_active) {
        unsafe {
            dropped = stbi_apng_dropped(recorder.writer)
        }
    }
    return JV(RecordStatusResult(
        active    = recorder_active,
        file      = recorder.file,
        frames    = recorder.frames_written,
        elapsed_s = recorder_active ? get_uptime() - recorder.start_t : 0.0f,
        fps       = recorder.fps,
        width     = recorder.width,
        height    = recorder.height,
        dropped   = dropped
    ))
}

// Capture hook — fires every frame via [before_update]. No-op when not active.
[before_update]
def record_tick() {
    return if (!recorder_active)
    let now = get_uptime()
    if (recorder.max_seconds > 0.0f && (now - recorder.start_t) >= recorder.max_seconds) {
        record_stop(null)
        return
    }
    return if (now < recorder.next_capture_t)
    // Skip-missed scheduling: after a stall (long frame, gc pause, etc.)
    // resync the deadline to `now + interval` instead of catching up frame
    // by frame. The catch-up form would burst-capture every tick until it
    // overtook `now`, which both exceeds the requested fps and produces
    // wrong playback timing because delay_ms below is a constant. Derive
    // delay_ms from the actual gap so playback reflects real spacing.
    let last_capture_t = recorder.next_capture_t - recorder.frame_interval_s
    let gap_s          = now - last_capture_t
    recorder.next_capture_t = now + recorder.frame_interval_s
    let row_bytes = recorder.width * 4
    var pixels : array<uint8>
    pixels |> resize(recorder.width * recorder.height * 4)
    unsafe {
        glReadPixels(0, 0, recorder.width, recorder.height, GL_RGBA, GL_UNSIGNED_BYTE, addr(pixels[0]))
    }
    let delay_ms = int(gap_s * 1000.0f)
    var ok : int
    unsafe {
        ok = stbi_apng_frame(recorder.writer, addr(pixels[0]), row_bytes, delay_ms)
    }
    // Local `var array<T>` doesn't finalize on scope exit (no smart_ptr). Explicit
    // delete avoids growing the heap by width*height*4 bytes per captured frame.
    delete pixels
    if (ok != 0) {
        recorder.frames_written++
    } else {
        record_stop(null)
    }
}
