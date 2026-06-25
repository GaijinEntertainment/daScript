options gen2

// std140 interface-block layout — the single source of truth shared by every shader backend's
// uniform binder. compute_block_layout walks a daslang struct and yields per-field std140 byte
// offsets / sizes / matrix + composite flags; the SPIR-V emitter (dasSpirv) lays out its interface
// block from it, the OpenGL host binder (dasOpenGL) packs its UBO bytes from it, and dasVulkan's
// host binder writes mapped memory from it — so all rails agree on the byte layout. Pure compile-time
// AST logic (no backend types), which is why it lives in daslib and not in either emitter module.

module shader_block_layout shared public

require math
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

// Type -> scalar class: 0 signed int, 1 unsigned int, 2 float (scalars and their 2/3/4 vectors);
// -1 for anything else. The supported interface-block leaf set is exactly these 32-bit numerics.
def public scalar_class(bt : Type) : int {
    if (bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4) return 0
    if (bt == Type.tUInt || bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4) return 1
    if (bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4) return 2
    return -1
}

// Lane count of a scalar/vector Type: 1/2/3/4; 0 for non-scalar/vector types.
def public component_count(bt : Type) : int {
    if (bt == Type.tInt || bt == Type.tUInt || bt == Type.tFloat || bt == Type.tBool) return 1
    if (bt == Type.tInt2 || bt == Type.tUInt2 || bt == Type.tFloat2) return 2
    if (bt == Type.tInt3 || bt == Type.tUInt3 || bt == Type.tFloat3) return 3
    if (bt == Type.tInt4 || bt == Type.tUInt4 || bt == Type.tFloat4) return 4
    return 0
}

// ===== std140 layout for a UBO interface block =====
// std140 base alignment: scalar -> 4, vec2 -> 8, vec3/vec4 -> 16. The base *size* of a vec3 is 12
// (only its alignment is 16), so a scalar can pack into the trailing 4 bytes after a vec3.
def public std140_align(bt : Type) : int {
    let cc = component_count(bt)
    if (cc == 1) return 4
    if (cc == 2) return 8
    return 16
}

def public std140_size(bt : Type) : int {
    return component_count(bt) * 4
}

def public round_up(off, a : int) : int {
    return ((off + a - 1) / a) * a
}

//! One std140-laid-out field of a @uniform / @push_constant interface block. Returned by
//! compute_block_layout and consumed by host-side bind macros (e.g. dasVulkan's SpirvVulkanShader
//! and dasOpenGL's GlslOpenGLShader) to emit one write per field at the right byte offset.
//! `is_matrix` just means "this field IS a matrix" (and gets MatrixStride 16 in SPIR-V) — it does
//! NOT by itself imply the daslang storage and std140 layout differ. For float4x4 the columns are
//! already vec4-wide so a single memcpy works; for float3x3 / float3x4 the daslang storage is packed
//! (3-wide columns) while std140 pads each column to vec4, so bind code must repack per-column.
//! **Use `matrix_width` to choose**: `matrix_width == 4` → safe to memcpy the field; `matrix_width <
//! 4` → repack per column.
//!
//! **Composite fields** (`is_array` / `is_struct`) carry the std140 layout of nested data but
//! cannot be written by a single host-side `write_field(ptr, offset, value)` call. Host bind
//! macros should detect `is_composite` and either recurse member-by-member using the published
//! `offset` + `array_stride` / nested layout, or skip with a clear "manual bind required" message.
struct public BlockField {
    name              : string //!< field name as declared in the daslang struct
    offset            : int    //!< std140 byte offset from the block base
    size              : int    //!< std140 base size of the field (matrices: 16 * cols; composites: total laid-out bytes)
    is_matrix         : bool   //!< true if this field is a matrix (MatrixStride 16 in SPIR-V); see struct docstring re: repack
    matrix_width      : int    //!< matrix column-vector width — 4 for float4x4 (memcpy-safe); 3 for float3x3/3x4 (per-column repack)
    matrix_cols       : int    //!< matrix column count (only valid when is_matrix); 0 otherwise
    is_array          : bool   //!< true if this field is a fixed-array (scalar / vector / nested-struct elements)
    array_count       : int    //!< number of array elements (only valid when is_array)
    array_stride      : int    //!< per-element byte stride (std140 rounds each element up to 16); 0 otherwise
    is_struct         : bool   //!< the struct-element marker. True when the field is a nested struct (`is_array=false`) OR a fixed-array whose elements are nested structs (`is_array=true` — disambiguate via is_array). Host bind macros that handle composites recurse into the inner Structure via compute_block_layout in either case.
    is_composite      : bool   //!< convenience flag — true when is_array OR is_struct (host bind macros can't write this with a flat overload)
}

