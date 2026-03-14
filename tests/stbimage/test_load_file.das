options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

require stbimage
require daslib/safe_addr
require dastest/testing_boost public

def test_image_path(name : string) : string {
    return "{get_das_root()}/tests/stbimage/{name}"
}

// ---- stbi_load from file ----

[test]
def test_load_png(t : T?) {
    t |> run("load PNG from file") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_load(test_image_path("test.png"), safe_addr(x), safe_addr(y), safe_addr(comp), 4)
        tt |> success(pixels != null, "pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        tt |> equal(comp, 4)
        // first pixel: R=0, G=0, B=128, A=255
        tt |> equal(int(unsafe(pixels[0])), 0)
        tt |> equal(int(unsafe(pixels[2])), 128)
        tt |> equal(int(unsafe(pixels[3])), 255)
        stbi_image_free(pixels)
    }
}

[test]
def test_load_bmp(t : T?) {
    t |> run("load BMP from file") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_load(test_image_path("test.bmp"), safe_addr(x), safe_addr(y), safe_addr(comp), 4)
        tt |> success(pixels != null, "pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        stbi_image_free(pixels)
    }
}

[test]
def test_load_tga(t : T?) {
    t |> run("load TGA from file") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_load(test_image_path("test.tga"), safe_addr(x), safe_addr(y), safe_addr(comp), 4)
        tt |> success(pixels != null, "pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        stbi_image_free(pixels)
    }
}

[test]
def test_load_jpg(t : T?) {
    t |> run("load JPG from file") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_load(test_image_path("test.jpg"), safe_addr(x), safe_addr(y), safe_addr(comp), 3)
        tt |> success(pixels != null, "pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        tt |> equal(comp, 3)
        stbi_image_free(pixels)
    }
}

// ---- stbi_info from file ----

[test]
def test_info_png(t : T?) {
    t |> run("info PNG from file") <| @(tt : T?) {
        var x, y, comp : int
        var ok = stbi_info(test_image_path("test.png"), safe_addr(x), safe_addr(y), safe_addr(comp))
        tt |> success(ok != 0, "info ok")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        tt |> equal(comp, 4)
    }
}

[test]
def test_info_bmp(t : T?) {
    t |> run("info BMP from file") <| @(tt : T?) {
        var x, y, comp : int
        var ok = stbi_info(test_image_path("test.bmp"), safe_addr(x), safe_addr(y), safe_addr(comp))
        tt |> success(ok != 0, "info ok")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
    }
}

[test]
def test_info_jpg(t : T?) {
    t |> run("info JPG from file") <| @(tt : T?) {
        var x, y, comp : int
        var ok = stbi_info(test_image_path("test.jpg"), safe_addr(x), safe_addr(y), safe_addr(comp))
        tt |> success(ok != 0, "info ok")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        tt |> equal(comp, 3)
    }
}

// ---- stbi_load nonexistent file ----

[test]
def test_load_nonexistent(t : T?) {
    t |> run("load nonexistent file returns null") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_load(test_image_path("does_not_exist.png"), safe_addr(x), safe_addr(y), safe_addr(comp), 0)
        tt |> success(pixels == null, "null for missing file")
    }
}

// ---- HDR file ----

[test]
def test_is_hdr_file(t : T?) {
    t |> run("HDR file detected as HDR") <| @(tt : T?) {
        var ok = stbi_is_hdr(test_image_path("test.hdr"))
        tt |> success(ok != 0, "is HDR")
    }
}

[test]
def test_is_not_hdr_png(t : T?) {
    t |> run("PNG file not detected as HDR") <| @(tt : T?) {
        var ok = stbi_is_hdr(test_image_path("test.png"))
        tt |> equal(ok, 0)
    }
}

[test]
def test_loadf_hdr(t : T?) {
    t |> run("loadf HDR from file") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_loadf(test_image_path("test.hdr"), safe_addr(x), safe_addr(y), safe_addr(comp), 4)
        tt |> success(pixels != null, "float pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        // first pixel: R~0, G~0, B~0.5
        tt |> success(unsafe(pixels[0]) < 0.05f, "R near 0")
        tt |> success(unsafe(pixels[1]) < 0.05f, "G near 0")
        tt |> success(unsafe(pixels[2]) > 0.3f && unsafe(pixels[2]) < 0.7f, "B near 0.5")
        stbi_image_free(pixels)
    }
}

// ---- stbi_load_16 from file ----

[test]
def test_load_16_png(t : T?) {
    t |> run("load_16 PNG from file") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_load_16(test_image_path("test.png"), safe_addr(x), safe_addr(y), safe_addr(comp), 4)
        tt |> success(pixels != null, "16-bit pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        stbi_image_free(pixels)
    }
}

// ---- stbi_is_16_bit from file ----

[test]
def test_is_16_bit_file(t : T?) {
    t |> run("8-bit PNG not detected as 16-bit") <| @(tt : T?) {
        var ok = stbi_is_16_bit(test_image_path("test.png"))
        tt |> equal(ok, 0)
    }
}

// ---- loadf from non-HDR file ----

[test]
def test_loadf_png(t : T?) {
    t |> run("loadf PNG converts to float") <| @(tt : T?) {
        var x, y, comp : int
        var pixels = stbi_loadf(test_image_path("test.png"), safe_addr(x), safe_addr(y), safe_addr(comp), 4)
        tt |> success(pixels != null, "float pixels loaded")
        tt |> equal(x, 64)
        tt |> equal(y, 64)
        stbi_image_free(pixels)
    }
}
