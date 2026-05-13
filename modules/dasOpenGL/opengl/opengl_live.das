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
// at an fps throttle, with a max_seconds safety auto-stop.
//
// Readback uses a ring of N GL_PIXEL_PACK_BUFFER PBOs (N = capture_pbo_count,
// configurable). Each tick: kick off an async glReadPixels into pbos[K%N]
// (returns immediately), then map pbos[(K-N+1)%N] (oldest filled, GPU done
// by now) and feed it to stbi_apng_frame. record_stop drains the remaining
// N-1 in-flight buffers in order before glDeleteBuffers.
//
// Encode + file I/O happen on a worker thread inside the C++ writer (bounded
// queue, drops oldest on overflow). With the PBO ring the render loop never
// blocks on the GPU — sync glReadPixels was the bottleneck at 1280x720+.

var public capture_pbo_count : int = 4
    //! Number of GL_PIXEL_PACK_BUFFER PBOs in the readback ring. Higher = more
    //! GPU memory + latency, lower drop rate under encoder stalls. Clamped to
    //! [2, 8] at record_start. Overridable per-recording via the ``pbo_count``
    //! argument to ``record_start``.

struct RecorderState {
    writer            : void?    // StbiApngWriter* opaque handle
    fps               : float
    frame_interval_s  : float
    next_capture_t    : float
    max_seconds       : float
    start_t           : float
    frames_written    : int      // frames successfully passed to the apng writer
    frames_seen       : int      // total glReadPixels issues (writes to PBOs)
    width             : int
    height            : int
    file              : string
    pbos              : array<uint>
    pbo_delays_ms     : array<int>   // parallel ring: delay_ms captured at write time
    pbo_size          : int64
    prev_png_level    : int      // stb_image_write's global PNG zlib level at record_start; restored at record_stop
}

var private recorder : RecorderState
var private recorder_active : bool = false

struct RecordStartArgs {
    @optional file        : string = "record.apng"
    @optional fps         : float  = 30.0f
    @optional max_seconds : float  = 60.0f   // 0 = no cap
    @optional pbo_count   : int    = 0       // 0 = use capture_pbo_count default
}

struct RecordStartResult {
    file        : string
    fps         : float
    width       : int
    height      : int
    max_seconds : float
    pbo_count   : int
}

[live_command(description = "Begin APNG recording. Args: file, fps, max_seconds, pbo_count.")]
def record_start(input : JsonValue?) : JsonValue? {
    return JV((error = "already recording")) if (recorder_active)
    let args = from_JV(input, type<RecordStartArgs>)
    var w, h : int
    live_get_framebuffer_size(w, h)
    return JV((error = "no framebuffer")) if (w <= 0 || h <= 0)

    // Determine ring size: per-call arg > module default, clamped [2, 8].
    var n = args.pbo_count > 0 ? args.pbo_count : capture_pbo_count
    if (n < 2) {
        n = 2
    }
    if (n > 8) {
        n = 8
    }
    let pbo_size = int64(w) * int64(h) * 4l

    // Allocate the ring before opening the apng writer so a GL failure leaves
    // no half-state behind. glGenBuffers can't really fail with a sane driver,
    // but we still validate the IDs before proceeding.
    var pbos : array<uint>
    pbos |> resize(n)
    glGenBuffers(n, unsafe(addr(pbos[0])))
    for (id in pbos) {
        if (id == 0u) {
            glDeleteBuffers(n, unsafe(addr(pbos[0])))
            delete pbos
            return JV((error = "glGenBuffers failed"))
        }
    }
    for (id in pbos) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, id)
        glBufferData(GL_PIXEL_PACK_BUFFER, pbo_size, null, GL_STREAM_READ)
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0u)

    // Probe: stbi's default PNG compression level is 8 (high). That caps the
    // single worker thread to ~25-27 fps at 1280x720. Level 4 is the
    // tutorial-friendly sweet spot: 2-3x faster per frame at minor file-size
    // cost. We capture the previous level and restore it in record_stop so
    // unrelated PNG writes (screenshot, etc.) aren't affected. If keepers,
    // promote to a record_start arg.
    let prev_png_level = stbi_write_get_png_compression_level()
    stbi_write_set_png_compression_level(4)
    let writer_ptr = stbi_apng_begin(args.file, w, h, 4)
    if (writer_ptr == null) {
        stbi_write_set_png_compression_level(prev_png_level)
        glDeleteBuffers(n, unsafe(addr(pbos[0])))
        delete pbos
        return JV((error = "could not open file", file = args.file))
    }

    let effective_fps = max(args.fps, 1.0f)
    recorder.writer           = writer_ptr
    recorder.fps              = effective_fps
    recorder.frame_interval_s = 1.0f / effective_fps
    recorder.max_seconds      = args.max_seconds
    recorder.start_t          = get_uptime()
    recorder.next_capture_t   = recorder.start_t
    recorder.frames_written   = 0
    recorder.frames_seen      = 0
    recorder.width            = w
    recorder.height           = h
    recorder.file             = args.file
    recorder.pbos             <- pbos
    recorder.pbo_size         = pbo_size
    recorder.pbo_delays_ms |> resize(n)
    recorder.prev_png_level   = prev_png_level
    recorder_active           = true
    return JV(RecordStartResult(
        file        = args.file,
        fps         = effective_fps,
        width       = w,
        height      = h,
        max_seconds = args.max_seconds,
        pbo_count   = n
    ))
}

