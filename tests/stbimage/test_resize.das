options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

require stbimage
require daslib/safe_addr
require dastest/testing_boost public

// ---- enum values ----

[test(description = "the stbir_pixel_layout members 1CHANNEL, RGB, RGBA, 4CHANNEL, BGRA and RGBA_PM carry the stb values 1, 3, 4, 5, 6 and 11")]
def test_pixel_layout_enum(t : T?) {
    t |> run("stbir_pixel_layout enum values are correct") <| @(tt : T?) {
        tt |> equal(int(stbir_pixel_layout.STBIR_1CHANNEL), 1)
        tt |> equal(int(stbir_pixel_layout.STBIR_RGB), 3)
        tt |> equal(int(stbir_pixel_layout.STBIR_RGBA), 4)
        tt |> equal(int(stbir_pixel_layout.STBIR_4CHANNEL), 5)
        tt |> equal(int(stbir_pixel_layout.STBIR_BGRA), 6)
        tt |> equal(int(stbir_pixel_layout.STBIR_RGBA_PM), 11)
    }
}

[test(description = "the stbir_edge members CLAMP, REFLECT, WRAP and ZERO carry the stb values 0, 1, 2 and 3")]
def test_edge_enum(t : T?) {
    t |> run("stbir_edge enum values are correct") <| @(tt : T?) {
        tt |> equal(int(stbir_edge.STBIR_EDGE_CLAMP), 0)
        tt |> equal(int(stbir_edge.STBIR_EDGE_REFLECT), 1)
        tt |> equal(int(stbir_edge.STBIR_EDGE_WRAP), 2)
        tt |> equal(int(stbir_edge.STBIR_EDGE_ZERO), 3)
    }
}

[test(description = "the stbir_filter members DEFAULT, BOX, TRIANGLE, MITCHELL and POINT_SAMPLE carry the stb values 0, 1, 2, 5 and 6")]
def test_filter_enum(t : T?) {
    t |> run("stbir_filter enum values are correct") <| @(tt : T?) {
        tt |> equal(int(stbir_filter.STBIR_FILTER_DEFAULT), 0)
        tt |> equal(int(stbir_filter.STBIR_FILTER_BOX), 1)
        tt |> equal(int(stbir_filter.STBIR_FILTER_TRIANGLE), 2)
        tt |> equal(int(stbir_filter.STBIR_FILTER_MITCHELL), 5)
        tt |> equal(int(stbir_filter.STBIR_FILTER_POINT_SAMPLE), 6)
    }
}

[test(description = "the stbir_datatype members UINT8, UINT8_SRGB, UINT16, FLOAT and HALF_FLOAT carry the stb values 0, 1, 3, 4 and 5")]
def test_datatype_enum(t : T?) {
    t |> run("stbir_datatype enum values are correct") <| @(tt : T?) {
        tt |> equal(int(stbir_datatype.STBIR_TYPE_UINT8), 0)
        tt |> equal(int(stbir_datatype.STBIR_TYPE_UINT8_SRGB), 1)
        tt |> equal(int(stbir_datatype.STBIR_TYPE_UINT16), 3)
        tt |> equal(int(stbir_datatype.STBIR_TYPE_FLOAT), 4)
        tt |> equal(int(stbir_datatype.STBIR_TYPE_HALF_FLOAT), 5)
    }
}

// ---- resize uint8 srgb ----

[test(description = "enlarging a uniform mid-grey 2x2 RGBA image to 4x4 with the sRGB resizer succeeds and keeps every output byte between 100 and 156")]
def test_resize_uint8_srgb_upscale(t : T?) {
    t |> run("resize 2x2 to 4x4 srgb") <| @(tt : T?) {
        // 2x2 RGBA image
        var input : uint8[16]
        for (i in range(16)) {
            input[i] = uint8(128)
        }
        var output : uint8[64]  // 4x4 RGBA
        var result = stbir_resize_uint8_srgb(
            safe_addr(input[0]), 2, 2, 0,
            safe_addr(output[0]), 4, 4, 0,
            stbir_pixel_layout.STBIR_RGBA
        )
        tt |> success(result != null, "resize succeeded")
        // all pixels should be close to 128 (uniform input)
        for (i in range(64)) {
            tt |> success(int(output[i]) >= 100 && int(output[i]) <= 156, "pixel in range")
        }
    }
}

[test(description = "shrinking an all-white 4x4 RGBA image to 2x2 with the sRGB resizer succeeds and leaves every output byte at exactly 255")]
def test_resize_uint8_srgb_downscale(t : T?) {
    t |> run("resize 4x4 to 2x2 srgb") <| @(tt : T?) {
        var input : uint8[64]  // 4x4 RGBA, all white
        for (i in range(64)) {
            input[i] = uint8(255)
        }
        var output : uint8[16]  // 2x2 RGBA
        var result = stbir_resize_uint8_srgb(
            safe_addr(input[0]), 4, 4, 0,
            safe_addr(output[0]), 2, 2, 0,
            stbir_pixel_layout.STBIR_RGBA
        )
        tt |> success(result != null, "resize succeeded")
        // output should be all 255 (white averaged = white)
        for (i in range(16)) {
            tt |> equal(int(output[i]), 255)
        }
    }
}

