// Tutorial STBIMAGE-06: TrueType Fonts
//
// This tutorial covers:
//   - Loading a TrueType (.ttf) font and packing a glyph atlas (load_ttf)
//   - Querying font metrics (ascent, descent, line height) and per-glyph metrics
//   - Measuring rendered text width
//   - Rasterizing a single glyph to a 1-channel bitmap (codepoint_bitmap)
//   - Rendering text onto an RGBA canvas (render_text) and saving it
//
// Run: daslang.exe tutorials/dasStbImage/06_truetype_fonts.das

options gen2
options indenting = 4

require daslib/fio
require stbimage/stbimage_boost
require stbimage/stbimage_ttf

// ──────────────────────────────────────────────────────────────────────────
// Helper — load the DroidSansMono font bundled with dasStbImage
// ──────────────────────────────────────────────────────────────────────────
//
// load_ttf reads the .ttf and packs a range of glyphs (default ASCII 32..127)
// into a 1-channel atlas bitmap kept on the Font. Resolve the bundled font via
// get_das_root() so the tutorial runs from any working directory.

def load_bundled_font(pixel_height : float) : Font {
    let font_file = "{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf"
    return <- load_ttf(font_file, [pixel_height = pixel_height])
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Font metrics
// ──────────────────────────────────────────────────────────────────────────
//
// font_metrics returns ascent/descent/line_height in PIXELS at the font's
// loaded pixel_height. font_vmetrics returns the same in raw font units.

def section_metrics(font : Font) {
    let m = font_metrics(font)
    assert(m.ascent > 0.0 && m.line_height > 0.0, "degenerate font metrics")
    print("  pixels:     ascent={m.ascent}, descent={m.descent}, line_height={m.line_height}\n")
    let vm = font_vmetrics(font)
    print("  font units: ascent={vm.ascent}, descent={vm.descent}, line_gap={vm.line_gap}\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Per-glyph metrics and text measurement
// ──────────────────────────────────────────────────────────────────────────
//
// codepoint_hmetrics gives the advance width / left side bearing of one glyph
// (in font units). measure_text sums advances to get a pixel width.

def section_glyph_metrics(font : Font) {
    let hm = codepoint_hmetrics(font, 'A')
    assert(hm.advance_width > 0, "glyph 'A' has no advance")
    print("  'A': advance_width={hm.advance_width}, left_side_bearing={hm.left_side_bearing} (font units)\n")
    let width = measure_text(font, "Hello", 32.0)
    assert(width > 0.0, "measured zero width")
    print("  measure_text(\"Hello\", 32px) = {width} px\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Rasterizing a single glyph
// ──────────────────────────────────────────────────────────────────────────
//
// codepoint_bitmap renders one glyph to a 1-channel (alpha) Image at a scale
// computed from the desired pixel height, plus a pixel offset from the origin.

def section_glyph_bitmap(font : Font) {
    let scale = scale_for_pixel_height(font, 32.0)
    var bm = codepoint_bitmap(font, 'A', scale, scale)
    assert(bm.image.channels == 1 && bm.image.bpc == 1, "glyph bitmap should be 1-channel uint8")
    assert(bm.image.width > 0 && bm.image.height > 0, "glyph bitmap is empty")
    var inked = 0
    bm.image |> with_pixels() $(var px : array<uint8>#) {
        for (p in px) {
            if (p != uint8(0)) {
                inked++
            }
        }
    }
    assert(inked > 0, "glyph 'A' rasterized with no ink")
    print("  glyph 'A': {bm.image.width}x{bm.image.height} px, offset ({bm.offset.x},{bm.offset.y}), {inked} inked pixels\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Rendering text onto an RGBA canvas
// ──────────────────────────────────────────────────────────────────────────
//
// render_text blits the packed atlas glyphs onto a 4-channel RGBA Image at a
// baseline origin, in the given color. We draw black text on a white canvas,
// confirm some pixels were inked, then save + reload to prove the round-trip.

def section_render_text(font : Font) {
    let W = 256
    let H = 64
    var canvas = make_image(W, H, 4)
    canvas.fill_rect(0, 0, W, H, 0xFFFFFFFFu)        // white background
    let m = font_metrics(font)
    render_text(canvas, font, "Hello, daslang!", 8, int(m.ascent) + 8, 0, 0, 0)
    var inked = 0
    canvas |> with_pixels() $(var px : array<uint8>#) {
        for (i in range(W * H)) {
            if (px[i * 4] != uint8(255)) {            // R channel no longer white
                inked++
            }
        }
    }
    assert(inked > 0, "no text pixels were drawn")
    print("  rendered text, {inked} inked pixels on a {W}x{H} canvas\n")

    let path = "_tutorial_text.png"
    let (ok, err) = canvas.save(path)
    if (!ok) {
        print("  save error: {err}\n")
    }
    assert(ok, "save failed")
    var inscope reloaded : Image
    let (ok2, err2) = reloaded.load(path)
    if (!ok2) {
        print("  reload error: {err2}\n")
    }
    assert(ok2, "reload failed")
    assert(reloaded.width == W && reloaded.height == H, "round-trip changed dimensions")
    print("  saved + reloaded {reloaded.width}x{reloaded.height}\n")
    remove(path)
}

[export]
def main() {
    print("=== Section 0: Loading a font ===\n")
    let font <- load_bundled_font(32.0)
    assert(is_valid(font), "failed to load bundled DroidSansMono font")
    print("  atlas {font.bitmap.width}x{font.bitmap.height}, packed range [{font.char_range.x}..{font.char_range.x + font.char_range.y})\n")

    print("\n=== Section 1: Font metrics ===\n")
    section_metrics(font)

    print("\n=== Section 2: Per-glyph metrics ===\n")
    section_glyph_metrics(font)

    print("\n=== Section 3: Rasterizing a glyph ===\n")
    section_glyph_bitmap(font)

    print("\n=== Section 4: Rendering text ===\n")
    section_render_text(font)

    print("\nDone.\n")
}
