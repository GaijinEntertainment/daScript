options gen2
options _comment_hygiene = true

module dasllama_vision shared public

require math
require daslib/fio
require dasllama/dasllama_env

//! The vision preprocessing rail: dynamic-resolution geometry, the letterbox resize
//! (aspect-preserving bilinear + center pad on black), and u8→f32 normalize. Conventions
//! mirror llama.cpp's mtmd `img_tool` exactly — corner-aligned bilinear ratio, float lerp,
//! truncating u8 cast, PAD_CEIL letterbox — so the tier-0 oracle gates are bit-exact by
//! construction (`vision_plan.md` §pipeline, dumps in models/gemma4-vision-oracle/).

struct public VisionImage {
    width : int
    height : int
    rgb : array<uint8>   // interleaved RGB, row-major
}

//! The target canvas under dynamic resolution: sides round to the nearest `align` multiple,
//! then floor-align ÷ β over `max_tokens` / ceil-align × β under `min_tokens`. Never yields
//! more than `max_tokens` patches — an extreme aspect ratio clamps the long side.
def public vision_target_size(width, height : int; align, min_tokens, max_tokens : int) : int2 {
    let f = float(align)
    let min_pixels = int64(min_tokens) * int64(align) * int64(align)
    let max_pixels = int64(max_tokens) * int64(align) * int64(align)
    let src_pixels = int64(height) * int64(width)
    // ties AWAY like std::round — roundi is trunc(x+copysign(0.5)) on every platform
    var w_bar = max(align, roundi(float(width) / f) * align)
    var h_bar = max(align, roundi(float(height) / f) * align)
    if (int64(h_bar) * int64(w_bar) > max_pixels) {
        let beta = sqrt(float(src_pixels) / float(max_pixels))
        w_bar = max(align, floori(float(width) / beta / f) * align)
        h_bar = max(align, floori(float(height) / beta / f) * align)
    } elif (int64(h_bar) * int64(w_bar) < min_pixels) {
        let beta = sqrt(float(min_pixels) / float(src_pixels))
        w_bar = ceili(float(width) * beta / f) * align
        h_bar = ceili(float(height) * beta / f) * align
    }
    while (int64(w_bar / align) * int64(h_bar / align) > int64(max_tokens)) {
        if (w_bar >= h_bar && w_bar > align) { w_bar -= align } elif (h_bar > align) { h_bar -= align } else { break }
    }
    return int2(w_bar, h_bar)
}

//! Soft tokens a `target` canvas produces: one per align×align superpatch.
def public vision_token_count(target : int2; align : int) : int => (target.x / align) * (target.y / align)

// mirrors img_tool::resize_bilinear: corner-aligned (src-1)/(dst-1) ratio, truncating u8 cast.
// The lerp accumulates in double - single-rounded like the fmadd clang contracts the oracle's
// lerp into, and deterministic across interp/JIT/AOT (unfused floats missed 2/112896 px by 1).
def private resize_bilinear(src : VisionImage; tw, th : int) : VisionImage {
    var dst = VisionImage(width = tw, height = th)
    dst.rgb |> reserve(tw * th * 3)
    dst.rgb |> resize(tw * th * 3)
    let x_ratio = tw > 1 ? float(src.width - 1) / float(tw - 1) : 0.0
    let y_ratio = th > 1 ? float(src.height - 1) / float(th - 1) : 0.0
    for (y in range(th)) {
        let py = float(y) * y_ratio
        let y0 = min(int(py), src.height - 1)
        let y1 = min(y0 + 1, src.height - 1)
        let yf = py - float(y0)
        for (x in range(tw)) {
            let px = float(x) * x_ratio
            let x0 = min(int(px), src.width - 1)
            let x1 = min(x0 + 1, src.width - 1)
            let xf = px - float(x0)
            let p00 = (y0 * src.width + x0) * 3
            let p10 = (y0 * src.width + x1) * 3
            let p01 = (y1 * src.width + x0) * 3
            let p11 = (y1 * src.width + x1) * 3
            let d = (y * tw + x) * 3
            for (c in range(3)) {
                let a = float(src.rgb[p00 + c])
                let top = float(double(a) + double(float(src.rgb[p10 + c]) - a) * double(xf))
                let b = float(src.rgb[p01 + c])
                let bottom = float(double(b) + double(float(src.rgb[p11 + c]) - b) * double(xf))
                dst.rgb[d + c] = uint8(float(double(top) + double(bottom - top) * double(yf)))
            }
        }
    }
    return <- dst
}

