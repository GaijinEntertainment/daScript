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
    if (w <= 0 || h <= 0) {
        return JV("no framebuffer")
    }
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
