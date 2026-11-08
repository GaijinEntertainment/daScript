options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

require stbimage/stbimage_boost
require daslib/safe_addr
require dastest/testing_boost public

def test_image_path(name : string) : string => "{get_das_root()}/tests/stbimage/{name}"

// ---- to_channels: uint8 ----

[test(description = "converting the 64x64 RGBA image to 3 channels drops the alpha and keeps the first pixel as (0, 0, 128)")]
def test_to_channels_rgba_to_rgb(t : T?) {
    t |> run("convert 4ch to 3ch") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        tt |> equal(img.channels, 4)
        let rgb <- img.to_channels(3)
        tt |> equal(rgb.channels, 3)
        tt |> equal(rgb.width, 64)
        tt |> equal(rgb.height, 64)
        tt |> success(!rgb.has_alpha(), "no alpha")
        // verify pixel(0,0) = R=0, G=0, B=128
        tt |> equal(int(rgb.bytes[0]), 0)
        tt |> equal(int(rgb.bytes[1]), 0)
        tt |> equal(int(rgb.bytes[2]), 128)
    }
}

[test(description = "converting a 3-channel image to 4 channels adds an opaque alpha of 255 and leaves the first pixel's RGB unchanged")]
def test_to_channels_rgb_to_rgba(t : T?) {
    t |> run("convert 3ch to 4ch") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"), 3)
        tt |> equal(img.channels, 3)
        let rgba <- img.to_channels(4)
        tt |> equal(rgba.channels, 4)
        tt |> success(rgba.has_alpha(), "has alpha")
        // alpha should be 255
        tt |> equal(int(rgba.bytes[3]), 255)
        // RGB preserved
        tt |> equal(int(rgba.bytes[0]), int(img.bytes[0]))
        tt |> equal(int(rgba.bytes[1]), int(img.bytes[1]))
        tt |> equal(int(rgba.bytes[2]), int(img.bytes[2]))
    }
}

[test(description = "converting the RGBA image to 1 channel gives an 8-bit grey image whose first pixel (0, 0, 128) becomes luminance 15")]
def test_to_channels_rgba_to_grey(t : T?) {
    t |> run("convert 4ch to 1ch grey") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let grey <- img.to_channels(1)
        tt |> equal(grey.channels, 1)
        tt |> equal(grey.width, 64)
        tt |> equal(grey.bpc, 1)
        // pixel(0,0) = R=0,G=0,B=128 → grey = (0*77+0*150+128*29+128)>>8 = (3712+128)>>8 = 15
        tt |> equal(int(grey.bytes[0]), 15)
    }
}

[test(description = "converting a grey image to 3 channels copies each grey value into red, green and blue")]
def test_to_channels_grey_to_rgb(t : T?) {
    t |> run("convert 1ch to 3ch") <| @(tt : T?) {
        var img <- make_image(2, 2, 1)
        img.bytes[0] = uint8(100)
        img.bytes[1] = uint8(200)
        img.bytes[2] = uint8(50)
        img.bytes[3] = uint8(0)
        let rgb <- img.to_channels(3)
        tt |> equal(rgb.channels, 3)
        // grey=100 → R=100,G=100,B=100
        tt |> equal(int(rgb.bytes[0]), 100)
        tt |> equal(int(rgb.bytes[1]), 100)
        tt |> equal(int(rgb.bytes[2]), 100)
        // grey=200 → R=200,G=200,B=200
        tt |> equal(int(rgb.bytes[3]), 200)
        tt |> equal(int(rgb.bytes[4]), 200)
        tt |> equal(int(rgb.bytes[5]), 200)
    }
}

[test(description = "converting a grey image to 2 channels keeps each grey value and adds an opaque alpha of 255")]
def test_to_channels_grey_to_grey_alpha(t : T?) {
    t |> run("convert 1ch to 2ch") <| @(tt : T?) {
        var img <- make_image(2, 1, 1)
        img.bytes[0] = uint8(42)
        img.bytes[1] = uint8(200)
        let ga <- img.to_channels(2)
        tt |> equal(ga.channels, 2)
        tt |> equal(int(ga.bytes[0]), 42)
        tt |> equal(int(ga.bytes[1]), 255)
        tt |> equal(int(ga.bytes[2]), 200)
        tt |> equal(int(ga.bytes[3]), 255)
    }
}

