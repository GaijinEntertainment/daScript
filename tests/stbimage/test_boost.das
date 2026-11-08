options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

require stbimage/stbimage_boost
require daslib/safe_addr
require daslib/fio
require dastest/testing_boost public

def test_image_path(name : string) : string => "{get_das_root()}/tests/stbimage/{name}"

// ---- struct queries ----

[test(description = "an Image that was declared but never loaded reports valid() as false")]
def test_default_image_invalid(t : T?) {
    t |> run("freshly declared Image is not valid") <| @(tt : T?) {
        let img : Image
        tt |> success(!img.valid(), "default image not valid")
    }
}

[test(description = "loading test.png succeeds and leaves the Image reporting valid() as true")]
def test_valid_after_load(t : T?) {
    t |> run("Image is valid after load") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("test.png"))
        tt |> success(ok, "load ok: {error}")
        tt |> success(img.valid(), "image is valid")
    }
}

[test(description = "the stride of the loaded 64x64 RGBA 8-bit image is 256 bytes, one row of 64 four-byte pixels")]
def test_stride(t : T?) {
    t |> run("stride of 64x64 RGBA uint8 image") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        tt |> equal(img.stride(), 64 * 4 * 1)
    }
}

[test(description = "the stride of the 64x64 four-channel HDR image is 1024 bytes because each channel is a 4-byte float")]
def test_stride_hdr(t : T?) {
    t |> run("stride of 64x64 HDR image") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        tt |> equal(img.stride(), 64 * 4 * 4)
    }
}

[test(description = "has_alpha is true for the PNG loaded as four channels and false for the JPG loaded as three")]
def test_has_alpha(t : T?) {
    t |> run("has_alpha for RGBA and RGB") <| @(tt : T?) {
        var img4 : Image
        img4.load(test_image_path("test.png"))
        tt |> success(img4.has_alpha(), "4ch has alpha")
        var img3 : Image
        img3.load(test_image_path("test.jpg"), 3)
        tt |> success(!img3.has_alpha(), "3ch no alpha")
    }
}

[test(description = "pixel_size is 4 bytes for the RGBA 8-bit PNG and 16 bytes for the RGBA float HDR image")]
def test_pixel_size(t : T?) {
    t |> run("pixel_size for uint8 and float") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        tt |> equal(img.pixel_size(), 4)
        var hdr : Image
        hdr.load_hdr(test_image_path("test.hdr"), 4)
        tt |> equal(hdr.pixel_size(), 16)
    }
}

[test(description = "is_hdr is false for the 8-bit PNG Image and true for the Image loaded from the .hdr file")]
def test_is_hdr_query(t : T?) {
    t |> run("is_hdr query on uint8 and float images") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        tt |> success(!img.is_hdr(), "uint8 not HDR")
        var hdr : Image
        hdr.load_hdr(test_image_path("test.hdr"), 4)
        tt |> success(hdr.is_hdr(), "float is HDR")
    }
}

[test(description = "data_size of the loaded 64x64 RGBA 8-bit image is 16384 bytes")]
def test_data_size(t : T?) {
    t |> run("data_size matches expected") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        tt |> equal(img.data_size(), 64 * 64 * 4 * 1)
    }
}

// ---- load from file ----

[test(description = "loading test.png succeeds and yields a 64x64 image with 4 channels and 1 byte per channel")]
def test_load_png(t : T?) {
    t |> run("load PNG from file") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("test.png"))
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.width, 64)
        tt |> equal(img.height, 64)
        tt |> equal(img.channels, 4)
        tt |> equal(img.bpc, 1)
    }
}

[test(description = "loading test.bmp succeeds and yields a 64x64 image with 1 byte per channel")]
def test_load_bmp(t : T?) {
    t |> run("load BMP from file") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("test.bmp"))
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.width, 64)
        tt |> equal(img.height, 64)
        tt |> equal(img.bpc, 1)
    }
}

[test(description = "loading test.tga succeeds and yields a 64x64 image with 1 byte per channel")]
def test_load_tga(t : T?) {
    t |> run("load TGA from file") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("test.tga"))
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.width, 64)
        tt |> equal(img.height, 64)
        tt |> equal(img.bpc, 1)
    }
}

