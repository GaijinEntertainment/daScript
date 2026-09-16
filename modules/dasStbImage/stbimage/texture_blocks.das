options gen2
module texture_blocks shared public
require stbimage
require math
require daslib/fio

let BLOCK_TEXTURE_VERSION = 1
struct BlockMip {
    //! One block-compressed mip level and its logical pixel dimensions.
    width, height : int //! Logical width and height; edge blocks are padded by the compressor.
    data : array<uint8> //! Encoded BC block bytes in row-major block order.
}
struct BlockTexture {
    //! A complete BC-compressed texture with a deterministic content key and mip chain.
    version : int //! Serialization layout version; must equal `BLOCK_TEXTURE_VERSION`.
    width, height, format : int //! Base dimensions and format code: BC1, BC3, BC4, or BC5 (0..3).
    srgb : bool //! Whether mip reduction treats RGB input as sRGB-encoded color.
    key : uint64 //! Content-derived cache key including dimensions, format, color space, and version.
    mips : array<BlockMip> //! Full mip chain from the base level through 1x1.
}
def block_texture_valid(t : BlockTexture) : bool {
    //! Return true when version, dimensions, format, mip count, and every encoded byte size
    //! satisfy the serialized block-texture contract. This does not decode or assess image quality.
    if (t.version != BLOCK_TEXTURE_VERSION || t.width < 1 || t.height < 1 || t.width > 32768 || t.height > 32768 || t.format < 0 || t.format > 3){return false}
    var w = t.width; var h = t.height; var count = 0
    while (true){count++; if (w == 1 && h == 1){break}; w = max(1, w / 2); h = max(1, h / 2)}
    if (length(t.mips) != count){return false}
    w = t.width; h = t.height
    for (m in t.mips){
        let expected = ((w + 3) / 4) * ((h + 3) / 4) * ((t.format == 0 || t.format == 2) ? 8 : 16)
        if (m.width != w || m.height != h || length(m.data) != expected){return false}
        w = max(1, w / 2); h = max(1, h / 2)
    }
    return true
}
def block_texture_key(pixels : array<uint8>; width, height, format : int; srgb : bool) : uint64 {
    //! Compute the deterministic cache key for tightly packed RGBA8 pixels and their compression
    //! settings. Callers must include exactly the same metadata later used for compression.
    return hash(pixels) ^ hash((width = width, height = height, format = format, srgb = srgb, version = BLOCK_TEXTURE_VERSION))
}
def compress_block_texture(pixels : array<uint8>; width, height, format : int; srgb : bool) : BlockTexture {
    //! Compress tightly packed RGBA8 pixels into BC1/BC3/BC4/BC5 (`format` 0..3), generating a
    //! box-filtered mip chain through 1x1. Panics when dimensions do not match the input byte count.
    if (width < 1 || height < 1 || int64(width) * int64(height) * 4l != long_length(pixels)){panic("Invalid texture pixels")}
    var result = BlockTexture(version = BLOCK_TEXTURE_VERSION, width = width, height = height, format = format, srgb = srgb, key = block_texture_key(pixels, width, height, format, srgb))
    var current <- clone(pixels); var w = width; var h = height
    while (true){
        var mip = BlockMip(width = w, height = h)
        stb_compress_blocks(current, w, h, format, true, mip.data)
        result.mips |> emplace(mip)
        if (w == 1 && h == 1){break}
        let nw = max(1, w / 2); let nh = max(1, h / 2)
        let smaller_size = int64(nw) * int64(nh) * 4l
        var smaller : array<uint8>; smaller |> reserve(smaller_size); smaller |> resize(smaller_size)
        unsafe{
            stbir_resize(addr(current[0]), w, h, 0, addr(smaller[0]), nw, nh, 0,
                stbir_pixel_layout.STBIR_RGBA, srgb ? stbir_datatype.STBIR_TYPE_UINT8_SRGB : stbir_datatype.STBIR_TYPE_UINT8,
                stbir_edge.STBIR_EDGE_CLAMP, stbir_filter.STBIR_FILTER_BOX)
        }
        delete current; current <- smaller; w = nw; h = nh
    }
    delete current; return <- result
}
def cached_block_texture(pixels : array<uint8>; width, height, format : int; srgb : bool; directory : string; var hit : bool&) : BlockTexture {
    //! Load a valid content-keyed `.das_tex` entry from `directory`, or compress and atomically
    //! populate it on a miss. Sets `hit` only for a valid matching entry; an empty directory disables I/O.
    let key = block_texture_key(pixels, width, height, format, srgb)
    let file = "{directory}/{key}.das_tex"
    var result : BlockTexture; hit = false
    if (!empty(directory)){fopen(file, "rb") $(f){if (f != null){try{hit = fload(f, result); if (hit){hit = block_texture_valid(result) && result.key == key}}recover{hit = false}}}}
    if (hit){return <- result}
    delete result; result <- compress_block_texture(pixels, width, height, format, srgb)
    if (!empty(directory)){
        mkdir_rec(directory)
        let temporary = "{file}.tmp-{ref_time_ticks()}"
        var written = false
        fopen(temporary, "wb") $(f){if (f != null){written = fsave(f, result) > 0}}
        if (written){var error : string; if (!rename(temporary, file, error)){remove(temporary)}}
    }
    return <- result
}
def private block_u16(data : array<uint8>; at : int) : uint {return uint(data[at]) | (uint(data[at + 1]) << 8)}
def private block_color(v : uint) : uint4 {
    let r = (v >> 11) & 31u; let g = (v >> 5) & 63u; let b = v & 31u
    return uint4((r << 3) | (r >> 2), (g << 2) | (g >> 4), (b << 3) | (b >> 2), 255u)
}
def private block_channel(data : array<uint8>; at, pixel : int) : uint {
    let a = uint(data[at]); let b = uint(data[at + 1]); var bits = 0ul
    for (i in range(6)){bits |= uint64(data[at + 2 + i]) << (uint64(i) * 8ul)}
    let code = uint((bits >> (uint64(pixel) * 3ul)) & 7ul)
    if (code == 0u){return a}; if (code == 1u){return b}
    if (a > b){return ((8u - code) * a + (code - 1u) * b) / 7u}
    if (code == 6u){return 0u}; if (code == 7u){return 255u}
    return ((6u - code) * a + (code - 1u) * b) / 5u
}
def decode_block_mip(texture : BlockTexture; level : int) : array<uint8> {
    //! Decode one BC mip to tightly packed RGBA8 pixels. Panics if the texture contract is invalid
    //! or `level` is outside the stored mip chain.
    if (!block_texture_valid(texture) || level < 0 || level >= length(texture.mips)){panic("Invalid compressed texture")}
    let m & = unsafe(texture.mips[level]); let result_size = int64(m.width) * int64(m.height) * 4l
    var result : array<uint8>; result |> reserve(result_size); result |> resize(result_size)
    let stride = (texture.format == 0 || texture.format == 2) ? 8 : 16
    for (y in range(m.height)){for (x in range(m.width)){
        let at = ((y / 4) * ((m.width + 3) / 4) + x / 4) * stride; let pixel = (y % 4) * 4 + x % 4
        var color = uint4(0u, 0u, 0u, 255u)
        if (texture.format < 2){
            let rgb = at + (texture.format == 1 ? 8 : 0)
            let c0 = block_u16(m.data, rgb); let c1 = block_u16(m.data, rgb + 2)
            let a = block_color(c0); let b = block_color(c1)
            let code = (uint(m.data[rgb + 4 + pixel / 4]) >> uint((pixel % 4) * 2)) & 3u
            if (code == 0u){color = a}elif (code == 1u){color = b}
            elif (c0 > c1 || texture.format == 1){color = code == 2u ? (a * 2u + b) / 3u : (a + b * 2u) / 3u}
            else{color = code == 2u ? (a + b) / 2u : uint4(0u)}
            if (texture.format == 1){color.w = block_channel(m.data, at, pixel)}
        }else{
            color.x = block_channel(m.data, at, pixel)
            if (texture.format == 3){color.y = block_channel(m.data, at + 8, pixel)}
        }
        let out = (y * m.width + x) * 4
        for (c in range(4)){result[out + c] = uint8(color[c])}
    }}
    return <- result
}
