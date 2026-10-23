options gen2
options indenting = 4

// The CUDA side of the type rail: the spellings are `daslib/shader_emit_common`'s - the 32-bit scalars and
// their 2/3/4 vectors are CUDA's own (`float4`, `int2`, `uint3` - the built-in vector types NVRTC declares
// with no header), the 8/16-bit integer lattice its `char`/`short` families, and the 16-bit float lattice
// the prelude's `half` family (NVRTC ships no `cuda_fp16.h`). CUDA's `float3` is 12 bytes at 4-byte
// alignment, the same bytes das packs, so a 3-lane buffer element needs no packed twin.

module cuda_types shared public

require daslib/ast
require daslib/rtti
require daslib/shader_block_layout public
require daslib/shader_emit_common public

//! the 16-bit float family lowers to the prelude's own types, which a kernel text carries only when it names one
def public cuda_is_half_family(t : TypeDecl?) : bool =>
    t != null && is_float_class(t.baseType) && scalar_width(t.baseType) == 16

//! byte size of a shared-memory element: scalars and vectors at CUDA's own layout (a 3-lane vector strides three lanes)
def public cuda_elem_bytes(t : TypeDecl?) : uint64 {
    if (t == null) {
        return 0ul
    }
    if (t.baseType == Type.tBool) {
        return 1ul
    }
    if (scalar_class(t.baseType) < 0) {
        return 0ul
    }
    return uint64(scalar_width(t.baseType) / 8) * uint64(component_count(t.baseType))
}