[test(description = "loading test.jpg with three requested channels yields a 64x64 image with 3 channels and 1 byte per channel")]
def test_load_jpg(t : T?) {
    t |> run("load JPG from file") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("test.jpg"), 3)
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.width, 64)
        tt |> equal(img.height, 64)
        tt |> equal(img.channels, 3)
        tt |> equal(img.bpc, 1)
    }
}

[test(description = "load_hdr on test.hdr succeeds and yields a 64x64 image stored as 4-byte floats")]
def test_load_hdr_file(t : T?) {
    t |> run("load HDR from file") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load_hdr(test_image_path("test.hdr"), 4)
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.width, 64)
        tt |> equal(img.height, 64)
        tt |> equal(img.bpc, 4)
    }
}

[test(description = "load_16 on the 8-bit test.png succeeds and yields a 64x64 image widened to 2 bytes per channel")]
def test_load_16_file(t : T?) {
    t |> run("load 16-bit from file") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load_16(test_image_path("test.png"))
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.width, 64)
        tt |> equal(img.height, 64)
        tt |> equal(img.bpc, 2)
    }
}

[test(description = "loading the RGBA test.png with three requested channels returns an image with exactly 3 channels")]
def test_load_requested_channels(t : T?) {
    t |> run("load with requested_channels changes channel count") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("test.png"), 3)
        tt |> success(ok, "load ok: {error}")
        tt |> equal(img.channels, 3)
    }
}

[test(description = "loading a file that does not exist reports failure with a non-empty error and leaves the Image invalid")]
def test_load_failure(t : T?) {
    t |> run("load nonexistent file returns error") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.load(test_image_path("does_not_exist.png"))
        tt |> success(!ok, "load should fail")
        tt |> success(!empty(error), "error message not empty")
        tt |> success(!img.valid(), "image not valid after failure")
    }
}

// ---- load from memory ----

[test(description = "PNG bytes encoded from test.png decode through load_from_memory into a 64x64 image with 1 byte per channel")]
def test_load_from_memory(t : T?) {
    t |> run("load from memory buffer") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var png_bytes : array<uint8>
        let (enc_ok, enc_err) = img.encode("png", png_bytes)
        tt |> success(enc_ok, "encode ok: {enc_err}")
        var img2 : Image
        let (ok, error) = img2.load_from_memory(png_bytes)
        tt |> success(ok, "load_from_memory ok: {error}")
        tt |> equal(img2.width, 64)
        tt |> equal(img2.height, 64)
        tt |> equal(img2.bpc, 1)
    }
}

[test(description = "load_from_memory on 16 bytes that are not an image reports failure with a non-empty error")]
def test_load_from_memory_failure(t : T?) {
    t |> run("load from garbage memory fails") <| @(tt : T?) {
        var garbage : array<uint8>
        garbage |> resize(16)
        for (i in range(16)) {
            garbage[i] = uint8(i)
        }
        var img : Image
        let (ok, error) = img.load_from_memory(garbage)
        tt |> success(!ok, "should fail")
        tt |> success(!empty(error), "error not empty")
    }
}

[test(description = "load_16_from_memory on 8-bit PNG bytes succeeds and returns an image with 2 bytes per channel")]
def test_load_16_from_memory(t : T?) {
    t |> run("load 16-bit from memory") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var png_bytes : array<uint8>
        img.encode("png", png_bytes)
        var img2 : Image
        let (ok, error) = img2.load_16_from_memory(png_bytes)
        tt |> success(ok, "load_16_from_memory ok: {error}")
        tt |> equal(img2.bpc, 2)
    }
}

[test(description = "load_from_memory on RGBA PNG bytes with three requested channels returns an image with exactly 3 channels")]
def test_load_from_memory_requested_channels(t : T?) {
    t |> run("load from memory with requested channels") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var png_bytes : array<uint8>
        img.encode("png", png_bytes)
        var img2 : Image
        let (ok, error) = img2.load_from_memory(png_bytes, 3)
        tt |> success(ok, "ok: {error}")
        tt |> equal(img2.channels, 3)
    }
}