[test(description = "converting a grey-plus-alpha image to 1 channel keeps the grey values and discards the alpha")]
def test_to_channels_grey_alpha_to_grey(t : T?) {
    t |> run("convert 2ch to 1ch") <| @(tt : T?) {
        var img <- make_image(2, 1, 2)
        img.bytes[0] = uint8(42)
        img.bytes[1] = uint8(128)
        img.bytes[2] = uint8(200)
        img.bytes[3] = uint8(255)
        let grey <- img.to_channels(1)
        tt |> equal(grey.channels, 1)
        tt |> equal(int(grey.bytes[0]), 42)
        tt |> equal(int(grey.bytes[1]), 200)
    }
}

[test(description = "to_channels with the image's own channel count returns the same first 16 bytes")]
def test_to_channels_same(t : T?) {
    t |> run("same channel count is identity") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let same <- img.to_channels(4)
        tt |> equal(same.channels, 4)
        for (i in range(16)) {
            tt |> equal(int(img.bytes[i]), int(same.bytes[i]))
        }
    }
}

[test(description = "converting an RGBA pixel (100, 100, 100, 200) to 2 channels gives grey 100 and keeps alpha 200")]
def test_to_channels_rgba_to_grey_alpha(t : T?) {
    t |> run("convert 4ch to 2ch grey+alpha") <| @(tt : T?) {
        var img <- make_image(1, 1, 4)
        img.bytes[0] = uint8(100)   // R
        img.bytes[1] = uint8(100)   // G
        img.bytes[2] = uint8(100)   // B
        img.bytes[3] = uint8(200)   // A
        let ga <- img.to_channels(2)
        tt |> equal(ga.channels, 2)
        // luminance of (100,100,100) = (100*77+100*150+100*29+128)>>8 = (25600+128)>>8 = 100
        tt |> equal(int(ga.bytes[0]), 100)
        tt |> equal(int(ga.bytes[1]), 200)
    }
}

// ---- to_channels: float (HDR) ----

[test(description = "converting the float HDR image to 3 channels keeps 4-byte floats and the last pixel's red and green near 1 and blue near 0.5")]
def test_to_channels_hdr_rgba_to_rgb(t : T?) {
    t |> run("convert HDR 4ch to 3ch") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        tt |> equal(img.channels, 4)
        tt |> equal(img.bpc, 4)
        var rgb <- img.to_channels(3)
        tt |> equal(rgb.channels, 3)
        tt |> equal(rgb.bpc, 4)
        // verify float pixel (63,63): R~0.98, G~0.98, B~0.5
        rgb |> with_pixels_f() $(var pixels : array<float>#) {
            let idx = (63 * 64 + 63) * 3
            tt |> success(pixels[idx] > 0.9f, "R near 1.0")
            tt |> success(pixels[idx + 1] > 0.9f, "G near 1.0")
            tt |> success(pixels[idx + 2] > 0.3f && pixels[idx + 2] < 0.7f, "B near 0.5")
        }
    }
}

[test(description = "converting the 3-channel HDR image to 1 channel keeps floats and turns the dark blue first pixel into a grey below 0.15")]
def test_to_channels_hdr_rgb_to_grey(t : T?) {
    t |> run("convert HDR 3ch to 1ch grey") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 3)
        var grey <- img.to_channels(1)
        tt |> equal(grey.channels, 1)
        tt |> equal(grey.bpc, 4)
        // pixel(0,0) is (0,0,0.5) → grey = 0.0*0.299 + 0.0*0.587 + 0.5*0.114 = 0.057
        grey |> with_pixels_f() $(var pixels : array<float>#) {
            tt |> success(pixels[0] < 0.15f, "corner grey near 0.057")
        }
    }
}

