options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module stbimage_ttf shared public

//! TrueType font API with safe wrappers over stb_truetype.
//!
//! Two layers:
//!   - Low-level safe wrappers (``stbtt_*``) that take ``stbtt_fontinfo`` by reference.
//!     All ``unsafe`` code is confined here.
//!   - High-level ``Font`` API for loading, metrics, shape queries, and rendering.
//!     Zero ``unsafe`` blocks — calls low-level wrappers internally.

require stbtruetype public
require stbimage/stbimage_boost
require daslib/safe_addr
require math
require daslib/fio
require strings

// ============================================================================
// Low-level safe wrappers over stbtt_* C bindings
// ============================================================================
//
// Each function wraps one or more raw stbtt_* calls, hiding the unsafe
// addr() operations internally. Take stbtt_fontinfo by reference.
// Public so advanced users can use them without the Font struct.

def public stbtt_init(var info : stbtt_fontinfo; var data : array<uint8>; font_index : int = 0) : bool {
    //! Initialize a ``stbtt_fontinfo`` from font data. Returns ``true`` on success.
    unsafe {
        let offset = stbtt_GetFontOffsetForIndex(addr(data[0]), font_index)
        if (offset < 0) {
            return false
        }
        return stbtt_InitFont(addr(info), addr(data[0]), offset) != 0
    }
}

def public stbtt_scale_for_pixel_height(info : stbtt_fontinfo; pixel_height : float) : float {
    //! Compute scale factor for a given pixel height.
    return stbtt_ScaleForPixelHeight(unsafe(addr(info)), pixel_height)
}

def public stbtt_scale_for_em(info : stbtt_fontinfo; pixels : float) : float {
    //! Compute scale factor for mapping em to pixels.
    return stbtt_ScaleForMappingEmToPixels(unsafe(addr(info)), pixels)
}

def public stbtt_font_vmetrics(info : stbtt_fontinfo) : tuple<ascent : int; descent : int; line_gap : int> {
    //! Returns vertical metrics in font units: ascent, descent, line gap.
    var ascent, descent, line_gap : int
    stbtt_GetFontVMetrics(unsafe(addr(info)), safe_addr(ascent), safe_addr(descent), safe_addr(line_gap))
    return (ascent = ascent, descent = descent, line_gap = line_gap)
}

def public stbtt_codepoint_hmetrics(info : stbtt_fontinfo; codepoint : int) : tuple<advance_width : int; left_side_bearing : int> {
    //! Returns horizontal metrics in font units for a codepoint.
    var advance_width, left_side_bearing : int
    stbtt_GetCodepointHMetrics(unsafe(addr(info)), codepoint, safe_addr(advance_width), safe_addr(left_side_bearing))
    return (advance_width = advance_width, left_side_bearing = left_side_bearing)
}

def public stbtt_codepoint_kern(info : stbtt_fontinfo; ch1, ch2 : int) : int {
    //! Returns kerning advance between two codepoints in font units.
    return stbtt_GetCodepointKernAdvance(unsafe(addr(info)), ch1, ch2)
}

def public stbtt_codepoint_box(info : stbtt_fontinfo; codepoint : int) : int4 {
    //! Returns glyph bounding box ``int4(x0, y0, x1, y1)`` in font units.
    //! Returns ``int4(0)`` if the codepoint has no outline.
    var x0, y0, x1, y1 : int
    stbtt_GetCodepointBox(unsafe(addr(info)), codepoint, safe_addr(x0), safe_addr(y0), safe_addr(x1), safe_addr(y1))
    return int4(x0, y0, x1, y1)
}

def public stbtt_font_bbox(info : stbtt_fontinfo) : int4 {
    //! Returns font bounding box ``int4(x0, y0, x1, y1)`` in font units.
    var x0, y0, x1, y1 : int
    stbtt_GetFontBoundingBox(unsafe(addr(info)), safe_addr(x0), safe_addr(y0), safe_addr(x1), safe_addr(y1))
    return int4(x0, y0, x1, y1)
}

def public stbtt_codepoint_shape(info : stbtt_fontinfo; codepoint : int; blk : block<(v : stbtt_vertex_das) : void>) {
    //! Iterate glyph shape vertices for a codepoint via callback.
    stbtt_GetCodepointShape(unsafe(addr(info)), codepoint, blk)
}

def public stbtt_codepoint_shape_count(info : stbtt_fontinfo; codepoint : int) : int {
    //! Returns the number of shape vertices for a codepoint.
    return stbtt_GetCodepointShape_count(unsafe(addr(info)), codepoint)
}