// ---- resize uint8 linear ----

[test(description = "enlarging a 2x2 RGB 8-bit image to 4x4 with the linear resizer succeeds")]
def test_resize_uint8_linear(t : T?) {
    t |> run("resize linear mode works") <| @(tt : T?) {
        var input : uint8[12]  // 2x2 RGB
        for (i in range(12)) {
            input[i] = uint8(200)
        }
        var output : uint8[48]  // 4x4 RGB
        var result = stbir_resize_uint8_linear(
            safe_addr(input[0]), 2, 2, 0,
            safe_addr(output[0]), 4, 4, 0,
            stbir_pixel_layout.STBIR_RGB
        )
        tt |> success(result != null, "resize succeeded")
    }
}

// ---- resize float linear ----

[test(description = "enlarging a uniform 0.5 float 2x2 RGBA image to 4x4 with the float resizer succeeds and keeps every output value between 0.4 and 0.6")]
def test_resize_float_linear(t : T?) {
    t |> run("resize float data works") <| @(tt : T?) {
        var input : float[16]  // 2x2 RGBA float
        for (i in range(16)) {
            input[i] = 0.5f
        }
        var output : float[64]  // 4x4 RGBA float
        var result = stbir_resize_float_linear(
            safe_addr(input[0]), 2, 2, 0,
            safe_addr(output[0]), 4, 4, 0,
            stbir_pixel_layout.STBIR_RGBA
        )
        tt |> success(result != null, "resize succeeded")
        // output should be ~0.5
        for (i in range(64)) {
            tt |> success(output[i] > 0.4f && output[i] < 0.6f, "float pixel in range")
        }
    }
}

// ---- resize medium API ----

[test(description = "stbir_resize with an explicit Mitchell filter and clamp edge mode enlarges a 2x2 RGBA 8-bit image to 3x3 successfully")]
def test_resize_medium_api(t : T?) {
    t |> run("medium API with explicit filter and edge mode") <| @(tt : T?) {
        var input : uint8[16]  // 2x2 RGBA
        for (i in range(16)) {
            input[i] = uint8(100)
        }
        var output : uint8[36]  // 3x3 RGBA
        var result = stbir_resize(
            safe_addr(input[0]), 2, 2, 0,
            safe_addr(output[0]), 3, 3, 0,
            stbir_pixel_layout.STBIR_RGBA,
            stbir_datatype.STBIR_TYPE_UINT8,
            stbir_edge.STBIR_EDGE_CLAMP,
            stbir_filter.STBIR_FILTER_MITCHELL
        )
        tt |> success(result != null, "medium API resize succeeded")
    }
}

[test(description = "point-sampling a single opaque red RGBA pixel up to 2x2 gives four pixels that are exactly (255, 0, 0, 255)")]
def test_resize_medium_api_point_sample(t : T?) {
    t |> run("point sample filter produces exact copies") <| @(tt : T?) {
        // 1x1 red pixel
        var input : uint8[4]
        input[0] = uint8(255)
        input[1] = uint8(0)
        input[2] = uint8(0)
        input[3] = uint8(255)
        var output : uint8[16]  // 2x2 RGBA
        var result = stbir_resize(
            safe_addr(input[0]), 1, 1, 0,
            safe_addr(output[0]), 2, 2, 0,
            stbir_pixel_layout.STBIR_RGBA,
            stbir_datatype.STBIR_TYPE_UINT8,
            stbir_edge.STBIR_EDGE_CLAMP,
            stbir_filter.STBIR_FILTER_POINT_SAMPLE
        )
        tt |> success(result != null, "point sample resize succeeded")
        // all 4 pixels should be red
        for (p in range(4)) {
            tt |> equal(int(output[p * 4 + 0]), 255)
            tt |> equal(int(output[p * 4 + 1]), 0)
            tt |> equal(int(output[p * 4 + 2]), 0)
            tt |> equal(int(output[p * 4 + 3]), 255)
        }
    }
}

// ---- resize with null output (auto-allocate) ----

[test(description = "passing a null output buffer to the sRGB resizer makes it allocate and return the 4x4 result itself")]
def test_resize_auto_allocate(t : T?) {
    t |> run("resize with null output allocates result") <| @(tt : T?) {
        var input : uint8[16]
        for (i in range(16)) {
            input[i] = uint8(128)
        }
        var result = stbir_resize_uint8_srgb(
            safe_addr(input[0]), 2, 2, 0,
            null, 4, 4, 0,
            stbir_pixel_layout.STBIR_RGBA
        )
        tt |> success(result != null, "auto-allocated resize succeeded")
        stbi_image_free(result)
    }
}