[test(description = "converting a float grey image to 4 channels copies a grey of 0.75 into red, green and blue and sets alpha to 1")]
def test_to_channels_hdr_grey_to_rgba(t : T?) {
    t |> run("convert HDR 1ch to 4ch") <| @(tt : T?) {
        var img <- make_image(2, 1, 1, 4)
        img |> with_pixels_f() $(var pixels : array<float>#) {
            pixels[0] = 0.75f
            pixels[1] = 0.25f
        }
        var rgba <- img.to_channels(4)
        tt |> equal(rgba.channels, 4)
        tt |> equal(rgba.bpc, 4)
        rgba |> with_pixels_f() $(var pixels : array<float>#) {
            tt |> success(pixels[0] > 0.74f && pixels[0] < 0.76f, "R=grey")
            tt |> success(pixels[1] > 0.74f && pixels[1] < 0.76f, "G=grey")
            tt |> success(pixels[2] > 0.74f && pixels[2] < 0.76f, "B=grey")
            tt |> success(pixels[3] > 0.99f, "A=1.0")
        }
    }
}

// ---- to_channels: uint16 ----

[test(description = "converting the 16-bit RGBA image to 3 channels gives a valid 64-wide image that stays 16-bit")]
def test_to_channels_16bit_rgba_to_rgb(t : T?) {
    t |> run("convert 16-bit 4ch to 3ch") <| @(tt : T?) {
        var img : Image
        img.load_16(test_image_path("test.png"), 4)
        tt |> equal(img.bpc, 2)
        tt |> equal(img.channels, 4)
        let rgb <- img.to_channels(3)
        tt |> equal(rgb.channels, 3)
        tt |> equal(rgb.bpc, 2)
        tt |> equal(rgb.width, 64)
        tt |> success(rgb.valid(), "valid after conversion")
    }
}

[test(description = "converting a 16-bit grey pixel of 32768 to 4 channels copies it into red, green and blue and sets alpha to 65535")]
def test_to_channels_16bit_grey_to_rgba(t : T?) {
    t |> run("convert 16-bit 1ch to 4ch") <| @(tt : T?) {
        var img <- make_image(1, 1, 1, 2)
        img |> with_pixels_16() $(var pixels : array<uint16>#) {
            pixels[0] = uint16(32768)
        }
        var rgba <- img.to_channels(4)
        tt |> equal(rgba.channels, 4)
        tt |> equal(rgba.bpc, 2)
        rgba |> with_pixels_16() $(var pixels : array<uint16>#) {
            tt |> equal(int(pixels[0]), 32768)
            tt |> equal(int(pixels[1]), 32768)
            tt |> equal(int(pixels[2]), 32768)
            tt |> equal(int(pixels[3]), 65535)
        }
    }
}

// ---- to_bpc: all 6 conversion paths ----

[test(description = "converting 8-bit values 255, 0, 128, 255 to floats gives about 1.0, 0.0, 0.502 and 1.0")]
def test_to_bpc_u8_to_f32(t : T?) {
    t |> run("convert uint8 to float") <| @(tt : T?) {
        var img <- make_image(2, 2, 4)
        img.bytes[0] = uint8(255)
        img.bytes[1] = uint8(0)
        img.bytes[2] = uint8(128)
        img.bytes[3] = uint8(255)
        var hdr <- img.to_bpc(4)
        tt |> equal(hdr.bpc, 4)
        tt |> success(hdr.is_hdr(), "is HDR")
        tt |> equal(hdr.channels, 4)
        hdr |> with_pixels_f() $(var pixels : array<float>#) {
            tt |> success(pixels[0] > 0.99f, "R=255 → ~1.0")
            tt |> success(pixels[1] < 0.01f, "G=0 → ~0.0")
            tt |> success(pixels[2] > 0.49f && pixels[2] < 0.52f, "B=128 → ~0.502")
            tt |> success(pixels[3] > 0.99f, "A=255 → ~1.0")
        }
    }
}

[test(description = "converting floats 1.0, 0.0, 0.5, 1.0 to 8-bit gives exactly 255, 0, 128 and 255")]
def test_to_bpc_f32_to_u8(t : T?) {
    t |> run("convert float to uint8") <| @(tt : T?) {
        var img <- make_image(2, 2, 4, 4)
        img |> with_pixels_f() $(var pixels : array<float>#) {
            pixels[0] = 1.0f
            pixels[1] = 0.0f
            pixels[2] = 0.5f
            pixels[3] = 1.0f
        }
        let u8img <- img.to_bpc(1)
        tt |> equal(u8img.bpc, 1)
        tt |> equal(int(u8img.bytes[0]), 255)
        tt |> equal(int(u8img.bytes[1]), 0)
        tt |> equal(int(u8img.bytes[2]), 128)
        tt |> equal(int(u8img.bytes[3]), 255)
    }
}

