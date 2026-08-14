options gen2

// daslang TypeDecl -> SPIR-V type result-id (the *value* type; pointers are emitted by the
// visitor via type_pointer). Shader subset: scalars + 2/3/4 vectors. Arrays, structs, and
// matrices arrive in later phases. Backed by the cached/deduplicated primitives in
// spirv_builder, so repeated requests for the same type collapse to one id.

module spirv_types shared public

require spirv_builder
require daslib/ast
require daslib/shader_block_layout

// Scalar type-id for a numeric_info class + width: OpTypeFloat for the float class, OpTypeInt with
// the class's signedness otherwise. type_int declares the Int16 / Int64 capability for the narrow
// and wide widths on the way through.
def public numeric_scalar_type(var m : SpirvModule; cls, width : int) : uint {
    if (cls == 2) return type_float(m, uint(width))
    return type_int(m, uint(width), cls == 0 ? 1u : 0u)
}

def public emit_type(var m : SpirvModule; t : TypeDecl?) : uint {
    if (t == null) {
        panic("emit_type: null TypeDecl")
    }
    let mi = matrix_info(t)
    if (mi.ok) {
        return type_matrix(m, type_vector(m, type_float(m, 32u), mi.width), mi.cols)
    }
    // an enum lowers as its own underlying integer, so it lands on that integer's branch below
    let bt = value_base_type(t)
    if (bt == Type.tVoid) return type_void(m)
    if (bt == Type.tBool) return type_bool(m)
    // Every scalar / shader-legal vector, 32-bit and 64-bit alike plus the 16/8-bit lattice: the
    // component type comes from (class, width) and the vector wrapper from the lane count, so the
    // narrow type factories pull the Float16/Int16/Int8 capabilities on the way through. The
    // 8/16-lane lattice forms classify as -1, so they fall through to the panic (fail-closed).
    let ni = numeric_info(bt)
    if (ni.ok) {
        let scalar = numeric_scalar_type(m, ni.cls, ni.width)
        return ni.lanes == 1 ? scalar : type_vector(m, scalar, ni.lanes)
    }
    // fixed array: float2[3] -> OpTypeArray(element, length). Outermost dim is fixedDim; the
    // element (firstType) may itself be a fixed array for multi-dim, so recurse.
    if (bt == Type.tFixedArray && t.firstType != null) {
        return type_array(m, emit_type(m, t.firstType), t.fixedDim)
    }
    panic("emit_type: unsupported baseType {bt}")
    return 0u
}

// Scalar component type-id for a vector/scalar TypeDecl — used when emitting a per-lane
// constant or an OpAccessChain into a vector.
def public emit_component_type(var m : SpirvModule; t : TypeDecl?) : uint {
    if (t == null) {
        panic("emit_component_type: null TypeDecl")
    }
    let bt = t.baseType
    let ni = numeric_info(bt)
    if (ni.ok) return numeric_scalar_type(m, ni.cls, ni.width)
    if (bt == Type.tBool) return type_bool(m)
    if (bt == Type.tFloat16 || bt == Type.tHalf2 || bt == Type.tHalf3 || bt == Type.tHalf4) return type_float(m, 16u)
    if (bt == Type.tInt16 || bt == Type.tShort2 || bt == Type.tShort3 || bt == Type.tShort4) return type_int(m, 16u, 1u)
    if (bt == Type.tUInt16 || bt == Type.tUShort2 || bt == Type.tUShort3 || bt == Type.tUShort4) return type_int(m, 16u, 0u)
    if (bt == Type.tInt8 || bt == Type.tByte2 || bt == Type.tByte3 || bt == Type.tByte4) return type_int(m, 8u, 1u)
    if (bt == Type.tUInt8 || bt == Type.tUByte2 || bt == Type.tUByte3 || bt == Type.tUByte4) return type_int(m, 8u, 0u)
    panic("emit_component_type: unsupported baseType {bt}")
    return 0u
}
