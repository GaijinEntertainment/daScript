options gen2
options indenting = 4

// The MSL side of the type rail: the spellings are
// `daslib/shader_emit_common`'s (the 32-bit scalars, their 2/3/4
// vectors and the 16/8-bit lattice are MSL's own
// half/short/ushort/char/uchar); here the two layout units MSL adds -
// threadgroup elements at the plain stride and buffer elements at the
// packed one. 8/16-lane lattice forms are CPU-only by design (shader
// vectors cap at 4 lanes) — callers key their dedicated
// fail-closed diagnostic on cpu_only_lattice_width.

module msl_types shared public

require daslib/ast
require daslib/rtti
require daslib/shader_block_layout public
require daslib/shader_emit_common public

//! MSL byte size of a THREADGROUP-resident element — the dynamic-shmem layout unit. Unlike the
//! packed buffer path, threadgroup memory uses MSL's plain (non-packed) spellings, so 3-lane
//! vectors stride four lanes.
def public msl_tg_elem_bytes(t : TypeDecl?) : uint64 {
    if (t == null) {
        return 0ul
    }
    if (t.baseType == Type.tBool) {
        return 1ul
    }
    if (scalar_class(t.baseType) < 0) {
        return 0ul
    }
    let cc = component_count(t.baseType)
    return uint64(scalar_width(t.baseType) / 8) * uint64(cc == 3 ? 4 : cc)
}

//! MSL spelling for a buffer/uniform ELEMENT — the layout-bearing position. das packs vectors
//! tightly (float3 = 12B, half3 = 6B, byte3 = 3B) while non-packed MSL 3-vectors stride padded
//! (float3 = 16B, half3 = 8B); with unified memory the das array IS the buffer, so 3-lane
//! elements must take MSL's packed_T3 spelling (identical bytes to das). 1/2/4-lane sizes match
//! the plain spelling already.
def public msl_buffer_elem_name(t : TypeDecl?) : string {
    let plain = shader_type_name(t)
    if (empty(plain)) {
        return ""
    }
    return component_count(t.baseType) == 3 ? "packed_{plain}" : plain
}
