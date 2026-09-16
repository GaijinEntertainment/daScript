options gen2
options max_unreserved_size = 65536
require dastest/testing_boost public
require stbimage/texture_blocks

[test]
def test_block_textures_reserve_large_outputs(t : T?){
    let width = 256; let height = 512
    var pixels : array<uint8>; pixels |> reserve(width * height * 4); pixels |> resize(width * height * 4)
    for (i in range(width * height)){
        pixels[i * 4] = uint8(i); pixels[i * 4 + 1] = uint8(i >> 3)
        pixels[i * 4 + 2] = uint8(i >> 7); pixels[i * 4 + 3] = uint8(i)
    }
    for (format in [0, 1]){
        var texture <- compress_block_texture(pixels, width, height, format, false)
        t |> success(block_texture_valid(texture), "large BC texture and mip chain")
        var decoded <- decode_block_mip(texture, 0)
        t |> equal(length(decoded), width * height * 4, "large decoded RGBA output")
        delete texture; delete decoded
    }
    delete pixels
}
