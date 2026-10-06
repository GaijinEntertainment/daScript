options gen2
options indenting = 4

module gltf_accessor shared

require math

// Little-endian typed reads from a raw byte buffer. glTF is always little-endian
// (matches x86 native), so these compose bytes directly. Pattern mirrors the
// dasLLAMA GGUF reader (modules/dasLLAMA/dasllama/dasllama_gguf.das).

def gltf_rd_u8(b : array<uint8> | #; o : int64) : uint => uint(b[o])

def gltf_rd_u16(b : array<uint8> | #; o : int64) : uint => uint(b[o]) | (uint(b[o + 1l]) << 8u)

def gltf_rd_u32(b : array<uint8> | #; o : int64) : uint =>
    uint(b[o]) | (uint(b[o + 1l]) << 8u) | (uint(b[o + 2l]) << 16u) | (uint(b[o + 3l]) << 24u)

def gltf_rd_i8(b : array<uint8> | #; o : int64) : int {
    let v = int(b[o])
    return v >= 128 ? v - 256 : v
}

def gltf_rd_i16(b : array<uint8> | #; o : int64) : int {
    let v = int(gltf_rd_u16(b, o))
    return v >= 32768 ? v - 65536 : v
}

def gltf_rd_f32(b : array<uint8> | #; o : int64) : float =>
    unsafe(reinterpret<float>(gltf_rd_u32(b, o)))

// ===== Component-type helpers =====
// glTF componentType enum: 5120 BYTE, 5121 UNSIGNED_BYTE, 5122 SHORT,
// 5123 UNSIGNED_SHORT, 5125 UNSIGNED_INT, 5126 FLOAT.

def gltf_comp_size(compType : int) : int64 {
    //! Byte size of one accessor component for the given glTF componentType.
    if (compType == 5120 || compType == 5121) return 1l
    if (compType == 5122 || compType == 5123) return 2l
    return 4l
}

def gltf_type_ncomp(t : string) : int {
    //! Number of components for a glTF accessor `type` string (SCALAR/VECn/MATn).
    if (t == "SCALAR") return 1
    if (t == "VEC2") return 2
    if (t == "VEC3") return 3
    if (t == "VEC4" || t == "MAT2") return 4
    if (t == "MAT3") return 9
    if (t == "MAT4") return 16
    return 0
}

def gltf_read_comp_float(b : array<uint8> | #; o : int64; compType : int; normalized : bool) : float {
    //! Read one component at byte offset `o` as float, applying glTF `normalized`
    //! integer→float conversion (unsigned: /max; signed: max(v/max, -1)).
    if (compType == 5126) return gltf_rd_f32(b, o)
    if (compType == 5125) return float(gltf_rd_u32(b, o))
    if (!normalized) {
        if (compType == 5121) return float(gltf_rd_u8(b, o))
        if (compType == 5123) return float(gltf_rd_u16(b, o))
        if (compType == 5120) return float(gltf_rd_i8(b, o))
        if (compType == 5122) return float(gltf_rd_i16(b, o))
    } else {
        if (compType == 5121) return float(gltf_rd_u8(b, o)) / 255.
        if (compType == 5123) return float(gltf_rd_u16(b, o)) / 65535.
        if (compType == 5120) return max(float(gltf_rd_i8(b, o)) / 127., -1.)
        if (compType == 5122) return max(float(gltf_rd_i16(b, o)) / 32767., -1.)
    }
    return 0.
}

def gltf_read_comp_uint(b : array<uint8> | #; o : int64; compType : int) : uint {
    //! Read one component at byte offset `o` as uint (index accessors: u8/u16/u32).
    if (compType == 5121) return gltf_rd_u8(b, o)
    if (compType == 5123) return gltf_rd_u16(b, o)
    return gltf_rd_u32(b, o)
}