[test(description = "the raw bytes of test.hdr decode through load_hdr_from_memory into a 64x64 float image with at least one channel")]
def test_load_hdr_from_memory(t : T?) {
    t |> run("load HDR from memory buffer") <| @(tt : T?) {
        // load HDR file to get raw float data, encode as HDR to memory, then load_hdr_from_memory
        var hdr : Image
        let (ok1, err1) = hdr.load_hdr(test_image_path("test.hdr"), 4)
        tt |> success(ok1, "initial load_hdr ok: {err1}")
        // read the raw .hdr file bytes
        var file_bytes : array<uint8>
        fopen(test_image_path("test.hdr"), "rb") $(f) {
            if (f != null) {
                fmap(f) $(var data : array<uint8>#) {
                    file_bytes := data
                }
            }
        }
        tt |> success(!empty(file_bytes), "read .hdr file bytes")
        var img2 : Image
        let (ok2, err2) = img2.load_hdr_from_memory(file_bytes)
        tt |> success(ok2, "load_hdr_from_memory ok: {err2}")
        tt |> equal(img2.bpc, 4)
        tt |> equal(img2.width, 64)
        tt |> equal(img2.height, 64)
        tt |> success(img2.channels > 0, "has channels")
    }
}

// ---- save to file ----

[test(description = "an image saved to a .png path loads back successfully as a 64x64 image")]
def test_save_png(t : T?) {
    t |> run("save as PNG and load back") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let out_path = path_join(test_temp_dir(), "_test_out.png")
        let (ok, error) = img.save(out_path)
        tt |> success(ok, "save ok: {error}")
        var img2 : Image
        let (ok2, error2) = img2.load(out_path)
        tt |> success(ok2, "reload ok: {error2}")
        tt |> equal(img2.width, 64)
        tt |> equal(img2.height, 64)
        fio::remove(out_path)
    }
}

[test(description = "an image saved to a .bmp path loads back successfully with width 64")]
def test_save_bmp(t : T?) {
    t |> run("save as BMP and load back") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let out_path = path_join(test_temp_dir(), "_test_out.bmp")
        let (ok, error) = img.save(out_path)
        tt |> success(ok, "save ok: {error}")
        var img2 : Image
        let (ok2, error2) = img2.load(out_path)
        tt |> success(ok2, "reload ok: {error2}")
        tt |> equal(img2.width, 64)
        fio::remove(out_path)
    }
}

[test(description = "an image saved to a .tga path loads back successfully with width 64")]
def test_save_tga(t : T?) {
    t |> run("save as TGA and load back") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let out_path = path_join(test_temp_dir(), "_test_out.tga")
        let (ok, error) = img.save(out_path)
        tt |> success(ok, "save ok: {error}")
        var img2 : Image
        let (ok2, error2) = img2.load(out_path)
        tt |> success(ok2, "reload ok: {error2}")
        tt |> equal(img2.width, 64)
        fio::remove(out_path)
    }
}

[test(description = "a three-channel image saved to a .jpg path with the default quality loads back successfully with width 64")]
def test_save_jpg(t : T?) {
    t |> run("save as JPG with default quality") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"), 3)
        let out_path = path_join(test_temp_dir(), "_test_out.jpg")
        let (ok, error) = img.save(out_path)
        tt |> success(ok, "save ok: {error}")
        var img2 : Image
        let (ok2, error2) = img2.load(out_path, 3)
        tt |> success(ok2, "reload ok: {error2}")
        tt |> equal(img2.width, 64)
        fio::remove(out_path)
    }
}

[test(description = "saving an Image that holds no pixels reports failure with a non-empty error")]
def test_save_invalid_image(t : T?) {
    t |> run("save invalid image returns error") <| @(tt : T?) {
        var img : Image
        let (ok, error) = img.save(path_join(test_temp_dir(), "_test_out.png"))
        tt |> success(!ok, "should fail")
        tt |> success(!empty(error), "error not empty")
    }
}

// ---- encode to memory ----