// identity resize is a copy (the bilinear would be bit-equal, this is just cheaper)
def private content_for(src : VisionImage; new_w, new_h : int) : VisionImage {
    if (new_w == src.width && new_h == src.height) {
        var same = VisionImage(width = src.width, height = src.height)
        same.rgb := src.rgb
        return <- same
    }
    return <- resize_bilinear(src, new_w, new_h)
}

//! Letterbox `src` onto `target` (mtmd `img_tool::resize`, PAD_CEIL): content scale =
//! min(tw/sw, th/sh), sides min(ceil(side×scale), target), bilinear, composited centered on
//! a BLACK canvas at floor((target−content)/2); an unchanged overall size is a plain copy.
def public vision_letterbox(src : VisionImage; target : int2) : VisionImage {
    if (target.x == src.width && target.y == src.height) {
        var same = VisionImage(width = src.width, height = src.height)
        same.rgb := src.rgb
        return <- same
    }
    let scale_w = float(target.x) / float(src.width)
    let scale_h = float(target.y) / float(src.height)
    let scale = min(scale_w, scale_h)
    let new_w = min(ceili(float(src.width) * scale), target.x)
    let new_h = min(ceili(float(src.height) * scale), target.y)
    let content <- content_for(src, new_w, new_h)
    var dst = VisionImage(width = target.x, height = target.y)
    dst.rgb |> reserve(target.x * target.y * 3)
    dst.rgb |> resize(target.x * target.y * 3)   // fresh elements are zero — the black canvas
    let off_x = (target.x - new_w) / 2
    let off_y = (target.y - new_h) / 2
    for (y in range(new_h)) {
        let srow = y * new_w * 3
        let drow = ((y + off_y) * target.x + off_x) * 3
        for (i in range(new_w * 3)) {
            dst.rgb[drow + i] = content.rgb[srow + i]
        }
    }
    return <- dst
}

//! Write `img` as a binary PPM (P6) — pure das, no image library, so the engine-side dump knob
//! never pulls stbimage into the core chat module. macOS Preview and every viewer open P6.
def public write_image_ppm(img : VisionImage; path : string) : bool {
    if (img.width <= 0 || img.height <= 0) {
        return false
    }
    let f = fopen(path, "wb")
    if (f == null) {
        return false
    }
    fwrite(f, "P6\n{img.width} {img.height}\n255\n")
    fwrite(f, img.rgb)
    fclose(f)
    return true
}

//! The DASLLAMA_VISION_DUMP knob: when set, write `img` as <prefix><tag>.ppm — the human smoke
//! test; a red never requires new instrumentation before someone can see what the model saw.
def public maybe_dump_vision(img : VisionImage; tag : string) {
    let prefix = g_env_vision.vision_dump
    if (empty(prefix)) {
        return
    }
    let path = "{prefix}{tag}.ppm"
    if (write_image_ppm(img, path)) {
        to_log(LOG_INFO, "dasLLAMA vision: dumped {img.width}x{img.height} letterboxed image to {path}\n")
    } else {
        to_log(LOG_WARNING, "dasLLAMA vision: could not write the dump to {path}\n")
    }
}

//! u8 → f32 planes for the embedder: exactly x/255, interleaved layout preserved — gemma-4's
//! `image_mean`/`image_std` are identity and nothing else touches the values.
def public vision_normalize(img : VisionImage) : array<float> {
    var planes : array<float>
    planes |> reserve(length(img.rgb))
    planes |> resize(length(img.rgb))
    for (d, s in planes, img.rgb) {
        d = float(s) / 255.0
    }
    return <- planes
}