[test(description = "converting 8-bit values 255, 0, 128, 1 to 16-bit replicates each byte into both halves, giving 65535, 0, 32896 and 257")]
def test_to_bpc_u8_to_u16(t : T?) {
    t |> run("convert uint8 to uint16") <| @(tt : T?) {
        var img <- make_image(2, 2, 1)
        img.bytes[0] = uint8(255)
        img.bytes[1] = uint8(0)
        img.bytes[2] = uint8(128)
        img.bytes[3] = uint8(1)
        var u16img <- img.to_bpc(2)
        tt |> equal(u16img.bpc, 2)
        u16img |> with_pixels_16() $(var pixels : array<uint16>#) {
            tt |> equal(int(pixels[0]), 65535)
            tt |> equal(int(pixels[1]), 0)
            tt |> equal(int(pixels[2]), 32896)
            tt |> equal(int(pixels[3]), 257)
        }
    }
}

[test(description = "converting 16-bit values 65535, 0, 32896, 257 to 8-bit gives 255, 0, 128 and 1")]
def test_to_bpc_u16_to_u8(t : T?) {
    t |> run("convert uint16 to uint8") <| @(tt : T?) {
        var img <- make_image(2, 2, 1, 2)
        img |> with_pixels_16() $(var pixels : array<uint16>#) {
            pixels[0] = uint16(65535)
            pixels[1] = uint16(0)
            pixels[2] = uint16(32896)
            pixels[3] = uint16(257)
        }
        let u8img <- img.to_bpc(1)
        tt |> equal(u8img.bpc, 1)
        tt |> equal(int(u8img.bytes[0]), 255)
        tt |> equal(int(u8img.bytes[1]), 0)
        tt |> equal(int(u8img.bytes[2]), 128)
        tt |> equal(int(u8img.bytes[3]), 1)
    }
}

[test(description = "converting 16-bit values 65535, 0, 32768, 65535 to floats gives about 1.0, 0.0, 0.5 and 1.0")]
def test_to_bpc_u16_to_f32(t : T?) {
    t |> run("convert uint16 to float") <| @(tt : T?) {
        var img <- make_image(1, 1, 4, 2)
        img |> with_pixels_16() $(var pixels : array<uint16>#) {
            pixels[0] = uint16(65535)
            pixels[1] = uint16(0)
            pixels[2] = uint16(32768)
            pixels[3] = uint16(65535)
        }
        var hdr <- img.to_bpc(4)
        tt |> equal(hdr.bpc, 4)
        hdr |> with_pixels_f() $(var pixels : array<float>#) {
            tt |> success(pixels[0] > 0.99f, "65535 → ~1.0")
            tt |> success(pixels[1] < 0.01f, "0 → ~0.0")
            tt |> success(pixels[2] > 0.49f && pixels[2] < 0.51f, "32768 → ~0.5")
            tt |> success(pixels[3] > 0.99f, "65535 → ~1.0")
        }
    }
}

[test(description = "converting floats 1.0, 0.0, 0.5, 1.0 to 16-bit gives exactly 65535, 0, 32768 and 65535")]
def test_to_bpc_f32_to_u16(t : T?) {
    t |> run("convert float to uint16") <| @(tt : T?) {
        var img <- make_image(1, 1, 4, 4)
        img |> with_pixels_f() $(var pixels : array<float>#) {
            pixels[0] = 1.0f
            pixels[1] = 0.0f
            pixels[2] = 0.5f
            pixels[3] = 1.0f
        }
        var u16img <- img.to_bpc(2)
        tt |> equal(u16img.bpc, 2)
        u16img |> with_pixels_16() $(var pixels : array<uint16>#) {
            tt |> equal(int(pixels[0]), 65535)
            tt |> equal(int(pixels[1]), 0)
            tt |> equal(int(pixels[2]), 32768)
            tt |> equal(int(pixels[3]), 65535)
        }
    }
}