[test(description = "encoding the image as png succeeds and the output starts with the PNG signature bytes 137 80 78 71")]
def test_encode_png(t : T?) {
    t |> run("encode PNG to memory with magic bytes") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var out : array<uint8>
        let (ok, error) = img.encode("png", out)
        tt |> success(ok, "encode ok: {error}")
        tt |> success(length(out) > 8, "has data")
        // PNG magic: 137 80 78 71
        tt |> equal(int(out[0]), 137)
        tt |> equal(int(out[1]), 80)
        tt |> equal(int(out[2]), 78)
        tt |> equal(int(out[3]), 71)
    }
}

[test(description = "encoding the image as bmp succeeds and the output starts with the bytes 'B' 'M'")]
def test_encode_bmp(t : T?) {
    t |> run("encode BMP to memory with magic bytes") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var out : array<uint8>
        let (ok, error) = img.encode("bmp", out)
        tt |> success(ok, "encode ok: {error}")
        // BMP magic: 66 77 ('BM')
        tt |> equal(int(out[0]), 66)
        tt |> equal(int(out[1]), 77)
    }
}

[test(description = "encoding the three-channel image as jpg succeeds and the output starts with the JPEG marker bytes 255 216")]
def test_encode_jpg(t : T?) {
    t |> run("encode JPG to memory with magic bytes") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"), 3)
        var out : array<uint8>
        let (ok, error) = img.encode("jpg", out)
        tt |> success(ok, "encode ok: {error}")
        // JPEG magic: 255 216
        tt |> equal(int(out[0]), 255)
        tt |> equal(int(out[1]), 216)
    }
}

[test(description = "encoding the image as tga succeeds and produces a non-empty byte array")]
def test_encode_tga(t : T?) {
    t |> run("encode TGA to memory") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var out : array<uint8>
        let (ok, error) = img.encode("tga", out)
        tt |> success(ok, "encode ok: {error}")
        tt |> success(!empty(out), "has data")
    }
}

[test(description = "encoding to the unsupported format gif reports failure with a non-empty error")]
def test_encode_invalid_format(t : T?) {
    t |> run("encode unsupported format returns error") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var out : array<uint8>
        let (ok, error) = img.encode("gif", out)
        tt |> success(!ok, "should fail")
        tt |> success(!empty(error), "error not empty")
    }
}

// ---- pixel access: with_pixels ----

[test(description = "with_pixels over bytes shows test.png's top-left pixel as (0, 0, 128, 255) and its bottom-right pixel as (252, 252, 128)")]
def test_with_pixels_uint8(t : T?) {
    t |> run("with_pixels uint8 reads corner pixels") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        img |> with_pixels() $(var pixels : array<uint8>#) {
            // pixel(0,0) = R=0, G=0, B=128, A=255
            tt |> equal(int(pixels[0]), 0)
            tt |> equal(int(pixels[1]), 0)
            tt |> equal(int(pixels[2]), 128)
            tt |> equal(int(pixels[3]), 255)
            // pixel(63,63) = R=252, G=252, B=128, A=255
            let last = (63 * 64 + 63) * 4
            tt |> equal(int(pixels[last + 0]), 252)
            tt |> equal(int(pixels[last + 1]), 252)
            tt |> equal(int(pixels[last + 2]), 128)
        }
    }
}

[test(description = "with_pixels over floats shows the HDR image's first pixel as red and green near 0, blue near 0.5 and alpha near 1")]
def test_with_pixels_float(t : T?) {
    t |> run("with_pixels float reads HDR data") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        img |> with_pixels() $(var pixels : array<float>#) {
            // pixel(0,0): R~0, G~0, B~0.5, A~1.0
            tt |> success(pixels[0] < 0.05f, "R near 0")
            tt |> success(pixels[1] < 0.05f, "G near 0")
            tt |> success(pixels[2] > 0.3f && pixels[2] < 0.7f, "B near 0.5")
            tt |> success(pixels[3] > 0.9f, "A near 1.0")
        }
    }
}

