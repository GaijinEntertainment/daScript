options gen2
options _comment_hygiene = true

module dasllama_vision_io shared public

require stbimage
require daslib/safe_addr
require dasllama/dasllama_vision

//! Image decode to RGB8 `VisionImage` — the only dasLLAMA file that talks to stbimage.
//! Mirrors `dasllama_audio_io`: the engine facade takes decoded pixels, not a path, so
//! image-file IO stays out of the core module; apps require this optional companion when
//! they have a file or a byte blob (the server's data-URI arm). A zero-size image on a
//! decode failure. Decode parity with the mtmd oracle is free — it vendors stb_image too.

def public load_image_rgb(path : string) : VisionImage {
    var img = VisionImage()
    var x, y, comp : int
    var pixels = stbi_load(path, safe_addr(x), safe_addr(y), safe_addr(comp), 3)
    if (pixels == null) {
        return <- img
    }
    img.width = x
    img.height = y
    img.rgb |> reserve(x * y * 3)   // a decoded photo can exceed max_unreserved_size; size it exactly
    img.rgb |> resize(x * y * 3)
    for (i in range(x * y * 3)) {
        img.rgb[i] = unsafe(pixels[i])
    }
    stbi_image_free(pixels)
    return <- img
}

def public load_image_rgb(bytes : array<uint8>) : VisionImage {
    var img = VisionImage()
    if (empty(bytes)) {
        return <- img
    }
    var x, y, comp : int
    var pixels = stbi_load_from_memory(unsafe(addr(bytes[0])), length(bytes),
        safe_addr(x), safe_addr(y), safe_addr(comp), 3)
    if (pixels == null) {
        return <- img
    }
    img.width = x
    img.height = y
    img.rgb |> reserve(x * y * 3)   // a decoded photo can exceed max_unreserved_size; size it exactly
    img.rgb |> resize(x * y * 3)
    for (i in range(x * y * 3)) {
        img.rgb[i] = unsafe(pixels[i])
    }
    stbi_image_free(pixels)
    return <- img
}

//! Write `img` as a PNG. The engine's own dump knob (`DASLLAMA_VISION_DUMP`) writes PPM from
//! `dasllama_vision` instead — it must not pull stbimage into the core; this is for apps that
//! already require this module and want a viewer-friendly format.
def public dump_image_png(img : VisionImage; path : string) : bool {
    if (img.width == 0 || img.height == 0) {
        return false
    }
    return stbi_write_png(path, img.width, img.height, 3,
        unsafe(addr(img.rgb[0])), img.width * 3) != 0
}
