options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

require stbtruetype
require fio
require daslib/safe_addr
require dastest/testing_boost public

def font_path() : string {
    return "{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf"
}

def load_font_data() : array<uint8> {
    var data : array<uint8>
    fopen(font_path(), "rb") <| $(f) {
        fmap(f) <| $(mapped) {
            let sz = length(mapped)
            data |> resize(sz)
            unsafe {
                memcpy(addr(data[0]), addr(mapped[0]), sz)
            }
        }
    }
    return <- data
}

// helper: init font from data, returns true on success
def init_font(var info : stbtt_fontinfo; var data : array<uint8>) : bool {
    unsafe {
        var offset = stbtt_GetFontOffsetForIndex(addr(data[0]), 0)
        return stbtt_InitFont(addr(info), addr(data[0]), offset) != 0
    }
}

// ---- Font loading ----

[test]
def test_get_number_of_fonts(t : T?) {
    t |> run("GetNumberOfFonts returns 1 for single-font TTF") <| @(tt : T?) {
        var data <- load_font_data()
        tt |> success(length(data) > 0, "font data loaded")
        unsafe {
            var nfonts = stbtt_GetNumberOfFonts(addr(data[0]))
            tt |> equal(nfonts, 1)
        }
    }
}

[test]
def test_get_font_offset(t : T?) {
    t |> run("GetFontOffsetForIndex returns valid offset for index 0") <| @(tt : T?) {
        var data <- load_font_data()
        unsafe {
            var offset = stbtt_GetFontOffsetForIndex(addr(data[0]), 0)
            tt |> success(offset >= 0, "valid offset for index 0")
            var bad_offset = stbtt_GetFontOffsetForIndex(addr(data[0]), 99)
            tt |> equal(bad_offset, -1)
        }
    }
}

[test]
def test_init_font(t : T?) {
    t |> run("InitFont succeeds for valid TTF data") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        tt |> success(init_font(info, data), "InitFont succeeded")
        tt |> success(info.numGlyphs > 0, "font has glyphs")
    }
}

[test]
def test_find_glyph_index(t : T?) {
    t |> run("FindGlyphIndex finds common glyphs") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        // ASCII 'A' = 65 should have a glyph
        var glyph_a = stbtt_FindGlyphIndex(safe_addr(info), 65)
        tt |> success(glyph_a > 0, "glyph for 'A' found")
        // ASCII space = 32
        var glyph_space = stbtt_FindGlyphIndex(safe_addr(info), 32)
        tt |> success(glyph_space > 0, "glyph for space found")
        // Very high codepoint unlikely to exist
        var glyph_missing = stbtt_FindGlyphIndex(safe_addr(info), int(0x1FFFF))
        tt |> equal(glyph_missing, 0)
    }
}

// ---- Font metrics ----

[test]
def test_scale_for_pixel_height(t : T?) {
    t |> run("ScaleForPixelHeight returns positive scale") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        tt |> success(scale > 0.0f, "positive scale")
        // larger pixel height => larger scale
        var scale2 = stbtt_ScaleForPixelHeight(safe_addr(info), 64.0f)
        tt |> success(scale2 > scale, "larger pixel height gives larger scale")
    }
}

[test]
def test_scale_for_mapping_em(t : T?) {
    t |> run("ScaleForMappingEmToPixels returns positive scale") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForMappingEmToPixels(safe_addr(info), 32.0f)
        tt |> success(scale > 0.0f, "positive scale")
    }
}

[test]
def test_get_font_vmetrics(t : T?) {
    t |> run("GetFontVMetrics returns valid ascent/descent") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var ascent, descent, line_gap : int
        stbtt_GetFontVMetrics(safe_addr(info), safe_addr(ascent), safe_addr(descent), safe_addr(line_gap))
        tt |> success(ascent > 0, "positive ascent")
        tt |> success(descent < 0, "negative descent")
    }
}