struct RecordStopResult {
    saved      : string
    frames     : int
    dropped    : int
    duration_s : float
    ok         : bool
}

def private harvest_one_pbo() : bool {
    //! Map pbos[read_idx % N], feed its pixel data to stbi_apng_frame with the
    //! delay_ms that was captured at write time. Returns true on success.
    let n = length(recorder.pbos)
    let read_idx = recorder.frames_written % n
    glBindBuffer(GL_PIXEL_PACK_BUFFER, recorder.pbos[read_idx])
    let ptr = unsafe(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY))
    if (ptr == null) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0u)
        return false
    }
    let ok = unsafe(stbi_apng_frame(recorder.writer, ptr, recorder.width * 4, recorder.pbo_delays_ms[read_idx]))
    // glUnmapBuffer returns GL_FALSE if the driver invalidated the buffer
    // contents while it was mapped — count it as a harvest failure so the
    // frame doesn't slip in with junk pixels.
    let unmap_ok = glUnmapBuffer(GL_PIXEL_PACK_BUFFER)
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0u)
    return false if (ok == 0 || !unmap_ok)
    recorder.frames_written++
    return true
}

[live_command(description = "Stop APNG recording. Returns saved path + frame count.")]
def record_stop(input : JsonValue?) : JsonValue? {
    return JV((error = "not recording")) if (!recorder_active)

    // Drain any PBOs still in flight (frames seen but not yet read back).
    // Best effort: a failed harvest here breaks the drain loop and leaves the
    // remaining in-flight frames unwritten — they don't show up in `dropped`
    // (that counts encoder queue overflows inside the writer, not GL-side
    // failures) but the resulting `frames` count reflects what actually made
    // it to disk.
    while (recorder.frames_written < recorder.frames_seen) {
        break if (!harvest_one_pbo())
    }

    let dropped = stbi_apng_dropped(recorder.writer)
    let ok      = stbi_apng_end(recorder.writer)
    let n = length(recorder.pbos)
    if (n > 0) {
        glDeleteBuffers(n, unsafe(addr(recorder.pbos[0])))
    }
    // Restore stb's global PNG zlib level so unrelated PNG writes (screenshot,
    // etc.) aren't permanently throttled to the tutorial-friendly setting.
    stbi_write_set_png_compression_level(recorder.prev_png_level)
    let elapsed = get_uptime() - recorder.start_t
    let saved   = recorder.file
    let frames  = recorder.frames_written
    recorder.writer = null
    delete recorder.pbos
    delete recorder.pbo_delays_ms
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
    pbo_count     : int
}

[live_command(description = "Recording status — active state, frame count, elapsed.")]
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
        dropped   = dropped,
        pbo_count = length(recorder.pbos)
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
    // by frame. Derive delay_ms from the actual gap so playback timing
    // reflects real spacing.
    let last_capture_t = recorder.next_capture_t - recorder.frame_interval_s
    let gap_s          = now - last_capture_t
    recorder.next_capture_t = now + recorder.frame_interval_s
    let n = length(recorder.pbos)

    // Write side: kick off async readback into the newest PBO. glReadPixels
    // with a null pointer + a bound GL_PIXEL_PACK_BUFFER queues the copy on
    // the GPU and returns immediately — no CPU block.
    let write_idx = recorder.frames_seen % n
    recorder.pbo_delays_ms[write_idx] = int(gap_s * 1000.0f)
    glBindBuffer(GL_PIXEL_PACK_BUFFER, recorder.pbos[write_idx])
    glReadPixels(0, 0, recorder.width, recorder.height, GL_RGBA, GL_UNSIGNED_BYTE, null)
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0u)
    recorder.frames_seen++

    // Read side: harvest the oldest filled PBO once the ring is full. By the
    // time we get here, the GPU has had N-1 frame periods to complete the
    // readback for the oldest buffer, so glMapBuffer is effectively non-
    // blocking under steady-state operation.
    if (recorder.frames_seen - recorder.frames_written >= n) {
        if (!harvest_one_pbo()) {
            record_stop(null)
        }
    }
}