def public stbtt_find_glyph(info : stbtt_fontinfo; codepoint : int) : int {
    //! Returns glyph index for a codepoint. Returns 0 if not found.
    return stbtt_FindGlyphIndex(unsafe(addr(info)), codepoint)
}

def public stbtt_pack(var data : array<uint8>; var pixels : array<uint8>; atlas_w, atlas_h : int;
                      var chardata : array<stbtt_packedchar>;
                      font_size : float; first_char, num_chars : int;
                      oversampling : int = 1; font_index : int = 0) {
    //! Pack a character range into an atlas bitmap.
    //! Wraps PackBegin + SetOversampling + PackFontRange + PackEnd.
    var spc : stbtt_pack_context
    unsafe {
        stbtt_PackBegin(addr(spc), addr(pixels[0]), atlas_w, atlas_h, 0, 1, null)
        stbtt_PackSetOversampling(addr(spc), uint(oversampling), uint(oversampling))
        stbtt_PackFontRange(addr(spc), addr(data[0]), font_index, font_size, first_char, num_chars, addr(chardata[0]))
        stbtt_PackEnd(addr(spc))
    }
}

def public stbtt_num_fonts(var data : array<uint8>) : int {
    //! Returns the number of fonts in a TrueType collection (.ttc), or 1 for a single .ttf.
    return stbtt_GetNumberOfFonts(unsafe(addr(data[0])))
}

def public stbtt_codepoint_bitmap_box(info : stbtt_fontinfo; codepoint : int; scale_x, scale_y : float) : int4 {
    //! Returns pixel-space bounding box ``int4(ix0, iy0, ix1, iy1)`` for a codepoint bitmap at the given scale.
    var ix0, iy0, ix1, iy1 : int
    stbtt_GetCodepointBitmapBox(unsafe(addr(info)), codepoint, scale_x, scale_y,
                                safe_addr(ix0), safe_addr(iy0), safe_addr(ix1), safe_addr(iy1))
    return int4(ix0, iy0, ix1, iy1)
}

def public stbtt_codepoint_bitmap_box_subpixel(info : stbtt_fontinfo; codepoint : int;
                                               scale_x, scale_y, shift_x, shift_y : float) : int4 {
    //! Returns pixel-space bounding box with subpixel shift.
    var ix0, iy0, ix1, iy1 : int
    stbtt_GetCodepointBitmapBoxSubpixel(unsafe(addr(info)), codepoint, scale_x, scale_y, shift_x, shift_y,
                                        safe_addr(ix0), safe_addr(iy0), safe_addr(ix1), safe_addr(iy1))
    return int4(ix0, iy0, ix1, iy1)
}

def public stbtt_codepoint_bitmap(info : stbtt_fontinfo; codepoint : int; scale_x, scale_y : float) : tuple<image : Image; offset : int2> {
    //! Renders a codepoint glyph bitmap at the given scale. Returns a 1-channel ``Image`` and pixel offset.
    //! The returned Image owns the pixel data (copied from stb allocation).
    var w, h, xoff, yoff : int
    var ptr = stbtt_GetCodepointBitmap(unsafe(addr(info)), scale_x, scale_y, codepoint,
                                       safe_addr(w), safe_addr(h), safe_addr(xoff), safe_addr(yoff))
    if (ptr == null || w <= 0 || h <= 0) {
        return (image = Image(), offset = int2(0))
    }
    var img = make_image(w, h, 1)
    unsafe {
        memcpy(addr(img.bytes[0]), ptr, w * h)
    }
    stbtt_FreeBitmap(ptr, null)
    return (image = img, offset = int2(xoff, yoff))
}

def public stbtt_codepoint_bitmap_subpixel(info : stbtt_fontinfo; codepoint : int;
                                           scale_x, scale_y, shift_x, shift_y : float) : tuple<image : Image; offset : int2> {
    //! Renders a codepoint glyph bitmap with subpixel shift. Returns a 1-channel ``Image`` and pixel offset.
    var w, h, xoff, yoff : int
    var ptr = stbtt_GetCodepointBitmapSubpixel(unsafe(addr(info)), scale_x, scale_y, shift_x, shift_y, codepoint,
                                               safe_addr(w), safe_addr(h), safe_addr(xoff), safe_addr(yoff))
    if (ptr == null || w <= 0 || h <= 0) {
        return (image = Image(), offset = int2(0))
    }
    var img = make_image(w, h, 1)
    unsafe {
        memcpy(addr(img.bytes[0]), ptr, w * h)
    }
    stbtt_FreeBitmap(ptr, null)
    return (image = img, offset = int2(xoff, yoff))
}