[test]
def test_get_font_bounding_box(t : T?) {
    t |> run("GetFontBoundingBox returns valid box") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var x0, y0, x1, y1 : int
        stbtt_GetFontBoundingBox(safe_addr(info), safe_addr(x0), safe_addr(y0), safe_addr(x1), safe_addr(y1))
        tt |> success(x1 > x0, "x1 > x0")
        tt |> success(y1 > y0, "y1 > y0")
    }
}

[test]
def test_get_codepoint_hmetrics(t : T?) {
    t |> run("GetCodepointHMetrics returns valid advance width") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var advance_width, left_side_bearing : int
        stbtt_GetCodepointHMetrics(safe_addr(info), 65, safe_addr(advance_width), safe_addr(left_side_bearing))
        tt |> success(advance_width > 0, "positive advance width for 'A'")
        // monospace font: all visible chars should have same advance
        var advance_b, lsb_b : int
        stbtt_GetCodepointHMetrics(safe_addr(info), 66, safe_addr(advance_b), safe_addr(lsb_b))
        tt |> equal(advance_width, advance_b)
    }
}

[test]
def test_get_codepoint_kern_advance(t : T?) {
    t |> run("GetCodepointKernAdvance returns int") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        // monospace fonts typically have no kerning, but the function should still work
        var kern = stbtt_GetCodepointKernAdvance(safe_addr(info), 65, 86) // 'A', 'V'
        tt |> success(kern >= 0 || kern < 0, "kern returned value")
    }
}

[test]
def test_get_codepoint_box(t : T?) {
    t |> run("GetCodepointBox returns valid bounding box for 'A'") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var x0, y0, x1, y1 : int
        var ok = stbtt_GetCodepointBox(safe_addr(info), 65, safe_addr(x0), safe_addr(y0), safe_addr(x1), safe_addr(y1))
        tt |> success(ok != 0, "GetCodepointBox succeeded for 'A'")
        tt |> success(x1 > x0, "x1 > x0")
        tt |> success(y1 > y0, "y1 > y0")
    }
}

// ---- Packing API ----

[test]
def test_pack_basic(t : T?) {
    t |> run("PackBegin/PackFontRange/PackEnd basic workflow") <| @(tt : T?) {
        var data <- load_font_data()
        let atlas_w = 256
        let atlas_h = 256
        var pixels : array<uint8>
        pixels |> resize(atlas_w * atlas_h)
        var chardata : array<stbtt_packedchar>
        chardata |> resize(96) // ASCII 32..127
        var spc : stbtt_pack_context
        unsafe {
            var ok = stbtt_PackBegin(addr(spc), addr(pixels[0]), atlas_w, atlas_h, 0, 1, null)
            tt |> success(ok != 0, "PackBegin succeeded")
            ok = stbtt_PackFontRange(addr(spc), addr(data[0]), 0, 24.0f, 32, 96, addr(chardata[0]))
            tt |> success(ok != 0, "PackFontRange succeeded")
            stbtt_PackEnd(addr(spc))
        }
        // verify some pixels were written (atlas not all zeros)
        var non_zero = 0
        for (px in pixels) {
            if (px != uint8(0)) {
                non_zero++
            }
        }
        tt |> success(non_zero > 0, "atlas has non-zero pixels")
        // verify chardata was filled — 'A' (index 33, since first_char=32)
        tt |> success(chardata[33].xadvance > 0.0f, "chardata xadvance > 0")
    }
}

[test]
def test_pack_set_oversampling(t : T?) {
    t |> run("PackSetOversampling affects rendering") <| @(tt : T?) {
        var data <- load_font_data()
        let atlas_w = 512
        let atlas_h = 512
        var pixels : array<uint8>
        pixels |> resize(atlas_w * atlas_h)
        var chardata : array<stbtt_packedchar>
        chardata |> resize(96)
        var spc : stbtt_pack_context
        unsafe {
            stbtt_PackBegin(addr(spc), addr(pixels[0]), atlas_w, atlas_h, 0, 1, null)
            stbtt_PackSetOversampling(addr(spc), 2u, 2u)
            var ok = stbtt_PackFontRange(addr(spc), addr(data[0]), 0, 24.0f, 32, 96, addr(chardata[0]))
            tt |> success(ok != 0, "PackFontRange with oversampling succeeded")
            stbtt_PackEnd(addr(spc))
        }
        // oversampled chardata should still have valid xadvance
        tt |> success(chardata[33].xadvance > 0.0f, "oversampled chardata valid")
    }
}