[test(description = "with_pixels over uint16 shows the 16-bit load's first pixel with red and green 0 and blue scaled up to a non-zero value")]
def test_with_pixels_16(t : T?) {
    t |> run("with_pixels uint16 reads 16-bit data") <| @(tt : T?) {
        var img : Image
        img.load_16(test_image_path("test.png"))
        img |> with_pixels() $(var pixels : array<uint16>#) {
            // pixel(0,0) in 16-bit: R=0, G=0
            tt |> equal(int(pixels[0]), 0)
            tt |> equal(int(pixels[1]), 0)
            // B channel should be non-zero (128 scaled to 16-bit)
            tt |> success(int(pixels[2]) > 0, "B non-zero in 16-bit")
        }
    }
}

[test(description = "a byte written inside one mutable with_pixels block is still there when the pixels are viewed again")]
def test_with_pixels_modify(t : T?) {
    t |> run("mutable with_pixels modifies pixel data") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        img |> with_pixels() $(var pixels : array<uint8>#) {
            pixels[0] = uint8(42)
        }
        // verify change persisted
        img |> with_pixels() $(var pixels : array<uint8>#) {
            tt |> equal(int(pixels[0]), 42)
        }
    }
}

[test(description = "with_pixels_f exposes the HDR image as floats whose first alpha is near 1")]
def test_with_pixels_named_f(t : T?) {
    t |> run("with_pixels_f explicit name on HDR") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        img |> with_pixels_f() $(var pixels : array<float>#) {
            tt |> success(pixels[3] > 0.9f, "alpha near 1.0")
        }
    }
}

[test(description = "with_pixels_16 exposes the 16-bit load as 64*64*4 uint16 values")]
def test_with_pixels_named_16(t : T?) {
    t |> run("with_pixels_16 explicit name") <| @(tt : T?) {
        var img : Image
        img.load_16(test_image_path("test.png"))
        img |> with_pixels_16() $(var pixels : array<uint16>#) {
            tt |> success(length(pixels) == 64 * 64 * 4, "correct length")
        }
    }
}

// ---- pixel access: with_row ----

[test(description = "with_row 0 exposes 256 bytes whose first pixel has red and green equal to 0")]
def test_with_row(t : T?) {
    t |> run("with_row reads first row") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        img |> with_row(0) $(var row : array<uint8>#) {
            tt |> equal(length(row), 64 * 4)
            // first pixel row 0: R=0, G=0
            tt |> equal(int(row[0]), 0)
            tt |> equal(int(row[1]), 0)
        }
    }
}

[test(description = "with_row 63 exposes 256 bytes whose first pixel has red 0 and green 252, unlike row 0")]
def test_with_row_last(t : T?) {
    t |> run("with_row reads last row") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        img |> with_row(63) $(var row : array<uint8>#) {
            tt |> equal(length(row), 64 * 4)
            // last row, first pixel: R=0, G=252
            tt |> equal(int(row[0]), 0)
            tt |> equal(int(row[1]), 252)
        }
    }
}

[test(description = "with_row over floats on the HDR image exposes 256 values whose first blue is near 0.5")]
def test_with_row_float(t : T?) {
    t |> run("with_row float on HDR image") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        img |> with_row(0) $(var row : array<float>#) {
            tt |> equal(length(row), 64 * 4)
            tt |> success(row[2] > 0.3f && row[2] < 0.7f, "B near 0.5")
        }
    }
}

[test(description = "a byte written inside a mutable with_row block is still there when the same row is viewed again")]
def test_with_row_modify(t : T?) {
    t |> run("mutable with_row modifies row data") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        img |> with_row(0) $(var row : array<uint8>#) {
            row[0] = uint8(99)
        }
        img |> with_row(0) $(var row : array<uint8>#) {
            tt |> equal(int(row[0]), 99)
        }
    }
}

// ---- resize ----

[test(description = "resizing the 64x64 RGBA image to 32x32 gives a valid 32x32 image that keeps 4 channels and 1 byte per channel")]
def test_resize_downscale(t : T?) {
    t |> run("resize 64x64 to 32x32") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let small <- img.resize(32, 32)
        tt |> equal(small.width, 32)
        tt |> equal(small.height, 32)
        tt |> equal(small.channels, 4)
        tt |> equal(small.bpc, 1)
        tt |> success(small.valid(), "resized is valid")
    }
}