def public stbtt_make_codepoint_bitmap(info : stbtt_fontinfo; var buf : array<uint8>;
                                       out_w, out_h, out_stride : int;
                                       scale_x, scale_y : float; codepoint : int) {
    //! Renders a codepoint glyph into a caller-provided buffer.
    //! ``buf`` must be at least ``out_h * out_stride`` bytes.
    stbtt_MakeCodepointBitmap(unsafe(addr(info)), unsafe(addr(buf[0])), out_w, out_h, out_stride, scale_x, scale_y, codepoint)
}

def public stbtt_make_codepoint_bitmap_subpixel(info : stbtt_fontinfo; var buf : array<uint8>;
                                                out_w, out_h, out_stride : int;
                                                scale_x, scale_y, shift_x, shift_y : float; codepoint : int) {
    //! Renders a codepoint glyph with subpixel shift into a caller-provided buffer.
    stbtt_MakeCodepointBitmapSubpixel(unsafe(addr(info)), unsafe(addr(buf[0])), out_w, out_h, out_stride,
                                      scale_x, scale_y, shift_x, shift_y, codepoint)
}

def public stbtt_packed_quad(chardata : array<stbtt_packedchar>; atlas_w, atlas_h, char_index : int;
                             var xpos : float&; align_to_integer : bool = false) : stbtt_aligned_quad {
    //! Returns the packed quad for a character, advancing ``xpos``.
    //! ``char_index`` is relative to the first packed character.
    var ypos = 0.0
    var quad : stbtt_aligned_quad
    unsafe {
        stbtt_GetPackedQuad(addr(chardata[0]), atlas_w, atlas_h, char_index,
                            addr(xpos), safe_addr(ypos), safe_addr(quad), align_to_integer ? 1 : 0)
    }
    return quad
}

// ============================================================================
// Font struct and high-level API
// ============================================================================
//
// All functions below use the low-level wrappers above.
// Zero unsafe blocks in this section.

struct public Font {
    //! Font loaded from a TTF file. Supports both atlas-based rendering
    //! (via ``load_ttf``) and lightweight metrics/shape queries (via ``load_font``).
    fontinfo     : stbtt_fontinfo          //! Initialized font info (for metrics queries).
    chardata     : array<stbtt_packedchar> //! Packed character data for the loaded range.
    bitmap       : Image                   //! Font atlas as 1-channel uint8 Image.
    font_data    : array<uint8>            //! Raw TTF file data (must outlive fontinfo).
    pixel_height : float                   //! Font size in pixels (0 if loaded without atlas).
    char_range   : int2                    //! (first_char, num_chars) of the packed range.
    tex          : uint64                  //! Opaque texture handle (0 by default, set by graphics layer).
}

def public is_valid(font : Font) : bool {
    //! Returns ``true`` if the font has been successfully loaded.
    return !empty(font.font_data)
}

def public load_font(fname : string) : Font {
    //! Load a TrueType font from file for metrics and shape queries (no atlas).
    //!
    //! Returns a Font with ``pixel_height = 0`` and empty atlas fields.
    //! Use ``scale_for_pixel_height`` to compute a scale factor for a desired size.
    var font : Font
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
    stbtt_init(font.fontinfo, font.font_data)
    return <- font
}

def public load_ttf(fname : string; pixel_height : float = 32.0; atlas_dim : int2 = int2(512, 512);
                    first_char : int = 32; num_chars : int = 96; oversampling : int = 1) : Font {
    //! Load a TrueType font from file and pack a character range into an atlas.
    //!
    //! Returns a Font with bitmap stored as a 1-channel Image.
    //! If loading fails, returns a Font with an invalid bitmap.
    var font <- load_font(fname)
    if (!is_valid(font)) {
        return <- font
    }
    font.pixel_height = pixel_height
    font.char_range = int2(first_char, num_chars)
    font.chardata |> resize(num_chars)
    font.bitmap = make_image(atlas_dim.x, atlas_dim.y, 1)
    stbtt_pack(font.font_data, font.bitmap.bytes, atlas_dim.x, atlas_dim.y,
               font.chardata, pixel_height, first_char, num_chars, oversampling)
    return <- font
}

def public scale_for_pixel_height(font : Font; pixel_height : float) : float {
    //! Compute scale factor for a given pixel height.
    return stbtt_scale_for_pixel_height(font.fontinfo, pixel_height)
}

def public scale_for_em(font : Font; pixels : float) : float {
    //! Compute scale factor for mapping em to pixels.
    return stbtt_scale_for_em(font.fontinfo, pixels)
}

def public font_vmetrics(font : Font) : tuple<ascent : int; descent : int; line_gap : int> {
    //! Returns vertical metrics in font units: ascent, descent, line gap.
    return stbtt_font_vmetrics(font.fontinfo)
}