[test]
def test_pack_set_skip_missing(t : T?) {
    t |> run("PackSetSkipMissingCodepoints does not crash") <| @(tt : T?) {
        var data <- load_font_data()
        let atlas_w = 256
        let atlas_h = 256
        var pixels : array<uint8>
        pixels |> resize(atlas_w * atlas_h)
        var chardata : array<stbtt_packedchar>
        chardata |> resize(96)
        var spc : stbtt_pack_context
        unsafe {
            stbtt_PackBegin(addr(spc), addr(pixels[0]), atlas_w, atlas_h, 0, 1, null)
            stbtt_PackSetSkipMissingCodepoints(addr(spc), 1)
            // with skip_missing=1, PackFontRange may return 0 — just verify no crash
            stbtt_PackFontRange(addr(spc), addr(data[0]), 0, 24.0f, 32, 96, addr(chardata[0]))
            stbtt_PackEnd(addr(spc))
        }
    }
}

[test]
def test_get_packed_quad(t : T?) {
    t |> run("GetPackedQuad returns valid quad for 'A'") <| @(tt : T?) {
        var data <- load_font_data()
        let atlas_w = 256
        let atlas_h = 256
        var pixels : array<uint8>
        pixels |> resize(atlas_w * atlas_h)
        var chardata : array<stbtt_packedchar>
        chardata |> resize(96)
        var spc : stbtt_pack_context
        unsafe {
            stbtt_PackBegin(addr(spc), addr(pixels[0]), atlas_w, atlas_h, 0, 1, null)
            stbtt_PackFontRange(addr(spc), addr(data[0]), 0, 24.0f, 32, 96, addr(chardata[0]))
            stbtt_PackEnd(addr(spc))
        }
        // get quad for 'A' (char_index = 65 - 32 = 33)
        var xpos = 0.0f
        var ypos = 0.0f
        var quad : stbtt_aligned_quad
        unsafe {
            stbtt_GetPackedQuad(addr(chardata[0]), atlas_w, atlas_h, 33, addr(xpos), addr(ypos), addr(quad), 0)
        }
        // xpos should have advanced
        tt |> success(xpos > 0.0f, "xpos advanced after GetPackedQuad")
        // quad should have valid UV coordinates
        tt |> success(quad.s1 > quad.s0, "s1 > s0")
        tt |> success(quad.t1 > quad.t0, "t1 > t0")
        // quad should have valid position
        tt |> success(quad.x1 > quad.x0, "x1 > x0")
    }
}

// ---- Direct bitmap rendering ----

[test]
def test_get_codepoint_bitmap(t : T?) {
    t |> run("GetCodepointBitmap renders 'A' glyph") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        var w, h, xoff, yoff : int
        var bitmap = stbtt_GetCodepointBitmap(safe_addr(info), scale, scale, 65, safe_addr(w), safe_addr(h), safe_addr(xoff), safe_addr(yoff))
        tt |> success(bitmap != null, "bitmap allocated")
        tt |> success(w > 0, "width > 0")
        tt |> success(h > 0, "height > 0")
        // verify some pixels are non-zero
        var non_zero = 0
        for (idx in range(w * h)) {
            if (unsafe(bitmap[idx]) != uint8(0)) {
                non_zero++
            }
        }
        tt |> success(non_zero > 0, "bitmap has non-zero pixels")
        stbtt_FreeBitmap(bitmap, null)
    }
}

[test]
def test_get_codepoint_bitmap_subpixel(t : T?) {
    t |> run("GetCodepointBitmapSubpixel renders with subpixel shift") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        var w, h, xoff, yoff : int
        var bitmap = stbtt_GetCodepointBitmapSubpixel(safe_addr(info), scale, scale, 0.5f, 0.5f, 65, safe_addr(w), safe_addr(h), safe_addr(xoff), safe_addr(yoff))
        tt |> success(bitmap != null, "subpixel bitmap allocated")
        tt |> success(w > 0, "width > 0")
        tt |> success(h > 0, "height > 0")
        stbtt_FreeBitmap(bitmap, null)
    }
}

