options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vision_io shared public

require dasllama/dasllama_lint public
require stbimage
require daslib/safe_addr
require dasllama/dasllama_vision

//! Image decode to RGB8 `VisionImage` — the only dasLLAMA file that talks to stbimage.
//! Takes a path or a byte blob; the engine facade takes pixels, not paths (`dasllama_audio_io`'s shape).
//! Zero-size image on decode failure, or when `max_pixels` (0 = uncapped) is exceeded.
//! The cap reads the header only — an oversized file never reaches the decode allocation.
//! Decode parity with the mtmd oracle is free — it vendors stb_image too.

def private image_from_stb(pixels; x, y : int) : VisionImage {
    var img = VisionImage(width = x, height = y)
    let n = int64(x) * int64(y) * 3l   // widen BEFORE the multiply: an int product wraps past ~715 MP
    img.rgb |> reserve(n)   // a decoded photo can exceed max_unreserved_size; size it exactly
    img.rgb |> resize(n)
    unsafe {
        memcpy(addr<void?>(img.rgb[0]), reinterpret<void?>(pixels), n)
    }
    return <- img
}

def public load_image_rgb(path : string; max_pixels : int64 = 0l) : VisionImage {
    var img = VisionImage()
    var x, y, comp : int
    if (max_pixels > 0l) {
        if (stbi_info(path, safe_addr(x), safe_addr(y), safe_addr(comp)) == 0
                || int64(x) * int64(y) > max_pixels) {
            return <- img
        }
    }
    var pixels = stbi_load(path, safe_addr(x), safe_addr(y), safe_addr(comp), 3)
    if (pixels == null) {
        return <- img
    }
    img <- image_from_stb(pixels, x, y)
    stbi_image_free(pixels)
    return <- img
}

//! Header-only probe: the encoded image's dimensions without decoding it, `int2(0, 0)` when the
//! header is unreadable. Lets a caller tell an over-cap image from a corrupt one after a capped
//! `load_image_rgb` returns zero-size.
def public probe_image_size(bytes : array<uint8>) : int2 {
    if (empty(bytes)) {
        return int2(0, 0)
    }
    var x, y, comp : int
    if (stbi_info_from_memory(unsafe(addr(bytes[0])), length(bytes),
            safe_addr(x), safe_addr(y), safe_addr(comp)) == 0) {
        return int2(0, 0)
    }
    return int2(x, y)
}

def public load_image_rgb(bytes : array<uint8>; max_pixels : int64 = 0l) : VisionImage {
    var img = VisionImage()
    if (empty(bytes)) {
        return <- img
    }
    var x, y, comp : int
    if (max_pixels > 0l) {
        if (stbi_info_from_memory(unsafe(addr(bytes[0])), length(bytes),
                safe_addr(x), safe_addr(y), safe_addr(comp)) == 0
                || int64(x) * int64(y) > max_pixels) {
            return <- img
        }
    }
    var pixels = stbi_load_from_memory(unsafe(addr(bytes[0])), length(bytes),
        safe_addr(x), safe_addr(y), safe_addr(comp), 3)
    if (pixels == null) {
        return <- img
    }
    img <- image_from_stb(pixels, x, y)
    stbi_image_free(pixels)
    return <- img
}

//! Write `img` as a PNG. The engine's own dump knob (`DASLLAMA_VISION_DUMP`) writes PPM from
//! `dasllama_vision` instead — it must not pull stbimage into the core; this is for apps that
//! already require this module and want a viewer-friendly format.
def public dump_image_png(img : VisionImage; path : string) : bool {
    if (img.width == 0 || img.height == 0
            || long_length(img.rgb) < int64(img.width) * int64(img.height) * 3l) {
        return false
    }
    return stbi_write_png(path, img.width, img.height, 3,
        unsafe(addr(img.rgb[0])), img.width * 3) != 0
}
