options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module stbimage_ttf shared public

//! High-level TrueType font loading and text rendering API.
//!
//! Uses stb_truetype packing API for font atlas generation.
//! Font bitmaps are stored as ``Image`` from ``stbimage_boost``,
//! enabling save/resize/convert via the image API.

require stbtruetype
require stbimage/stbimage_boost
require daslib/safe_addr
require math
require fio
require strings

struct public Font {
    //! Font loaded via packing API with atlas stored as Image.
    fontinfo   : stbtt_fontinfo    //! Initialized font info (for metrics queries).
    chardata   : array<stbtt_packedchar>  //! Packed character data for the loaded range.
    bitmap     : Image             //! Font atlas as 1-channel uint8 Image.
    font_data  : array<uint8>      //! Raw TTF file data (must outlive fontinfo).
    pixel_height : float           //! Font size in pixels.
    char_range : int2              //! (first_char, num_chars) of the packed range.
    tex        : uint64            //! Opaque texture handle (0 by default, set by graphics layer).
}

def public load_ttf(fname : string; pixel_height : float = 32.0; atlas_dim : int2 = int2(512, 512);
                    first_char : int = 32; num_chars : int = 96; oversampling : int = 1) : Font {
    //! Load a TrueType font from file and pack a character range into an atlas.
    //!
    //! Returns a Font with bitmap stored as a 1-channel Image.
    //! If loading fails, returns a Font with an invalid bitmap.
    var font : Font
    font.pixel_height = pixel_height
    font.char_range = int2(first_char, num_chars)
    fopen(fname, "rb") <| $(f) {
        if (f != null) {
            fmap(f) <| $(mapped) {
                font.font_data := mapped
            }
        }
    }
    if (empty(font.font_data)) {
        return <- font
    }
    // init font info for metrics queries
    unsafe {
        let offset = stbtt_GetFontOffsetForIndex(addr(font.font_data[0]), 0)
        stbtt_InitFont(addr(font.fontinfo), addr(font.font_data[0]), offset)
    }
    // pack font into atlas
    font.chardata |> resize(num_chars)
    font.bitmap = make_image(atlas_dim.x, atlas_dim.y, 1)
    var spc : stbtt_pack_context
    unsafe {
        stbtt_PackBegin(addr(spc), addr(font.bitmap.bytes[0]), atlas_dim.x, atlas_dim.y, 0, 1, null)
        stbtt_PackSetOversampling(addr(spc), uint(oversampling), uint(oversampling))
        stbtt_PackFontRange(addr(spc), addr(font.font_data[0]), 0, pixel_height, first_char, num_chars, addr(font.chardata[0]))
        stbtt_PackEnd(addr(spc))
    }
    return <- font
}

def public font_metrics(font : Font) : tuple<ascent : float; descent : float; line_height : float> {
    //! Returns font metrics in pixels: ascent (baseline to top), descent (baseline to bottom, negative),
    //! and recommended line height (ascent - descent + lineGap).
    var ascent, descent, line_gap : int
    unsafe {
        stbtt_GetFontVMetrics(addr(font.fontinfo), safe_addr(ascent), safe_addr(descent), safe_addr(line_gap))
        let scale = stbtt_ScaleForPixelHeight(addr(font.fontinfo), font.pixel_height)
        return (ascent = float(ascent) * scale, descent = float(descent) * scale, line_height = float(ascent - descent + line_gap) * scale)
    }
}

def public render_text(var dst : Image; font : Font; text : string; x, y : int; r, g, b : int) {
    //! Render text onto a 4-channel RGBA image using pixel-to-pixel alpha blit.
    //!
    //! ``x, y`` is the position of the text baseline origin.
    //! Each glyph is blitted from the 1-channel font atlas with the given (r,g,b) color.
    //! Uses packedchar fields directly for pixel-to-pixel blit.
    //! Best results with oversampling=1 (the default for software rendering).
    //! Clips to destination bounds automatically.
    assert(dst.channels == 4 && dst.bpc == 1, "render_text requires 4-channel uint8 destination")
    let first = font.char_range.x
    let num_chars = font.char_range.y
    var cx = float(x)
    for (ch in text) {
        let idx = ch - first
        if (idx >= 0 && idx < num_chars) {
            let pc = font.chardata[idx]
            // atlas source rect (integer pixel coords from packing)
            let sx = int(pc.x0)
            let sy = int(pc.y0)
            let gw = int(pc.x1) - sx
            let gh = int(pc.y1) - sy
            // screen position: cursor + offset, rounded to int
            let gx = int(round(cx + pc.xoff))
            let gy = y + int(round(pc.yoff))
            dst.blit_alpha(font.bitmap, sx, sy, gx, gy, gw, gh, r, g, b)
            cx += pc.xadvance
        }
    }
}