[test(description = "resizing the 64x64 RGBA image up to 128x128 gives a 128x128 image that keeps 4 channels")]
def test_resize_upscale(t : T?) {
    t |> run("resize 64x64 to 128x128") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let big <- img.resize(128, 128)
        tt |> equal(big.width, 128)
        tt |> equal(big.height, 128)
        tt |> equal(big.channels, 4)
    }
}

[test(description = "enlarging an all-255 2x2 image to 4x4 leaves every output byte at 255")]
def test_resize_uniform_color(t : T?) {
    t |> run("resize uniform white stays white") <| @(tt : T?) {
        var img = Image(width = 2, height = 2, channels = 4, bpc = 1)
        img.bytes |> resize(2 * 2 * 4)
        for (i in range(16)) {
            img.bytes[i] = uint8(255)
        }
        var big <- img.resize(4, 4)
        big |> with_pixels() $(var pixels : array<uint8>#) {
            for (i in range(length(pixels))) {
                tt |> equal(int(pixels[i]), 255)
            }
        }
    }
}

[test(description = "point-sample enlarging a single opaque red pixel to 2x2 gives four opaque red pixels")]
def test_resize_with_filter(t : T?) {
    t |> run("resize with POINT_SAMPLE filter") <| @(tt : T?) {
        var img = Image(width = 1, height = 1, channels = 4, bpc = 1)
        img.bytes |> resize(4)
        img.bytes[0] = uint8(255)
        img.bytes[1] = uint8(0)
        img.bytes[2] = uint8(0)
        img.bytes[3] = uint8(255)
        var big <- img.resize(2, 2, stbir_filter.STBIR_FILTER_POINT_SAMPLE)
        big |> with_pixels() $(var pixels : array<uint8>#) {
            for (p in range(4)) {
                tt |> equal(int(pixels[p * 4 + 0]), 255)
                tt |> equal(int(pixels[p * 4 + 1]), 0)
                tt |> equal(int(pixels[p * 4 + 2]), 0)
                tt |> equal(int(pixels[p * 4 + 3]), 255)
            }
        }
    }
}

[test(description = "resizing the HDR image to 32x32 gives a valid 32x32 image that is still stored as 4-byte floats")]
def test_resize_hdr(t : T?) {
    t |> run("resize HDR preserves bpc=4") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        let small <- img.resize(32, 32)
        tt |> equal(small.bpc, 4)
        tt |> equal(small.width, 32)
        tt |> equal(small.height, 32)
        tt |> success(small.valid(), "resized HDR is valid")
    }
}

// ---- flip vertical ----

[test(description = "flip_vertical moves test.png's top-left pixel (0, 0, 128) to the last row and its bottom-left row's green 252 to the first row")]
def test_flip_vertical(t : T?) {
    t |> run("flip vertical reverses rows") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var flipped <- img.flip_vertical()
        // original pixel(0,0) = R=0,G=0 should now be at row 63
        flipped |> with_row(63) $(var row : array<uint8>#) {
            tt |> equal(int(row[0]), 0)
            tt |> equal(int(row[1]), 0)
            tt |> equal(int(row[2]), 128)
        }
        // original pixel(0,63) = R=0,G=252 should now be at row 0
        flipped |> with_row(0) $(var row : array<uint8>#) {
            tt |> equal(int(row[0]), 0)
            tt |> equal(int(row[1]), 252)
        }
    }
}

[test(description = "flipping an image vertically twice reproduces the first 16 bytes of the original")]
def test_flip_vertical_roundtrip(t : T?) {
    t |> run("double flip restores original") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var flipped <- img.flip_vertical()
        let restored <- flipped.flip_vertical()
        for (i in range(16)) {
            tt |> equal(int(img.bytes[i]), int(restored.bytes[i]))
        }
    }
}

[test(description = "flipping the HDR image vertically keeps its 64x64 size and 4-byte float storage")]
def test_flip_vertical_hdr(t : T?) {
    t |> run("flip vertical on HDR preserves bpc") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        let flipped <- img.flip_vertical()
        tt |> equal(flipped.bpc, 4)
        tt |> equal(flipped.width, 64)
        tt |> equal(flipped.height, 64)
    }
}

