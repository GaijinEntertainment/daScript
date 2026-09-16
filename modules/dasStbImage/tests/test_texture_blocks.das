options gen2
require dastest/testing_boost public
require stbimage/texture_blocks
require math
require daslib/fio
[test]
def test_block_mips_cache_and_decode(t : T?){
    var pixels : array<uint8>; pixels |> resize(7 * 5 * 4)
    for (i in range(35)){pixels[i * 4] = 80u8; pixels[i * 4 + 1] = 120u8; pixels[i * 4 + 2] = 180u8; pixels[i * 4 + 3] = 255u8}
    var image <- compress_block_texture(pixels, 7, 5, 0, true)
    t |> success(block_texture_valid(image), "odd dimensions have complete valid mip chains")
    t |> equal(length(image.mips), 3)
    var decoded <- decode_block_mip(image, 0)
    for (pixel, decoded_pixel in pixels, decoded){t |> success(abs(int(pixel) - int(decoded_pixel)) <= 6, "solid colour survives BC1")}
    var hit = false
    let dir_r = create_temp_directory_result("texture_blocks")
    if (!(dir_r is value)){t |> failure("create texture cache directory"); delete pixels; delete image; delete decoded; return }
    let dir = unsafe(dir_r.value)
    var first <- cached_block_texture(pixels, 7, 5, 0, true, dir, hit)
    var second <- cached_block_texture(pixels, 7, 5, 0, true, dir, hit)
    t |> success(hit, "second lookup reads cached compressed data")
    t |> equal(first.key, second.key)
    let cache_file = "{dir}/{first.key}.das_tex"
    t |> success(fwrite(cache_file, "corrupt"), "replace cache entry with malformed data")
    var rebuilt <- cached_block_texture(pixels, 7, 5, 0, true, dir, hit)
    t |> success(!hit, "corrupt cache entry is rebuilt")
    t |> success(block_texture_valid(rebuilt) && rebuilt.key == first.key, "rebuilt cache entry is valid")
    t |> success(block_texture_key(pixels, 7, 5, 0, false) != first.key, "colour-space policy affects cache identity")
    pixels[0] = 10u8
    t |> success(block_texture_key(pixels, 7, 5, 0, true) != first.key, "content affects cache identity")
    rmdir_rec_result(dir)
    delete pixels; delete image; delete decoded; delete first; delete second; delete rebuilt
}

[test]
def test_block_texture_rejects_bad_pixel_length(t : T?){
    var pixels : array<uint8>; pixels |> resize(4 * 4 * 4 - 1)
    var rejected = false
    try {
        var image <- compress_block_texture(pixels, 4, 4, 0, false)
        delete image
    } recover {
        rejected = true
    }
    t |> success(rejected, "compression rejects pixel buffers that do not match dimensions")
    delete pixels
}
[test]
def test_bc3_preserves_alpha(t : T?){
    var pixels : array<uint8>; pixels |> resize(64)
    for (i in range(16)){pixels[i * 4] = 180u8; pixels[i * 4 + 1] = 90u8; pixels[i * 4 + 2] = 30u8; pixels[i * 4 + 3] = uint8(i * 17)}
    var image <- compress_block_texture(pixels, 4, 4, 1, true)
    var decoded <- decode_block_mip(image, 0)
    for (i in range(16)){t |> success(abs(int(decoded[i * 4 + 3]) - i * 17) <= 20, "BC3 alpha gradient survives")}
    t |> equal(decoded[3], 0u8); t |> equal(decoded[63], 255u8)
    delete pixels; delete image; delete decoded
}

[test]
def test_bc4_and_bc5_channels(t : T?){
    var pixels : array<uint8>; pixels |> resize(64)
    for (i in range(16)){pixels[i * 4] = uint8(i * 17); pixels[i * 4 + 1] = uint8(255 - i * 17); pixels[i * 4 + 3] = 255u8}
    for (format in [2, 3]){
        var image <- compress_block_texture(pixels, 4, 4, format, false)
        var decoded <- decode_block_mip(image, 0)
        for (i in range(16)){
            t |> success(abs(int(decoded[i * 4]) - i * 17) <= 20, "red channel survives")
            if (format == 3){t |> success(abs(int(decoded[i * 4 + 1]) - (255 - i * 17)) <= 20, "green channel survives")}
        }
        delete image; delete decoded
    }
    delete pixels
}