def public font_metrics(font : Font) : tuple<ascent : float; descent : float; line_height : float> {
    //! Returns font metrics in pixels using the font's ``pixel_height``.
    //! Ascent is baseline to top, descent is baseline to bottom (negative),
    //! line_height is recommended line spacing.
    let vm = stbtt_font_vmetrics(font.fontinfo)
    let scale = stbtt_scale_for_pixel_height(font.fontinfo, font.pixel_height)
    return (ascent = float(vm.ascent) * scale, descent = float(vm.descent) * scale,
            line_height = float(vm.ascent - vm.descent + vm.line_gap) * scale)
}

def public font_metrics(font : Font; pixel_height : float) : tuple<ascent : float; descent : float; line_height : float> {
    //! Returns font metrics in pixels at the given ``pixel_height``.
    let vm = stbtt_font_vmetrics(font.fontinfo)
    let scale = stbtt_scale_for_pixel_height(font.fontinfo, pixel_height)
    return (ascent = float(vm.ascent) * scale, descent = float(vm.descent) * scale,
            line_height = float(vm.ascent - vm.descent + vm.line_gap) * scale)
}

def public codepoint_hmetrics(font : Font; codepoint : int) : tuple<advance_width : int; left_side_bearing : int> {
    //! Returns horizontal metrics in font units for a codepoint.
    return stbtt_codepoint_hmetrics(font.fontinfo, codepoint)
}

def public codepoint_kern(font : Font; ch1, ch2 : int) : int {
    //! Returns kerning advance between two codepoints in font units.
    return stbtt_codepoint_kern(font.fontinfo, ch1, ch2)
}

def public codepoint_box(font : Font; codepoint : int) : int4 {
    //! Returns glyph bounding box ``int4(x0, y0, x1, y1)`` in font units.
    return stbtt_codepoint_box(font.fontinfo, codepoint)
}

def public font_bounding_box(font : Font) : int4 {
    //! Returns font bounding box ``int4(x0, y0, x1, y1)`` in font units.
    return stbtt_font_bbox(font.fontinfo)
}

def public find_glyph(font : Font; codepoint : int) : int {
    //! Returns glyph index for a codepoint. Returns 0 if not found.
    return stbtt_find_glyph(font.fontinfo, codepoint)
}

def public codepoint_shape(font : Font; codepoint : int; blk : block<(v : stbtt_vertex_das) : void>) {
    //! Iterate glyph shape vertices for a codepoint via callback.
    stbtt_codepoint_shape(font.fontinfo, codepoint, blk)
}

def public codepoint_shape_count(font : Font; codepoint : int) : int {
    //! Returns the number of shape vertices for a codepoint.
    return stbtt_codepoint_shape_count(font.fontinfo, codepoint)
}

def public codepoint_bitmap_box(font : Font; codepoint : int; scale_x, scale_y : float) : int4 {
    //! Returns pixel-space bounding box ``int4(ix0, iy0, ix1, iy1)`` for a codepoint bitmap.
    return stbtt_codepoint_bitmap_box(font.fontinfo, codepoint, scale_x, scale_y)
}

def public codepoint_bitmap_box_subpixel(font : Font; codepoint : int;
                                         scale_x, scale_y, shift_x, shift_y : float) : int4 {
    //! Returns pixel-space bounding box with subpixel shift.
    return stbtt_codepoint_bitmap_box_subpixel(font.fontinfo, codepoint, scale_x, scale_y, shift_x, shift_y)
}

def public codepoint_bitmap(font : Font; codepoint : int; scale_x, scale_y : float) : tuple<image : Image; offset : int2> {
    //! Renders a codepoint glyph bitmap at the given scale. Returns a 1-channel ``Image`` and pixel offset.
    return stbtt_codepoint_bitmap(font.fontinfo, codepoint, scale_x, scale_y)
}

def public codepoint_bitmap_subpixel(font : Font; codepoint : int;
                                     scale_x, scale_y, shift_x, shift_y : float) : tuple<image : Image; offset : int2> {
    //! Renders a codepoint glyph bitmap with subpixel shift.
    return stbtt_codepoint_bitmap_subpixel(font.fontinfo, codepoint, scale_x, scale_y, shift_x, shift_y)
}

def public measure_text(font : Font; text : string; pixel_height : float) : float {
    //! Measure the width of a string in pixels at the given pixel height.
    let scale = stbtt_scale_for_pixel_height(font.fontinfo, pixel_height)
    var width = 0.0
    for (ch in text) {
        let hm = stbtt_codepoint_hmetrics(font.fontinfo, int(ch))
        width += float(hm.advance_width) * scale
    }
    return width
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
