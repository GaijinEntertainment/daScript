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