// std140 element stride for a fixed-array element type. Scalars/vectors round up to vec4 (16 bytes);
// nested-struct elements take their std140 struct-size rounded up to the struct alignment (which is
// already a multiple of 16 by std140's struct rule). Matrix elements and nested-array elements are
// out of PR-G scope and rejected by the caller before reaching here.
def private std140_array_element_stride(bt : Type) : int {
    return round_up(std140_size(bt), 16)
}

//! Compute the std140 layout of one interface block. Pure: takes a Structure?, fills `fields` with
//! per-field offsets / sizes / matrix and composite flags, returns the block total size and max
//! member alignment. Used by build_block_struct (SPIR-V type emit), the OpenGL UBO pack binder, and
//! dasVulkan's mapped-memory binder. Rejects bool and non-numeric fields fail-closed (no physical
//! std140 layout). `sname` is only used in error messages. `fields` is cleared at entry so reuse
//! across calls and partial-failure returns can't bleed. Supported field shapes (PR-G complete):
//! 32-bit scalar / vector / matrix; nested struct (recursive); fixed-array of scalar / vector
//! (std140 ArrayStride = element-size rounded to 16); fixed-array of nested struct (ArrayStride =
//! laid-out struct size rounded to struct alignment). Out of scope (clean rejection): matrix-of-array,
//! array-of-array, runtime arrays (use `@ssbo` for those).
//! [macro_function]: callable from a backend's bind-gen macro at compile time, mirrors generate_spirv.
[macro_function]
def public compute_block_layout(sname : string; st : Structure?; var fields : array<BlockField>; var errors : array<string>) : tuple<ok : bool; size : int; align : int> {
    fields |> clear
    if (st == null || empty(st.fields)) {
        errors |> push("interface block '{sname}' must be a struct with at least one field")
        return (ok = false, size = 0, align = 0)
    }
    var off = 0
    var max_align = 0
    for (fld in st.fields) {
        // matrix field (terminal)
        let mi = matrix_info(fld._type)
        if (mi.ok) {
            off = round_up(off, 16)
            fields |> emplace(BlockField(name := string(fld.name), offset = off, size = 16 * mi.cols,
                is_matrix = true, matrix_width = mi.width, matrix_cols = mi.cols))
            off += 16 * mi.cols
            max_align = max(max_align, 16)
            continue
        }
        // nested struct field (PR-G): recurse. Base alignment = round_up(inner_align, 16);
        // size = round_up(inner_size, base_align). The inner field list isn't propagated up
        // (host bind macros that recurse must re-invoke compute_block_layout on the nested
        // Structure?). is_composite=true signals "no flat write_field overload exists".
        if (fld._type != null && fld._type.baseType == Type.tStructure && fld._type.structType != null) {
            var inner : array<BlockField>
            let inner_layout = compute_block_layout("{sname}.{fld.name}", fld._type.structType, inner, errors)
            delete inner
            if (!inner_layout.ok) {
                fields |> clear
                return (ok = false, size = 0, align = 0)
            }
            let struct_align = round_up(inner_layout.align, 16)
            off = round_up(off, struct_align)
            let struct_size = round_up(inner_layout.size, struct_align)
            fields |> emplace(BlockField(name := string(fld.name), offset = off, size = struct_size,
                is_matrix = false, matrix_width = 0, matrix_cols = 0,
                is_array = false, array_count = 0, array_stride = 0,
                is_struct = true, is_composite = true))
            off += struct_size
            max_align = max(max_align, struct_align)
            continue
        }
        // fixed-array field (PR-G): element kind decides stride. Scalar/vector elements stride by
        // their size rounded up to 16 (the std140 array padding rule — every array slot is at
        // least vec4-wide). Nested-struct elements stride by their laid-out struct size. Matrix
        // elements and nested-array elements (array of array) are rejected; wrap in a struct.
        if (fld._type != null && fld._type.baseType == Type.tFixedArray && fld._type.firstType != null) {
            let et = fld._type.firstType
            let nelem = fld._type.fixedDim
            if (nelem <= 0) {
                errors |> push("interface block '{sname}' field '{fld.name}': array must have positive count (got {nelem})")
                fields |> clear
                return (ok = false, size = 0, align = 0)
            }
            // scalar / vector element
            if (et.baseType != Type.tStructure && et.baseType != Type.tFixedArray && !matrix_info(et).ok) {
                let ebt = et.baseType
                if (scalar_class(ebt) < 0) {
                    errors |> push("interface block '{sname}' field '{fld.name}': array element type {ebt} is not supported (only 32-bit int/uint/float scalars, their vectors, or nested structs of those)")
                    fields |> clear
                    return (ok = false, size = 0, align = 0)
                }
                let stride = std140_array_element_stride(ebt)
                off = round_up(off, 16)        // std140 array base alignment is 16
                let total = stride * nelem
                fields |> emplace(BlockField(name := string(fld.name), offset = off, size = total,
                    is_matrix = false, matrix_width = 0, matrix_cols = 0,
                    is_array = true, array_count = nelem, array_stride = stride,
                    is_struct = false, is_composite = true))
                off += total
                max_align = max(max_align, 16)
                continue
            }
            // array-of-struct element
            if (et.baseType == Type.tStructure && et.structType != null) {
                var inner : array<BlockField>
                let inner_layout = compute_block_layout("{sname}.{fld.name}[]", et.structType, inner, errors)
                delete inner
                if (!inner_layout.ok) {
                    fields |> clear
                    return (ok = false, size = 0, align = 0)
                }
                let struct_align = round_up(inner_layout.align, 16)
                let stride = round_up(inner_layout.size, struct_align)
                off = round_up(off, struct_align)
                let total = stride * nelem
                fields |> emplace(BlockField(name := string(fld.name), offset = off, size = total,
                    is_matrix = false, matrix_width = 0, matrix_cols = 0,
                    is_array = true, array_count = nelem, array_stride = stride,
                    is_struct = true, is_composite = true))
                off += total
                max_align = max(max_align, struct_align)
                continue
            }
            errors |> push("interface block '{sname}' field '{fld.name}': array element type {et.baseType} is not supported (only scalar/vector or nested-struct elements; matrix-of-array and array-of-array are out of scope)")
            fields |> clear
            return (ok = false, size = 0, align = 0)
        }
        // terminal scalar / vector field
        let fbt = fld._type.baseType
        if (scalar_class(fbt) < 0) {
            errors |> push("interface block '{sname}' field '{fld.name}': type {fbt} is not supported (only 32-bit int/uint/float scalars, their vectors, matrices, fixed-arrays of those, and nested structs of those; bool has no physical interface-block layout in Vulkan)")
            fields |> clear
            return (ok = false, size = 0, align = 0)
        }
        let falign = std140_align(fbt)
        off = round_up(off, falign)
        fields |> emplace(BlockField(name := string(fld.name), offset = off, size = std140_size(fbt),
            is_matrix = false, matrix_width = 0, matrix_cols = 0))
        off += std140_size(fbt)
        max_align = max(max_align, falign)
    }
    return (ok = true, size = off, align = max_align)
}

//! True iff every field of the block packs into std140 with a single memcpy at its offset: a 32-bit
//! scalar / vector / float4x4. False for float3x3 / float3x4 (std140 pads each column to vec4 ->
//! per-column repack), fixed-arrays and nested structs (composite -> stride / recursion), and any block
//! compute_block_layout rejects. A backend uses this to choose the std140 UBO pack path vs a loose
//! by-name fallback for a given @uniform block, and the emitter + host binder MUST agree by both
//! consulting it. (The simple-field restriction is the current GL UBO binder's scope, not a std140 law.)
[macro_function]
def public std140_block_is_simple(st : Structure?) : bool {
    var fields : array<BlockField>
    var errors : array<string>
    let layout = compute_block_layout("", st, fields, errors)
    var simple = layout.ok
    if (simple) {
        for (f in fields) {
            if (f.is_composite || (f.is_matrix && f.matrix_width != 4)) {
                simple = false
                break
            }
        }
    }
    delete fields
    delete errors
    return simple
}