// ---- standalone functions ----

[test(description = "load_image on test.png returns a valid Image of width 64")]
def test_load_image_convenience(t : T?) {
    t |> run("load_image convenience returns valid Image") <| @(tt : T?) {
        let img <- load_image(test_image_path("test.png"))
        tt |> success(img.valid(), "loaded and valid")
        tt |> equal(img.width, 64)
    }
}

[test(description = "load_image on a missing file returns an invalid Image and fills the error string")]
def test_load_image_with_error(t : T?) {
    t |> run("load_image with error out-param") <| @(tt : T?) {
        var error : string
        let img <- load_image(test_image_path("does_not_exist.png"), error)
        tt |> success(!img.valid(), "not valid")
        tt |> success(!empty(error), "error populated")
    }
}

[test(description = "image_info on test.png reports width 64, height 64 and 4 components without decoding it")]
def test_image_info(t : T?) {
    t |> run("image_info returns dimensions") <| @(tt : T?) {
        let info = image_info(test_image_path("test.png"))
        tt |> equal(info.w, 64)
        tt |> equal(info.h, 64)
        tt |> equal(info.comp, 4)
    }
}

[test(description = "the path-based is_hdr is true for test.hdr and false for test.png")]
def test_is_hdr_standalone(t : T?) {
    t |> run("is_hdr standalone function") <| @(tt : T?) {
        tt |> success(stbimage_boost::is_hdr(test_image_path("test.hdr")), "HDR detected")
        tt |> success(!stbimage_boost::is_hdr(test_image_path("test.png")), "PNG not HDR")
    }
}

[test(description = "the path-based is_16_bit is false for the 8-bit test.png")]
def test_is_16_bit_standalone(t : T?) {
    t |> run("is_16_bit standalone function") <| @(tt : T?) {
        tt |> success(!stbimage_boost::is_16_bit(test_image_path("test.png")), "8-bit PNG not 16-bit")
    }
}

[test(description = "is_hdr_from_memory is false for the bytes of an encoded PNG")]
def test_is_hdr_from_memory_standalone(t : T?) {
    t |> run("is_hdr_from_memory on PNG bytes") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var png_bytes : array<uint8>
        img.encode("png", png_bytes)
        tt |> success(!is_hdr_from_memory(png_bytes), "PNG bytes not HDR")
    }
}

// ---- roundtrip integrity ----

[test(description = "saving test.png to a new PNG file and loading it back gives the same byte count and zero differing bytes")]
def test_save_load_pixel_roundtrip(t : T?) {
    t |> run("save PNG then load back preserves pixels") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        let out_path = path_join(test_temp_dir(), "_test_roundtrip.png")
        img.save(out_path)
        var img2 : Image
        img2.load(out_path)
        tt |> equal(length(img.bytes), length(img2.bytes))
        var mismatch = 0
        for (i in range(length(img.bytes))) {
            if (img.bytes[i] != img2.bytes[i]) {
                mismatch++
            }
        }
        tt |> equal(mismatch, 0)
        fio::remove(out_path)
    }
}

[test(description = "encoding test.png to PNG bytes and decoding them back gives the same size, channel count and zero differing bytes")]
def test_encode_decode_roundtrip(t : T?) {
    t |> run("encode to PNG memory then decode preserves pixels") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var png_bytes : array<uint8>
        img.encode("png", png_bytes)
        var img2 : Image
        img2.load_from_memory(png_bytes)
        tt |> equal(img.width, img2.width)
        tt |> equal(img.height, img2.height)
        tt |> equal(img.channels, img2.channels)
        var mismatch = 0
        for (i in range(length(img.bytes))) {
            if (img.bytes[i] != img2.bytes[i]) {
                mismatch++
            }
        }
        tt |> equal(mismatch, 0)
    }
}

// ---- make_image ----

