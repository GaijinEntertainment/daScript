options gen2
options indenting = 4

module _q4k_disk shared public

require daslib/f16_cvt

def private put_f16(var b : array<uint8>; off : int64; v : float) {
    let bits = f32_to_f16(v)
    b[off] = uint8(bits & 0xFFu)
    b[off + 1l] = uint8(bits >> 8u)
}

//! one Q4_K superblock in the GGUF's disk layout: two f16 scales, 12 bytes of 6-bit sub-scales, 128 bytes of nibbles
def q4k_disk_superblock() : array<uint8> {
    var b : array<uint8>
    b |> resize(144l)
    put_f16(b, 0l, 0.375)
    put_f16(b, 2l, 0.125)
    for (i in range64(12l)) {
        b[4l + i] = uint8((i * 37l + 13l) % 256l)
    }
    for (i in range64(128l)) {
        b[16l + i] = uint8((i * 29l + 7l) % 256l)
    }
    return <- b
}
