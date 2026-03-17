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

[live_command(description="Save framebuffer to PNG. Args: file")]
def screenshot(input : JsonValue?) : JsonValue? {
    var fname = "screenshot.png"
    if (input != null && input.value is _object) {
        let tab & = unsafe(input.value as _object)
        var fv = tab?["file"] ?? null
        if (fv != null && fv.value is _string) {
            fname = fv.value as _string
        }
    }
    var w, h : int
    live_get_framebuffer_size(w, h)
    if (w <= 0 || h <= 0) {
        return JV("\{\"error\": \"no framebuffer\"}")
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
    return JV("\{\"saved\": \"{fname}\", \"width\": {w}, \"height\": {h}}")
}
