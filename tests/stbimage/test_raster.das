options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require stbimage/stbimage_boost
require daslib/safe_addr
require dastest/testing_boost public
require math

def test_image_path(name : string) : string => "{get_das_root()}/tests/stbimage/{name}"

// helper: fill image bytes with position-based pattern
// each pixel's first byte = (x + y * width) & 0xFF
def fill_pattern(var img : Image) {
    let px = img.channels * img.bpc
    for (y in range(img.height)) {
        for (x in range(img.width)) {
            let idx = (y * img.width + x) * px
            img.bytes[idx] = uint8((x + y * img.width) & int(0xFF))
            // set remaining bytes to a fixed value so we can detect corruption
            for (b in range(1, px)) {
                img.bytes[idx + b] = uint8(0xAB)
            }
        }
    }
}

// helper: verify pattern matches expected pixel mapping after transforms
def verify_equal(tt : T?; a : Image; b : Image) {
    tt |> equal(length(a.bytes), length(b.bytes))
    var mismatch = 0
    for (i in range(length(a.bytes))) {
        if (a.bytes[i] != b.bytes[i]) {
            mismatch++
        }
    }
    tt |> equal(mismatch, 0)
}

// ========================================================
// flip_horizontal — test every pixel size dispatch path
// ========================================================

// px=1: 1ch, bpc=1 (grey uint8)
[test(description = "flipping a 4x2 grey 8-bit image horizontally moves pixel 3 of the first row to the front, and flipping again restores the original")]
def test_flip_h_px1(t : T?) {
    t |> run("flip_horizontal px=1 (grey uint8)") <| @(tt : T?) {
        var img <- make_image(4, 2, 1, 1)
        fill_pattern(img)
        // pixel(0,0)=0, pixel(3,0)=3
        tt |> equal(int(img.bytes[0]), 0)
        tt |> equal(int(img.bytes[3]), 3)
        var flipped <- img.flip_horizontal()
        // after flip: pixel(0,0) should be old pixel(3,0)=3
        tt |> equal(int(flipped.bytes[0]), 3)
        tt |> equal(int(flipped.bytes[3]), 0)
        // roundtrip
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=2: 2ch, bpc=1 (grey+alpha uint8)
[test(description = "flipping a 4x2 grey-plus-alpha 8-bit image horizontally moves whole 2-byte pixels, so the alpha byte stays with its grey, and flipping again restores the original")]
def test_flip_h_px2(t : T?) {
    t |> run("flip_horizontal px=2 (grey+alpha uint8)") <| @(tt : T?) {
        var img <- make_image(4, 2, 2, 1)
        fill_pattern(img)
        // pixel(0,0) first byte=0, pixel(3,0) first byte=3
        tt |> equal(int(img.bytes[0]), 0)
        tt |> equal(int(img.bytes[6]), 3)  // 3*2=6
        var flipped <- img.flip_horizontal()
        tt |> equal(int(flipped.bytes[0]), 3)
        tt |> equal(int(flipped.bytes[1]), int(uint8(0xAB)))  // second byte preserved
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=3: 3ch, bpc=1 (RGB uint8)
[test(description = "flipping a 4x2 RGB 8-bit image horizontally moves whole 3-byte pixels without scrambling their bytes, and flipping again restores the original")]
def test_flip_h_px3(t : T?) {
    t |> run("flip_horizontal px=3 (RGB uint8)") <| @(tt : T?) {
        var img <- make_image(4, 2, 3, 1)
        fill_pattern(img)
        var flipped <- img.flip_horizontal()
        // pixel(0,0) of flipped = pixel(3,0) of original
        tt |> equal(int(flipped.bytes[0]), 3)
        tt |> equal(int(flipped.bytes[1]), int(uint8(0xAB)))
        tt |> equal(int(flipped.bytes[2]), int(uint8(0xAB)))
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=4: 4ch, bpc=1 (RGBA uint8)
[test(description = "flipping the gradient test.png horizontally swaps the red of pixel 0 (0) and pixel 63 (252), and flipping again restores the original")]
def test_flip_h_px4(t : T?) {
    t |> run("flip_horizontal px=4 (RGBA uint8)") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        // gradient: pixel(0,0) R=0, pixel(63,0) R=252
        var flipped <- img.flip_horizontal()
        tt |> equal(int(flipped.bytes[0]), 252)
        tt |> equal(int(flipped.bytes[63 * 4]), 0)
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=6: 3ch, bpc=2 (RGB uint16)
[test(description = "flipping a 4x2 RGB 16-bit image horizontally moves whole 6-byte pixels to the opposite end, and flipping again restores the original")]
def test_flip_h_px6(t : T?) {
    t |> run("flip_horizontal px=6 (RGB uint16)") <| @(tt : T?) {
        var img <- make_image(4, 2, 3, 2)
        fill_pattern(img)
        var flipped <- img.flip_horizontal()
        // pixel(0,0) of flipped = pixel(3,0) of original, first byte = 3
        tt |> equal(int(flipped.bytes[0]), 3)
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=8: 4ch, bpc=2 (RGBA uint16)
[test(description = "flipping a 4x2 RGBA 16-bit image horizontally moves whole 8-byte pixels to the opposite end, and flipping again restores the original")]
def test_flip_h_px8(t : T?) {
    t |> run("flip_horizontal px=8 (RGBA uint16)") <| @(tt : T?) {
        var img <- make_image(4, 2, 4, 2)
        fill_pattern(img)
        var flipped <- img.flip_horizontal()
        tt |> equal(int(flipped.bytes[0]), 3)
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=12: 3ch, bpc=4 (RGB float)
[test(description = "flipping a 4x2 RGB float image horizontally moves whole 12-byte pixels to the opposite end, and flipping again restores the original")]
def test_flip_h_px12(t : T?) {
    t |> run("flip_horizontal px=12 (RGB float)") <| @(tt : T?) {
        var img <- make_image(4, 2, 3, 4)
        fill_pattern(img)
        var flipped <- img.flip_horizontal()
        tt |> equal(int(flipped.bytes[0]), 3)
        let restored <- flipped.flip_horizontal()
        verify_equal(tt, img, restored)
    }
}

// px=16: 4ch, bpc=4 (RGBA float)
[test(description = "flipping the HDR image horizontally puts the old last-column red near 1 in the first pixel, and flipping again puts the red near 0 back")]
def test_flip_h_px16(t : T?) {
    t |> run("flip_horizontal px=16 (RGBA float)") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        var flipped <- img.flip_horizontal()
        tt |> equal(flipped.bpc, 4)
        // pixel(0,0) of flipped = pixel(63,0) of original: R=63/64~0.98
        flipped |> with_pixels() $(var pixels : array<float>#) {
            tt |> success(pixels[0] > 0.9f, "flipped(0,0) R near 1.0")
        }
        var restored <- flipped.flip_horizontal()
        // restore should match original
        restored |> with_pixels() $(var rpx : array<float>#) {
            tt |> success(rpx[0] < 0.05f, "restored(0,0) R near 0")
        }
    }
}

// ========================================================
// blit — test every pixel size dispatch path
// ========================================================

// px=1: 1ch, bpc=1
[test(description = "blitting a 3x3 grey 8-bit image at (2, 2) onto an 8x8 one sets pixels inside that square to 200 and leaves pixels outside it at 0")]
def test_blit_px1(t : T?) {
    t |> run("blit px=1 (grey uint8)") <| @(tt : T?) {
        var dst <- make_image(8, 8, 1, 1)
        var src <- make_image(3, 3, 1, 1)
        for (i in range(9)) {
            src.bytes[i] = uint8(200)
        }
        dst.blit(src, 2, 2)
        // pixel(2,2) of dst = 200
        tt |> equal(int(dst.bytes[2 * 8 + 2]), 200)
        // pixel(4,4) of dst = 200 (still inside 3x3)
        tt |> equal(int(dst.bytes[4 * 8 + 4]), 200)
        // pixel(5,2) of dst = 0 (outside src)
        tt |> equal(int(dst.bytes[2 * 8 + 5]), 0)
    }
}

// px=2: 2ch, bpc=1
[test(description = "blitting a 2x2 grey-plus-alpha 8-bit image at (3, 3) copies both bytes of each pixel there and leaves the pixel to its left at 0")]
def test_blit_px2(t : T?) {
    t |> run("blit px=2 (grey+alpha uint8)") <| @(tt : T?) {
        var dst <- make_image(8, 8, 2, 1)
        var src <- make_image(2, 2, 2, 1)
        for (i in range(2 * 2)) {
            src.bytes[i * 2 + 0] = uint8(100)
            src.bytes[i * 2 + 1] = uint8(255)
        }
        dst.blit(src, 3, 3)
        let idx = (3 * 8 + 3) * 2
        tt |> equal(int(dst.bytes[idx + 0]), 100)
        tt |> equal(int(dst.bytes[idx + 1]), 255)
        // outside
        let idx2 = (3 * 8 + 2) * 2
        tt |> equal(int(dst.bytes[idx2 + 0]), 0)
    }
}

// px=3: 3ch, bpc=1
[test(description = "blitting a 4x4 RGB 8-bit image at (5, 5) copies the colour (200, 100, 50) into place and leaves the pixel to its left at 0")]
def test_blit_px3(t : T?) {
    t |> run("blit px=3 (RGB uint8)") <| @(tt : T?) {
        var dst <- make_image(16, 16, 3, 1)
        var src <- make_image(4, 4, 3, 1)
        for (i in range(4 * 4)) {
            src.bytes[i * 3 + 0] = uint8(200)
            src.bytes[i * 3 + 1] = uint8(100)
            src.bytes[i * 3 + 2] = uint8(50)
        }
        dst.blit(src, 5, 5)
        let idx = (5 * 16 + 5) * 3
        tt |> equal(int(dst.bytes[idx + 0]), 200)
        tt |> equal(int(dst.bytes[idx + 1]), 100)
        tt |> equal(int(dst.bytes[idx + 2]), 50)
        let idx2 = (5 * 16 + 4) * 3
        tt |> equal(int(dst.bytes[idx2 + 0]), 0)
    }
}

// px=4: 4ch, bpc=1
[test(description = "blitting a 4x4 opaque red RGBA 8-bit image at (10, 10) copies (255, 0, 0, 255) into place and leaves the pixel to its left at 0")]
def test_blit_px4(t : T?) {
    t |> run("blit px=4 (RGBA uint8)") <| @(tt : T?) {
        var dst <- make_image(32, 32, 4, 1)
        var src <- make_image(4, 4, 4, 1)
        for (i in range(4 * 4)) {
            src.bytes[i * 4 + 0] = uint8(255)
            src.bytes[i * 4 + 3] = uint8(255)
        }
        dst.blit(src, 10, 10)
        let idx = (10 * 32 + 10) * 4
        tt |> equal(int(dst.bytes[idx + 0]), 255)
        tt |> equal(int(dst.bytes[idx + 1]), 0)
        tt |> equal(int(dst.bytes[idx + 2]), 0)
        tt |> equal(int(dst.bytes[idx + 3]), 255)
        let idx2 = (10 * 32 + 9) * 4
        tt |> equal(int(dst.bytes[idx2 + 0]), 0)
    }
}

// px=6: 3ch, bpc=2
[test(description = "blitting a 2x2 RGB 16-bit image at (1, 1) copies its bytes into the 6-byte-pixel destination and leaves the pixel to its left at 0")]
def test_blit_px6(t : T?) {
    t |> run("blit px=6 (RGB uint16)") <| @(tt : T?) {
        var dst <- make_image(8, 8, 3, 2)
        var src <- make_image(2, 2, 3, 2)
        // fill all src bytes with 0xCC
        for (i in range(length(src.bytes))) {
            src.bytes[i] = uint8(0xCC)
        }
        dst.blit(src, 1, 1)
        let idx = (1 * 8 + 1) * 6
        tt |> equal(int(dst.bytes[idx]), int(0xCC))
        let idx2 = (1 * 8 + 0) * 6
        tt |> equal(int(dst.bytes[idx2]), 0)
    }
}

// px=8: 4ch, bpc=2
[test(description = "blitting a 2x2 RGBA 16-bit image at (3, 3) copies its bytes into the 8-byte-pixel destination and leaves the pixel to its left at 0")]
def test_blit_px8(t : T?) {
    t |> run("blit px=8 (RGBA uint16)") <| @(tt : T?) {
        var dst <- make_image(8, 8, 4, 2)
        var src <- make_image(2, 2, 4, 2)
        for (i in range(length(src.bytes))) {
            src.bytes[i] = uint8(0xDD)
        }
        dst.blit(src, 3, 3)
        let idx = (3 * 8 + 3) * 8
        tt |> equal(int(dst.bytes[idx]), int(0xDD))
        let idx2 = (3 * 8 + 2) * 8
        tt |> equal(int(dst.bytes[idx2]), 0)
    }
}

// px=12: 3ch, bpc=4
[test(description = "blitting a 2x2 RGB float image at (4, 4) copies its bytes into the 12-byte-pixel destination and leaves the pixel to its left at 0")]
def test_blit_px12(t : T?) {
    t |> run("blit px=12 (RGB float)") <| @(tt : T?) {
        var dst <- make_image(8, 8, 3, 4)
        var src <- make_image(2, 2, 3, 4)
        for (i in range(length(src.bytes))) {
            src.bytes[i] = uint8(0xEE)
        }
        dst.blit(src, 4, 4)
        let idx = (4 * 8 + 4) * 12
        tt |> equal(int(dst.bytes[idx]), int(0xEE))
        let idx2 = (4 * 8 + 3) * 12
        tt |> equal(int(dst.bytes[idx2]), 0)
    }
}

// px=16: 4ch, bpc=4
[test(description = "blitting a 4x4 RGBA float image of (0.75, 0.5, 0.25, 1) at (2, 2) copies those floats into place and leaves the pixel to its left at 0")]
def test_blit_px16(t : T?) {
    t |> run("blit px=16 (RGBA float)") <| @(tt : T?) {
        var dst <- make_image(16, 16, 4, 4)
        var src <- make_image(4, 4, 4, 4)
        src |> with_pixels() $(var pixels : array<float>#) {
            for (i in range(4 * 4)) {
                pixels[i * 4 + 0] = 0.75f
                pixels[i * 4 + 1] = 0.5f
                pixels[i * 4 + 2] = 0.25f
                pixels[i * 4 + 3] = 1.0f
            }
        }
        dst.blit(src, 2, 2)
        dst |> with_pixels() $(var pixels : array<float>#) {
            let idx = (2 * 16 + 2) * 4
            tt |> success(pixels[idx + 0] > 0.7f && pixels[idx + 0] < 0.8f, "R=0.75")
            tt |> success(pixels[idx + 1] > 0.45f && pixels[idx + 1] < 0.55f, "G=0.5")
            let idx2 = (2 * 16 + 1) * 4
            tt |> success(pixels[idx2 + 0] < 0.01f, "untouched pixel is zero")
        }
    }
}

// ========================================================
// blit — clipping and edge cases
// ========================================================

[test(description = "blitting a 4x4 image at (6, 6) onto an 8x8 one writes only the 2x2 corner that fits and exactly 4 pixels change")]
def test_blit_clipping(t : T?) {
    t |> run("blit clips at destination bounds") <| @(tt : T?) {
        var dst <- make_image(8, 8, 4, 1)
        var src <- make_image(4, 4, 4, 1)
        for (i in range(4 * 4)) {
            src.bytes[i * 4 + 0] = uint8(200)
            src.bytes[i * 4 + 3] = uint8(255)
        }
        // blit at (6,6) — only 2x2 pixels should land
        dst.blit(src, 6, 6)
        let idx = (6 * 8 + 6) * 4
        tt |> equal(int(dst.bytes[idx + 0]), 200)
        let idx2 = (7 * 8 + 7) * 4
        tt |> equal(int(dst.bytes[idx2 + 0]), 200)
        var count = 0
        for (i in range(8 * 8)) {
            if (dst.bytes[i * 4] != uint8(0)) {
                count++
            }
        }
        tt |> equal(count, 4)  // 2x2 region
    }
}

[test(description = "blitting a 4x4 image at (-2, -2) lands only its bottom-right 2x2 quarter at the destination's top-left and leaves pixel (2, 0) at 0")]
def test_blit_negative_offset(t : T?) {
    t |> run("blit with negative offset clips source") <| @(tt : T?) {
        var dst <- make_image(8, 8, 4, 1)
        var src <- make_image(4, 4, 4, 1)
        for (i in range(4 * 4)) {
            src.bytes[i * 4 + 0] = uint8(150)
            src.bytes[i * 4 + 3] = uint8(255)
        }
        // blit at (-2,-2) — only bottom-right 2x2 of src should land at dst(0,0)
        dst.blit(src, -2, -2)
        tt |> equal(int(dst.bytes[0]), 150)
        let idx2 = (1 * 8 + 1) * 4
        tt |> equal(int(dst.bytes[idx2]), 150)
        let idx3 = (0 * 8 + 2) * 4
        tt |> equal(int(dst.bytes[idx3]), 0)
    }
}

// ========================================================
// crop — core tests + HDR
// ========================================================

[test(description = "cropping a 16x16 region at (10, 20) from the gradient test.png gives a 16x16 RGBA image whose first pixel is (40, 80, 128, 255)")]
def test_crop(t : T?) {
    t |> run("crop extracts sub-region with pixel verification") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let cropped <- img.crop(10, 20, 16, 16)
        tt |> equal(cropped.width, 16)
        tt |> equal(cropped.height, 16)
        tt |> equal(cropped.channels, 4)
        tt |> equal(cropped.bpc, 1)
        // pixel(0,0) of cropped = pixel(10,20) of original = R=10*4=40, G=20*4=80, B=128, A=255
        tt |> equal(int(cropped.bytes[0]), 40)
        tt |> equal(int(cropped.bytes[1]), 80)
        tt |> equal(int(cropped.bytes[2]), 128)
        tt |> equal(int(cropped.bytes[3]), 255)
    }
}

[test(description = "cropping 16x16 at (60, 60) from the 64x64 image is clamped to the 4x4 area that exists")]
def test_crop_clamp(t : T?) {
    t |> run("crop clamps to image bounds") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let cropped <- img.crop(60, 60, 16, 16)
        tt |> equal(cropped.width, 4)
        tt |> equal(cropped.height, 4)
    }
}

[test(description = "cropping a rectangle that lies entirely outside the 64x64 image returns an invalid Image")]
def test_crop_out_of_bounds(t : T?) {
    t |> run("crop fully outside returns invalid image") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let cropped <- img.crop(100, 100, 10, 10)
        tt |> success(!cropped.valid(), "cropped out of bounds is invalid")
    }
}

[test(description = "cropping the whole 64x64 image gives a 64x64 image with exactly the same bytes")]
def test_crop_full(t : T?) {
    t |> run("crop of full image preserves all pixels") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let cropped <- img.crop(0, 0, 64, 64)
        tt |> equal(cropped.width, 64)
        tt |> equal(cropped.height, 64)
        verify_equal(tt, img, cropped)
    }
}

[test(description = "cropping 16x16 at (10, 10) from the HDR image keeps 4-byte floats and starts at the source pixel whose red and green are near 0.156")]
def test_crop_hdr(t : T?) {
    t |> run("crop HDR image preserves bpc and pixels") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        var cropped <- img.crop(10, 10, 16, 16)
        tt |> equal(cropped.width, 16)
        tt |> equal(cropped.height, 16)
        tt |> equal(cropped.bpc, 4)
        cropped |> with_pixels() $(var pixels : array<float>#) {
            tt |> success(pixels[0] > 0.1f && pixels[0] < 0.25f, "R near 0.156")
            tt |> success(pixels[1] > 0.1f && pixels[1] < 0.25f, "G near 0.156")
        }
    }
}

[test(description = "cropping 4x4 at (2, 2) from an 8x8 grey image keeps one channel and starts at source pixel value 18")]
def test_crop_grey(t : T?) {
    t |> run("crop on 1-channel image") <| @(tt : T?) {
        var img <- make_image(8, 8, 1, 1)
        fill_pattern(img)
        let cropped <- img.crop(2, 2, 4, 4)
        tt |> equal(cropped.width, 4)
        tt |> equal(cropped.height, 4)
        tt |> equal(cropped.channels, 1)
        // pixel(0,0) of cropped = pixel(2,2) of original = (2 + 2*8) & 0xFF = 18
        tt |> equal(int(cropped.bytes[0]), 18)
    }
}

// ---- fill_rect (4-channel RGBA) ----

[test(description = "fill_rect of a 4x2 rectangle at (2, 3) sets every pixel in it to the given RGBA colour and leaves pixel (0, 0) at 0")]
def test_fill_rect_rgba(t : T?) {
    t |> run("fill_rect fills correct region with RGBA color") <| @(tt : T?) {
        var img = make_image(10, 10, 4)
        // zero out
        for (b in img.bytes) { b = uint8(0); }
        let color = 0xAABBCCDDu
        img.fill_rect(2, 3, 4, 2, color)
        // check filled region
        for (y in range(3, 5)) {
            for (x in range(2, 6)) {
                let idx = (y * 10 + x) * 4
                let px = uint(img.bytes[idx]) | (uint(img.bytes[idx + 1]) << 8u) | (uint(img.bytes[idx + 2]) << 16u) | (uint(img.bytes[idx + 3]) << 24u)
                tt |> equal(px, color)
            }
        }
        // check outside is still zero
        let idx_outside = (0 * 10 + 0) * 4
        tt |> equal(int(img.bytes[idx_outside]), 0)
    }
}

[test(description = "fill_rect of a 3x3 rectangle at (-1, -1) paints pixel (0, 0) but not pixel (2, 2), because the part outside the image is clipped")]
def test_fill_rect_clips(t : T?) {
    t |> run("fill_rect clips to image bounds") <| @(tt : T?) {
        var img = make_image(5, 5, 4)
        for (b in img.bytes) { b = uint8(0); }
        // fill extending beyond bounds
        img.fill_rect(-1, -1, 3, 3, 0xFFFFFFFFu)
        // pixel at (0,0) should be filled
        let idx00 = 0
        tt |> equal(int(img.bytes[idx00]), 255)
        // pixel at (2,2) should NOT be filled (clip: -1+3=2, so x range is 0..1, y range is 0..1)
        let idx22 = (2 * 5 + 2) * 4
        tt |> equal(int(img.bytes[idx22]), 0)
    }
}

// ---- fill_rect (1-channel) ----

[test(description = "fill_rect of a 3x3 rectangle at (1, 1) on a grey image sets pixel (1, 1) to 128 and leaves pixels (4, 1) and (0, 0) at 0")]
def test_fill_rect_grey(t : T?) {
    t |> run("fill_rect fills 1-channel image") <| @(tt : T?) {
        var img = make_image(8, 8, 1)
        for (b in img.bytes) { b = uint8(0); }
        img.fill_rect(1, 1, 3, 3, uint8(128))
        tt |> equal(int(img.bytes[1 * 8 + 1]), 128)
        tt |> equal(int(img.bytes[1 * 8 + 4]), 0)
        tt |> equal(int(img.bytes[0 * 8 + 0]), 0)
    }
}

// ---- blit_alpha ----

[test(description = "blit_alpha with a fully opaque mask of 255 replaces the destination pixel with the given colour (255, 128, 64) and alpha 255")]
def test_blit_alpha_full(t : T?) {
    t |> run("blit_alpha with 255 alpha overwrites color") <| @(tt : T?) {
        var dst = make_image(4, 4, 4)
        for (b in dst.bytes) { b = uint8(0); }
        var src = make_image(2, 2, 1)
        // full alpha
        for (b in src.bytes) { b = uint8(255); }
        dst.blit_alpha(src, 0, 0, 1, 1, 2, 2, 255, 128, 64)
        // check pixel at (1,1)
        let idx = (1 * 4 + 1) * 4
        tt |> equal(int(dst.bytes[idx + 0]), 255)
        tt |> equal(int(dst.bytes[idx + 1]), 128)
        tt |> equal(int(dst.bytes[idx + 2]), 64)
        tt |> equal(int(dst.bytes[idx + 3]), 255)
    }
}

[test(description = "blit_alpha with a fully transparent mask of 0 leaves the destination pixel at its original (100, 150, 200)")]
def test_blit_alpha_zero(t : T?) {
    t |> run("blit_alpha with 0 alpha leaves dest unchanged") <| @(tt : T?) {
        var dst = make_image(4, 4, 4)
        // fill with known color
        for (i in range(4 * 4)) {
            let idx = i * 4
            dst.bytes[idx + 0] = uint8(100)
            dst.bytes[idx + 1] = uint8(150)
            dst.bytes[idx + 2] = uint8(200)
            dst.bytes[idx + 3] = uint8(255)
        }
        var src = make_image(2, 2, 1)
        for (b in src.bytes) { b = uint8(0); }
        dst.blit_alpha(src, 0, 0, 1, 1, 2, 2, 255, 0, 0)
        // dest should be unchanged
        let idx = (1 * 4 + 1) * 4
        tt |> equal(int(dst.bytes[idx + 0]), 100)
        tt |> equal(int(dst.bytes[idx + 1]), 150)
        tt |> equal(int(dst.bytes[idx + 2]), 200)
    }
}

[test(description = "blit_alpha of white with a mask of 128 over black gives a red value within 1 of 128")]
def test_blit_alpha_blend(t : T?) {
    t |> run("blit_alpha with 128 alpha blends correctly") <| @(tt : T?) {
        var dst = make_image(4, 4, 4)
        for (i in range(4 * 4)) {
            let idx = i * 4
            dst.bytes[idx + 0] = uint8(0)
            dst.bytes[idx + 1] = uint8(0)
            dst.bytes[idx + 2] = uint8(0)
            dst.bytes[idx + 3] = uint8(255)
        }
        var src = make_image(2, 2, 1)
        for (b in src.bytes) { b = uint8(128); }
        dst.blit_alpha(src, 0, 0, 0, 0, 2, 2, 255, 255, 255)
        // expected: (128 * 255 + 127 * 0 + 127) / 255 = 128
        let idx = 0
        let val = int(dst.bytes[idx])
        tt |> success(val >= 127 && val <= 129, "blended value ~128, got {val}")
    }
}

[test(description = "blit_alpha to destination (-1, -1) writes only the one pixel that falls inside the image and leaves pixel (1, 0) at 0")]
def test_blit_alpha_clips(t : T?) {
    t |> run("blit_alpha clips to both src and dst bounds") <| @(tt : T?) {
        var dst = make_image(4, 4, 4)
        for (b in dst.bytes) { b = uint8(0); }
        var src = make_image(2, 2, 1)
        for (b in src.bytes) { b = uint8(255); }
        // blit with negative dest offset — should clip
        dst.blit_alpha(src, 0, 0, -1, -1, 2, 2, 255, 255, 255)
        // only pixel (0,0) should be affected (from src[1,1])
        let idx00 = 0
        tt |> equal(int(dst.bytes[idx00]), 255)
        // pixel (1,0) should be untouched
        let idx10 = 1 * 4
        tt |> equal(int(dst.bytes[idx10]), 0)
    }
}

// ---- detailed rast_blit_alpha correctness ----

[test(description = "blit_alpha over a known pixel leaves it unchanged at mask 0, gives the exact colour at 255, and for masks 1 to 254 each channel is within 1 of (a*src + (255-a)*dst)/255 with alpha min(200 + a, 255)")]
def test_blit_alpha_detailed(t : T?) {
    t |> run("blit_alpha blend math is exact for key alpha values") <| @(tt : T?) {
        // test blending onto a known background with specific alpha values
        // formula: out = (alpha * src + (255 - alpha) * dst) / 255  (exact integer)
        let test_alphas = fixed_array(0, 1, 64, 127, 128, 200, 254, 255)
        let src_r = 220; let src_g = 100; let src_b = 30
        let dst_r = 40;  let dst_g = 180; let dst_b = 250
        for (a in test_alphas) {
            var dst = make_image(1, 1, 4)
            dst.bytes[0] = uint8(dst_r)
            dst.bytes[1] = uint8(dst_g)
            dst.bytes[2] = uint8(dst_b)
            dst.bytes[3] = uint8(200)
            var src = make_image(1, 1, 1)
            src.bytes[0] = uint8(a)
            dst.blit_alpha(src, 0, 0, 0, 0, 1, 1, src_r, src_g, src_b)
            if (a == 0) {
                // zero alpha: dest unchanged
                tt |> equal(int(dst.bytes[0]), dst_r)
                tt |> equal(int(dst.bytes[1]), dst_g)
                tt |> equal(int(dst.bytes[2]), dst_b)
                tt |> equal(int(dst.bytes[3]), 200)
            } elif (a == 255) {
                // full alpha: exact source color
                tt |> equal(int(dst.bytes[0]), src_r)
                tt |> equal(int(dst.bytes[1]), src_g)
                tt |> equal(int(dst.bytes[2]), src_b)
                tt |> equal(int(dst.bytes[3]), 255)
            } else {
                // blended: verify within ±1 of exact formula
                let expect_r = (a * src_r + (255 - a) * dst_r + 127) / 255
                let expect_g = (a * src_g + (255 - a) * dst_g + 127) / 255
                let expect_b = (a * src_b + (255 - a) * dst_b + 127) / 255
                let got_r = int(dst.bytes[0])
                let got_g = int(dst.bytes[1])
                let got_b = int(dst.bytes[2])
                tt |> success(abs(got_r - expect_r) <= 1, "R alpha={a}: got {got_r}, expected {expect_r}")
                tt |> success(abs(got_g - expect_g) <= 1, "G alpha={a}: got {got_g}, expected {expect_g}")
                tt |> success(abs(got_b - expect_b) <= 1, "B alpha={a}: got {got_b}, expected {expect_b}")
                // alpha channel: min(dst_a + src_a, 255)
                let expect_a = min(200 + a, 255)
                tt |> equal(int(dst.bytes[3]), expect_a)
            }
        }
    }
}

[test(description = "blit_alpha copies only the requested sub-rectangle to the requested destination position")]
def test_blit_alpha_regions(t : T?) {
    t |> run("blit_alpha sub-rect positions are correct") <| @(tt : T?) {
        // 4x4 dest, 4x4 src alpha — blit a 2x2 sub-rect from src(1,1) to dst(2,2)
        var dst = make_image(4, 4, 4)
        for (b in dst.bytes) { b = uint8(0); }
        var src = make_image(4, 4, 1)
        for (b in src.bytes) { b = uint8(0); }
        // set src pixels at (1,1), (2,1), (1,2), (2,2) to 255
        src.bytes[1 * 4 + 1] = uint8(255)
        src.bytes[1 * 4 + 2] = uint8(255)
        src.bytes[2 * 4 + 1] = uint8(255)
        src.bytes[2 * 4 + 2] = uint8(255)
        dst.blit_alpha(src, 1, 1, 2, 2, 2, 2, 128, 64, 32)
        // dst pixels at (2,2), (3,2), (2,3), (3,3) should be colored
        for (dy in range(4)) {
            for (dx in range(4)) {
                let idx = (dy * 4 + dx) * 4
                if (dx >= 2 && dy >= 2) {
                    tt |> equal(int(dst.bytes[idx + 0]), 128)
                    tt |> equal(int(dst.bytes[idx + 1]), 64)
                    tt |> equal(int(dst.bytes[idx + 2]), 32)
                } else {
                    tt |> equal(int(dst.bytes[idx + 0]), 0)
                }
            }
        }
    }
    t |> run("blit_alpha preserves dest outside blitted region") <| @(tt : T?) {
        var dst = make_image(4, 4, 4)
        // fill entire dest with known color
        for (i in range(4 * 4)) {
            let idx = i * 4
            dst.bytes[idx + 0] = uint8(11)
            dst.bytes[idx + 1] = uint8(22)
            dst.bytes[idx + 2] = uint8(33)
            dst.bytes[idx + 3] = uint8(44)
        }
        var src = make_image(1, 1, 1)
        src.bytes[0] = uint8(255)
        dst.blit_alpha(src, 0, 0, 1, 1, 1, 1, 200, 100, 50)
        // only (1,1) changed
        let idx_changed = (1 * 4 + 1) * 4
        tt |> equal(int(dst.bytes[idx_changed + 0]), 200)
        // check corners are untouched
        for (corner in fixed_array(int2(0, 0), int2(3, 0), int2(0, 3), int2(3, 3))) {
            let idx = (corner.y * 4 + corner.x) * 4
            tt |> equal(int(dst.bytes[idx + 0]), 11)
            tt |> equal(int(dst.bytes[idx + 1]), 22)
            tt |> equal(int(dst.bytes[idx + 2]), 33)
            tt |> equal(int(dst.bytes[idx + 3]), 44)
        }
    }
}