[test(description = "make_image(32, 16, 4) returns an 8-bit image of that size whose 2048 bytes are all zero")]
def test_make_image(t : T?) {
    t |> run("make_image creates blank image") <| @(tt : T?) {
        let img <- make_image(32, 16, 4)
        tt |> equal(img.width, 32)
        tt |> equal(img.height, 16)
        tt |> equal(img.channels, 4)
        tt |> equal(img.bpc, 1)
        tt |> equal(length(img.bytes), 32 * 16 * 4)
        // all bytes should be zero
        var nonzero = 0
        for (i in range(length(img.bytes))) {
            if (img.bytes[i] != uint8(0)) {
                nonzero++
            }
        }
        tt |> equal(nonzero, 0)
    }
}

[test(description = "make_image with 4 bytes per channel returns an image that reports is_hdr and holds 8*8*3*4 bytes")]
def test_make_image_hdr(t : T?) {
    t |> run("make_image with bpc=4 creates HDR image") <| @(tt : T?) {
        let img <- make_image(8, 8, 3, 4)
        tt |> equal(img.bpc, 4)
        tt |> success(img.is_hdr(), "is HDR")
        tt |> equal(length(img.bytes), 8 * 8 * 3 * 4)
    }
}

// ---- save HDR ----

[test(description = "an HDR image saved to a .hdr path loads back with load_hdr as a 64x64 float image")]
def test_save_hdr(t : T?) {
    t |> run("save as HDR and load back") <| @(tt : T?) {
        var img : Image
        img.load_hdr(test_image_path("test.hdr"), 4)
        let out_path = path_join(test_temp_dir(), "_test_out.hdr")
        let (ok, error) = img.save(out_path)
        tt |> success(ok, "save ok: {error}")
        var img2 : Image
        let (ok2, error2) = img2.load_hdr(out_path, 4)
        tt |> success(ok2, "reload ok: {error2}")
        tt |> equal(img2.width, 64)
        tt |> equal(img2.height, 64)
        tt |> equal(img2.bpc, 4)
        fio::remove(out_path)
    }
}

[test(description = "saving the same image as JPG at quality 100 produces a larger file than at quality 10")]
def test_save_jpg_quality(t : T?) {
    t |> run("save JPG with explicit quality") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"), 3)
        let out_low = path_join(test_temp_dir(), "_test_q10.jpg")
        let out_high = path_join(test_temp_dir(), "_test_q100.jpg")
        let (ok1, err1) = img.save(out_low, 10)
        tt |> success(ok1, "save q10 ok: {err1}")
        let (ok2, err2) = img.save(out_high, 100)
        tt |> success(ok2, "save q100 ok: {err2}")
        // higher quality should produce a larger file
        var sz_low, sz_high : int
        fopen(out_low, "rb") $(f) {
            if (f != null) {
                fmap(f) $(var data : array<uint8>#) {
                    sz_low = length(data)
                }
            }
        }
        fopen(out_high, "rb") $(f) {
            if (f != null) {
                fmap(f) $(var data : array<uint8>#) {
                    sz_high = length(data)
                }
            }
        }
        tt |> success(sz_high > sz_low, "q100 ({sz_high}) > q10 ({sz_low})")
        fio::remove(out_low)
        fio::remove(out_high)
    }
}

// ---- is_16_bit_from_memory ----

[test(description = "is_16_bit_from_memory is false for the bytes of an encoded 8-bit PNG")]
def test_is_16_bit_from_memory_standalone(t : T?) {
    t |> run("is_16_bit_from_memory on PNG bytes") <| @(tt : T?) {
        var img : Image
        img.load(test_image_path("test.png"))
        var png_bytes : array<uint8>
        img.encode("png", png_bytes)
        tt |> success(!is_16_bit_from_memory(png_bytes), "8-bit PNG bytes not 16-bit")
    }
}

// ---- make_image with bpc=2 ----

[test(description = "make_image with 2 bytes per channel returns a valid 8x8 four-channel image holding 8*8*4*2 bytes")]
def test_make_image_16(t : T?) {
    t |> run("make_image with bpc=2 creates 16-bit image") <| @(tt : T?) {
        let img <- make_image(8, 8, 4, 2)
        tt |> equal(img.bpc, 2)
        tt |> equal(img.channels, 4)
        tt |> equal(length(img.bytes), 8 * 8 * 4 * 2)
        tt |> success(img.valid(), "16-bit image valid")
    }
}
