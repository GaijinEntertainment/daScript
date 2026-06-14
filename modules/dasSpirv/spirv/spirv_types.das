options gen2

// daslang TypeDecl -> SPIR-V type result-id (the *value* type; pointers are emitted by the
// visitor via type_pointer). Shader subset: scalars + 2/3/4 vectors. Arrays, structs, and
// matrices arrive in later phases. Backed by the cached/deduplicated primitives in
// spirv_builder, so repeated requests for the same type collapse to one id.

module spirv_types shared public

require spirv_builder
require daslib/ast

// daslang matrices are handled (tHandle) types named float<W>x<C> — W = column-vector width,
// C = column count (MatrixAnnotation<floatW, C>, column-major: m[i] is column i). Returns the
// dimensions so emit_type / the std140 layout / matrix arithmetic can treat them structurally.
def public matrix_info(t : TypeDecl?) : tuple<ok : bool; width : int; cols : int> {
    if (t == null || t.baseType != Type.tHandle || t.annotation == null) return (ok = false, width = 0, cols = 0)
    let nm = "{t.annotation.name}"
    if (nm == "float4x4") return (ok = true, width = 4, cols = 4)
    if (nm == "float3x3") return (ok = true, width = 3, cols = 3)
    if (nm == "float3x4") return (ok = true, width = 3, cols = 4)
    return (ok = false, width = 0, cols = 0)
}

def public emit_type(var m : SpirvModule; t : TypeDecl?) : uint {
    if (t == null) {
        panic("emit_type: null TypeDecl")
    }
    let mi = matrix_info(t)
    if (mi.ok) {
        return type_matrix(m, type_vector(m, type_float(m, 32u), mi.width), mi.cols)
    }
    let bt = t.baseType
    if (bt == Type.tVoid) return type_void(m)
    if (bt == Type.tBool) return type_bool(m)
    if (bt == Type.tInt) return type_int(m, 32u, 1u)
    if (bt == Type.tUInt) return type_int(m, 32u, 0u)
    if (bt == Type.tFloat) return type_float(m, 32u)
    if (bt == Type.tInt2) return type_vector(m, type_int(m, 32u, 1u), 2)
    if (bt == Type.tInt3) return type_vector(m, type_int(m, 32u, 1u), 3)
    if (bt == Type.tInt4) return type_vector(m, type_int(m, 32u, 1u), 4)
    if (bt == Type.tUInt2) return type_vector(m, type_int(m, 32u, 0u), 2)
    if (bt == Type.tUInt3) return type_vector(m, type_int(m, 32u, 0u), 3)
    if (bt == Type.tUInt4) return type_vector(m, type_int(m, 32u, 0u), 4)
    if (bt == Type.tFloat2) return type_vector(m, type_float(m, 32u), 2)
    if (bt == Type.tFloat3) return type_vector(m, type_float(m, 32u), 3)
    if (bt == Type.tFloat4) return type_vector(m, type_float(m, 32u), 4)
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
    if (bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4) return type_int(m, 32u, 1u)
    if (bt == Type.tUInt || bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4) return type_int(m, 32u, 0u)
    if (bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4) return type_float(m, 32u)
    if (bt == Type.tBool) return type_bool(m)
    panic("emit_component_type: unsupported baseType {bt}")
    return 0u
}