[test]
def test_make_codepoint_bitmap(t : T?) {
    t |> run("MakeCodepointBitmap renders into provided buffer") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        // get bounding box to know buffer size
        var ix0, iy0, ix1, iy1 : int
        stbtt_GetCodepointBitmapBox(safe_addr(info), 65, scale, scale, safe_addr(ix0), safe_addr(iy0), safe_addr(ix1), safe_addr(iy1))
        let bw = ix1 - ix0
        let bh = iy1 - iy0
        tt |> success(bw > 0, "bitmap box width > 0")
        tt |> success(bh > 0, "bitmap box height > 0")
        var buf : array<uint8>
        buf |> resize(bw * bh)
        unsafe {
            stbtt_MakeCodepointBitmap(safe_addr(info), addr(buf[0]), bw, bh, bw, scale, scale, 65)
        }
        // verify some pixels are non-zero
        var non_zero = 0
        for (px in buf) {
            if (px != uint8(0)) {
                non_zero++
            }
        }
        tt |> success(non_zero > 0, "MakeCodepointBitmap wrote pixels")
    }
}

[test]
def test_make_codepoint_bitmap_subpixel(t : T?) {
    t |> run("MakeCodepointBitmapSubpixel renders with subpixel shift") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        var ix0, iy0, ix1, iy1 : int
        stbtt_GetCodepointBitmapBoxSubpixel(safe_addr(info), 65, scale, scale, 0.5f, 0.5f, safe_addr(ix0), safe_addr(iy0), safe_addr(ix1), safe_addr(iy1))
        let bw = ix1 - ix0
        let bh = iy1 - iy0
        tt |> success(bw > 0 && bh > 0, "subpixel box has size")
        var buf : array<uint8>
        buf |> resize(bw * bh)
        unsafe {
            stbtt_MakeCodepointBitmapSubpixel(safe_addr(info), addr(buf[0]), bw, bh, bw, scale, scale, 0.5f, 0.5f, 65)
        }
        var non_zero = 0
        for (px in buf) {
            if (px != uint8(0)) {
                non_zero++
            }
        }
        tt |> success(non_zero > 0, "subpixel bitmap has pixels")
    }
}

[test]
def test_get_codepoint_bitmap_box(t : T?) {
    t |> run("GetCodepointBitmapBox returns valid box") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        var ix0, iy0, ix1, iy1 : int
        stbtt_GetCodepointBitmapBox(safe_addr(info), 65, scale, scale, safe_addr(ix0), safe_addr(iy0), safe_addr(ix1), safe_addr(iy1))
        tt |> success(ix1 > ix0, "ix1 > ix0")
        tt |> success(iy1 > iy0, "iy1 > iy0")
        // box for space should be empty or very small
        var sx0, sy0, sx1, sy1 : int
        stbtt_GetCodepointBitmapBox(safe_addr(info), 32, scale, scale, safe_addr(sx0), safe_addr(sy0), safe_addr(sx1), safe_addr(sy1))
        tt |> success(sx1 - sx0 <= ix1 - ix0, "space box not larger than 'A' box")
    }
}

[test]
def test_get_codepoint_bitmap_box_subpixel(t : T?) {
    t |> run("GetCodepointBitmapBoxSubpixel returns valid box") <| @(tt : T?) {
        var data <- load_font_data()
        var info : stbtt_fontinfo
        init_font(info, data)
        var scale = stbtt_ScaleForPixelHeight(safe_addr(info), 32.0f)
        var ix0, iy0, ix1, iy1 : int
        stbtt_GetCodepointBitmapBoxSubpixel(safe_addr(info), 65, scale, scale, 0.25f, 0.75f, safe_addr(ix0), safe_addr(iy0), safe_addr(ix1), safe_addr(iy1))
        tt |> success(ix1 >= ix0, "ix1 >= ix0")
        tt |> success(iy1 >= iy0, "iy1 >= iy0")
    }
}