// ---- to_bpc: identity and roundtrip ----

[test(description = "to_bpc with the image's own bytes per channel returns the same first 16 bytes")]
def test_to_bpc_same(t : T?) {
    t |> run("to_bpc same bpc is identity") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let same <- img.to_bpc(1)
        tt |> equal(same.bpc, 1)
        for (i in range(16)) {
            tt |> equal(int(img.bytes[i]), int(same.bytes[i]))
        }
    }
}

[test(description = "converting a 4x4 RGBA image holding bytes 0..63 to floats and back to 8-bit leaves zero differing bytes")]
def test_to_bpc_roundtrip_u8_f32_u8(t : T?) {
    t |> run("u8 → f32 → u8 roundtrip preserves values") <| @(tt : T?) {
        var img <- make_image(4, 4, 4)
        for (i in range(length(img.bytes))) {
            img.bytes[i] = uint8(i & int(0xFF))
        }
        var hdr <- img.to_bpc(4)
        let back <- hdr.to_bpc(1)
        var mismatch = 0
        for (i in range(length(img.bytes))) {
            if (img.bytes[i] != back.bytes[i]) {
                mismatch++
            }
        }
        tt |> equal(mismatch, 0)
    }
}

[test(description = "converting a 4x4 RGBA image holding bytes 0..63 to 16-bit and back to 8-bit leaves zero differing bytes")]
def test_to_bpc_roundtrip_u8_u16_u8(t : T?) {
    t |> run("u8 → u16 → u8 roundtrip preserves values") <| @(tt : T?) {
        var img <- make_image(4, 4, 4)
        for (i in range(length(img.bytes))) {
            img.bytes[i] = uint8(i & int(0xFF))
        }
        var u16 <- img.to_bpc(2)
        let back <- u16.to_bpc(1)
        var mismatch = 0
        for (i in range(length(img.bytes))) {
            if (img.bytes[i] != back.bytes[i]) {
                mismatch++
            }
        }
        tt |> equal(mismatch, 0)
    }
}

// ---- to_bpc: clamping ----

[test(description = "converting floats 2.0 and -0.5 to 8-bit clamps them to 255 and 0 while 0.5 and 1.0 give 128 and 255")]
def test_to_bpc_f32_clamp(t : T?) {
    t |> run("float to uint8 clamps out-of-range") <| @(tt : T?) {
        var img <- make_image(1, 1, 4, 4)
        img |> with_pixels_f() $(var pixels : array<float>#) {
            pixels[0] = 2.0f     // over 1.0
            pixels[1] = -0.5f    // negative
            pixels[2] = 0.5f
            pixels[3] = 1.0f
        }
        let u8img <- img.to_bpc(1)
        tt |> equal(int(u8img.bytes[0]), 255)
        tt |> equal(int(u8img.bytes[1]), 0)
        tt |> equal(int(u8img.bytes[2]), 128)
        tt |> equal(int(u8img.bytes[3]), 255)
    }
}

// ---- combined: to_channels + to_bpc ----

[test(description = "converting the RGBA 8-bit image to 3 channels and then to floats gives a valid 64-wide 3-channel float image")]
def test_convert_channels_then_bpc(t : T?) {
    t |> run("RGBA u8 → RGB u8 → RGB float") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var rgb <- img.to_channels(3)
        let hdr <- rgb.to_bpc(4)
        tt |> equal(hdr.channels, 3)
        tt |> equal(hdr.bpc, 4)
        tt |> equal(hdr.width, 64)
        tt |> success(hdr.valid(), "valid after two conversions")
    }
}

[test(description = "converting the RGBA 8-bit image to floats and then to 1 channel gives a valid 64-wide grey float image")]
def test_convert_bpc_then_channels(t : T?) {
    t |> run("RGBA u8 → RGBA float → grey float") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var hdr <- img.to_bpc(4)
        let grey <- hdr.to_channels(1)
        tt |> equal(grey.channels, 1)
        tt |> equal(grey.bpc, 4)
        tt |> equal(grey.width, 64)
        tt |> success(grey.valid(), "valid after two conversions")
    }
}
