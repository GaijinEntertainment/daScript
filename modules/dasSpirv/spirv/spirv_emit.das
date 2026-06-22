options gen2

// daslang AST -> SPIR-V codegen via the SpirvEmit AstVisitor (modeled on llvm_jit's SSA backend).
// SPIR-V's value/pointer model (SSA + OpAccessChain) is threaded through per-node side-maps: e2id for
// rvalue ids, e2ptr/e2pty for lvalue pointers; ExprRef2Value (still present at the annotation's patch()
// pre-fold stage) is the explicit load marker. generate_spirv is the [macro_function] entry, called by
// both the shader annotation's patch and the unit tests, so opcode assertions hit the real codegen path.

module spirv_emit shared public

require spirv_builder
require spirv_types
require spirv_grammar
require spirv_reflect
require math
require daslib/ast
require daslib/rtti
require daslib/ast_boost

// The shader stage drives the entry-point execution model + execution mode and which builtins
// are legal. Vertex needs no execution mode; Fragment needs OriginUpperLeft; Compute needs LocalSize.
// Mesh + Task (VK_EXT_mesh_shader) pull in the MeshShadingEXT capability + SPV_EXT_mesh_shader
// extension; both need LocalSize and Mesh additionally needs OutputVertices + OutputPrimitivesEXT
// + an output-topology mode (triangles / lines / points), all configurable via the [mesh_shader] args.
enum public ShaderStage {
    Compute
    Vertex
    Fragment
    Mesh
    Task
}

// Mesh-shader output configuration, parsed from the [mesh_shader] annotation args
// (max_vertices / max_primitives / output_topology) and threaded into generate_spirv. Drives the
// OutputVertices + OutputPrimitivesEXT counts and the output-topology execution mode
// (OutputTrianglesEXT / OutputLinesEXT / OutputPoints). The defaults (64 / 64 / triangles) match
// PR-D's hardcode, so a [mesh_shader] with no output args emits an unchanged blob.
enum public MeshOutputTopology {
    triangles
    lines
    points
}

struct public MeshOutputConfig {
    max_vertices   : int               = 64
    max_primitives : int               = 64
    topology       : MeshOutputTopology = MeshOutputTopology.triangles
}

// ===== per-global classification =====
struct GlobalInfo {
    var_id     : uint
    storage    : SpvStorageClass
    value_type : uint               // SPIR-V type-id of the whole variable's value
    is_ssbo    : bool               // wrapped in a Block-decorated struct (member 0 = the array)
    is_block   : bool               // a Block-decorated struct (ssbo OR ubo); member access -> OpAccessChain by field index
    is_sampler : bool               // a combined image sampler (UniformConstant OpTypeSampledImage); used via texture()
    is_storage_image : bool         // a read-write storage image (UniformConstant OpTypeImage); used via imageLoad/imageStore
    is_separate_image : bool        // a standalone sampled image (texture2D, OpTypeImage); combined at the call via OpSampledImage
    is_separate_sampler : bool      // a standalone sampler (OpTypeSampler)
    is_subpass_input : bool         // a subpass-input attachment (UniformConstant OpTypeImage Dim=SubpassData); used via subpassLoad
    is_sampler_array : bool         // a fixed-count descriptor array of combined image samplers (sampler2D[N], etc.) -- UniformConstant OpTypeArray of OpTypeSampledImage; accessed via samplers[idx] -> OpAccessChain
    is_mesh_output : bool           // a mesh per-vertex / per-primitive Output array (gl_MeshVerticesEXT / gl_PrimitiveTriangleIndicesEXT); a BARE Output array indexed by a single AccessChain (no member-0 SSBO indirection), pointee from elem_type
    arr_count  : int                // descriptor-array element count (when is_sampler_array; > 0); 0 for non-array
    elem_type  : uint               // SSBO runtime-array element type-id (vec/struct elements can't go through emit_type) OR descriptor-array element type-id (sampled-image / image / sampler)
    is_constant : bool              // a module-scope `let X = <const>` folded to an OpConstant; references resolve to const_id directly (no OpVariable / OpLoad)
    const_id   : uint               // the OpConstant result-id (when is_constant)
}

// A memory-backed local: an OpVariable in Function storage. Mutable locals (`var x`) and loop
// induction variables get one; reads become OpLoad, writes OpStore. The driver mem2reg's them,
// so we never hand-roll OpPhi. Immutable value-lets (`let x`) stay pure SSA ids (ctx.locals).
struct LocalVar {
    ptr_id     : uint    // OpVariable result-id (Function storage)
    value_type : uint    // SPIR-V type-id of the pointee
}

struct EmitCtx {
    globals       : table<uint64; GlobalInfo>   // intptr(Variable) -> classification
    locals        : table<uint64; uint>         // intptr(Variable) -> SSA result-id (value lets)
    local_vars    : table<uint64; LocalVar>     // intptr(Variable) -> memory-backed local (var / loop var)
    interface_ids : array<uint>                 // EntryPoint interface (Input/Output vars, SPIR-V <= 1.3)
    loop_stack    : array<LoopTargets>          // innermost loop's {merge, continue} for break/continue
    terminated    : bool                        // current block already has a terminator
    glsl_ext      : uint                        // OpExtInstImport "GLSL.std.450" set id (0 = not imported yet)
    stage         : ShaderStage                 // the shader stage (texture() implicit-LOD sampling is fragment-only)
    uses_frag_depth : bool                      // gl_FragDepth referenced (Output-only, so = written) -> emit DepthReplacing
    image_query_cap : bool                      // OpCapability ImageQuery already emitted (textureSize)
    input_attachment_cap : bool                 // OpCapability InputAttachment already emitted (subpassLoad)
    draw_parameters_cap : bool                  // OpCapability DrawParameters already emitted (gl_DrawID)
    descriptor_indexing_cap : bool              // OpCapability ShaderNonUniform + OpExtension "SPV_EXT_descriptor_indexing" already emitted (nonuniformEXT, descriptor arrays)
    sampled_image_nu_cap : bool                 // OpCapability SampledImageArrayNonUniformIndexing already emitted (NonUniform-indexed sampler descriptor array)
    mesh_shading_cap : bool                     // OpCapability MeshShadingEXT + OpExtension "SPV_EXT_mesh_shader" already emitted (mesh / task stages)
    mesh_max_vertices : int                     // mesh-output array sizing (from MeshOutputConfig): gl_MeshVerticesEXT length
    mesh_max_primitives : int                   // mesh-output array sizing: gl_PrimitiveTriangleIndicesEXT length
    mesh_topology : MeshOutputTopology          // declared output topology; the index-array builtin must match it (triangles -> gl_PrimitiveTriangleIndicesEXT)
    task_payload_id : uint                      // the @task_payload TaskPayloadWorkgroupEXT variable id (0 = none); EmitMeshTasksEXT appends it as the 4th operand
    nonuniform_ids : table<uint>                // set of result ids that carry the NonUniform decoration (nonuniformEXT marker + cascade through OpAccessChain / OpLoad on descriptor arrays)
    user_funcs    : table<uint64; UserFunc>     // intptr(Function) -> emittable user-defined shader function
    user_order    : array<uint64>               // user_funcs keys in stable emission order (collect_dependencies order)
    call_temps    : table<uint64; CallTemp>     // intptr(ref-arg Expression) -> its copy-in/out Function-storage temp
}

// A user-defined daslang function called from a shader: emitted as its own OpFunction. Ids are
// pre-allocated in a first pass so an OpFunctionCall can forward-reference a not-yet-emitted callee.
struct UserFunc {
    fn      : Function const?   // the function (collect_dependencies yields pointee-const pointers)
    fn_id   : uint          // OpFunction result id (pre-allocated; 0 when !valid)
    type_id : uint          // OpTypeFunction id
    ret_type : uint         // result type id (void's type id when is_void)
    is_void : bool
    valid   : bool          // false = signature rejected at registration (error already emitted)
}

// Per ref-argument copy-in/copy-out scratch: a Function-storage OpVariable hoisted into the calling
// function's entry block. SPIR-V (Logical addressing) forbids passing an access-chain pointer as a
// call argument, so every by-reference argument goes through one of these temps.
struct CallTemp {
    ptr_id     : uint
    value_type : uint
}

// break -> branch to `merge`, continue -> branch to `cont`. Pushed per active loop.
struct LoopTargets {
    merge : uint
    cont  : uint
}

// ===== per-control-flow-node label ids (visitor side-maps; allocated to match the hand-walker's
// linear id order so the emitted blob stays byte-identical) =====
struct IteIds {
    merge   : uint
    then_l  : uint
    else_l  : uint   // == merge when there is no else branch (no separate else label emitted)
}

struct LoopIds {
    header : uint
    cond_l : uint
    body_l : uint
    cont_l : uint
    merge  : uint
}

def finalize(var c : EmitCtx) {
    delete c.globals
    delete c.locals
    delete c.local_vars
    delete c.interface_ids
    delete c.loop_stack
    unsafe {
        delete c.user_funcs   // value type holds a FunctionPtr -> delete needs unsafe
    }
    delete c.user_order
    delete c.call_temps
    delete c.nonuniform_ids
}

// ===== builtin name -> SpvBuiltIn + storage class + the stage it's legal in =====
// Most builtins are read-only Inputs; gl_Position is the one written Output. The storage class
// decides Input vs Output pointer type; `req` is the single stage the builtin is valid in, so
// classify_global can reject a cross-stage reference (e.g. gl_FragCoord in a vertex shader) with a
// clean error instead of emitting a blob that only spirv-val would catch.
def private builtin_info(name : string) : tuple<ok : bool; bi : SpvBuiltIn; storage : SpvStorageClass; req : ShaderStage> {
    // compute
    if (name == "gl_GlobalInvocationID") return (ok = true, bi = SpvBuiltIn.GlobalInvocationId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_LocalInvocationID") return (ok = true, bi = SpvBuiltIn.LocalInvocationId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_WorkGroupID") return (ok = true, bi = SpvBuiltIn.WorkgroupId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_NumWorkGroups") return (ok = true, bi = SpvBuiltIn.NumWorkgroups, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_LocalInvocationIndex") return (ok = true, bi = SpvBuiltIn.LocalInvocationIndex, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    // vertex
    if (name == "gl_Position") return (ok = true, bi = SpvBuiltIn.Position, storage = SpvStorageClass.Output, req = ShaderStage.Vertex)
    if (name == "gl_PointSize") return (ok = true, bi = SpvBuiltIn.PointSize, storage = SpvStorageClass.Output, req = ShaderStage.Vertex)
    if (name == "gl_VertexIndex") return (ok = true, bi = SpvBuiltIn.VertexIndex, storage = SpvStorageClass.Input, req = ShaderStage.Vertex)
    if (name == "gl_InstanceIndex") return (ok = true, bi = SpvBuiltIn.InstanceIndex, storage = SpvStorageClass.Input, req = ShaderStage.Vertex)
    if (name == "gl_DrawID") return (ok = true, bi = SpvBuiltIn.DrawIndex, storage = SpvStorageClass.Input, req = ShaderStage.Vertex)
    // fragment
    if (name == "gl_FragCoord") return (ok = true, bi = SpvBuiltIn.FragCoord, storage = SpvStorageClass.Input, req = ShaderStage.Fragment)
    if (name == "gl_FragDepth") return (ok = true, bi = SpvBuiltIn.FragDepth, storage = SpvStorageClass.Output, req = ShaderStage.Fragment)
    if (name == "gl_PointCoord") return (ok = true, bi = SpvBuiltIn.PointCoord, storage = SpvStorageClass.Input, req = ShaderStage.Fragment)
    // gl_PrimitiveID: fragment Input (the per-primitive id a mesh shader wrote via gl_MeshPrimitivesEXT).
    // In a mesh pipeline it is NOT fixed-function-generated, so reading it pulls in the MeshShadingEXT
    // capability on the fragment module (handled at classification).
    if (name == "gl_PrimitiveID") return (ok = true, bi = SpvBuiltIn.PrimitiveId, storage = SpvStorageClass.Input, req = ShaderStage.Fragment)
    return (ok = false, bi = SpvBuiltIn.Position, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
}

def private stage_name(stage : ShaderStage) : string {
    if (stage == ShaderStage.Vertex) return "vertex"
    if (stage == ShaderStage.Fragment) return "fragment"
    if (stage == ShaderStage.Mesh) return "mesh"
    if (stage == ShaderStage.Task) return "task"
    return "compute"
}

// ===== std140 layout for a UBO interface block =====
// std140 base alignment: scalar -> 4, vec2 -> 8, vec3/vec4 -> 16. The base *size* of a vec3 is 12
// (only its alignment is 16), so a scalar can pack into the trailing 4 bytes after a vec3.
def private std140_align(bt : Type) : int {
    let cc = component_count(bt)
    if (cc == 1) return 4
    if (cc == 2) return 8
    return 16
}

def private std140_size(bt : Type) : int {
    return component_count(bt) * 4
}

def private round_up(off, a : int) : int {
    return ((off + a - 1) / a) * a
}

//! One std140-laid-out field of a @uniform / @push_constant interface block. Returned by
//! compute_block_layout and consumed by host-side bind macros (e.g. dasVulkan's SpirvVulkanShader)
//! to emit one write per field at the right byte offset. `is_matrix` just means "this field IS a
//! matrix" (and gets MatrixStride 16 in SPIR-V) — it does NOT by itself imply the daslang storage
//! and std140 layout differ. For float4x4 the columns are already vec4-wide so a single memcpy
//! works; for float3x3 / float3x4 the daslang storage is packed (3-wide columns) while std140 pads
//! each column to vec4, so bind code must repack per-column. **Use `matrix_width` to choose**:
//! `matrix_width == 4` → safe to memcpy the field; `matrix_width < 4` → repack per column.
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
//! member alignment. Used by build_block_struct (SPIR-V type emit) and by host-side bind macros.
//! Rejects bool and non-numeric fields fail-closed (no physical std140 layout). `sname` is only used
//! in error messages. `fields` is cleared at entry so reuse across calls and partial-failure returns
//! can't bleed. Supported field shapes (PR-G complete): 32-bit scalar / vector / matrix; nested
//! struct (recursive); fixed-array of scalar / vector (std140 ArrayStride = element-size rounded to
//! 16); fixed-array of nested struct (ArrayStride = laid-out struct size rounded to struct
//! alignment). Out of scope (clean rejection): matrix-of-array, array-of-array, runtime arrays
//! (use `@ssbo` for those).
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

// Build a laid-out interface struct from a daslang struct's fields. Supported member shapes:
// 32-bit scalar / vector / matrix; fixed-array of scalar / vector / nested struct (std140
// ArrayStride decoration on the OpTypeArray); nested struct (recursive non-Block OpTypeStruct with
// its own Offset cascade). `as_block` toggles the Block decoration on the *outer* struct (true for
// @uniform/@push_constant blocks, false for an SSBO runtime-array element struct); recursively-built
// nested structs are always emitted non-Block. Returns the OpTypeStruct id plus the std140 size
// (offset past the last member) and the struct's max-member alignment (which the SSBO caller rounds
// the struct size up to before using it as the OpTypeRuntimeArray's stride).
def private build_block_struct(var m : SpirvModule; sname : string; st : Structure?; as_block : bool; var errors : array<string>) : tuple<ok : bool; type_id : uint; size : int; align : int> {
    var fields : array<BlockField>
    let layout = compute_block_layout(sname, st, fields, errors)
    if (!layout.ok) {
        delete fields
        return (ok = false, type_id = 0u, size = 0, align = 0)
    }
    var members : array<uint>
    var offsets : array<int>
    var mat_strides : array<int>
    var fi = 0
    for (fld in st.fields) {
        let f = fields[fi]
        // composite field: fixed-array or nested struct — build its SPIR-V type recursively. Plain
        // emit_type(fld._type) doesn't lower tStructure (would panic) and would emit a strideless
        // OpTypeArray for a fixed-array member (invalid in a Block). Layout was already validated
        // by compute_block_layout above; here we just construct the matching type ids.
        if (f.is_array) {
            let et = fld._type.firstType
            var elem_type : uint = 0u
            if (et.baseType == Type.tStructure && et.structType != null) {
                let inner = build_block_struct(m, "{sname}.{f.name}[]", et.structType, false, errors)
                if (!inner.ok) {
                    delete members; delete offsets; delete mat_strides; delete fields
                    return (ok = false, type_id = 0u, size = 0, align = 0)
                }
                elem_type = inner.type_id
            } else {
                elem_type = emit_type(m, et)
            }
            members |> push(type_array_strided(m, elem_type, f.array_count, f.array_stride))
            offsets |> push(f.offset)
            mat_strides |> push(0)
            fi++
            continue
        }
        if (f.is_struct) {
            let inner = build_block_struct(m, "{sname}.{f.name}", fld._type.structType, false, errors)
            if (!inner.ok) {
                delete members; delete offsets; delete mat_strides; delete fields
                return (ok = false, type_id = 0u, size = 0, align = 0)
            }
            members |> push(inner.type_id)
            offsets |> push(f.offset)
            mat_strides |> push(0)
            fi++
            continue
        }
        members |> push(emit_type(m, fld._type))
        offsets |> push(f.offset)
        // std140 matrix: ColMajor, each column padded to vec4 (MatrixStride 16) — see BlockField docs.
        mat_strides |> push(f.is_matrix ? 16 : 0)
        fi++
    }
    let id = (as_block ? type_struct_block(m, members, offsets, mat_strides)
                       : type_struct_layout(m, members, offsets, mat_strides))
    delete members
    delete offsets
    delete mat_strides
    delete fields
    return (ok = true, type_id = id, size = layout.size, align = layout.align)
}

// std430 layout of an SSBO runtime-array element. Returns the element SPIR-V type-id + the
// ArrayStride. A 32-bit scalar/vector element strides by its std430 base size rounded up to its base
// alignment (vec3 -> 16, vec2 -> 8, scalar -> 4); a struct element is laid out non-Block and strides
// by its size rounded to the struct alignment. Matrix and nested-array elements are rejected (wrap a
// matrix in a struct). std140_size/std140_align are reused: they equal the std430 base size/alignment
// for these flat scalar/vector kinds (std430 differs only on array/struct padding, handled here).
def private ssbo_element_layout(var m : SpirvModule; sname : string; et : TypeDecl?; var errors : array<string>) : tuple<ok : bool; elem_type : uint; stride : int> {
    if (et == null) {
        errors |> push("ssbo '{sname}' has no element type")
        return (ok = false, elem_type = 0u, stride = 0)
    }
    if (matrix_info(et).ok) {
        errors |> push("ssbo '{sname}' element is a matrix — wrap it in a struct (a bare matrix runtime-array element is not supported)")
        return (ok = false, elem_type = 0u, stride = 0)
    }
    let bt = et.baseType
    if (bt == Type.tStructure) {
        let bs = build_block_struct(m, "{sname} element", et.structType, false, errors)
        if (!bs.ok) return (ok = false, elem_type = 0u, stride = 0)
        return (ok = true, elem_type = bs.type_id, stride = round_up(bs.size, bs.align))
    }
    // 32-bit scalar or its 2/3/4 vector (scalar_class rejects bool and non-numeric base types)
    if (component_count(bt) >= 1 && scalar_class(bt) >= 0) {
        return (ok = true, elem_type = emit_type(m, et), stride = round_up(std140_size(bt), std140_align(bt)))
    }
    errors |> push("ssbo '{sname}' element type {bt} is not supported (a 32-bit scalar, a vector of those, or a struct of those)")
    return (ok = false, elem_type = 0u, stride = 0)
}

// The single SpirvStageFlags bit for one shader stage. A shader module is one stage, so every
// binding/range it reflects carries this single bit; the host ORs across stages when merging.
def private stage_flag(stage : ShaderStage) : SpirvStageFlags {
    if (stage == ShaderStage.Vertex) return SpirvStageFlags.vertex
    if (stage == ShaderStage.Fragment) return SpirvStageFlags.fragment
    if (stage == ShaderStage.Mesh) return SpirvStageFlags.mesh
    if (stage == ShaderStage.Task) return SpirvStageFlags.task
    return SpirvStageFlags.compute
}

// ===== combined image sampler detection =====
// sampler globals are recognized by their opaque marker struct name (sampler2D, ...). Returns the
// OpTypeImage operands classify_global needs: dimensionality (dim), the Arrayed flag (arrayed), and
// the Depth flag (depth, 1 for a sampler2DShadow comparison sampler, else 0).
def private sampler_info(t : TypeDecl?) : tuple<ok : bool; dim : SpvDim; arrayed : uint; depth : uint> {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return (ok = false, dim = SpvDim._2D, arrayed = 0u, depth = 0u)
    let n = "{t.structType.name}"
    if (n == "sampler2D") return (ok = true, dim = SpvDim._2D, arrayed = 0u, depth = 0u)
    if (n == "sampler3D") return (ok = true, dim = SpvDim._3D, arrayed = 0u, depth = 0u)
    if (n == "samplerCube") return (ok = true, dim = SpvDim.Cube, arrayed = 0u, depth = 0u)
    if (n == "sampler2DArray") return (ok = true, dim = SpvDim._2D, arrayed = 1u, depth = 0u)
    // sampler2DShadow: a 2D DEPTH-comparison sampler (Depth=1). Sampled only via textureCompare (Dref).
    if (n == "sampler2DShadow") return (ok = true, dim = SpvDim._2D, arrayed = 0u, depth = 1u)
    return (ok = false, dim = SpvDim._2D, arrayed = 0u, depth = 0u)
}

// Storage-image globals are recognized by their opaque marker struct name (image2D, ...). Returns the
// SPIR-V image dimensionality so classify_global can build the read-write OpTypeImage (sampled=2).
def private storage_image_info(t : TypeDecl?) : tuple<ok : bool; dim : SpvDim> {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return (ok = false, dim = SpvDim._2D)
    if ("{t.structType.name}" == "image2D") return (ok = true, dim = SpvDim._2D)
    return (ok = false, dim = SpvDim._2D)
}

// Map a GLSL-style storage-image format qualifier (`@format = "r32f"`) to its SpvImageFormat. Only the
// float-sampled formats are supported -- integer formats (r32i/r32ui/...) would need int/uint
// imageLoad/imageStore overloads, a separate scope. ok=false -> caller emits a fail-closed error.
def private parse_storage_image_format(s : string) : tuple<ok : bool; fmt : SpvImageFormat> {
    if (s == "" || s == "rgba8") return (ok = true, fmt = SpvImageFormat.Rgba8)
    if (s == "rgba8_snorm") return (ok = true, fmt = SpvImageFormat.Rgba8Snorm)
    if (s == "rgba16f") return (ok = true, fmt = SpvImageFormat.Rgba16f)
    if (s == "rgba32f") return (ok = true, fmt = SpvImageFormat.Rgba32f)
    if (s == "r32f") return (ok = true, fmt = SpvImageFormat.R32f)
    return (ok = false, fmt = SpvImageFormat.Rgba8)
}

// subpassInput is the opaque marker for a Vulkan subpass input attachment. Same recognition pattern
// as sampler/image globals -- a tStructure named "subpassInput" -> Dim=SubpassData OpTypeImage in
// UniformConstant storage (sampled=2, no format), decorated with InputAttachmentIndex, sampled via
// subpassLoad -> OpImageRead with an int2(0,0) coord.
def private is_subpass_input_type(t : TypeDecl?) : bool {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return false
    return "{t.structType.name}" == "subpassInput"
}

// Exactly the type set emit_type lowers without panicking: a scalar/vector (scalar_class >= 0), bool,
// a matrix, or a fixed array of those. Lets the @workgroup path reject an unsupported shared type with
// a clean diagnostic instead of crashing in emit_type (struct/handle/dynamic-array shared vars).
def private is_emittable_value_type(t : TypeDecl?) : bool {
    if (t == null) return false
    if (matrix_info(t).ok) return true
    let bt = t.baseType
    if (bt == Type.tFixedArray && t.firstType != null) return is_emittable_value_type(t.firstType)
    return scalar_class(bt) >= 0 || bt == Type.tBool
}

// gl_MeshVerticesEXT / gl_PrimitiveTriangleIndicesEXT — the mesh per-vertex / per-primitive Output
// arrays (VK_EXT_mesh_shader). Unlike the scalar builtins these are ARRAYS of a builtin, sized from
// the [mesh_shader] output config (stashed on ctx):
//   gl_MeshVerticesEXT             -> Output array[max_vertices] of a Block { vec4 } whose member 0 is
//                                     BuiltIn Position (so gl_MeshVerticesEXT[i].gl_Position writes clip pos)
//   gl_PrimitiveTriangleIndicesEXT -> Output array[max_primitives] of uvec3, BuiltIn-decorated on the var
// Both are BARE Output arrays (is_mesh_output): an `[i]` index is a single AccessChain off the variable
// (no member-0 SSBO indirection), and the per-vertex block's gl_Position field chains member 0 via the
// generic visitExprField path. The daslang `array<>` decls in spirv_builtins exist only so the indexing
// + field access typecheck; the real SPIR-V sizing comes from the config, not the (empty) daslang array.
def private classify_mesh_output(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; vname : string; stage : ShaderStage; var errors : array<string>) {
    if (stage != ShaderStage.Mesh) {
        errors |> push("mesh output '{vname}' is only available in a mesh shader, not {stage_name(stage)}")
        return
    }
    if (vname == "gl_MeshVerticesEXT") {
        let n = ctx.mesh_max_vertices
        if (n <= 0) {
            errors |> push("gl_MeshVerticesEXT: max_vertices must be > 0 (got {n})")
            return
        }
        // per-vertex Block { gl_Position : vec4 } — member 0 = BuiltIn Position, Block-decorated.
        let f4 = type_vector(m, type_float(m, 32u), 4)
        let blk = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.TypeStruct, blk, f4)
        emit(m, SEC_DECOR, SpvOp.Decorate, blk, uint(SpvDecoration.Block))
        emit(m, SEC_DECOR, SpvOp.MemberDecorate, blk, 0u, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.Position))
        let arr = type_array(m, blk, n)
        let pt = type_pointer(m, SpvStorageClass.Output, arr)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Output))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Output, value_type = arr, elem_type = blk, is_mesh_output = true))
        ctx.interface_ids |> push(vid)
        return
    }
    if (vname == "gl_PrimitiveTriangleIndicesEXT") {
        // PR-E2 ships triangles only; the line (uvec2) / point (uint) index arrays + their topology
        // arrive in a follow-up. Reject a mismatch fail-closed rather than emit indices the declared
        // topology can't consume.
        if (ctx.mesh_topology != MeshOutputTopology.triangles) {
            errors |> push("gl_PrimitiveTriangleIndicesEXT requires output_topology=\"triangles\" (lines / points index arrays land in a follow-up)")
            return
        }
        let n = ctx.mesh_max_primitives
        if (n <= 0) {
            errors |> push("gl_PrimitiveTriangleIndicesEXT: max_primitives must be > 0 (got {n})")
            return
        }
        let u3 = type_vector(m, type_uint(m), 3)
        let arr = type_array(m, u3, n)
        let pt = type_pointer(m, SpvStorageClass.Output, arr)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Output))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PrimitiveTriangleIndicesEXT))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Output, value_type = arr, elem_type = u3, is_mesh_output = true))
        ctx.interface_ids |> push(vid)
        return
    }
    if (vname == "gl_MeshPrimitivesEXT") {
        // per-primitive builtin Block (the per-primitive analog of gl_MeshVerticesEXT). PR-F ships just
        // gl_PrimitiveID; gl_Layer / gl_ViewportIndex / gl_CullPrimitiveEXT are follow-up members.
        let n = ctx.mesh_max_primitives
        if (n <= 0) {
            errors |> push("gl_MeshPrimitivesEXT: max_primitives must be > 0 (got {n})")
            return
        }
        // Block { gl_PrimitiveID : int } — member 0 = BuiltIn PrimitiveId + PerPrimitiveEXT (a Builtin
        // PrimitiveId in a mesh shader MUST carry PerPrimitiveEXT, VUID-PrimitiveId-PrimitiveId-07040;
        // in a Block both ride as MEMBER decorations alongside the BuiltIn).
        let i32 = type_int(m, 32u, 1u)
        let blk = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.TypeStruct, blk, i32)
        emit(m, SEC_DECOR, SpvOp.Decorate, blk, uint(SpvDecoration.Block))
        emit(m, SEC_DECOR, SpvOp.MemberDecorate, blk, 0u, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PrimitiveId))
        ensure_per_primitive(m, ctx)
        emit(m, SEC_DECOR, SpvOp.MemberDecorate, blk, 0u, uint(SpvDecoration.PerPrimitiveEXT))
        let arr = type_array(m, blk, n)
        let pt = type_pointer(m, SpvStorageClass.Output, arr)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Output))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Output, value_type = arr, elem_type = blk, is_mesh_output = true))
        ctx.interface_ids |> push(vid)
        return
    }
}

// Emit a PLAIN struct type (each field via emit_type, NO Block / Offset / ArrayStride decorations) for a
// logical storage class. Used for the @task_payload (TaskPayloadWorkgroupEXT) struct, which -- unlike a
// UBO/SSBO Block -- has natural layout. Member type-ids come from emit_type so they match the
// field_pointee_type the is_block member-access path uses. Fields must be emittable (scalar / vector /
// matrix / fixed array); returns 0 + a pushed error otherwise.
def private build_plain_struct(var m : SpirvModule; sname : string; st : Structure?; var errors : array<string>) : uint {
    if (st == null || empty(st.fields)) {
        errors |> push("@task_payload '{sname}' must be a struct with at least one field")
        return 0u
    }
    var members : array<uint>
    for (fld in st.fields) {
        // Scalar / vector / matrix / fixed-array-of-those all lower plainly via emit_type (no Block /
        // Offset / ArrayStride). Nested-struct fields would need a recursive PLAIN struct (build_block_struct
        // adds offsets, which TaskPayloadWorkgroupEXT must not have), so reject those fail-closed for now.
        if (!is_emittable_value_type(fld._type)) {
            errors |> push("@task_payload '{sname}': field '{fld.name}' must be a scalar / vector / matrix / fixed array of those (nested-struct payload fields are not yet supported)")
            delete members
            return 0u
        }
        members |> push(emit_type(m, fld._type))
    }
    let id = alloc_id(m)
    var ops <- [id]
    ops |> push_from(members)
    emit_n(m, SEC_TYPES, SpvOp.TypeStruct, ops)
    delete ops
    delete members
    return id
}

// ===== global variable classification + emission =====
def private classify_global(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; stage : ShaderStage; var reflection : SpirvReflection; var errors : array<string>) {
    let vname = "{v.name}"
    let bi = builtin_info(vname)
    if (bi.ok) {
        // The workgroup-grid builtins (gl_GlobalInvocationID / gl_LocalInvocationID / gl_WorkGroupID /
        // gl_NumWorkGroups / gl_LocalInvocationIndex, all req=Compute in builtin_info) are equally valid
        // in MESH and TASK shaders -- those stages are workgroup-dispatched (LocalSize) too, and a real
        // mesh/task shader needs them to index its slice of the work (e.g. the payload survivor-list).
        let workgroup_builtin = bi.req == ShaderStage.Compute
        let stage_ok = (bi.req == stage ||
            (workgroup_builtin && (stage == ShaderStage.Mesh || stage == ShaderStage.Task)))
        if (!stage_ok) {
            errors |> push("builtin '{vname}' is only available in the {stage_name(bi.req)} stage, not {stage_name(stage)}")
            return
        }
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, bi.storage, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(bi.storage))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.BuiltIn), uint(bi.bi))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = bi.storage, value_type = vt, is_ssbo = false))
        ctx.interface_ids |> push(vid)   // Input + Output builtins both list in the entry interface
        // gl_FragDepth is Output-only, so any reference to it is a write -> the shader replaces the
        // fixed-function depth and must declare DepthReplacing. Flagged here at classification.
        if (bi.bi == SpvBuiltIn.FragDepth) {
            ctx.uses_frag_depth = true
        }
        // gl_DrawID -> BuiltIn DrawIndex pulls in the DrawParameters capability (the multi-draw
        // draw-call ordinal isn't part of the base Shader cap).
        if (bi.bi == SpvBuiltIn.DrawIndex) {
            ensure_draw_parameters(m, ctx)
        }
        // gl_PrimitiveID in a fragment of a mesh pipeline: the id is not fixed-function-generated, so
        // reading it pulls in MeshShadingEXT + SPV_EXT_mesh_shader (→ SPIR-V 1.4), same as the mesh stage.
        // It is an integer fragment Input, so it MUST be Flat (no interpolation for ints; VUID-…-Flat-04744).
        if (bi.bi == SpvBuiltIn.PrimitiveId) {
            ensure_per_primitive(m, ctx)
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Flat))
        }
        return
    }
    // mesh per-vertex / per-primitive Output arrays (gl_MeshVerticesEXT / gl_PrimitiveTriangleIndicesEXT):
    // recognized by name BEFORE the @ssbo array path below (their daslang type is array<...>, which the
    // SSBO path would otherwise claim). Sized from the [mesh_shader] output config on ctx.
    if (vname == "gl_MeshVerticesEXT" || vname == "gl_PrimitiveTriangleIndicesEXT" || vname == "gl_MeshPrimitivesEXT") {
        classify_mesh_output(m, ctx, v, vname, stage, errors)
        return
    }
    // stage I/O: @in / @out user variables carry an explicit @location. They become Input/Output
    // OpVariables with a Location decoration and list in the entry-point interface.
    let is_in = v.annotation |> find_arg("in") is tBool
    let is_out = v.annotation |> find_arg("out") is tBool
    if (is_in || is_out) {
        if (stage == ShaderStage.Compute) {
            errors |> push("stage variable '{vname}': @in/@out Location I/O is not valid in a compute shader")
            return
        }
        if (is_in && is_out) {
            errors |> push("stage variable '{vname}' cannot be both @in and @out")
            return
        }
        let loc = v.annotation |> find_arg("location") ?as tInt ?? -1
        if (loc < 0) {
            errors |> push("stage variable '{vname}' must specify a non-negative @location (e.g. @in @location = 0)")
            return
        }
        let storage = is_in ? SpvStorageClass.Input : SpvStorageClass.Output
        let is_per_prim = v.annotation |> find_arg("per_primitive") is tBool
        // Mesh per-vertex / per-primitive USER output: in a mesh shader an @out varying is declared as
        // an ARRAY (out array<float3> v_color, written v_color[i] = …) and lowers to a bare Output
        // array sized by max_vertices (per-vertex) or max_primitives (@per_primitive), Location-decorated
        // (+ PerPrimitiveEXT). The matching fragment @in is the per-fragment scalar — the rasteriser
        // expands the array. Same is_mesh_output access path as gl_MeshVerticesEXT[i].
        if (is_out && stage == ShaderStage.Mesh && v._type.baseType == Type.tArray) {
            // Fail-closed: the element must be an emittable varying type (scalar / vector / matrix), or
            // emit_type would panic on it (e.g. array<double> / array<SomeStruct>) -- mirror the
            // @workgroup guard so an unsupported element type is a clean compile error, not a crash.
            if (!is_emittable_value_type(v._type.firstType)) {
                errors |> push("mesh output '{vname}': array element type is not a supported varying type (scalar / vector / matrix)")
                return
            }
            let count = is_per_prim ? ctx.mesh_max_primitives : ctx.mesh_max_vertices
            if (count <= 0) {
                errors |> push("mesh output '{vname}': max_{is_per_prim ? "primitives" : "vertices"} must be > 0 (got {count})")
                return
            }
            let elem_t = emit_type(m, v._type.firstType)
            let arr = type_array(m, elem_t, count)
            let apt = type_pointer(m, SpvStorageClass.Output, arr)
            let avid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, apt, avid, uint(SpvStorageClass.Output))
            emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.Location), uint(loc))
            if (is_per_prim) {
                ensure_per_primitive(m, ctx)
                emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.PerPrimitiveEXT))
            }
            // Same Flat rule as scalar stage I/O, keyed on the ELEMENT type: an int/uint varying (or an
            // explicit @flat) must be Flat so it matches the fragment-side scalar input (where Flat is
            // auto-applied for integers); without it the output/input interface decorations mismatch.
            let elem_bt = v._type.firstType.baseType
            let elem_is_int = scalar_class(elem_bt) >= 0 && scalar_class(elem_bt) <= 1
            if (elem_is_int || (v.annotation |> find_arg("flat") is tBool)) {
                emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.Flat))
            }
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = avid, storage = SpvStorageClass.Output, value_type = arr, elem_type = elem_t, is_mesh_output = true))
            ctx.interface_ids |> push(avid)
            return
        }
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, storage, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(storage))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Location), uint(loc))
        // A fragment @per_primitive @in is the per-fragment side of a mesh per-primitive output: it must
        // carry the matching PerPrimitiveEXT decoration so the rasteriser feeds it per-primitive. The
        // fragment stage doesn't otherwise enable mesh-shading, so a @per_primitive @in alone must pull in
        // the MeshShadingEXT capability (-> SPIR-V 1.4) that PerPrimitiveEXT requires.
        if (is_in && is_per_prim) {
            ensure_per_primitive(m, ctx)
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.PerPrimitiveEXT))
        }
        // Flat decoration. Vulkan mandates Flat on any integer/uint varying (no valid interpolation
        // mode exists for integers — the driver returns garbage without it), so emit it automatically
        // for those. Floats default to perspective-correct smooth interpolation; honour an explicit
        // @flat annotation when the user wants flat-shaded floats (low-poly stylised look, provoking-
        // vertex picks per primitive). Reflection bit not added — Flat is purely a SPIR-V decoration,
        // it doesn't change the descriptor or pipeline layout.
        let bt = v._type.baseType
        let is_int_kind = scalar_class(bt) >= 0 && scalar_class(bt) <= 1
        let want_flat = is_int_kind || (v.annotation |> find_arg("flat") is tBool)
        if (want_flat) {
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Flat))
        }
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = storage, value_type = vt, is_ssbo = false))
        ctx.interface_ids |> push(vid)
        return
    }
    // descriptor array of combined image samplers (sampler2D[N], samplerCube[N], etc.) -- the
    // VK_EXT_descriptor_indexing surface that lets one shader index a pool of resources by a
    // per-invocation value. Lowers to UniformConstant OpTypeArray of OpTypeSampledImage, sampled via
    // texture(arr[i], uv) -> OpAccessChain + OpLoad + OpImageSample*. PR-C scope is fixed-count only;
    // runtime-sized arrays (sampler2D[]) need RuntimeDescriptorArray + variable-descriptor-count
    // binding plumbing and ship in a follow-up. Other resource-array flavours (image2D[N], texture2D
    // [N], sampler[N], subpassInput[N]) likewise follow.
    if (v._type != null && v._type.baseType == Type.tFixedArray && v._type.firstType != null) {
        let esi = sampler_info(v._type.firstType)
        if (esi.ok) {
            // post-0.6.3 tFixedArray is structural: one node per dim, fixedDim is THIS level's count.
            // Multi-dim sampler arrays are not a Vulkan thing (descriptors are 1-D), so reject any
            // outer fixed-array whose element is itself a fixed-array.
            if (v._type.firstType.baseType == Type.tFixedArray) {
                errors |> push("sampler descriptor array '{vname}' must be single-dimensional; got nested fixed-arrays")
                return
            }
            if (v._type.fixedDim <= 0) {
                errors |> push("sampler descriptor array '{vname}' must have positive count (got {v._type.fixedDim})")
                return
            }
            let n = v._type.fixedDim
            let img = type_image(m, type_float(m, 32u), esi.dim, esi.arrayed, 1u, SpvImageFormat.Unknown, esi.depth)
            let simg = type_sampled_image(m, img)
            let arr = type_array(m, simg, n)
            let pt = type_pointer(m, SpvStorageClass.UniformConstant, arr)
            let vid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
            let set = v.annotation |> find_arg("set") ?as tInt ?? 0
            let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
            if (set < 0 || binding < 0) {
                errors |> push("sampler descriptor array '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
                return
            }
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant,
                value_type = arr, is_sampler_array = true, arr_count = n, elem_type = simg))
            let kind = esi.depth != 0u ? SpirvDescriptorKind.comparison_sampler : SpirvDescriptorKind.combined_image_sampler
            reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
                kind = kind, count = n, stages = stage_flag(stage)))
            return
        }
    }
    // combined image sampler: a global of an opaque sampler type (sampler2D) -> UniformConstant
    // OpTypeSampledImage + DescriptorSet/Binding. Used only via texture(); not in the entry interface.
    let si = sampler_info(v._type)
    if (si.ok) {
        let img = type_image(m, type_float(m, 32u), si.dim, si.arrayed, 1u, SpvImageFormat.Unknown, si.depth)
        let simg = type_sampled_image(m, img)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, simg)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("sampler '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant, value_type = simg, is_sampler = true))
        // a Depth=1 sampler is a comparison sampler (sampled via textureCompare/Dref); the host binds a
        // VkSampler created with compareEnable. Otherwise an ordinary combined image sampler.
        let kind = si.depth != 0u ? SpirvDescriptorKind.comparison_sampler : SpirvDescriptorKind.combined_image_sampler
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = kind, count = 1, stages = stage_flag(stage)))
        return
    }
    // storage image: a global of an opaque image type (image2D) -> UniformConstant OpTypeImage with a
    // KNOWN format (default Rgba8, or @format = "r32f"/"rgba16f"/...; sampled=2) so no without-format
    // capability is required. DescriptorSet/Binding; read/written via imageLoad/imageStore. Not in the
    // entry interface (UniformConstant, SPIR-V <= 1.3).
    let ii = storage_image_info(v._type)
    if (ii.ok) {
        let fmt_str = v.annotation |> find_arg("format") ?as tString ?? "rgba8"
        let pf = parse_storage_image_format(fmt_str)
        if (!pf.ok) {
            errors |> push("storage image '{vname}': unsupported @format '{fmt_str}' (supported: rgba8, rgba8_snorm, rgba16f, rgba32f, r32f)")
            return
        }
        let img = type_image(m, type_float(m, 32u), ii.dim, 0u, 2u, pf.fmt)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, img)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("storage image '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant, value_type = img, is_storage_image = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.storage_image, count = 1, stages = stage_flag(stage)))
        return
    }
    // subpass input attachment: a global of the opaque subpassInput type. OpTypeImage with Dim=
    // SubpassData (Sampled=2, no format), UniformConstant storage, decorated with both Descriptor-
    // Set/Binding (like any other descriptor) AND InputAttachmentIndex (from @input_attachment_index).
    // Sampled only via subpassLoad() -- which emits OpImageRead with an internal int2(0,0) coord.
    // Requires the InputAttachment capability (emitted once via ensure_input_attachment). Used by
    // Vulkan deferred-rendering subpasses to read the previous subpass's color/depth at the same
    // fragment coordinate (the int2(0,0) is an OFFSET relative to gl_FragCoord, not an absolute pos).
    if (is_subpass_input_type(v._type)) {
        // SubpassData images + the InputAttachment capability are fragment-only per the Vulkan
        // spec; reject the global at emit time with a clean message rather than letting spirv-val
        // surface it as a structural error.
        if (stage != ShaderStage.Fragment) {
            errors |> push("subpassInput '{vname}' is fragment-only (Vulkan subpass-input attachments)")
            return
        }
        let img = type_image(m, type_float(m, 32u), SpvDim.SubpassData, 0u, 2u, SpvImageFormat.Unknown)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, img)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        let iax = v.annotation |> find_arg("input_attachment_index") ?as tInt ?? -1
        if (set < 0 || binding < 0) {
            errors |> push("subpassInput '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        if (iax < 0) {
            errors |> push("subpassInput '{vname}' must specify @input_attachment_index = N (non-negative)")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.InputAttachmentIndex), uint(iax))
        ensure_input_attachment(m, ctx)
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant, value_type = img, is_subpass_input = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.input_attachment, count = 1, stages = stage_flag(stage)))
        return
    }
    // separate image + sampler (Vulkan's split model): a texture2D is a standalone sampled OpTypeImage,
    // a sampler is a standalone OpTypeSampler -- each its own UniformConstant descriptor. sampleTexture()
    // combines them at the call site (OpSampledImage).
    let stn = v._type != null && v._type.baseType == Type.tStructure && v._type.structType != null ? "{v._type.structType.name}" : ""
    if (stn == "texture2D" || stn == "sampler") {
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("'{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        let is_img = stn == "texture2D"
        let vt = is_img ? type_image(m, type_float(m, 32u), SpvDim._2D) : type_sampler(m)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant,
            value_type = vt, is_separate_image = is_img, is_separate_sampler = !is_img))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = is_img ? SpirvDescriptorKind.sampled_image : SpirvDescriptorKind.sampler,
            count = 1, stages = stage_flag(stage)))
        return
    }
    if (v.annotation |> find_arg("ssbo") is tBool) {
        if (v._type.baseType != Type.tArray || v._type.firstType == null) {
            errors |> push("ssbo '{vname}' must be an array<T>")
            return
        }
        // std430 element layout: 32-bit scalar/vector elements stride by their aligned base size,
        // struct elements are laid out non-Block and stride by their aligned struct size.
        let el = ssbo_element_layout(m, vname, v._type.firstType, errors)
        if (!el.ok) return
        let rta = type_runtime_array(m, el.elem_type, el.stride)
        var members <- [rta]
        var offsets <- [0]
        let st = type_struct_block(m, members, offsets)
        delete members
        delete offsets
        let pt = type_pointer(m, SpvStorageClass.StorageBuffer, st)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.StorageBuffer))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("ssbo '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.StorageBuffer, value_type = st, is_ssbo = true, is_block = true, elem_type = el.elem_type))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.storage_buffer, count = 1, stages = stage_flag(stage)))
        return
    }
    // uniform buffer: @uniform struct -> Uniform storage + Block-decorated struct (std140) +
    // DescriptorSet/Binding. Member reads go through ExprField -> OpAccessChain by field index.
    if (v.annotation |> find_arg("uniform") is tBool) {
        if (v._type.baseType != Type.tStructure || v._type.structType == null) {
            errors |> push("uniform '{vname}' must be a struct")
            return
        }
        let bs = build_block_struct(m, vname, v._type.structType, true, errors)
        if (!bs.ok) return
        let pt = type_pointer(m, SpvStorageClass.Uniform, bs.type_id)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Uniform))
        let set = v.annotation |> find_arg("set") ?as tInt ?? 0
        let binding = v.annotation |> find_arg("binding") ?as tInt ?? 0
        if (set < 0 || binding < 0) {
            errors |> push("uniform '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Uniform, value_type = bs.type_id, is_ssbo = false, is_block = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = stage_flag(stage)))
        return
    }
    // push constants: @push_constant struct -> PushConstant storage + Block-decorated struct. Same
    // std140-style member layout + ExprField access as a UBO, but NO descriptor set/binding (push
    // constants are not descriptors) and not listed in the entry interface (SPIR-V <= 1.3).
    if (v.annotation |> find_arg("push_constant") is tBool) {
        if (v._type.baseType != Type.tStructure || v._type.structType == null) {
            errors |> push("push_constant '{vname}' must be a struct")
            return
        }
        let bs = build_block_struct(m, vname, v._type.structType, true, errors)
        if (!bs.ok) return
        let pt = type_pointer(m, SpvStorageClass.PushConstant, bs.type_id)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.PushConstant))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.PushConstant, value_type = bs.type_id, is_ssbo = false, is_block = true))
        reflection.push_constants |> push(SpirvPushConstantRange(offset = 0, size = bs.size, stages = stage_flag(stage)))
        return
    }
    // workgroup shared memory: @workgroup global -> Workgroup storage (GLSL `shared`). Not a
    // descriptor and not in the entry interface (Workgroup is not Input/Output, SPIR-V <= 1.3), so no
    // set/binding and nothing reflected. Compute-only. Read/written via OpLoad/OpStore; a fixed-array
    // shared global is indexed by visitExprAt's Workgroup branch. (`shared` is a reserved keyword, so
    // the annotation is spelled @workgroup, matching the SPIR-V storage class.)
    if (v.annotation |> find_arg("workgroup") is tBool) {
        if (stage != ShaderStage.Compute) {
            errors |> push("shared variable '{vname}': @workgroup memory is only valid in a compute shader")
            return
        }
        if (!is_emittable_value_type(v._type)) {
            errors |> push("shared variable '{vname}': @workgroup type must be a scalar, vector, matrix, or fixed array of those")
            return
        }
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, SpvStorageClass.Workgroup, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Workgroup))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Workgroup, value_type = vt, is_ssbo = false))
        return
    }
    // task -> mesh payload: @task_payload struct -> a TaskPayloadWorkgroupEXT OpVariable (a logical
    // storage class, plain struct, no Block / Offset). The TASK shader writes it and EmitMeshTasksEXT
    // carries its id to the dispatched mesh workgroups; the MESH shader declares the same struct and reads
    // it. Member access reuses the is_block path (OpAccessChain by field index in the payload's storage).
    // Mesh + task stages only; one payload per shader (the last @task_payload wins ctx.task_payload_id,
    // which is fine -- a shader declares exactly one).
    if (v.annotation |> find_arg("task_payload") is tBool) {
        if (stage != ShaderStage.Task && stage != ShaderStage.Mesh) {
            errors |> push("@task_payload '{vname}' is only valid in a task or mesh shader")
            return
        }
        if (v._type.baseType != Type.tStructure || v._type.structType == null) {
            errors |> push("@task_payload '{vname}' must be a struct")
            return
        }
        let st = build_plain_struct(m, vname, v._type.structType, errors)
        if (st == 0u) return
        let pt = type_pointer(m, SpvStorageClass.TaskPayloadWorkgroupEXT, st)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.TaskPayloadWorkgroupEXT))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.TaskPayloadWorkgroupEXT, value_type = st, is_block = true))
        ctx.interface_ids |> push(vid)
        ctx.task_payload_id = vid
        return
    }
    // module-scope constant: an immutable `let X = <const-expr>` referenced by the shader body. Fold the
    // initializer to an OpConstant and resolve references straight to it (no OpVariable / OpLoad), so a
    // shader can hoist view rectangles, iteration counts, etc. to module scope and share them with the
    // host. Gated on a const-qualified type (a `let`, not a `var`) AND a const-foldable initializer;
    // a mutable `var` global, or a non-const initializer, falls through to the error below.
    if (v.init != null && v._type != null && v._type.flags.constant) {
        var probe : array<string>
        let cc = emit_const(m, v.init, probe)
        delete probe
        if (cc.ok) {
            ctx.globals |> insert(intptr(v), GlobalInfo(value_type = emit_type(m, v._type), is_constant = true, const_id = cc.id))
            return
        }
    }
    errors |> push("unsupported global '{vname}' (expected gl_* builtin, @ssbo, @uniform, @push_constant, @workgroup, a constant `let`, or an opaque type e.g. sampler2D / image2D / subpassInput)")
}

// ===== member index of a struct field by name =====
// ExprField.fieldIndex is still -1 at the annotation's patch (pre-fold) stage, so resolve the
// member position by name from the (fully-known) struct type instead.
def private field_index_by_name(st : Structure?; name : string) : int {
    if (st == null) return -1
    var i = 0
    for (fld in st.fields) {
        if ("{fld.name}" == name) return i
        i++
    }
    return -1
}

// ===== emit a global ExprVar's Variable (or null) =====
def private global_var_of(e : ExpressionPtr) : Variable const? {
    let ev = e ?as ExprVar
    if (ev == null || ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block) return null
    return ev.variable
}

// ===== root global of an lvalue chain (for an atomic target's memory scope) =====
// Peels ExprAt/ExprField/ExprSwizzle/ExprRef2Value down to the underlying ExprVar and returns its
// global Variable (or null if the chain doesn't root in a global). Lets the atomic handler read the
// target's storage class to pick Workgroup vs Device memory scope.
def private root_global_of(e : ExpressionPtr) : Variable const? {
    if (e == null) return null
    let ev = e ?as ExprVar
    if (ev != null) {
        if (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block) return null
        return ev.variable
    }
    let at = e ?as ExprAt
    if (at != null) return root_global_of(at.subexpr)
    let fld = e ?as ExprField
    if (fld != null) return root_global_of(fld.value)
    let sw = e ?as ExprSwizzle
    if (sw != null) return root_global_of(sw.value)
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return root_global_of(r2v.subexpr)
    return null
}

// ===== atomic call name + operand class -> SPIR-V opcode =====
// cls: 0 = signed int, 1 = unsigned int. atomicMin/Max pick the signed (SMin/SMax) or unsigned
// (UMin/UMax) opcode by the operand's signedness; the rest are sign-agnostic. compswap flags the
// OpAtomicCompareExchange shape (an extra comparator operand + a second semantics id).
def private atomic_info(name : string; cls : int) : tuple<ok : bool; op : SpvOp; compswap : bool> {
    if (name == "atomicAdd") return (ok = true, op = SpvOp.AtomicIAdd, compswap = false)
    if (name == "atomicMin") return (ok = true, op = cls == 1 ? SpvOp.AtomicUMin : SpvOp.AtomicSMin, compswap = false)
    if (name == "atomicMax") return (ok = true, op = cls == 1 ? SpvOp.AtomicUMax : SpvOp.AtomicSMax, compswap = false)
    if (name == "atomicAnd") return (ok = true, op = SpvOp.AtomicAnd, compswap = false)
    if (name == "atomicOr") return (ok = true, op = SpvOp.AtomicOr, compswap = false)
    if (name == "atomicXor") return (ok = true, op = SpvOp.AtomicXor, compswap = false)
    if (name == "atomicExchange") return (ok = true, op = SpvOp.AtomicExchange, compswap = false)
    if (name == "atomicCompSwap") return (ok = true, op = SpvOp.AtomicCompareExchange, compswap = true)
    return (ok = false, op = SpvOp.AtomicIAdd, compswap = false)
}

// ===== arithmetic opcode selection =====
// 0 = signed int, 1 = unsigned int, 2 = float, -1 = other. Vectors classify by their lane kind
// (IAdd/FAdd/… operate component-wise on vectors with the same opcode as the scalar).
def private scalar_class(bt : Type) : int {
    if (bt == Type.tInt || bt == Type.tInt2 || bt == Type.tInt3 || bt == Type.tInt4) return 0
    if (bt == Type.tUInt || bt == Type.tUInt2 || bt == Type.tUInt3 || bt == Type.tUInt4) return 1
    if (bt == Type.tFloat || bt == Type.tFloat2 || bt == Type.tFloat3 || bt == Type.tFloat4) return 2
    return -1
}

// A scalar type-name used as a conversion call (float(x)/int(x)/uint(x)). Returns the destination
// scalar class (0 signed / 1 unsigned / 2 float) or -1 if `name` is not a scalar type name. The
// vecN constructor names (float2, ...) are handled separately by vector_ctor — they return -1 here.
def private scalar_type_name_class(name : string) : int {
    if (name == "int") return 0
    if (name == "uint") return 1
    if (name == "float") return 2
    return -1
}

// SPIR-V opcode for a scalar conversion from src class to dst class (caller handles same-class no-op).
// int<->uint is a same-width bit reinterpret (OpBitcast — daslang two's-complement cast semantics);
// float<->int/uint are value conversions.
def private convert_op(src_cls, dst_cls : int) : tuple<ok : bool; code : SpvOp> {
    if (dst_cls == 2) {                                             // -> float
        if (src_cls == 0) return (ok = true, code = SpvOp.ConvertSToF)
        if (src_cls == 1) return (ok = true, code = SpvOp.ConvertUToF)
    } elif (dst_cls == 0) {                                         // -> signed int
        if (src_cls == 2) return (ok = true, code = SpvOp.ConvertFToS)
        if (src_cls == 1) return (ok = true, code = SpvOp.Bitcast)
    } elif (dst_cls == 1) {                                         // -> unsigned int
        if (src_cls == 2) return (ok = true, code = SpvOp.ConvertFToU)
        if (src_cls == 0) return (ok = true, code = SpvOp.Bitcast)
    }
    return (ok = false, code = SpvOp.Nop)
}

// daslang binary op -> SPIR-V opcode. Add/Sub/Mul are two's-complement sign-agnostic (IAdd for
// both int kinds). Div/Rem split by kind. daslang `/` truncates toward zero and `%` takes the
// dividend's sign (probed) -> SDiv/SRem (not the floored SMod); unsigned -> UDiv/UMod; float ->
// FDiv/FRem (fmod, dividend's sign).
def private binop_code(op : string; cls : int) : tuple<ok : bool; code : SpvOp> {
    if (cls < 0) return (ok = false, code = SpvOp.Nop)   // unsupported scalar class: don't masquerade as int
    if (op == "+") return (ok = true, code = cls == 2 ? SpvOp.FAdd : SpvOp.IAdd)
    if (op == "-") return (ok = true, code = cls == 2 ? SpvOp.FSub : SpvOp.ISub)
    if (op == "*") return (ok = true, code = cls == 2 ? SpvOp.FMul : SpvOp.IMul)
    if (op == "/") {
        if (cls == 2) return (ok = true, code = SpvOp.FDiv)
        if (cls == 0) return (ok = true, code = SpvOp.SDiv)
        if (cls == 1) return (ok = true, code = SpvOp.UDiv)
    }
    if (op == "%") {
        if (cls == 2) return (ok = true, code = SpvOp.FRem)
        if (cls == 0) return (ok = true, code = SpvOp.SRem)
        if (cls == 1) return (ok = true, code = SpvOp.UMod)
    }
    return (ok = false, code = SpvOp.Nop)
}

// daslang unary op -> SPIR-V opcode. `-` negates (FNegate/SNegate); `~` is bitwise Not (int/uint).
def private unop_code(op : string; cls : int) : tuple<ok : bool; code : SpvOp> {
    if (cls < 0) return (ok = false, code = SpvOp.Nop)   // unsupported scalar class
    if (op == "-") return (ok = true, code = cls == 2 ? SpvOp.FNegate : SpvOp.SNegate)
    if (op == "~" && (cls == 0 || cls == 1)) return (ok = true, code = SpvOp.Not)   // bitwise: int/uint only
    return (ok = false, code = SpvOp.Nop)
}

// daslang comparison op -> SPIR-V opcode. Result is always bool. Equality (==/!=) is two's-complement
// sign-agnostic on integers (IEqual/INotEqual). Ordering splits signed/unsigned/float. `F*` uses the
// ordered (non-NaN) forms — matching daslang's total order on finite shader values.
def private cmp_code(op : string; cls : int) : tuple<ok : bool; code : SpvOp> {
    let is_int = cls == 0 || cls == 1
    if (op == "==") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdEqual)
        if (is_int) return (ok = true, code = SpvOp.IEqual)
    }
    if (op == "!=") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdNotEqual)
        if (is_int) return (ok = true, code = SpvOp.INotEqual)
    }
    if (op == "<") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdLessThan)
        if (cls == 0) return (ok = true, code = SpvOp.SLessThan)
        if (cls == 1) return (ok = true, code = SpvOp.ULessThan)
    }
    if (op == ">") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdGreaterThan)
        if (cls == 0) return (ok = true, code = SpvOp.SGreaterThan)
        if (cls == 1) return (ok = true, code = SpvOp.UGreaterThan)
    }
    if (op == "<=") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdLessThanEqual)
        if (cls == 0) return (ok = true, code = SpvOp.SLessThanEqual)
        if (cls == 1) return (ok = true, code = SpvOp.ULessThanEqual)
    }
    if (op == ">=") {
        if (cls == 2) return (ok = true, code = SpvOp.FOrdGreaterThanEqual)
        if (cls == 0) return (ok = true, code = SpvOp.SGreaterThanEqual)
        if (cls == 1) return (ok = true, code = SpvOp.UGreaterThanEqual)
    }
    return (ok = false, code = SpvOp.Nop)
}

// number of scalar lanes in a type (scalar = 1, vecN = N, anything else = 0/unknown).
def private component_count(bt : Type) : int {
    if (bt == Type.tInt || bt == Type.tUInt || bt == Type.tFloat || bt == Type.tBool) return 1
    if (bt == Type.tInt2 || bt == Type.tUInt2 || bt == Type.tFloat2) return 2
    if (bt == Type.tInt3 || bt == Type.tUInt3 || bt == Type.tFloat3) return 3
    if (bt == Type.tInt4 || bt == Type.tUInt4 || bt == Type.tFloat4) return 4
    return 0
}

// vector constructor name (float2/3/4, int2/3/4, uint2/3/4) -> lane count + component class
// (0 signed, 1 unsigned, 2 float), else ok=false. The class lets the constructor reject
// component-kind conversions (e.g. int4(float4(...))), which need conversion ops we don't emit yet.
def private vector_ctor(name : string) : tuple<ok : bool; n : int; cls : int> {
    if (name == "float2") return (ok = true, n = 2, cls = 2)
    if (name == "float3") return (ok = true, n = 3, cls = 2)
    if (name == "float4") return (ok = true, n = 4, cls = 2)
    if (name == "int2") return (ok = true, n = 2, cls = 0)
    if (name == "int3") return (ok = true, n = 3, cls = 0)
    if (name == "int4") return (ok = true, n = 4, cls = 0)
    if (name == "uint2") return (ok = true, n = 2, cls = 1)
    if (name == "uint3") return (ok = true, n = 3, cls = 1)
    if (name == "uint4") return (ok = true, n = 4, cls = 1)
    return (ok = false, n = 0, cls = -1)
}

// OpExtInstImport "GLSL.std.450" once per module; returns the cached set id.
def private ensure_glsl_ext(var m : SpirvModule; var ctx : EmitCtx) : uint {
    if (ctx.glsl_ext != 0u) return ctx.glsl_ext
    let id = alloc_id(m)
    var ops <- [id]
    var nm <- string_words("GLSL.std.450")
    ops |> push_from(nm)
    delete nm
    emit_n(m, SEC_EXTIMPORT, SpvOp.ExtInstImport, ops)
    delete ops
    ctx.glsl_ext = id
    return id
}

// OpImageQuerySizeLod (textureSize) requires the ImageQuery capability. Emit it once into SEC_CAPS
// (which concatenates ahead of the body), deduped via the ctx flag.
def private ensure_image_query(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.image_query_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.ImageQuery))
    ctx.image_query_cap = true
}

// Subpass-input attachments (OpTypeImage Dim=SubpassData) require the InputAttachment capability.
// Same emit-once + ctx-flag dedupe pattern as ensure_image_query.
def private ensure_input_attachment(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.input_attachment_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.InputAttachment))
    ctx.input_attachment_cap = true
}

// gl_DrawID -> BuiltIn DrawIndex requires the DrawParameters capability (in SPIR-V core at 1.3, no
// extension needed at our target). Emit-once + ctx-flag dedupe.
def private ensure_draw_parameters(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.draw_parameters_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.DrawParameters))
    ctx.draw_parameters_cap = true
}

// Descriptor indexing (Vulkan VK_EXT_descriptor_indexing): nonuniformEXT() emits the NonUniform
// decoration, and descriptor arrays index OpAccessChain into a runtime-sized resource pool. Both
// flavours require the ShaderNonUniform capability and the SPV_EXT_descriptor_indexing extension at
// SPIR-V <= 1.4 (in core at 1.5). Emit-once + ctx-flag dedupe; OpExtension goes to SEC_EXT (which
// concatenates between SEC_CAPS and the body).
def private ensure_descriptor_indexing(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.descriptor_indexing_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.ShaderNonUniform))
    var nm <- string_words("SPV_EXT_descriptor_indexing")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.descriptor_indexing_cap = true
}

// A NonUniform-indexed sampler descriptor array additionally requires the per-resource-type
// capability. SampledImageArrayNonUniformIndexing covers combined image samplers (the sampler2D[N]
// surface PR-C ships). Other descriptor-array flavours (storage images, SSBOs, UBOs, input
// attachments) carry their own per-resource caps -- they will land alongside their array surface.
def private ensure_sampled_image_array_nonuniform(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.sampled_image_nu_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.SampledImageArrayNonUniformIndexing))
    ctx.sampled_image_nu_cap = true
}

// Mesh / task stages (VK_EXT_mesh_shader): OpCapability MeshShadingEXT + OpExtension
// "SPV_EXT_mesh_shader". Emit-once + ctx-flag dedupe; the cap also gates SetMeshOutputsEXT /
// EmitMeshTasksEXT and the MeshEXT / TaskEXT execution models below.
def private ensure_mesh_shading(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.mesh_shading_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.MeshShadingEXT))
    var nm <- string_words("SPV_EXT_mesh_shader")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.mesh_shading_cap = true
}

// Emitting a PerPrimitiveEXT decoration requires the MeshShadingEXT capability + SPV_EXT_mesh_shader,
// which require SPIR-V >= 1.4. The mesh stage already enables both, but a FRAGMENT reading a
// @per_primitive @in (or gl_PrimitiveID) does not — so every PerPrimitiveEXT site routes through here.
def private ensure_per_primitive(var m : SpirvModule; var ctx : EmitCtx) {
    ensure_mesh_shading(m, ctx)
    m.version = SPV_VERSION_1_4
}

// daslang math builtin -> GLSL.std.450 ext-inst opcode. abs/min/max/clamp/sign pick the F/S/U
// variant by operand class; the rest are float-only. Operand class comes from arg 0 (vectors
// classify by lane kind, so length/normalize/cross/etc. read as float-class). ok=false => not a
// GLSL.std.450 builtin (or wrong class) -> caller emits a clean error. NOTE: keyed on the DASLANG name,
// which differs from the GLSL spelling for two: daslang `lerp` -> FMix, daslang `rsqrt` -> InverseSqrt.
// Only dependency-free builtin `math` functions appear here (no dead branches). Excluded on purpose:
// daslang has no mix.
def private glsl_ext_op(name : string; cls : int) : tuple<ok : bool; op : GLSLstd450> {
    if (name == "abs") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FAbs)
        if (cls == 0) return (ok = true, op = GLSLstd450.SAbs)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "min") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FMin)
        if (cls == 0) return (ok = true, op = GLSLstd450.SMin)
        if (cls == 1) return (ok = true, op = GLSLstd450.UMin)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "max") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FMax)
        if (cls == 0) return (ok = true, op = GLSLstd450.SMax)
        if (cls == 1) return (ok = true, op = GLSLstd450.UMax)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "clamp") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FClamp)
        if (cls == 0) return (ok = true, op = GLSLstd450.SClamp)
        if (cls == 1) return (ok = true, op = GLSLstd450.UClamp)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (name == "sign") {
        if (cls == 2) return (ok = true, op = GLSLstd450.FSign)
        if (cls == 0) return (ok = true, op = GLSLstd450.SSign)
        return (ok = false, op = GLSLstd450.Round)
    }
    if (cls != 2) return (ok = false, op = GLSLstd450.Round)   // the rest are float-only
    if (name == "sqrt") return (ok = true, op = GLSLstd450.Sqrt)
    if (name == "rsqrt") return (ok = true, op = GLSLstd450.InverseSqrt)   // daslang rsqrt == GLSL inversesqrt
    if (name == "sin") return (ok = true, op = GLSLstd450.Sin)
    if (name == "cos") return (ok = true, op = GLSLstd450.Cos)
    if (name == "tan") return (ok = true, op = GLSLstd450.Tan)
    if (name == "asin") return (ok = true, op = GLSLstd450.Asin)
    if (name == "acos") return (ok = true, op = GLSLstd450.Acos)
    if (name == "atan") return (ok = true, op = GLSLstd450.Atan)
    if (name == "atan2") return (ok = true, op = GLSLstd450.Atan2)
    if (name == "sinh") return (ok = true, op = GLSLstd450.Sinh)
    if (name == "cosh") return (ok = true, op = GLSLstd450.Cosh)
    if (name == "tanh") return (ok = true, op = GLSLstd450.Tanh)
    if (name == "asinh") return (ok = true, op = GLSLstd450.Asinh)
    if (name == "acosh") return (ok = true, op = GLSLstd450.Acosh)
    if (name == "atanh") return (ok = true, op = GLSLstd450.Atanh)
    if (name == "mad") return (ok = true, op = GLSLstd450.Fma)
    if (name == "radians") return (ok = true, op = GLSLstd450.Radians)
    if (name == "degrees") return (ok = true, op = GLSLstd450.Degrees)
    if (name == "pow") return (ok = true, op = GLSLstd450.Pow)
    if (name == "exp") return (ok = true, op = GLSLstd450.Exp)
    if (name == "log") return (ok = true, op = GLSLstd450.Log)
    if (name == "exp2") return (ok = true, op = GLSLstd450.Exp2)
    if (name == "log2") return (ok = true, op = GLSLstd450.Log2)
    if (name == "floor") return (ok = true, op = GLSLstd450.Floor)
    if (name == "ceil") return (ok = true, op = GLSLstd450.Ceil)
    if (name == "round") return (ok = true, op = GLSLstd450.Round)
    if (name == "trunc") return (ok = true, op = GLSLstd450.Trunc)
    if (name == "fract") return (ok = true, op = GLSLstd450.Fract)
    if (name == "step") return (ok = true, op = GLSLstd450.Step)
    if (name == "smoothstep") return (ok = true, op = GLSLstd450.SmoothStep)
    if (name == "lerp") return (ok = true, op = GLSLstd450.FMix)            // daslang lerp == GLSL mix
    if (name == "length") return (ok = true, op = GLSLstd450.Length)
    if (name == "distance") return (ok = true, op = GLSLstd450.Distance)
    if (name == "cross") return (ok = true, op = GLSLstd450.Cross)
    if (name == "normalize") return (ok = true, op = GLSLstd450.Normalize)
    if (name == "reflect") return (ok = true, op = GLSLstd450.Reflect)
    if (name == "refract") return (ok = true, op = GLSLstd450.Refract)
    return (ok = false, op = GLSLstd450.Round)
}

//! GLSL.std.450 ops where a scalar operand is allowed against a vector result and the SPIR-V spec
//! requires it to match the result type — mix/clamp/min/max/step/smoothstep. SPIR-V has no implicit
//! scalar broadcast, so the emitter must splat such a scalar to the result width (e.g. daslang
//! lerp(vec, vec, float) -> FMix wants a vector t). Ops NOT here (pow, atan2, distance, …) require
//! their operands to match exactly and must not be touched.
def private ext_op_broadcasts_scalar(op : GLSLstd450) : bool {
    return (op == GLSLstd450.FMix
        || op == GLSLstd450.FClamp || op == GLSLstd450.SClamp || op == GLSLstd450.UClamp
        || op == GLSLstd450.FMin   || op == GLSLstd450.SMin   || op == GLSLstd450.UMin
        || op == GLSLstd450.FMax   || op == GLSLstd450.SMax   || op == GLSLstd450.UMax
        || op == GLSLstd450.Step   || op == GLSLstd450.SmoothStep)
}

// ===== compile-time constants: scalars, all-const vector constructors, const arrays =====
// Produces a deduped OpConstant / OpConstantComposite for a fully-constant expression. Used for
// array literals (which must be constant initializers for now) and their constituents. ok=false =>
// not a compile-time constant (the caller emits a clean error).
def private emit_const(var m : SpirvModule; e : ExpressionPtr; var errors : array<string>) : tuple<ok : bool; id : uint> {
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return emit_const(m, r2v.subexpr, errors)
    let cu = e ?as ExprConstUInt
    if (cu != null) return (ok = true, id = const_uint(m, cu.value))
    let ci = e ?as ExprConstInt
    if (ci != null) return (ok = true, id = const_int(m, ci.value))
    let cf = e ?as ExprConstFloat
    if (cf != null) return (ok = true, id = const_float(m, cf.value))
    let cb = e ?as ExprConstBool
    if (cb != null) return (ok = true, id = const_bool(m, cb.value))
    // unary minus on a constant: depending on fold state `-0.6` can arrive as ExprOp1("-", const)
    // rather than a folded negative literal. Fold it here so both shapes produce the same constant.
    let neg = e ?as ExprOp1
    if (neg != null && "{neg.op}" == "-") {
        let inner = neg.subexpr ?as ExprRef2Value
        let sub = inner != null ? inner.subexpr : neg.subexpr
        let ncf = sub ?as ExprConstFloat
        if (ncf != null) return (ok = true, id = const_float(m, -ncf.value))
        let nci = sub ?as ExprConstInt
        if (nci != null) return (ok = true, id = const_int(m, -nci.value))
        return (ok = false, id = 0u)
    }
    // folded vector constants (float2(0.,−.6) etc. fold to ExprConstFloatN at patch time) ->
    // OpConstantComposite of per-lane scalar constants.
    let cf2 = e ?as ExprConstFloat2
    if (cf2 != null) {
        let v = cf2.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_float(m, v.x), const_float(m, v.y)]))
    }
    let cf3 = e ?as ExprConstFloat3
    if (cf3 != null) {
        let v = cf3.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_float(m, v.x), const_float(m, v.y), const_float(m, v.z)]))
    }
    let cf4 = e ?as ExprConstFloat4
    if (cf4 != null) {
        let v = cf4.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_float(m, v.x), const_float(m, v.y), const_float(m, v.z), const_float(m, v.w)]))
    }
    let ci2 = e ?as ExprConstInt2
    if (ci2 != null) {
        let v = ci2.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_int(m, v.x), const_int(m, v.y)]))
    }
    let ci3 = e ?as ExprConstInt3
    if (ci3 != null) {
        let v = ci3.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_int(m, v.x), const_int(m, v.y), const_int(m, v.z)]))
    }
    let ci4 = e ?as ExprConstInt4
    if (ci4 != null) {
        let v = ci4.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_int(m, v.x), const_int(m, v.y), const_int(m, v.z), const_int(m, v.w)]))
    }
    let cu2 = e ?as ExprConstUInt2
    if (cu2 != null) {
        let v = cu2.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_uint(m, v.x), const_uint(m, v.y)]))
    }
    let cu3 = e ?as ExprConstUInt3
    if (cu3 != null) {
        let v = cu3.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_uint(m, v.x), const_uint(m, v.y), const_uint(m, v.z)]))
    }
    let cu4 = e ?as ExprConstUInt4
    if (cu4 != null) {
        let v = cu4.value
        return (ok = true, id = const_composite(m, emit_type(m, e._type), [const_uint(m, v.x), const_uint(m, v.y), const_uint(m, v.z), const_uint(m, v.w)]))
    }
    // all-constant vector constructor -> OpConstantComposite
    let call = e ?as ExprCall
    if (call != null) {
        let vc = vector_ctor("{call.name}")
        if (!vc.ok) return (ok = false, id = 0u)
        var total = 0
        var parts : array<uint>
        parts |> reserve(length(call.arguments))
        for (a in call.arguments) {
            let cc = component_count(a._type.baseType)
            if (cc == 0 || scalar_class(a._type.baseType) != vc.cls) {
                errors |> push("{call.name}: constant constructor constituent of type {a._type.baseType} has the wrong component kind")
                delete parts
                return (ok = false, id = 0u)
            }
            total += cc
            let pr = emit_const(m, a, errors)
            if (!pr.ok) {
                errors |> push("{call.name}: constructor argument is not a compile-time constant")
                delete parts
                return (ok = false, id = 0u)
            }
            parts |> push(pr.id)
        }
        if (total != vc.n) {
            errors |> push("{call.name}: constant constituent lanes sum to {total}, expected {vc.n}")
            delete parts
            return (ok = false, id = 0u)
        }
        let cid = const_composite(m, emit_type(m, e._type), parts)
        delete parts
        return (ok = true, id = cid)
    }
    // const array literal -> OpConstantComposite of element constants
    let mk = e ?as ExprMakeArray
    if (mk != null) {
        // a dynamic array<T> literal is ALSO an ExprMakeArray; emit_type would panic on tArray.
        // Only fixed-array literals lower to a constant composite — reject the rest cleanly.
        if (e._type == null || e._type.baseType != Type.tFixedArray) {
            errors |> push("only fixed-array literals are supported (dynamic array<T> literals are not)")
            return (ok = false, id = 0u)
        }
        var parts : array<uint>
        parts |> reserve(length(mk.values))
        for (v in mk.values) {
            let pr = emit_const(m, v, errors)
            if (!pr.ok) {
                errors |> push("array literal element is not a compile-time constant")
                delete parts
                return (ok = false, id = 0u)
            }
            parts |> push(pr.id)
        }
        let cid = const_composite(m, emit_type(m, e._type), parts)
        delete parts
        return (ok = true, id = cid)
    }
    return (ok = false, id = 0u)
}

// Splat a scalar id to a vector of `n` lanes (OpCompositeConstruct of n copies of the scalar) ->
// the splatted vector id. SPIR-V has no implicit scalar->vector broadcast, so an integer vec*scalar
// must build the vector explicitly before the component-wise OpIMul.
def private splat_to_vec(var m : SpirvModule; vec_type : uint; scalar_id : uint; n : int) : uint {
    let res = alloc_id(m)
    var cons <- [vec_type, res]
    cons |> reserve(2 + n)
    for (_k in range(n)) {
        cons |> push(scalar_id)
    }
    emit_n(m, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
    delete cons
    return res
}

// ===== `*` over matrices / vectors / scalars =====
// SPIR-V splits multiplication by operand shape: OpMatrixTimesMatrix / OpMatrixTimesVector /
// OpVectorTimesScalar. Plain component-wise FMul/IMul (the scalar binop path) only works when both
// operands have the SAME shape, so the mixed cases are routed here first. Returns handled=false for
// same-shape operands (let binop_code emit FMul/IMul); handled=true with id=0 on an unsupported mix
// (error already pushed). Operands l/r are pre-emitted result-ids. Handled: matrix*matrix,
// matrix*vector, float vector*scalar (OpVectorTimesScalar, either order), and integer vector*scalar
// (splat + OpIMul, either order). daslang has no matrix*scalar or vector*matrix operator, so those
// SPIR-V ops are intentionally absent.
def private emit_mul(var m : SpirvModule; op2 : ExprOp2?; e : ExpressionPtr; l, r : uint; var errors : array<string>) : tuple<handled : bool; id : uint> {
    let lmat = matrix_info(op2.left._type)
    let rmat = matrix_info(op2.right._type)
    let lcc = component_count(op2.left._type.baseType)
    let rcc = component_count(op2.right._type.baseType)
    let lcls = scalar_class(op2.left._type.baseType)
    let rcls = scalar_class(op2.right._type.baseType)
    // same-shape (scalar*scalar or equal-width vector*vector): component-wise, defer to binop_code
    if (!lmat.ok && !rmat.ok && lcc == rcc) return (handled = false, id = 0u)
    let rt = emit_type(m, e._type)
    let res = alloc_id(m)
    if (lmat.ok && rmat.ok) {
        emit(m, SEC_FUNCS, SpvOp.MatrixTimesMatrix, rt, res, l, r)
    } elif (lmat.ok && rcc > 1) {
        emit(m, SEC_FUNCS, SpvOp.MatrixTimesVector, rt, res, l, r)
    } elif (lcc > 1 && rcc == 1 && lcls == 2 && rcls == 2) {
        emit(m, SEC_FUNCS, SpvOp.VectorTimesScalar, rt, res, l, r)
    } elif (rcc > 1 && lcc == 1 && lcls == 2 && rcls == 2) {
        emit(m, SEC_FUNCS, SpvOp.VectorTimesScalar, rt, res, r, l)   // SPIR-V wants the vector first
    } elif (lcc > 1 && rcc == 1 && lcls == rcls && lcls >= 0 && lcls != 2) {
        // integer vector * integer scalar: there is no OpVectorTimesScalar for ints, so splat the
        // scalar to the vec width and do a component-wise OpIMul (sign-agnostic, valid for int + uint).
        emit(m, SEC_FUNCS, SpvOp.IMul, rt, res, l, splat_to_vec(m, rt, r, lcc))
    } elif (rcc > 1 && lcc == 1 && lcls == rcls && lcls >= 0 && lcls != 2) {
        emit(m, SEC_FUNCS, SpvOp.IMul, rt, res, splat_to_vec(m, rt, l, rcc), r)
    } else {
        errors |> push("unsupported '*' between {op2.left._type.baseType} and {op2.right._type.baseType}")
        return (handled = true, id = 0u)
    }
    return (handled = true, id = res)
}

// ===== local variable declaration (Function-storage OpVariable) =====
// SPIR-V requires every Function-storage OpVariable to be among the first instructions of the
// function's first block. So we pre-scan the whole body (one walk, all scopes — SPIR-V locals are
// function-scoped, not block-scoped) BEFORE emitting any body code, declaring an OpVariable for each
// mutable local (`var`) and loop-induction variable. Reads/writes then resolve to OpLoad/OpStore.
def private declare_local_var(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; var errors : array<string>) {
    if (key_exists(ctx.local_vars, intptr(v))) return
    let bt = v._type.baseType
    if (scalar_class(bt) < 0 && bt != Type.tBool && bt != Type.tFixedArray) {
        // emit_type would panic on an unsupported base type; reject with a clean error instead.
        errors |> push("local '{v.name}': type {bt} is not supported yet (only 32-bit int/uint/float scalars+vectors, bool, and fixed arrays)")
        return
    }
    let vt = emit_type(m, v._type)
    let pt = type_pointer(m, SpvStorageClass.Function, vt)
    let pid = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Variable, pt, pid, uint(SpvStorageClass.Function))
    ctx.local_vars |> insert(intptr(v), LocalVar(ptr_id = pid, value_type = vt))
}

def private collect_locals(var m : SpirvModule; var ctx : EmitCtx; e : ExpressionPtr; var errors : array<string>) {
    if (e == null) return
    let blk = e ?as ExprBlock
    if (blk != null) {
        for (s in blk.list) {
            collect_locals(m, ctx, s, errors)
        }
        return
    }
    let lt = e ?as ExprLet
    if (lt != null) {
        for (v in lt.variables) {
            // `var` -> memory; scalar/vector `let` -> SSA; but a fixed-array `let` needs memory too
            // (dynamic indexing requires an addressable OpVariable + OpAccessChain).
            if (!v._type.flags.constant || v._type.baseType == Type.tFixedArray) {
                declare_local_var(m, ctx, v, errors)
            }
        }
        return
    }
    let ite = e ?as ExprIfThenElse
    if (ite != null) {
        collect_locals(m, ctx, ite.if_true, errors)
        collect_locals(m, ctx, ite.if_false, errors)
        return
    }
    let wh = e ?as ExprWhile
    if (wh != null) {
        collect_locals(m, ctx, wh.body, errors)
        return
    }
    let fr = e ?as ExprFor
    if (fr != null) {
        for (fv in fr.iteratorVariables) {
            declare_local_var(m, ctx, fv, errors)
        }
        collect_locals(m, ctx, fr.body, errors)
        return
    }
}

// ===== compound assignment + increment/decrement =====
def private compound_base(op : string) : string {
    if (op == "+=") return "+"
    if (op == "-=") return "-"
    if (op == "*=") return "*"
    if (op == "/=") return "/"
    if (op == "%=") return "%"
    return ""
}

def private const_one(var m : SpirvModule; cls : int) : uint {
    if (cls == 2) return const_float(m, 1.0f)
    if (cls == 1) return const_uint(m, 1u)
    return const_int(m, 1)
}

def private const_zero(var m : SpirvModule; cls : int) : uint {
    if (cls == 2) return const_float(m, 0.0f)
    if (cls == 1) return const_uint(m, 0u)
    return const_int(m, 0)
}

// An integer-valued constant for a loop bound (range bounds are always int or uint, never float).
def private const_int_for(var m : SpirvModule; cls : int; v : int) : uint {
    if (cls == 1) return const_uint(m, uint(v))
    return const_int(m, v)
}

// ===== SpirvEmit : AstVisitor =====
// The body-codegen visitor (modeled on llvm_jit). generate_spirv builds the module scaffold (caps /
// entry-point / OpFunction / trailing return) and drives this visitor over fn.body for all codegen.
// Value threading is llvm_jit's side-map model: each node records its result keyed by intptr — e2id for
// an rvalue SSA id, e2ptr/e2pty for an lvalue pointer + pointee-type. Parents read children back out.
// ExprRef2Value is the load marker; bare lvalue nodes stay pointers and value_of() loads on demand
// (matching the hand-walker's `_type.flags.ref` branch). The patch-stage AST arrives const; each hook
// takes its node as a `var` param (`def override visitExprX(var expr : ExprX?)`), which binds a non-const
// view directly — no reinterpret const-strip, and `return expr` type-checks. We only read the AST and emit
// into the side module (never mutate it), so the body reuses the existing non-const emit helpers verbatim.
class SpirvEmit : AstVisitor {
    adapter : VisitorAdapter?
    @do_not_delete m   : SpirvModule?    // borrowed (addr of generate_spirv's module) — not owned
    @do_not_delete ctx : EmitCtx?        // borrowed (addr of generate_spirv's ctx) — not owned
    e2id    : table<uint64; uint>        // intptr(Expression) -> rvalue SSA result-id
    e2ptr   : table<uint64; uint>        // intptr(Expression) -> lvalue pointer result-id
    e2pty   : table<uint64; uint>        // intptr(Expression) -> pointee type-id (paired with e2ptr)
    ite_ids : table<uint64; IteIds>      // intptr(ExprIfThenElse) -> its {merge, then, else} labels
    loop_ids : table<uint64; LoopIds>    // intptr(ExprWhile/ExprFor) -> its 5 loop-skeleton labels
    for_src : table<uint64>              // for-source range() call ptrs to intercept in visitExprCall (6.3)
    errs    : array<string>

    // coerce a visited child to an rvalue id: a value passes through; a pointer is OpLoad'd here (the
    // bare-ref load, matching emit_value's `_type.flags.ref` branch).
    def value_of(e : ExpressionPtr) : uint {
        let k = intptr(e)
        if (key_exists(e2id, k)) return e2id?[k] ?? 0u
        if (key_exists(e2ptr, k)) {
            let pid = e2ptr?[k] ?? 0u
            let pty = e2pty?[k] ?? 0u
            if (pid == 0u || pty == 0u) {   // unresolved pointer / pointee — never OpLoad with a 0 operand
                errs |> push("internal: cannot load {e.__rtti} (unresolved pointer or pointee type)")
                return 0u
            }
            var bm & = unsafe(*m)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, pty, res, pid)
            return res
        }
        errs |> push("internal: no rvalue available for {e.__rtti}")
        return 0u
    }
    // Resolve a sampler/sampler-array argument to a loaded sampled-image SSA id. Handles BOTH
    // the scalar form `texture(my_sampler, uv)` (gi.is_sampler) AND the descriptor-array form
    // `texture(my_arr[idx], uv)` (gi.is_sampler_array): emits OpAccessChain + OpLoad and cascades
    // the NonUniform decoration when the index value carries it (PR-C bindless rail). Returns
    // `sg_type` resolved to the ELEMENT sampler type so downstream sampler_info(sg_type) gets the
    // right Dim / Arrayed / Depth -- the caller does not need to know array vs scalar.
    def resolve_sampler_arg(arg : ExpressionPtr; intrinsic_name : string) : tuple<sg : Variable const?; sg_type : TypeDecl?; simg : uint; ok : bool> {
        var bm & = unsafe(*m)
        let direct = global_var_of(arg)
        if (direct != null) {
            let gi = ctx.globals?[intptr(direct)] ?? GlobalInfo()
            if (gi.is_sampler) {
                let simg_id = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, simg_id, gi.var_id)
                return (sg = direct, sg_type = direct._type, simg = simg_id, ok = true)
            }
            if (gi.is_sampler_array) {
                errs |> push("{intrinsic_name}: '{direct.name}' is a descriptor array; index into it with [idx]")
                return (sg = null, sg_type = null, simg = 0u, ok = false)
            }
            errs |> push("{intrinsic_name}: '{direct.name}' is not a sampler")
            return (sg = null, sg_type = null, simg = 0u, ok = false)
        }
        let at = arg ?as ExprAt
        if (at == null) {
            errs |> push("{intrinsic_name}: first argument must be a sampler global or a descriptor-array element (samplers[idx])")
            return (sg = null, sg_type = null, simg = 0u, ok = false)
        }
        let arr_g = global_var_of(at.subexpr)
        if (arr_g == null) {
            errs |> push("{intrinsic_name}: descriptor-array index must target a sampler-array global")
            return (sg = null, sg_type = null, simg = 0u, ok = false)
        }
        let gi = ctx.globals?[intptr(arr_g)] ?? GlobalInfo()
        if (!gi.is_sampler_array) {
            errs |> push("{intrinsic_name}: '{arr_g.name}' is not a sampler descriptor array")
            return (sg = null, sg_type = null, simg = 0u, ok = false)
        }
        let idx = value_of(at.index)
        if (idx == 0u) return (sg = null, sg_type = null, simg = 0u, ok = false)
        let elem_ptr_t = type_pointer(bm, SpvStorageClass.UniformConstant, gi.elem_type)
        let elem_ptr = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.AccessChain, elem_ptr_t, elem_ptr, gi.var_id, idx)
        // NonUniform cascade: NonUniform on the index propagates to the access-chain pointer AND
        // the loaded sampler value. The per-resource SampledImageArrayNonUniformIndexing cap rides
        // along; the base ShaderNonUniform cap + SPV_EXT_descriptor_indexing extension already
        // arrived with the nonuniformEXT marker (PR-B).
        let idx_nu = ctx.nonuniform_ids |> key_exists(idx)
        if (idx_nu) {
            ensure_sampled_image_array_nonuniform(bm, *ctx)
            emit(bm, SEC_DECOR, SpvOp.Decorate, elem_ptr, uint(SpvDecoration.NonUniform))
            ctx.nonuniform_ids |> insert(elem_ptr)
        }
        let simg_id = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, gi.elem_type, simg_id, elem_ptr)
        if (idx_nu) {
            emit(bm, SEC_DECOR, SpvOp.Decorate, simg_id, uint(SpvDecoration.NonUniform))
            ctx.nonuniform_ids |> insert(simg_id)
        }
        return (sg = arr_g, sg_type = arr_g._type.firstType, simg = simg_id, ok = true)
    }
    def ptr_of(e : ExpressionPtr) : tuple<id : uint; pty : uint> {
        let k = intptr(e)
        if (key_exists(e2ptr, k)) return (id = e2ptr?[k] ?? 0u, pty = e2pty?[k] ?? 0u)
        // a swizzle that produced a value (not a pointer) used as a write target: a multi-component
        // swizzle (`v.xy = …`) or a single-component swizzle of a value. Fail-closed with a clear
        // message rather than the generic "internal: not an lvalue".
        if (e is ExprSwizzle) {
            errs |> push("cannot assign through a swizzle — write-swizzle (e.g. `v.xy = …`) is not supported; assign the whole vector instead")
            return (id = 0u, pty = 0u)
        }
        errs |> push("internal: {e.__rtti} is not an lvalue")
        return (id = 0u, pty = 0u)
    }

    // ----- constants -----
    def override visitExprConstInt(var expr : ExprConstInt?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_int(bm, expr.value)
        return expr
    }
    def override visitExprConstUInt(var expr : ExprConstUInt?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_uint(bm, expr.value)
        return expr
    }
    def override visitExprConstFloat(var expr : ExprConstFloat?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_float(bm, expr.value)
        return expr
    }
    def override visitExprConstBool(var expr : ExprConstBool?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_bool(bm, expr.value)
        return expr
    }

    // Folded vector constants (float2(0.5,0.5) etc. fold to ExprConstFloatN/IntN/UIntN at patch time)
    // are leaf rvalues with no scalar-style visit registration. emit_const already lowers each to an
    // OpConstantComposite; record the id so value_of finds it when the node is an operand.
    def register_vec_const(e : ExpressionPtr) {
        var bm & = unsafe(*m)
        let cr = emit_const(bm, e, errs)
        if (cr.ok) {
            e2id[intptr(e)] = cr.id
        }
    }
    def override visitExprConstFloat2(var expr : ExprConstFloat2?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstFloat3(var expr : ExprConstFloat3?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstFloat4(var expr : ExprConstFloat4?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstInt2(var expr : ExprConstInt2?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstInt3(var expr : ExprConstInt3?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstInt4(var expr : ExprConstInt4?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstUInt2(var expr : ExprConstUInt2?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstUInt3(var expr : ExprConstUInt3?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }
    def override visitExprConstUInt4(var expr : ExprConstUInt4?) : ExpressionPtr {
        register_vec_const(expr)
        return expr
    }

    // ----- variables: value-let -> SSA id; memory local / global -> pointer -----
    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        let k = intptr(expr)
        if (expr.varFlags.local || expr.varFlags.argument || expr.varFlags._block) {
            let vk = intptr(expr.variable)
            if (key_exists(ctx.local_vars, vk)) {
                let lv = ctx.local_vars?[vk] ?? LocalVar()
                e2ptr[k] = lv.ptr_id
                e2pty[k] = lv.value_type
            } else {
                let lid = ctx.locals?[vk] ?? 0u
                if (lid == 0u) {   // value-let read before its init was visited -> no SSA id
                    errs |> push("local '{expr.name}' used before it was assigned an SSA id")
                }
                e2id[k] = lid
            }
        } else {
            let gi = ctx.globals?[intptr(expr.variable)] ?? GlobalInfo()
            if (gi.is_constant) {   // module-scope constant let -> the folded OpConstant id directly (no pointer/load)
                e2id[k] = gi.const_id
            } else {
                if (gi.var_id == 0u) {   // referenced global was not classified (unsupported / not collected)
                    errs |> push("global '{expr.name}' is not classified")
                }
                e2ptr[k] = gi.var_id
                e2pty[k] = gi.value_type
            }
        }
        return expr
    }

    // ----- swizzle: single-component global -> pointer (AccessChain, an lvalue); everything else
    // (multi-component, or a swizzle of any loaded value: local, UBO/SSBO member, sampler result) ->
    // value path: load the whole source composite, then CompositeExtract (1) / VectorShuffle (many) -----
    def override visitExprSwizzle(var expr : ExprSwizzle?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let k = intptr(expr)
        let gv = global_var_of(expr.value)
        // single-component global -> AccessChain to that one component (keeps it an lvalue: g.x = …)
        if (gv != null && length(expr.fields) == 1) {
            let gi = ctx.globals?[intptr(gv)] ?? GlobalInfo()
            if (gi.var_id == 0u) {   // global not classified -> never AccessChain through a 0 base
                errs |> push("swizzle target global '{gv.name}' is not classified")
                return expr
            }
            let pointee = emit_type(bm, expr._type)
            let ptr_t = type_pointer(bm, gi.storage, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, const_uint(bm, uint(expr.fields[0])))
            e2ptr[k] = res
            e2pty[k] = pointee
            return expr
        }
        // value path. value_of loads the whole composite (a bare global loads its OpVariable -> the
        // full vector; gl_FragCoord.xy, a_pos.xyz, … all reach here now), then extract / shuffle.
        let base = value_of(expr.value)
        if (base == 0u) return expr   // operand failed to lower (error already pushed) — no 0-operand extract/shuffle
        let rt = emit_type(bm, expr._type)
        let res = alloc_id(bm)
        if (length(expr.fields) == 1) {
            emit(bm, SEC_FUNCS, SpvOp.CompositeExtract, rt, res, base, uint(expr.fields[0]))
        } else {
            var ops <- [rt, res, base, base]
            for (f in expr.fields) {
                ops |> push(uint(f))
            }
            emit_n(bm, SEC_FUNCS, SpvOp.VectorShuffle, ops)
            delete ops
        }
        e2id[k] = res
        return expr
    }

    // ----- indexing: ssbo array or local fixed array -> pointer (AccessChain) -----
    def override visitExprAt(var expr : ExprAt?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let k = intptr(expr)
        let lev = expr.subexpr ?as ExprVar
        if (lev != null && (lev.varFlags.local || lev.varFlags.argument || lev.varFlags._block)) {
            let lv = ctx.local_vars?[intptr(lev.variable)] ?? LocalVar()
            if (lv.ptr_id == 0u) {
                errs |> push("indexing local '{lev.name}': only fixed-array `var`/`let` locals are indexable")
                return expr
            }
            let idx = value_of(expr.index)
            if (idx == 0u) return expr   // index failed to lower (error already pushed) — no 0-operand access chain
            let pointee = emit_type(bm, expr._type)
            let ptr_t = type_pointer(bm, SpvStorageClass.Function, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, lv.ptr_id, idx)
            e2ptr[k] = res
            e2pty[k] = pointee
            return expr
        }
        let bv = global_var_of(expr.subexpr)
        if (bv == null) {
            // PR-G: indexing a fixed-array field inside a Block (ubo.lights[i] / ubo.material.kernel[i]).
            // The subexpr is a chained ExprField whose e2ptr is a pointer to the array. Chain a
            // single index off it; the storage class is inherited from the root global. For a
            // struct element, plain emit_type would panic (it doesn't lower tStructure) and would
            // also not return the laid-out (deduped) type — so re-acquire it via build_block_struct,
            // which is idempotent under the type/decoration dedup pools.
            if (key_exists(e2ptr, intptr(expr.subexpr))) {
                let idx = value_of(expr.index)
                if (idx == 0u) return expr   // index failed to lower (error already pushed)
                let base = ptr_of(expr.subexpr)
                let rv = root_global_of(expr.subexpr)
                let rgi = rv != null ? (ctx.globals?[intptr(rv)] ?? GlobalInfo()) : GlobalInfo()
                let storage = rgi.var_id != 0u ? rgi.storage : SpvStorageClass.StorageBuffer
                let rname = rv != null ? string(rv.name) : "(?)"
                let pointee = field_pointee_type(bm, expr._type, "{rname}[]", storage)
                if (pointee == 0u) return expr
                let ptr_t = type_pointer(bm, storage, pointee)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, base.id, idx)
                e2ptr[k] = res
                e2pty[k] = pointee
                return expr
            }
            errs |> push("indexing is only supported on a global ssbo array, a local fixed array, or an array field inside a @uniform/@push_constant/@ssbo block")
            return expr
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        // BARE array global (not wrapped in a Block struct), so the access chain is a single index off
        // the variable -- no member-0 indirection like an SSBO. Two flavours: @workgroup shared arrays
        // and the mesh per-vertex / per-primitive Output arrays (gl_MeshVerticesEXT[i] /
        // gl_PrimitiveTriangleIndicesEXT[i]). The mesh per-vertex element is a Block struct that
        // emit_type can't lower, so use the classified elem_type; @workgroup scalar/vector elements
        // lower fine via emit_type.
        if (gi.storage == SpvStorageClass.Workgroup || gi.is_mesh_output) {
            let idx = value_of(expr.index)
            if (idx == 0u) return expr   // index failed to lower (error already pushed)
            let pointee = (gi.is_mesh_output && gi.elem_type != 0u) ? gi.elem_type : emit_type(bm, expr._type)
            let res = alloc_id(bm)
            let ptr_t = type_pointer(bm, gi.storage, pointee)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, idx)
            e2ptr[k] = res
            e2pty[k] = pointee
            return expr
        }
        // descriptor array of samplers (sampler2D[N], etc.): the ExprAt is read STRUCTURALLY by the
        // texture/textureLod/textureCompare/textureSize/texelFetch handler (resolve_sampler_arg),
        // which emits the OpAccessChain + OpLoad and cascades NonUniform when warranted. Skip
        // emission here -- leaving no e2id/e2ptr entry is fine because the call-site walks the AST
        // directly; any other consumer (which doesn't exist for an opaque sampler value) would
        // surface a clean "no rvalue available" fail-closed.
        if (gi.is_sampler_array) {
            return expr
        }
        if (!gi.is_ssbo) {
            errs |> push("only ssbo, @workgroup, or sampler descriptor-array indexing is supported for now")
            return expr
        }
        let idx = value_of(expr.index)
        if (idx == 0u) return expr   // index failed to lower (error already pushed) — no 0-operand access chain
        // use the classified element type-id: a struct element can't go through emit_type
        let pointee = gi.elem_type != 0u ? gi.elem_type : emit_type(bm, expr._type)
        let res = alloc_id(bm)
        let ptr_t = type_pointer(bm, gi.storage, pointee)
        var ops <- [ptr_t, res, gi.var_id, const_uint(bm, 0u), idx]
        emit_n(bm, SEC_FUNCS, SpvOp.AccessChain, ops)
        delete ops
        e2ptr[k] = res
        e2pty[k] = pointee
        return expr
    }

    // ----- the load marker: read an lvalue child as a value -----
    def override visitExprRef2Value(var expr : ExprRef2Value?) : ExpressionPtr {
        e2id[intptr(expr)] = value_of(expr.subexpr)
        return expr
    }

    // ----- unary: !, -, ~ (value) and statement-level ++/-- (load-op-store) -----
    def override visitExprOp1(var expr : ExprOp1?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let op = "{expr.op}"
        if (op == "++" || op == "--") {
            let cls = scalar_class(expr.subexpr._type.baseType)
            let bo = binop_code(op == "++" ? "+" : "-", cls)
            if (!bo.ok) {
                errs |> push("unsupported {expr.op} on {expr.subexpr._type.baseType}")
                return expr
            }
            let one = const_one(bm, cls)
            let p = ptr_of(expr.subexpr)
            if (p.id != 0u) {
                let cur = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, p.pty, cur, p.id)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, bo.code, p.pty, res, cur, one)
                emit(bm, SEC_FUNCS, SpvOp.Store, p.id, res)
            }
            return expr
        }
        let v = value_of(expr.subexpr)
        if (v == 0u) return expr
        if (op == "!") {
            let bt = type_bool(bm)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.LogicalNot, bt, res, v)
            e2id[intptr(expr)] = res
            return expr
        }
        let uo = unop_code(op, scalar_class(expr.subexpr._type.baseType))
        if (!uo.ok) {
            errs |> push("unsupported unary op '{expr.op}' on {expr.subexpr._type.baseType}")
            return expr
        }
        let rt = emit_type(bm, expr._type)
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, uo.code, rt, res, v)
        e2id[intptr(expr)] = res
        return expr
    }

    // ----- binary: arithmetic / comparison / logical, plus matrix-* via emit_mul; void op2 = compound -----
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let op = "{expr.op}"
        if (expr._type.baseType == Type.tVoid) {        // compound assignment: dst <op>= rhs
            let rhs = value_of(expr.right)
            let p = ptr_of(expr.left)
            if (p.id != 0u && rhs != 0u) {
                let cur = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, p.pty, cur, p.id)
                var res = 0u
                // `*=` over mixed shapes (vector *= scalar, matrix *= …) needs OpVectorTimesScalar /
                // OpMatrixTimes* exactly like the `*` binop — a component-wise OpFMul(vec, scalar) is
                // illegal SPIR-V. Route multiply through emit_mul; same-shape falls back to bo.code below.
                if (compound_base(op) == "*") {
                    let mr = emit_mul(bm, expr, expr.left, cur, rhs, errs)
                    if (mr.handled) {
                        if (mr.id == 0u) return expr      // unsupported mix; error already pushed
                        res = mr.id
                    }
                }
                if (res == 0u) {
                    let bo = binop_code(compound_base(op), scalar_class(expr.left._type.baseType))
                    if (!bo.ok) {
                        errs |> push("unsupported compound assignment '{expr.op}'")
                        return expr
                    }
                    res = alloc_id(bm)
                    emit(bm, SEC_FUNCS, bo.code, p.pty, res, cur, rhs)
                }
                emit(bm, SEC_FUNCS, SpvOp.Store, p.id, res)
            }
            return expr
        }
        let cls = scalar_class(expr.left._type.baseType)
        let l = value_of(expr.left)
        let r = value_of(expr.right)
        if (l == 0u || r == 0u) return expr
        if (op == "*") {
            let mr = emit_mul(bm, expr, expr, l, r, errs)
            if (mr.handled) {
                if (mr.id != 0u) {
                    e2id[intptr(expr)] = mr.id
                }
                return expr
            }
        }
        let cc = cmp_code(op, cls)
        if (cc.ok) {
            let bt = type_bool(bm)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, cc.code, bt, res, l, r)
            e2id[intptr(expr)] = res
            return expr
        }
        if (op == "&&" || op == "||") {
            let bt = type_bool(bm)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, op == "&&" ? SpvOp.LogicalAnd : SpvOp.LogicalOr, bt, res, l, r)
            e2id[intptr(expr)] = res
            return expr
        }
        let bo = binop_code(op, cls)
        if (!bo.ok) {
            errs |> push("unsupported binary op '{expr.op}' on {expr.left._type.baseType}")
            return expr
        }
        let rt = emit_type(bm, expr._type)
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, bo.code, rt, res, l, r)
        e2id[intptr(expr)] = res
        return expr
    }

    // ----- ternary cond ? a : b -> OpSelect. Branchless; both arms are evaluated (eager) which is
    // correct for side-effect-free shader operands, same rule as eager &&/||. A vector result with a
    // scalar bool condition is splatted to a bvecN: SPIR-V < 1.4 OpSelect requires the condition width
    // to match the result component count. -----
    def override visitExprOp3(var expr : ExprOp3?) : ExpressionPtr {
        var bm & = unsafe(*m)
        if ("{expr.op}" != "?") {
            errs |> push("unsupported ternary operator '{expr.op}'")
            return expr
        }
        let n = component_count(expr._type.baseType)
        if (n == 0) {
            errs |> push("unsupported ternary result type {expr._type.baseType}")
            return expr
        }
        let cond = value_of(expr.subexpr)
        let tv = value_of(expr.left)
        let fv = value_of(expr.right)
        if (cond == 0u || tv == 0u || fv == 0u) return expr
        let resty = emit_type(bm, expr._type)
        var sel_cond = cond
        if (n > 1 && expr.subexpr._type.baseType == Type.tBool) {   // splat scalar bool -> bvecN
            let bvec = type_vector(bm, type_bool(bm), n)
            sel_cond = alloc_id(bm)
            var sops <- [bvec, sel_cond]
            sops |> reserve(n + 2)
            for (_ in range(n)) {
                sops |> push(cond)
            }
            emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, sops)
            delete sops
        }
        let res = alloc_id(bm)
        var ops <- [resty, res, sel_cond, tv, fv]
        emit_n(bm, SEC_FUNCS, SpvOp.Select, ops)
        delete ops
        e2id[intptr(expr)] = res
        return expr
    }

    // ----- store: dst = rhs -----
    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let p = ptr_of(expr.left)
        let val = value_of(expr.right)
        if (p.id != 0u && val != 0u) {
            emit(bm, SEC_FUNCS, SpvOp.Store, p.id, val)
        }
        return expr
    }

    // ----- let: memory local -> store init; value-let -> bind SSA id -----
    def override visitExprLetVariable(cexpr : ExprLet?; var arg : VariablePtr; lastArg : bool) : VariablePtr {
        var bm & = unsafe(*m)
        if (arg.init == null) {
            errs |> push("uninitialized local '{arg.name}' is not supported (every let/var must have an initializer)")
            return arg
        }
        let vk = intptr(arg)
        if (key_exists(ctx.local_vars, vk)) {
            let lv = ctx.local_vars?[vk] ?? LocalVar()
            let val = value_of(arg.init)
            if (val != 0u) {
                emit(bm, SEC_FUNCS, SpvOp.Store, lv.ptr_id, val)
            }
        } else {
            ctx.locals[vk] = value_of(arg.init)
        }
        return arg
    }

    // ----- return -----
    // Valueless (`return` in the void entry / a void helper) -> OpReturn; a value return inside a
    // user-defined shader function -> OpReturnValue with the loaded subexpression. ctx.terminated is
    // ONLY set when an actual terminator was emitted — if value_of() failed (e.g. an upstream error
    // already pushed onto errs), leaving terminated=false lets the function-end fallback close the
    // block with OpUnreachable so the blob stays structurally valid (one terminator per block).
    def override visitExprReturn(var expr : ExprReturn?) : ExpressionPtr {
        var bm & = unsafe(*m)
        if (expr.subexpr != null) {
            let v = value_of(expr.subexpr)
            if (v != 0u) {
                emit(bm, SEC_FUNCS, SpvOp.ReturnValue, v)
                ctx.terminated = true
            }
        } else {
            emit(bm, SEC_FUNCS, SpvOp.Return)
            ctx.terminated = true
        }
        return expr
    }

    // Compute the OpAccessChain pointee type-id for a Block-member access. Scalars/vectors/matrices
    // pass through emit_type. Nested structs and fixed-arrays need their LAID-OUT type ids (with
    // Offset / MatrixStride / ArrayStride decorations) — produced by recursing through
    // build_block_struct (idempotent under the type/decoration dedup pools). Used by both
    // visitExprField (direct + chained branches) and any other site that needs a Block-member
    // pointee type. `sname` is only used in error messages emitted by the recursion.
    def field_pointee_type(var bm : SpirvModule; ft : TypeDecl?; sname : string; storage : SpvStorageClass) : uint {
        if (ft == null) {
            errs |> push("internal: null field type at {sname}")
            return 0u
        }
        // TaskPayloadWorkgroupEXT is a PLAIN logical storage class (no Block / Offset / ArrayStride), so a
        // field type lowers via emit_type -- the SAME id build_plain_struct used for the member, so the
        // OpAccessChain types match. (Block storages below take the std140/std430 strided path.)
        if (storage == SpvStorageClass.TaskPayloadWorkgroupEXT) {
            return emit_type(bm, ft)
        }
        // matrices come back through emit_type -> type_matrix correctly; only tStructure and
        // tFixedArray need the layout-aware path.
        if (ft.baseType == Type.tStructure && ft.structType != null) {
            let inner = build_block_struct(bm, sname, ft.structType, false, errs)
            if (!inner.ok) return 0u
            return inner.type_id
        }
        if (ft.baseType == Type.tFixedArray && ft.firstType != null) {
            let et = ft.firstType
            let nelem = ft.fixedDim
            var elem_type : uint = 0u
            if (et.baseType == Type.tStructure && et.structType != null) {
                let inner = build_block_struct(bm, "{sname}[]", et.structType, false, errs)
                if (!inner.ok) return 0u
                elem_type = inner.type_id
            } else {
                elem_type = emit_type(bm, et)
            }
            // std140 element stride: scalar/vector rounds up to vec4; struct element strides by its
            // laid-out size rounded to the struct alignment. matrix-of-array is out of PR-G scope.
            // The struct-element probe routes its diagnostics through the visitor's `errs` so a
            // layout failure here surfaces the same detailed message a direct classify_global path
            // would (was a local array that swallowed errors).
            var stride = 0
            if (et.baseType == Type.tStructure && et.structType != null) {
                var probe : array<BlockField>
                let pl = compute_block_layout("{sname}[]", et.structType, probe, errs)
                delete probe
                if (!pl.ok) return 0u
                let salign = round_up(pl.align, 16)
                stride = round_up(pl.size, salign)
            } else {
                stride = round_up(std140_size(et.baseType), 16)
            }
            return type_array_strided(bm, elem_type, nelem, stride)
        }
        return emit_type(bm, ft)
    }

    // ----- member of a Block global (ubo.color) -> OpAccessChain by field index -> pointer -----
    // Two shapes: a direct block global (ubo.color), and a field of an indexed / nested chain (e.g.
    // particles[i].pos, or ubo.outer.inner under PR-G) where expr.value is the previous step's
    // pointer — chain the field off it in the *root global's* storage class.
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let k = intptr(expr)
        let bv = global_var_of(expr.value)
        if (bv == null) {
            if (key_exists(e2ptr, intptr(expr.value))) {
                let base = ptr_of(expr.value)
                let est = expr.value._type.structType
                if (est == null) {
                    errs |> push("member access '.{expr.name}': base is not a struct value")
                    return expr
                }
                let midx = field_index_by_name(est, "{expr.name}")
                if (midx < 0) {
                    errs |> push("member access '.{expr.name}': no such field")
                    return expr
                }
                // chain the field pointer in the root global's storage class (StorageBuffer for an
                // SSBO, Uniform for a UBO, PushConstant for @push_constant). Walking the whole chain
                // via root_global_of catches deeply nested forms (ubo.outer.inner under PR-G); the
                // pre-PR-G code only handled ExprAt-rooted chains and defaulted everything else to
                // StorageBuffer, which mismatched the base pointer's type for Uniform/PushConstant.
                let rv = root_global_of(expr.value)
                let rgi = rv != null ? (ctx.globals?[intptr(rv)] ?? GlobalInfo()) : GlobalInfo()
                let storage = rgi.var_id != 0u ? rgi.storage : SpvStorageClass.StorageBuffer
                let pointee = field_pointee_type(bm, expr._type, "{est.name}.{expr.name}", storage)
                if (pointee == 0u) return expr
                let ptr_t = type_pointer(bm, storage, pointee)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, base.id, const_uint(bm, uint(midx)))
                e2ptr[k] = res
                e2pty[k] = pointee
                return expr
            }
            errs |> push("member access '.{expr.name}' is only supported on a @uniform/@ssbo/@push_constant block global or an indexed ssbo struct element for now")
            return expr
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        if (!gi.is_block) {
            errs |> push("member access '.{expr.name}': '{bv.name}' is not a block (uniform/ssbo/push_constant) global")
            return expr
        }
        let midx = field_index_by_name(bv._type.structType, "{expr.name}")
        if (midx < 0) {
            errs |> push("member access '.{expr.name}': no such field on '{bv.name}'")
            return expr
        }
        let pointee = field_pointee_type(bm, expr._type, "{bv.name}.{expr.name}", gi.storage)
        if (pointee == 0u) return expr
        let ptr_t = type_pointer(bm, gi.storage, pointee)
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, const_uint(bm, uint(midx)))
        e2ptr[k] = res
        e2pty[k] = pointee
        return expr
    }

    // ----- user-defined shader function call: OpFunctionCall + ref-param copy-in/copy-out -----
    // Value args pass their loaded value directly. Each by-reference arg goes through a Function-storage
    // temp (pre-allocated into this function's entry block by alloc_call_temps): copy the current value
    // in before the call, then copy the (mutated) temp back to the original lvalue after — SPIR-V's
    // Logical addressing forbids handing an access-chain pointer straight to a call.
    def emit_user_call(var bm : SpirvModule; expr : ExprCall?; uf : UserFunc) {
        let f = expr.func
        let res = alloc_id(bm)
        var ops <- [uf.ret_type, res, uf.fn_id]
        var ref_temps : array<tuple<temp : uint; orig : uint; vt : uint>>
        for (arg, i in expr.arguments, count()) {
            let is_ref = i < length(f.arguments) && f.arguments[i]._type.flags.ref && !f.arguments[i]._type.flags.constant
            if (is_ref) {
                let ct = ctx.call_temps?[intptr(arg)] ?? CallTemp()
                if (ct.ptr_id == 0u) {
                    errs |> push("internal: missing copy-in/out temp for a by-reference argument to '{f.name}'")
                    delete ops
                    delete ref_temps
                    return
                }
                let v = value_of(arg)        // copy-in: current value of the lvalue
                let p = ptr_of(arg)          // original lvalue, for copy-out
                if (v == 0u || p.id == 0u) {
                    delete ops
                    delete ref_temps
                    return
                }
                emit(bm, SEC_FUNCS, SpvOp.Store, ct.ptr_id, v)
                ops |> push(ct.ptr_id)
                ref_temps |> push((temp = ct.ptr_id, orig = p.id, vt = ct.value_type))
            } else {
                let v = value_of(arg)
                if (v == 0u) {
                    delete ops
                    delete ref_temps
                    return
                }
                ops |> push(v)
            }
        }
        emit_n(bm, SEC_FUNCS, SpvOp.FunctionCall, ops)
        delete ops
        for (rt in ref_temps) {
            let lv = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, rt.vt, lv, rt.temp)
            emit(bm, SEC_FUNCS, SpvOp.Store, rt.orig, lv)
        }
        delete ref_temps
        if (!uf.is_void) {
            e2id[intptr(expr)] = res
        }
    }

    // ----- calls: texture / vector constructors / dot / GLSL.std.450 math (args pre-visited) -----
    // Mirrors emit_call but reads operand ids via value_of (children already visited). for-source
    // range()/urange() calls are consumed by preVisitExprForBody and skipped here.
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (key_exists(for_src, intptr(expr))) return expr
        var bm & = unsafe(*m)
        let name = "{expr.name}"
        let k = intptr(expr)
        // discard(): abandon the fragment -> OpKill. A block terminator (sets ctx.terminated), so any
        // following statement on the same path is rejected as unreachable. Fragment-only.
        if (name == "discard") {
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("discard() is only valid in a fragment shader")
                return expr
            }
            if (!empty(expr.arguments)) {
                errs |> push("discard() takes no arguments")
                return expr
            }
            emit(bm, SEC_FUNCS, SpvOp.Kill)
            ctx.terminated = true
            return expr
        }
        // SetMeshOutputsEXT(vertex_count, primitive_count): mesh-shader prologue. SPV_EXT_mesh_shader
        // requires this call once per workgroup at workgroup-uniform control flow BEFORE any write to
        // a per-vertex or per-primitive output array -- it declares how much output storage to
        // allocate. (0, 0) is the valid empty-mesh form (emits OpSetMeshOutputsEXT %uint_0 %uint_0):
        // PR-D's empty-mesh fixture uses it to land the cap / extension / execution modes without the
        // per-vertex output array (which arrives in PR-E). Two uint operands, no result.
        if (name == "SetMeshOutputsEXT") {
            if (ctx.stage != ShaderStage.Mesh) {
                errs |> push("SetMeshOutputsEXT is only valid in a mesh shader")
                return expr
            }
            if (length(expr.arguments) != 2) {
                errs |> push("SetMeshOutputsEXT expects (vertex_count, primitive_count)")
                return expr
            }
            let nv = value_of(expr.arguments[0])
            let np = value_of(expr.arguments[1])
            // value_of returns an SSA result-id; 0 is the failed-to-emit sentinel, not a zero count
            // (a literal 0u lowers to a valid OpConstant id). Fail closed rather than drop the prologue.
            if (nv == 0u || np == 0u) {
                errs |> push("SetMeshOutputsEXT: could not evaluate the vertex / primitive count operand")
                return expr
            }
            emit(bm, SEC_FUNCS, SpvOp.SetMeshOutputsEXT, nv, np)
            return expr
        }
        // EmitMeshTasksEXT(x, y, z): task-shader (amplification) workgroup terminator. Dispatches an
        // x*y*z grid of mesh workgroups and ENDS the task workgroup (per the spec the call behaves
        // like OpReturn / OpKill), so set ctx.terminated = true so the visitor rejects anything after
        // it on the same control-flow path.
        if (name == "EmitMeshTasksEXT") {
            if (ctx.stage != ShaderStage.Task) {
                errs |> push("EmitMeshTasksEXT is only valid in a task shader")
                return expr
            }
            if (length(expr.arguments) != 3) {
                errs |> push("EmitMeshTasksEXT expects (group_count_x, group_count_y, group_count_z)")
                return expr
            }
            let gx = value_of(expr.arguments[0])
            let gy = value_of(expr.arguments[1])
            let gz = value_of(expr.arguments[2])
            // value_of returns an SSA result-id; 0 is the failed-to-emit sentinel. Fail closed rather
            // than drop the workgroup terminator.
            if (gx == 0u || gy == 0u || gz == 0u) {
                errs |> push("EmitMeshTasksEXT: could not evaluate a group-count operand")
                return expr
            }
            // SPV_EXT_mesh_shader: OpEmitMeshTasksEXT takes an OPTIONAL 4th operand, the
            // TaskPayloadWorkgroupEXT variable handed to the dispatched mesh workgroups. Append it when
            // the task shader declared a @task_payload; otherwise the 3-operand form (no payload).
            if (ctx.task_payload_id != 0u) {
                emit(bm, SEC_FUNCS, SpvOp.EmitMeshTasksEXT, gx, gy, gz, ctx.task_payload_id)
            } else {
                emit(bm, SEC_FUNCS, SpvOp.EmitMeshTasksEXT, gx, gy, gz)
            }
            ctx.terminated = true
            return expr
        }
        // barrier(): control + shared-memory barrier (OpControlBarrier at Workgroup execution+memory
        // scope, WorkgroupMemory|AcquireRelease semantics). memoryBarrierShared(): the memory-only
        // variant (OpMemoryBarrier, same scope/semantics). Both compute-only; not block terminators.
        if (name == "barrier" || name == "memoryBarrierShared") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("{name}() is only valid in a compute shader")
                return expr
            }
            if (!empty(expr.arguments)) {
                errs |> push("{name}() takes no arguments")
                return expr
            }
            let scope = const_uint(bm, uint(SpvScope.Workgroup))
            let sem = const_uint(bm, uint(SpvMemorySemantics.WorkgroupMemory) | uint(SpvMemorySemantics.AcquireRelease))
            if (name == "barrier") {
                emit(bm, SEC_FUNCS, SpvOp.ControlBarrier, scope, scope, sem)
            } else {
                emit(bm, SEC_FUNCS, SpvOp.MemoryBarrier, scope, sem)
            }
            return expr
        }
        // atomics: atomicAdd/Min/Max/And/Or/Xor/Exchange/CompSwap on a @workgroup or @ssbo lvalue,
        // returning the OLD value. The mem param is a reference (T&), so the first arg stays an lvalue
        // and ptr_of recovers its pointer (peel a defensive ExprRef2Value in case daslang loaded it).
        // Scope = Workgroup for shared memory, Device for buffer memory; Relaxed semantics (glslang's
        // default for GLSL atomics). 6-operand shape -> emit_n; CompSwap adds a comparator + neq-sem.
        let acls = empty(expr.arguments) ? -1 : scalar_class(expr.arguments[0]._type.baseType)
        let ainfo = atomic_info(name, acls)
        if (ainfo.ok) {
            let nargs = ainfo.compswap ? 3 : 2
            if (length(expr.arguments) != nargs) {
                errs |> push("{name} expects {nargs} arguments")
                return expr
            }
            if (acls != 0 && acls != 1) {
                errs |> push("{name}: atomic target must be int or uint")
                return expr
            }
            let arg0 = expr.arguments[0]
            let r2v = arg0 ?as ExprRef2Value
            let lvexpr = r2v != null ? r2v.subexpr : arg0
            let rg = root_global_of(lvexpr)
            if (rg == null) {
                errs |> push("{name}: target must be a @workgroup or @ssbo global lvalue")
                return expr
            }
            let p = ptr_of(lvexpr)
            if (p.id == 0u) {
                errs |> push("{name}: target is not an addressable lvalue")
                return expr
            }
            let rgi = ctx.globals?[intptr(rg)] ?? GlobalInfo()
            // only @workgroup (Workgroup) and @ssbo (StorageBuffer) are writable atomic targets; a
            // @uniform/@push_constant/builtin lvalue is read-only storage -> reject cleanly rather than
            // emit an OpAtomic* on an invalid storage class (fail-closed, not spirv-val-only).
            if (rgi.storage != SpvStorageClass.Workgroup && rgi.storage != SpvStorageClass.StorageBuffer) {
                errs |> push("{name}: atomic target must live in @workgroup or @ssbo memory")
                return expr
            }
            let scope = rgi.storage == SpvStorageClass.Workgroup ? SpvScope.Workgroup : SpvScope.Device
            let sc_id = const_uint(bm, uint(scope))
            let sem_id = const_uint(bm, uint(SpvMemorySemantics.Relaxed))
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            if (ainfo.compswap) {
                let val = value_of(expr.arguments[2])
                let cmp = value_of(expr.arguments[1])
                if (val == 0u || cmp == 0u) return expr
                // OpAtomicCompareExchange: rt res ptr scope eq-sem neq-sem value comparator
                var ops <- [rt, res, p.id, sc_id, sem_id, sem_id, val, cmp]
                emit_n(bm, SEC_FUNCS, SpvOp.AtomicCompareExchange, ops)
                delete ops
            } else {
                let val = value_of(expr.arguments[1])
                if (val == 0u) return expr
                var ops <- [rt, res, p.id, sc_id, sem_id, val]
                emit_n(bm, SEC_FUNCS, ainfo.op, ops)
                delete ops
            }
            e2id[k] = res
            return expr
        }
        // nonuniformEXT(x): the VK_EXT_descriptor_indexing marker. Emits OpCopyObject to give the
        // value a fresh result id, then OpDecorate <new_id> NonUniform so the marker scopes to this
        // use only (decorating the input id would leak NonUniform onto every other consumer of the
        // same SSA value). The marked id is recorded in ctx.nonuniform_ids so the descriptor-array
        // access path (texture(arr[i], uv)) can detect the marker on its index value and cascade
        // the NonUniform decoration onto the access-chain pointer + the loaded sampler.
        if (name == "nonuniformEXT") {
            if (length(expr.arguments) != 1) {
                errs |> push("nonuniformEXT expects 1 argument")
                return expr
            }
            let v = value_of(expr.arguments[0])
            if (v == 0u) return expr
            ensure_descriptor_indexing(bm, *ctx)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.CopyObject, rt, res, v)
            emit(bm, SEC_DECOR, SpvOp.Decorate, res, uint(SpvDecoration.NonUniform))
            ctx.nonuniform_ids |> insert(res)
            e2id[k] = res
            return expr
        }
        // dFdx/dFdy/fwidth: screen-space derivatives over the 2x2 quad (OpDPdx/OpDPdy/OpFwidth).
        // Fragment-only; one float/floatN operand, same-typed result. No extra capability (the basic
        // forms need only Shader; the *Fine/*Coarse variants would need DerivativeControl).
        if (name == "dFdx" || name == "dFdy" || name == "fwidth") {
            if (length(expr.arguments) != 1) {
                errs |> push("{name} expects 1 argument")
                return expr
            }
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("{name}: screen-space derivatives are only valid in a fragment shader")
                return expr
            }
            let a = value_of(expr.arguments[0])
            if (a == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            let op = (name == "dFdx" ? SpvOp.DPdx : (name == "dFdy" ? SpvOp.DPdy : SpvOp.Fwidth))
            emit(bm, SEC_FUNCS, op, rt, res, a)
            e2id[k] = res
            return expr
        }
        // textureSize(sampler, lod): mip-level dimensions in texels (OpImageQuerySizeLod). Extract the
        // image from the sampled image (OpImage), then query; needs the ImageQuery capability.
        if (name == "textureSize") {
            if (length(expr.arguments) != 2) {
                errs |> push("textureSize expects (sampler, lod)")
                return expr
            }
            let rs = resolve_sampler_arg(expr.arguments[0], "textureSize")
            if (!rs.ok) return expr
            ensure_image_query(bm, *ctx)
            let si = sampler_info(rs.sg_type)
            // reconstruct the SAME image type classify_global built for this sampler so OpImage's
            // result type matches the sampled image's image type -- depth included (type_image dedups on it)
            let img_type = type_image(bm, type_float(bm, 32u), si.dim, si.arrayed, 1u, SpvImageFormat.Unknown, si.depth)
            let img = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Image, img_type, img, rs.simg)
            let lod = value_of(expr.arguments[1])
            if (lod == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageQuerySizeLod, rt, res, img, lod)
            e2id[k] = res
            return expr
        }
        // textureCompare(sampler2DShadow, uv, compare): depth-compare sampling. OpLoad the comparison
        // sampler + OpImageSampleDrefImplicitLod with the reference value as the Dref operand; the
        // scalar result is the comparison (1 = lit, 0 = shadowed; filtered -> PCF average). Implicit
        // LOD -> fragment-only.
        if (name == "textureCompare") {
            if (length(expr.arguments) != 3) {
                errs |> push("textureCompare expects (sampler2DShadow, uv, compare)")
                return expr
            }
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("textureCompare: implicit-LOD depth-compare sampling is only valid in a fragment shader")
                return expr
            }
            let rs = resolve_sampler_arg(expr.arguments[0], "textureCompare")
            if (!rs.ok) return expr
            if (sampler_info(rs.sg_type).depth == 0u) {
                errs |> push("textureCompare: '{rs.sg.name}' is not a sampler2DShadow (depth-compare sampler)")
                return expr
            }
            let uv = value_of(expr.arguments[1])
            let dref = value_of(expr.arguments[2])
            if (uv == 0u || dref == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- [rt, res, rs.simg, uv, dref]
            emit_n(bm, SEC_FUNCS, SpvOp.ImageSampleDrefImplicitLod, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        // texture(sampler, uv): OpLoad the sampled image + OpImageSampleImplicitLod. Descriptor-array
        // form texture(arr[idx], uv) goes through resolve_sampler_arg, which emits the OpAccessChain +
        // OpLoad and cascades NonUniform when the index carries it (PR-C bindless rail).
        if (name == "texture") {
            if (length(expr.arguments) != 2) {
                errs |> push("texture expects (sampler, uv)")
                return expr
            }
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("texture() implicit-LOD sampling is only valid in a fragment shader")
                return expr
            }
            let rs = resolve_sampler_arg(expr.arguments[0], "texture")
            if (!rs.ok) return expr
            let uv = value_of(expr.arguments[1])
            if (uv == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageSampleImplicitLod, rt, res, rs.simg, uv)
            e2id[k] = res
            return expr
        }
        // textureLod(sampler, uv, lod): OpImageSampleExplicitLod with an explicit Lod operand. No
        // derivatives -> valid in any stage (not fragment-only like texture()).
        if (name == "textureLod") {
            if (length(expr.arguments) != 3) {
                errs |> push("textureLod expects (sampler, uv, lod)")
                return expr
            }
            let rs = resolve_sampler_arg(expr.arguments[0], "textureLod")
            if (!rs.ok) return expr
            let uv = value_of(expr.arguments[1])
            let lod = value_of(expr.arguments[2])
            if (uv == 0u || lod == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- [rt, res, rs.simg, uv, uint(SpvImageOperands.Lod), lod]
            emit_n(bm, SEC_FUNCS, SpvOp.ImageSampleExplicitLod, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        // texelFetch(sampler, coord, lod): OpImage to extract the image from the sampled image, then
        // OpImageFetch by integer coordinate + Lod. Unfiltered; stage-agnostic.
        if (name == "texelFetch") {
            if (length(expr.arguments) != 3) {
                errs |> push("texelFetch expects (sampler, coord, lod)")
                return expr
            }
            let rs = resolve_sampler_arg(expr.arguments[0], "texelFetch")
            if (!rs.ok) return expr
            // extract the underlying image (same dedup key classify_global used for the sampler)
            let si = sampler_info(rs.sg_type)
            // reconstruct the SAME image type classify_global built for this sampler so OpImage's
            // result type matches the sampled image's image type -- depth included (type_image dedups on it)
            let img_type = type_image(bm, type_float(bm, 32u), si.dim, si.arrayed, 1u, SpvImageFormat.Unknown, si.depth)
            let img = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Image, img_type, img, rs.simg)
            let coord = value_of(expr.arguments[1])
            let lod = value_of(expr.arguments[2])
            if (coord == 0u || lod == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- [rt, res, img, coord, uint(SpvImageOperands.Lod), lod]
            emit_n(bm, SEC_FUNCS, SpvOp.ImageFetch, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        // sampleTexture(texture2D, sampler, uv): combine the separate image + sampler with OpSampledImage,
        // then OpImageSampleImplicitLod. Implicit LOD -> fragment-only.
        if (name == "sampleTexture") {
            if (length(expr.arguments) != 3) {
                errs |> push("sampleTexture expects (texture, sampler, uv)")
                return expr
            }
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("sampleTexture: implicit-LOD sampling is only valid in a fragment shader")
                return expr
            }
            let tg = global_var_of(expr.arguments[0])
            let sgv = global_var_of(expr.arguments[1])
            if (tg == null || sgv == null) {
                errs |> push("sampleTexture: texture and sampler must be globals")
                return expr
            }
            let tgi = ctx.globals?[intptr(tg)] ?? GlobalInfo()
            let sgi = ctx.globals?[intptr(sgv)] ?? GlobalInfo()
            if (!tgi.is_separate_image) {
                errs |> push("sampleTexture: '{tg.name}' is not a texture2D")
                return expr
            }
            if (!sgi.is_separate_sampler) {
                errs |> push("sampleTexture: '{sgv.name}' is not a sampler")
                return expr
            }
            let imgv = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, tgi.value_type, imgv, tgi.var_id)
            let smpv = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, sgi.value_type, smpv, sgi.var_id)
            let simg_type = type_sampled_image(bm, tgi.value_type)
            let combined = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.SampledImage, simg_type, combined, imgv, smpv)
            let uv = value_of(expr.arguments[2])
            if (uv == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageSampleImplicitLod, rt, res, combined, uv)
            e2id[k] = res
            return expr
        }
        // imageStore(image, coord, value): OpLoad the image + OpImageWrite (void statement, no result)
        if (name == "imageStore") {
            if (length(expr.arguments) != 3) {
                errs |> push("imageStore expects (image, coord, value)")
                return expr
            }
            let ig = global_var_of(expr.arguments[0])
            if (ig == null) {
                errs |> push("imageStore: first argument must be a storage-image global")
                return expr
            }
            let gi = ctx.globals?[intptr(ig)] ?? GlobalInfo()
            if (!gi.is_storage_image) {
                errs |> push("imageStore: '{ig.name}' is not a storage image")
                return expr
            }
            let imgval = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, imgval, gi.var_id)
            let coord = value_of(expr.arguments[1])
            let texel = value_of(expr.arguments[2])
            if (coord == 0u || texel == 0u) return expr
            emit(bm, SEC_FUNCS, SpvOp.ImageWrite, imgval, coord, texel)
            return expr
        }
        // imageLoad(image, coord) : float4: OpLoad the image + OpImageRead
        if (name == "imageLoad") {
            if (length(expr.arguments) != 2) {
                errs |> push("imageLoad expects (image, coord)")
                return expr
            }
            let ig = global_var_of(expr.arguments[0])
            if (ig == null) {
                errs |> push("imageLoad: first argument must be a storage-image global")
                return expr
            }
            let gi = ctx.globals?[intptr(ig)] ?? GlobalInfo()
            if (!gi.is_storage_image) {
                errs |> push("imageLoad: '{ig.name}' is not a storage image")
                return expr
            }
            let imgval = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, imgval, gi.var_id)
            let coord = value_of(expr.arguments[1])
            if (coord == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageRead, rt, res, imgval, coord)
            e2id[k] = res
            return expr
        }
        // subpassLoad(input) : float4: OpLoad the SubpassData image + OpImageRead at int2(0, 0). For a
        // SubpassData image the coordinate is an OFFSET from gl_FragCoord (not absolute), so (0,0) is
        // "this fragment" -- which is the only useful subpass-input sampling pattern (subpasses share
        // pixel locality with the previous pass).
        if (name == "subpassLoad") {
            if (ctx.stage != ShaderStage.Fragment) {
                errs |> push("subpassLoad is fragment-only (Vulkan subpass-input attachments)")
                return expr
            }
            if (length(expr.arguments) != 1) {
                errs |> push("subpassLoad expects (input)")
                return expr
            }
            let ig = global_var_of(expr.arguments[0])
            if (ig == null) {
                errs |> push("subpassLoad: argument must be a subpassInput global")
                return expr
            }
            let gi = ctx.globals?[intptr(ig)] ?? GlobalInfo()
            if (!gi.is_subpass_input) {
                errs |> push("subpassLoad: '{ig.name}' is not a subpassInput")
                return expr
            }
            let imgval = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, imgval, gi.var_id)
            let it = type_int(bm, 32u, 1u)
            let ivec = type_vector(bm, it, 2)
            let zero = const_int(bm, 0)
            let coord = const_composite(bm, ivec, [zero, zero])
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageRead, rt, res, imgval, coord)
            e2id[k] = res
            return expr
        }
        // imageSize(image) : int2: the storage image's dimensions in texels (OpImageQuerySize, no LOD --
        // a storage image is Sampled=2, so the no-LOD query is legal). Needs the ImageQuery capability.
        if (name == "imageSize") {
            if (length(expr.arguments) != 1) {
                errs |> push("imageSize expects (image)")
                return expr
            }
            let ig = global_var_of(expr.arguments[0])
            if (ig == null) {
                errs |> push("imageSize: argument must be a storage-image global")
                return expr
            }
            let gi = ctx.globals?[intptr(ig)] ?? GlobalInfo()
            if (!gi.is_storage_image) {
                errs |> push("imageSize: '{ig.name}' is not a storage image")
                return expr
            }
            ensure_image_query(bm, *ctx)
            let imgval = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, imgval, gi.var_id)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageQuerySize, rt, res, imgval)
            e2id[k] = res
            return expr
        }
        // scalar numeric conversion: float(int/uint), int(float/uint), uint(float/int). Same-class is a
        // no-op passthrough; cross-class emits OpConvert* / OpBitcast. Scalar only (vector casts rejected).
        let dst_cls = scalar_type_name_class(name)
        if (dst_cls >= 0 && length(expr.arguments) == 1) {
            let a0 = expr.arguments[0]
            if (component_count(a0._type.baseType) != 1) {
                errs |> push("{name}: vector conversion is not supported (scalar only)")
                return expr
            }
            let aid = value_of(a0)
            if (aid == 0u) return expr
            let src_cls = scalar_class(a0._type.baseType)
            if (src_cls == dst_cls) {            // same-class no-op cast, e.g. float(someFloat)
                e2id[k] = aid
                return expr
            }
            let cc = convert_op(src_cls, dst_cls)
            if (!cc.ok) {
                errs |> push("{name}: conversion from {a0._type.baseType} is not supported")
                return expr
            }
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, cc.code, rt, res, aid)
            e2id[k] = res
            return expr
        }
        // vector constructor: OpCompositeConstruct (or splat / identity for one arg)
        let vc = vector_ctor(name)
        if (vc.ok) {
            let rt = emit_type(bm, expr._type)
            if (length(expr.arguments) == 1) {
                let a0 = expr.arguments[0]
                let cc = component_count(a0._type.baseType)
                if (scalar_class(a0._type.baseType) != vc.cls) {
                    errs |> push("{name}: component-kind conversion from {a0._type.baseType} is not supported yet")
                    return expr
                }
                let aid = value_of(a0)
                if (aid == 0u) return expr
                if (cc == vc.n) {                            // identity: float4(someFloat4)
                    e2id[k] = aid
                    return expr
                }
                if (cc == 1) {                               // splat: float4(scalar)
                    let res = alloc_id(bm)
                    var cons <- [rt, res]
                    cons |> reserve(2 + vc.n)
                    for (_l in range(vc.n)) {
                        cons |> push(aid)
                    }
                    emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
                    delete cons
                    e2id[k] = res
                    return expr
                }
                errs |> push("cannot construct {name} from a single {a0._type.baseType}")
                return expr
            }
            var total = 0
            let res = alloc_id(bm)
            var cons <- [rt, res]
            cons |> reserve(2 + length(expr.arguments))
            for (a in expr.arguments) {
                let cc = component_count(a._type.baseType)
                if (cc == 0) {
                    errs |> push("{name}: argument of type {a._type.baseType} is not a scalar/vector")
                    delete cons
                    return expr
                }
                if (scalar_class(a._type.baseType) != vc.cls) {
                    errs |> push("{name}: constituent of type {a._type.baseType} has a different component kind (conversion not supported yet)")
                    delete cons
                    return expr
                }
                total += cc
                let aid = value_of(a)
                if (aid == 0u) {
                    delete cons
                    return expr
                }
                cons |> push(aid)
            }
            if (total != vc.n) {
                errs |> push("{name}: constituent lanes sum to {total}, expected {vc.n}")
                delete cons
                return expr
            }
            emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
            delete cons
            e2id[k] = res
            return expr
        }
        // fmod: core OpFRem (C fmod semantics — sign-of-dividend). NOT a GLSL.std.450 ext-inst.
        if (name == "fmod") {
            if (length(expr.arguments) != 2) {
                errs |> push("fmod expects 2 arguments")
                return expr
            }
            let a = value_of(expr.arguments[0])
            let b = value_of(expr.arguments[1])
            if (a == 0u || b == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.FRem, rt, res, a, b)
            e2id[k] = res
            return expr
        }
        // dot: core OpDot (vector . vector -> scalar)
        if (name == "dot") {
            if (length(expr.arguments) != 2) {
                errs |> push("dot expects 2 arguments")
                return expr
            }
            let a = value_of(expr.arguments[0])
            let b = value_of(expr.arguments[1])
            if (a == 0u || b == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Dot, rt, res, a, b)
            e2id[k] = res
            return expr
        }
        // GLSL.std.450 math: OpExtInst
        let cls = empty(expr.arguments) ? -1 : scalar_class(expr.arguments[0]._type.baseType)
        let ge = glsl_ext_op(name, cls)
        if (ge.ok) {
            let set = ensure_glsl_ext(bm, *ctx)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- [rt, res, set, uint(ge.op)]
            // SPIR-V has no implicit scalar->vector broadcast. For the ops that allow a scalar operand
            // against a vector result (mix/clamp/min/max/step/smoothstep), splat any scalar operand to
            // the result width via OpCompositeConstruct so the OpExtInst is valid. This is what makes
            // daslang lerp(vec, vec, float) emit correctly instead of an invalid (scalar-t) FMix.
            let result_n = component_count(expr._type.baseType)
            let broadcast = result_n > 1 && ext_op_broadcasts_scalar(ge.op)
            for (a in expr.arguments) {
                var aid = value_of(a)
                if (aid == 0u) {
                    delete ops
                    return expr
                }
                if (broadcast && component_count(a._type.baseType) == 1) {
                    let sres = alloc_id(bm)
                    var cons <- [rt, sres]
                    cons |> reserve(2 + result_n)
                    for (_l in range(result_n)) {
                        cons |> push(aid)
                    }
                    emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
                    delete cons
                    aid = sres
                }
                ops |> push(aid)
            }
            emit_n(bm, SEC_FUNCS, SpvOp.ExtInst, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        // user-defined shader function -> OpFunctionCall (copy-in / copy-out around ref params)
        if (expr.func != null && key_exists(ctx.user_funcs, intptr(expr.func))) {
            let uf = ctx.user_funcs?[intptr(expr.func)] ?? UserFunc()
            if (!uf.valid) return expr   // signature rejected at registration; error already emitted
            emit_user_call(bm, expr, uf)
            return expr
        }
        // a resolved call we can't lower: a host/extern builtin (print, ...) or a daslang function in
        // an unsupported module. Fail-closed with a clear message rather than the generic one below.
        if (expr.func != null) {
            errs |> push("cannot call '{name}' in a SPIR-V shader — only user-defined shader functions and built-in shader intrinsics (texture/imageStore/dot/math/...) are callable")
            return expr
        }
        errs |> push("unsupported call '{name}' (not a vector constructor, dot, or GLSL.std.450 math builtin)")
        return expr
    }

    // ----- fixed-array literal -> OpConstantComposite (whole-array, deduped) -----
    // emit_const recurses into the (const) elements internally, so the elements' own default visit emits
    // nothing — at patch they fold to ExprConstFloatN leaves, not sub-call nodes. Records the array's
    // constant id; a fixed-array `let` then Stores it into the Function-storage OpVariable.
    def override visitExprMakeArray(var expr : ExprMakeArray?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let r = emit_const(bm, expr, errs)
        if (r.ok) {
            e2id[intptr(expr)] = r.id
        }
        return expr
    }

    // ----- if / elif / else (OpSelectionMerge + OpBranchConditional, multi-phase) -----
    // Block ids are allocated in the IfBlock hook — AFTER the cond is visited — so the id order
    // matches the hand-walker (cond ids first, then merge/then/else), keeping the blob byte-identical.
    // `elif` chains fall out for free: an `if_false` that is itself an ExprIfThenElse just recurses.
    def override preVisitExprIfThenElse(var expr : ExprIfThenElse?) : void {}

    def override preVisitExprIfThenElseIfBlock(var expr : ExprIfThenElse?; ifBlock : ExpressionPtr) : void {
        var bm & = unsafe(*m)
        let cond = value_of(expr.cond)
        let merge = alloc_id(bm)
        let then_l = alloc_id(bm)
        let else_l = expr.if_false != null ? alloc_id(bm) : merge
        ite_ids[intptr(expr)] = IteIds(merge = merge, then_l = then_l, else_l = else_l)
        if (cond != 0u) {   // a 0 cond means it failed to lower (error already pushed) — no 0-operand branch
            emit(bm, SEC_FUNCS, SpvOp.SelectionMerge, merge, uint(SpvSelectionControl.None))
            emit(bm, SEC_FUNCS, SpvOp.BranchConditional, cond, then_l, else_l)
        }
        emit(bm, SEC_FUNCS, SpvOp.Label, then_l)
        ctx.terminated = false
    }

    def override preVisitExprIfThenElseElseBlock(var expr : ExprIfThenElse?; elseBlock : ExpressionPtr) : void {
        let ids = ite_ids?[intptr(expr)] ?? IteIds()
        var bm & = unsafe(*m)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.else_l)
        ctx.terminated = false
    }

    def override visitExprIfThenElse(var expr : ExprIfThenElse?) : ExpressionPtr {
        let ids = ite_ids?[intptr(expr)] ?? IteIds()
        var bm & = unsafe(*m)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        ctx.terminated = false
        return expr
    }

    // ----- while (OpLoopMerge 4-block, multi-phase) -----
    def override preVisitExprWhile(var expr : ExprWhile?) : void {
        var bm & = unsafe(*m)
        let ids = LoopIds(header = alloc_id(bm), cond_l = alloc_id(bm), body_l = alloc_id(bm),
                          cont_l = alloc_id(bm), merge = alloc_id(bm))
        loop_ids[intptr(expr)] = ids
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.header)
        ctx.terminated = false
        emit(bm, SEC_FUNCS, SpvOp.LoopMerge, ids.merge, ids.cont_l, uint(SpvLoopControl.None))
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cond_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cond_l)
        ctx.terminated = false
    }

    def override preVisitExprWhileBody(var expr : ExprWhile?; right : ExpressionPtr) : void {
        let ids = loop_ids?[intptr(expr)] ?? LoopIds()
        var bm & = unsafe(*m)
        let c = value_of(expr.cond)
        if (c != 0u) {   // a 0 cond means it failed to lower (error already pushed) — no 0-operand branch
            emit(bm, SEC_FUNCS, SpvOp.BranchConditional, c, ids.body_l, ids.merge)
        }
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.body_l)
        ctx.terminated = false
        ctx.loop_stack |> push(LoopTargets(merge = ids.merge, cont = ids.cont_l))
    }

    def override visitExprWhile(var expr : ExprWhile?) : ExpressionPtr {
        let ids = loop_ids?[intptr(expr)] ?? LoopIds()
        var bm & = unsafe(*m)
        if (!empty(ctx.loop_stack)) pop(ctx.loop_stack)   // guard underflow if the body hook was skipped on an earlier error
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cont_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cont_l)
        ctx.terminated = false
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        ctx.terminated = false
        return expr
    }

    // ----- for (i in range(...)): synthesized 4-block loop. All setup runs in ForBody (after the
    // source is visited, so runtime range() endpoints are already in e2id); const ranges read
    // ExprConstRange.value directly. Block ids allocated here to match the hand-walker's order. -----
    def override preVisitExprFor(var expr : ExprFor?) : void {
        // record the for-source so a future visitExprCall (6.3) skips emitting the range() call; its
        // arguments are still visited normally, so runtime endpoints land in e2id.
        if (!empty(expr.sources)) {
            for_src |> insert(intptr(expr.sources[0]))
        }
    }

    def override preVisitExprForBody(var expr : ExprFor?) : void {
        var bm & = unsafe(*m)
        if (length(expr.iteratorVariables) != 1 || length(expr.sources) != 1) {
            errs |> push("for: only a single `i in range(...)` iterator is supported for now")
            return
        }
        let iv = expr.iteratorVariables[0]
        let lvr = ctx.local_vars?[intptr(iv)] ?? LocalVar()
        if (lvr.ptr_id == 0u) {
            errs |> push("for: induction variable '{iv.name}' was not declared")
            return
        }
        let cls = scalar_class(iv._type.baseType)
        if (cls != 0 && cls != 1) {
            errs |> push("for: induction variable '{iv.name}' must be int or uint (got {iv._type.baseType})")
            return
        }
        var lo = 0u
        var hi = 0u
        let cr = expr.sources[0] ?as ExprConstRange
        if (cr != null) {
            let fromto = unsafe(reinterpret<int2>(cr.value))   // range == { from : int; to : int }
            lo = const_int_for(bm, cls, fromto.x)
            hi = const_int_for(bm, cls, fromto.y)
        } else {
            let src = expr.sources[0] ?as ExprCall
            if (src == null) {
                errs |> push("for: source is {expr.sources[0].__rtti}, expected range()/urange()")
                return
            }
            if ("{src.name}" != "range" && "{src.name}" != "urange") {
                errs |> push("for: source call is '{src.name}', expected range()/urange()")
                return
            }
            let na = length(src.arguments)
            if (na != 1 && na != 2) {
                errs |> push("for: only range(n) and range(lo, hi) are supported for now (no step)")
                return
            }
            lo = na == 2 ? value_of(src.arguments[0]) : const_zero(bm, cls)
            hi = value_of(src.arguments[na - 1])
        }
        if (lo == 0u || hi == 0u) return
        emit(bm, SEC_FUNCS, SpvOp.Store, lvr.ptr_id, lo)
        let ids = LoopIds(header = alloc_id(bm), cond_l = alloc_id(bm), body_l = alloc_id(bm),
                          cont_l = alloc_id(bm), merge = alloc_id(bm))
        loop_ids[intptr(expr)] = ids
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.header)
        ctx.terminated = false
        emit(bm, SEC_FUNCS, SpvOp.LoopMerge, ids.merge, ids.cont_l, uint(SpvLoopControl.None))
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cond_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cond_l)
        ctx.terminated = false
        let cur = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, lvr.value_type, cur, lvr.ptr_id)
        let cc = cmp_code("<", cls)
        let cres = alloc_id(bm)
        emit(bm, SEC_FUNCS, cc.code, type_bool(bm), cres, cur, hi)
        emit(bm, SEC_FUNCS, SpvOp.BranchConditional, cres, ids.body_l, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.body_l)
        ctx.terminated = false
        ctx.loop_stack |> push(LoopTargets(merge = ids.merge, cont = ids.cont_l))
    }

    def override visitExprFor(var expr : ExprFor?) : ExpressionPtr {
        if (!key_exists(loop_ids, intptr(expr))) return expr   // setup failed; errors already pushed
        let ids = loop_ids?[intptr(expr)] ?? LoopIds()
        var bm & = unsafe(*m)
        let iv = expr.iteratorVariables[0]
        let lvr = ctx.local_vars?[intptr(iv)] ?? LocalVar()
        let cls = scalar_class(iv._type.baseType)
        pop(ctx.loop_stack)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.cont_l)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.cont_l)
        ctx.terminated = false
        let cur2 = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, lvr.value_type, cur2, lvr.ptr_id)
        let nxt = alloc_id(bm)
        emit(bm, SEC_FUNCS, binop_code("+", cls).code, lvr.value_type, nxt, cur2, const_one(bm, cls))
        emit(bm, SEC_FUNCS, SpvOp.Store, lvr.ptr_id, nxt)
        emit(bm, SEC_FUNCS, SpvOp.Branch, ids.header)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        ctx.terminated = false
        return expr
    }

    // ----- break / continue -> branch to the innermost loop's merge / continue block -----
    def override preVisitExprBreak(var expr : ExprBreak?) : void {
        var bm & = unsafe(*m)
        if (empty(ctx.loop_stack)) {
            errs |> push("break outside of a loop")
            return
        }
        emit(bm, SEC_FUNCS, SpvOp.Branch, ctx.loop_stack[length(ctx.loop_stack) - 1].merge)
        ctx.terminated = true
    }

    def override preVisitExprContinue(var expr : ExprContinue?) : void {
        var bm & = unsafe(*m)
        if (empty(ctx.loop_stack)) {
            errs |> push("continue outside of a loop")
            return
        }
        emit(bm, SEC_FUNCS, SpvOp.Branch, ctx.loop_stack[length(ctx.loop_stack) - 1].cont)
        ctx.terminated = true
    }

    // ----- reject dead code after a block terminator -----
    // SPIR-V forbids any instruction after a block terminator (OpReturn/OpBranch/...). The hand-walker
    // dropped such unreachable statements; the visitor can't skip a sibling subtree (no per-statement
    // canVisit gate), so fail closed instead: a statement reached while the current block is already
    // terminated is unreachable code (daslang only warns LINT001) and is rejected with a clean error.
    def override preVisitExprBlockExpression(blk : ExprBlock?; expr : ExpressionPtr) : void {
        if (ctx.terminated) {
            errs |> push("unreachable code after a block terminator (a statement following return/break/continue) is not supported in SPIR-V shaders")
        }
    }

    // ===== Fail-closed rejections (Phase 6.4) =====
    // Each shader-illegal construct gets an explicit preVisit override that records a clean error, so an
    // unsupported node can never be silently walked over into an empty/wrong blob. The value_of/ptr_of
    // backstop above is the complement: it catches a walked-but-unlowered node (incl. a future AST node
    // type we have no hook for) the moment a parent reads its missing id. Mirrors dasGlsl's discipline.
    def override preVisitExprNew(expr : ExprNew?) : void {
        errs |> push("new is not supported in SPIR-V shaders")
    }
    def override preVisitExprAscend(expr : ExprAscend?) : void {
        errs |> push("new (ascend) is not supported in SPIR-V shaders")
    }
    def override preVisitExprDelete(expr : ExprDelete?) : void {
        errs |> push("delete is not supported in SPIR-V shaders")
    }
    def override preVisitExprStringBuilder(expr : ExprStringBuilder?) : void {
        errs |> push("string builder is not supported in SPIR-V shaders")
    }
    def override preVisitExprNamedCall(expr : ExprNamedCall?) : void {
        errs |> push("named call is not supported in SPIR-V shaders")
    }
    def override preVisitExprNullCoalescing(expr : ExprNullCoalescing?) : void {
        errs |> push("null coalescing (??) is not supported in SPIR-V shaders")
    }
    def override preVisitExprCast(expr : ExprCast?) : void {
        errs |> push("cast is not supported in SPIR-V shaders")
    }
    def override preVisitExprPtr2Ref(expr : ExprPtr2Ref?) : void {
        errs |> push("pointer deref is not supported in SPIR-V shaders")
    }
    def override preVisitExprRef2Ptr(expr : ExprRef2Ptr?) : void {
        errs |> push("addr is not supported in SPIR-V shaders")
    }
    def override preVisitExprAddr(expr : ExprAddr?) : void {
        errs |> push("function address (@@) is not supported in SPIR-V shaders")
    }
    def override preVisitExprConstPtr(expr : ExprConstPtr?) : void {
        errs |> push("pointer constant is not supported in SPIR-V shaders")
    }
    def override preVisitExprSafeAt(expr : ExprSafeAt?) : void {
        errs |> push("safe index (?[]) is not supported in SPIR-V shaders")
    }
    def override preVisitExprSafeField(expr : ExprSafeField?) : void {
        errs |> push("safe field (?.) is not supported in SPIR-V shaders")
    }
    def override preVisitExprIsType(expr : ExprIs?; typeDecl : TypeDeclPtr) : void {
        errs |> push("'is' type test is not supported in SPIR-V shaders")
    }
    def override preVisitExprIsVariant(expr : ExprIsVariant?) : void {
        errs |> push("variant 'is' is not supported in SPIR-V shaders")
    }
    def override preVisitExprAsVariant(expr : ExprAsVariant?) : void {
        errs |> push("variant 'as' is not supported in SPIR-V shaders")
    }
    def override preVisitExprSafeAsVariant(expr : ExprSafeAsVariant?) : void {
        errs |> push("variant '?as' is not supported in SPIR-V shaders")
    }
    def override preVisitExprMoveRight(expr : ExprMove?; right : ExpressionPtr) : void {
        errs |> push("move (<-) is not supported in SPIR-V shaders")
    }
    def override preVisitExprCloneRight(expr : ExprClone?; right : ExpressionPtr) : void {
        errs |> push("clone (:=) is not supported in SPIR-V shaders")
    }
    def override preVisitExprWith(expr : ExprWith?) : void {
        errs |> push("with is not supported in SPIR-V shaders")
    }
    def override preVisitExprTryCatch(expr : ExprTryCatch?) : void {
        errs |> push("try/recover is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeVariant(expr : ExprMakeVariant?) : void {
        errs |> push("variant construction is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeTuple(expr : ExprMakeTuple?) : void {
        errs |> push("tuple construction is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeStruct(expr : ExprMakeStruct?) : void {
        errs |> push("struct construction is not supported in SPIR-V shaders")
    }
    def override canVisitExprMakeStructBody(expr : ExprMakeStruct?) : bool {
        return false   // already rejected in preVisit; don't descend into the (unlowerable) field inits
    }
    def override preVisitExprArrayComprehension(expr : ExprArrayComprehension?) : void {
        errs |> push("array comprehension is not supported in SPIR-V shaders")
    }
    def override preVisitExprTypeInfo(expr : ExprTypeInfo?) : void {
        errs |> push("typeinfo is not supported in SPIR-V shaders")
    }
    def override preVisitExprLabel(expr : ExprLabel?) : void {
        errs |> push("label is not supported in SPIR-V shaders")
    }
    def override preVisitExprGoto(expr : ExprGoto?) : void {
        errs |> push("goto is not supported in SPIR-V shaders")
    }
    def override preVisitExprAssert(expr : ExprAssert?) : void {
        errs |> push("assert is not supported in SPIR-V shaders")
    }
    def override preVisitExprStaticAssert(expr : ExprStaticAssert?) : void {
        errs |> push("static_assert is not supported in SPIR-V shaders")
    }
    def override preVisitExprQuote(expr : ExprQuote?) : void {
        errs |> push("quote is not supported in SPIR-V shaders")
    }
    def override preVisitExprDebug(expr : ExprDebug?) : void {
        errs |> push("debug is not supported in SPIR-V shaders")
    }
    def override preVisitExprInvoke(expr : ExprInvoke?) : void {
        errs |> push("invoke is not supported in SPIR-V shaders")
    }
    def override preVisitExprErase(expr : ExprErase?) : void {
        errs |> push("erase is not supported in SPIR-V shaders")
    }
    def override preVisitExprSetInsert(expr : ExprSetInsert?) : void {
        errs |> push("set insert is not supported in SPIR-V shaders")
    }
    def override preVisitExprFind(expr : ExprFind?) : void {
        errs |> push("find is not supported in SPIR-V shaders")
    }
    def override preVisitExprKeyExists(expr : ExprKeyExists?) : void {
        errs |> push("key_exists is not supported in SPIR-V shaders")
    }
    def override preVisitExprYield(expr : ExprYield?) : void {
        errs |> push("yield is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeBlock(expr : ExprMakeBlock?) : void {
        errs |> push("closure / block is not supported in SPIR-V shaders")
    }
    def override preVisitExprMakeGenerator(expr : ExprMakeGenerator?) : void {
        errs |> push("generator is not supported in SPIR-V shaders")
    }
    def override preVisitExprMemZero(expr : ExprMemZero?) : void {
        errs |> push("memzero is not supported in SPIR-V shaders")
    }
}

// Drive the SpirvEmit visitor over a function body. The visitor owns all body codegen; the module
// scaffold (caps / entry-point / OpFunction / trailing return) stays in generate_spirv.
def private drive_emit(var m : SpirvModule; var ctx : EmitCtx; fn : FunctionPtr; var errors : array<string>) {
    var v = new SpirvEmit(m = unsafe(addr(m)), ctx = unsafe(addr(ctx)))
    make_visitor(*v) $(mv_adapter) {
        v.adapter = mv_adapter
        visit(fn.body, v.adapter)
    }
    errors |> push_from(v.errs)
    unsafe {
        delete v
    }
}

// ===== user-defined shader functions =====
// A dependency function is emitted as its own OpFunction iff it is a real daslang function with a
// body that we control — not the entry point, not a host/extern builtin, and not a function whose
// calls are intercepted by name (math -> GLSL.std.450, spirv_builtins -> texture/imageStore/...).
def private is_user_shader_func(f : Function const?; entry : FunctionPtr) : bool {
    // `f == null` short-circuits first, so the later field reads never deref a null pointer.
    if (f == null || intptr(f) == intptr(entry) || f.flags.builtIn || f.body == null) return false
    // module "" is the user's own root module (a shader written without an explicit `module` line) —
    // a valid place for helper functions. Host/extern builtins are already excluded by flags.builtIn
    // above; the named modules below have their calls intercepted by name (math -> GLSL.std.450,
    // spirv_builtins -> texture/imageStore/...), so they must never be emitted as OpFunctions.
    let mn = "{f._module.name}"
    return !(mn == "builtin" || mn == "math" || mn == "math_bits" || mn == "spirv_builtins")
}

// Validate a user function's signature, allocate its OpFunction id + OpTypeFunction, and record it.
// Only 32-bit scalar/vector results+params are supported (by value, or by reference for params);
// anything else is rejected here with a clean error so a later call site doesn't emit invalid SPIR-V.
def private register_user_func(var m : SpirvModule; var ctx : EmitCtx; f : Function const?; var errors : array<string>) {
    if (key_exists(ctx.user_funcs, intptr(f))) return
    var uf = UserFunc(fn = f, valid = true, is_void = false, fn_id = 0u, type_id = 0u, ret_type = 0u)
    if (f.result.baseType == Type.tVoid) {
        uf.is_void = true
        uf.ret_type = type_void(m)
    } elif (is_emittable_value_type(f.result) && f.result.baseType != Type.tFixedArray) {
        uf.ret_type = emit_type(m, f.result)
    } else {
        errors |> push("shader function '{f.name}': return type {f.result.baseType} is not supported (only void and 32-bit scalar/vector/matrix results)")
        uf.valid = false
    }
    var ptypes : array<uint>
    for (arg in f.arguments) {
        let at = arg._type
        if (!is_emittable_value_type(at) || at.baseType == Type.tFixedArray) {
            errors |> push("shader function '{f.name}': parameter '{arg.name}' of type {at.baseType} is not supported (only 32-bit scalar/vector/matrix parameters, by value or by reference)")
            uf.valid = false
            continue
        }
        let vt = emit_type(m, at)
        if (at.flags.ref && !at.flags.constant) {
            ptypes |> push(type_pointer(m, SpvStorageClass.Function, vt))
        } else {
            ptypes |> push(vt)
        }
    }
    if (uf.valid) {
        uf.fn_id = alloc_id(m)
        uf.type_id = type_function(m, uf.ret_type, ptypes)
    }
    delete ptypes
    ctx.user_funcs |> insert(intptr(f), uf)
    ctx.user_order |> push(intptr(f))
}

// Mini-visitor: collect the user-function call targets directly invoked from one function body
// (for the recursion / call-graph check). Only records targets already in ctx.user_funcs.
class SpirvCalleeScan : AstVisitor {
    adapter : VisitorAdapter?
    @do_not_delete ctx : EmitCtx?
    callees : table<uint64>
    def override preVisitExprCall(expr : ExprCall?) : void {
        if (expr.func != null && key_exists(ctx.user_funcs, intptr(expr.func))) {
            callees |> insert(intptr(expr.func))
        }
    }
}

def private scan_callees(var ctx : EmitCtx; f : Function const?; var out : table<uint64>) {
    let fnc = unsafe(reinterpret<FunctionPtr>(f))   // read-only walk; strip pointee const for visit
    var v = new SpirvCalleeScan(ctx = unsafe(addr(ctx)))
    make_visitor(*v) $(mv_adapter) {
        v.adapter = mv_adapter
        visit(fnc.body, v.adapter)
    }
    for (k in keys(v.callees)) {
        out |> insert(k)
    }
    unsafe {
        delete v
    }
}

def private has_cycle_from(u : uint64; var adj : table<uint64; array<uint64>>; var color : table<uint64; int>) : bool {
    color[u] = 1
    if (key_exists(adj, u)) {
        for (w in adj[u]) {
            let c = color?[w] ?? 0
            if (c == 1 || (c == 0 && has_cycle_from(w, adj, color))) return true   // gray = back-edge -> cycle
        }
    }
    color[u] = 2
    return false
}

// SPIR-V (without recursion capability) forbids any function being part of a call cycle. Build the
// call graph over {entry} + user_funcs and report a clean error on any cycle (direct or mutual).
def private detect_recursion(var ctx : EmitCtx; entry : FunctionPtr; var errors : array<string>) : bool {
    var adj : table<uint64; array<uint64>>
    // entry node
    {
        var cs : table<uint64>
        scan_callees(ctx, entry, cs)
        adj[intptr(entry)] <- [for (k in keys(cs)); k]
        delete cs
    }
    for (key in ctx.user_order) {
        let uf = ctx.user_funcs?[key] ?? UserFunc()
        if (!uf.valid) continue
        var cs : table<uint64>
        scan_callees(ctx, uf.fn, cs)
        adj[key] <- [for (k in keys(cs)); k]
        delete cs
    }
    var color : table<uint64; int>
    var found = false
    if (has_cycle_from(intptr(entry), adj, color)) {
        found = true
    }
    for (key in ctx.user_order) {
        if (found) break
        if ((color?[key] ?? 0) == 0 && has_cycle_from(key, adj, color)) {
            found = true
        }
    }
    if (found) {
        errors |> push("recursion is not supported in SPIR-V shaders (a shader function is part of a call cycle)")
    }
    delete color
    delete adj
    return found
}

// Mini-visitor: hoist one Function-storage OpVariable into the current function's entry block for
// every by-reference argument of every user-function call in its body. Run after collect_locals and
// before the body walk, so the temps lead the entry block (SPIR-V requires all OpVariable there).
class SpirvTempAlloc : AstVisitor {
    adapter : VisitorAdapter?
    @do_not_delete m   : SpirvModule?
    @do_not_delete ctx : EmitCtx?
    def override preVisitExprCall(expr : ExprCall?) : void {
        let f = expr.func
        if (f == null || !key_exists(ctx.user_funcs, intptr(f))) return
        let uf = ctx.user_funcs?[intptr(f)] ?? UserFunc()
        if (!uf.valid) return
        var bm & = unsafe(*m)
        for (arg, i in expr.arguments, count()) {
            if (i >= length(f.arguments)) break
            let pt = f.arguments[i]._type
            if (pt.flags.ref && !pt.flags.constant) {
                if (key_exists(ctx.call_temps, intptr(arg))) continue
                let vt = emit_type(bm, pt)
                let ptr_t = type_pointer(bm, SpvStorageClass.Function, vt)
                let pid = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Variable, ptr_t, pid, uint(SpvStorageClass.Function))
                ctx.call_temps |> insert(intptr(arg), CallTemp(ptr_id = pid, value_type = vt))
            }
        }
    }
}

def private alloc_call_temps(var m : SpirvModule; var ctx : EmitCtx; f : FunctionPtr) {
    var v = new SpirvTempAlloc(m = unsafe(addr(m)), ctx = unsafe(addr(ctx)))
    make_visitor(*v) $(mv_adapter) {
        v.adapter = mv_adapter
        visit(f.body, v.adapter)
    }
    unsafe {
        delete v
    }
}

// Emit one user-defined shader function: OpFunction, its OpFunctionParameters (value params bind an
// SSA id; ref params bind a Function pointer), the entry block (locals + ref-arg temps), the body,
// and the trailing return. The result id / type were pre-allocated by register_user_func.
def private emit_user_function(var m : SpirvModule; var ctx : EmitCtx; uf : UserFunc; var errors : array<string>) {
    let f = unsafe(reinterpret<FunctionPtr>(uf.fn))   // read-only emission; strip pointee const
    emit(m, SEC_FUNCS, SpvOp.Function, uf.ret_type, uf.fn_id, 0u, uf.type_id)
    for (arg in f.arguments) {
        let at = arg._type
        let vt = emit_type(m, at)
        let pid = alloc_id(m)
        if (at.flags.ref && !at.flags.constant) {
            let pt = type_pointer(m, SpvStorageClass.Function, vt)
            emit(m, SEC_FUNCS, SpvOp.FunctionParameter, pt, pid)
            ctx.local_vars |> insert(intptr(arg), LocalVar(ptr_id = pid, value_type = vt))
        } else {
            emit(m, SEC_FUNCS, SpvOp.FunctionParameter, vt, pid)
            ctx.locals |> insert(intptr(arg), pid)
        }
    }
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    ctx.terminated = false
    collect_locals(m, ctx, f.body, errors)
    alloc_call_temps(m, ctx, f)
    drive_emit(m, ctx, f, errors)
    if (!ctx.terminated) {
        if (uf.is_void) {
            emit(m, SEC_FUNCS, SpvOp.Return)
        } else {
            errors |> push("shader function '{f.name}': not every control-flow path returns a value")
            emit(m, SEC_FUNCS, SpvOp.Unreachable)
        }
    }
    emit(m, SEC_FUNCS, SpvOp.FunctionEnd)
}

// ===== entry =====
def private execution_model(stage : ShaderStage) : SpvExecutionModel {
    if (stage == ShaderStage.Vertex) return SpvExecutionModel.Vertex
    if (stage == ShaderStage.Fragment) return SpvExecutionModel.Fragment
    if (stage == ShaderStage.Mesh) return SpvExecutionModel.MeshEXT
    if (stage == ShaderStage.Task) return SpvExecutionModel.TaskEXT
    return SpvExecutionModel.GLCompute
}

[macro_function]
def generate_spirv(fn : FunctionPtr; var errors : array<string>; stage : ShaderStage; local_size : int3; mesh_config : MeshOutputConfig; var reflection : SpirvReflection) : array<uint> {
    var m = make_module()
    var ctx : EmitCtx
    ctx.stage = stage

    // Fully overwrite the out-param: a caller that reuses a SpirvReflection (or retries after a
    // failed attempt) must not accumulate stale bindings / ranges from a previous call.
    reflection.stage = stage_flag(stage)
    reflection.entry_point := "main"
    // Mesh + Task workgroups are sized like compute via LocalSize, so the reflected local_size
    // mirrors the actual workgroup dispatch dims, not (1,1,1).
    let is_workgroupy = stage == ShaderStage.Compute || stage == ShaderStage.Mesh || stage == ShaderStage.Task
    reflection.local_size = is_workgroupy ? local_size : int3(1, 1, 1)
    reflection.bindings |> clear()
    reflection.push_constants |> clear()

    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
    emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))
    // Mesh / task stages pull in the mesh-shading capability + the SPV_EXT_mesh_shader extension.
    // Do this BEFORE the entry point so the capabilities section is closed and the extension is
    // visible by the time spirv-val checks the entry point's execution model. SPV_EXT_mesh_shader
    // requires SPIR-V >= 1.4, so the module header version is bumped from the 1.3 default; the other
    // stages stay at 1.3 (a 1.4 header would force every global into the entry-point interface).
    if (stage == ShaderStage.Mesh || stage == ShaderStage.Task) {
        m.version = SPV_VERSION_1_4
        ensure_mesh_shading(m, ctx)
    }
    // Mesh per-vertex / per-primitive Output arrays (gl_MeshVerticesEXT, gl_PrimitiveTriangleIndicesEXT)
    // are sized from the [mesh_shader] output config; classify_global reads these off ctx when it
    // emits the array types. Only the mesh stage has these outputs (task dispatches, it doesn't emit
    // geometry).
    ctx.mesh_max_vertices = mesh_config.max_vertices
    ctx.mesh_max_primitives = mesh_config.max_primitives
    ctx.mesh_topology = mesh_config.topology

    collect_dependencies(fn) $(vfun, vvar) {
        for (v in vvar) {
            classify_global(m, ctx, v, stage, reflection, errors)
        }
        // Register every user-defined dependency function (stable collect_dependencies order) so calls
        // can forward-reference a not-yet-emitted callee; their bodies are emitted after the entry point.
        for (uffn in vfun) {
            if (is_user_shader_func(uffn, fn)) {
                register_user_func(m, ctx, uffn, errors)
            }
        }
    }
    detect_recursion(ctx, fn, errors)

    let t_void = type_void(m)
    let t_fn = type_function_void(m, t_void)
    let id_main = alloc_id(m)

    // SPIR-V >= 1.4: the EntryPoint interface must list EVERY global the entry point references --
    // not only Input/Output (the <= 1.3 rule, already pushed by classify_global) but also Uniform /
    // StorageBuffer / PushConstant / UniformConstant / Workgroup. Append the globals not already
    // listed, ordered by var-id (= emission order) so the blob stays byte-reproducible.
    if (m.version >= SPV_VERSION_1_4) {
        var listed : table<uint>
        for (iid in ctx.interface_ids) {
            listed |> insert(iid)
        }
        var extra <- [for (gi in values(ctx.globals)); gi.var_id; where gi.var_id != 0u && !key_exists(listed, gi.var_id)]
        sort(extra)
        ctx.interface_ids |> push_from(extra)
        delete extra
        delete listed
    }

    var ep <- [uint(execution_model(stage)), id_main]
    var nm <- string_words("main")
    ep |> push_from(nm)
    delete nm
    ep |> push_from(ctx.interface_ids)
    emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
    delete ep

    // Compute pins the workgroup size; Fragment fixes the framebuffer origin; Vertex needs none.
    // Mesh + Task stages need LocalSize too; Mesh additionally needs OutputVertices,
    // OutputPrimitivesEXT, and an output-topology mode (OutputTrianglesEXT / OutputLinesEXT /
    // OutputPoints) — all three are MANDATORY per the SPV_EXT_mesh_shader spec.
    if (stage == ShaderStage.Compute || stage == ShaderStage.Mesh || stage == ShaderStage.Task) {
        let lx = local_size.x > 0 ? uint(local_size.x) : 1u
        let ly = local_size.y > 0 ? uint(local_size.y) : 1u
        let lz = local_size.z > 0 ? uint(local_size.z) : 1u
        var em <- [id_main, uint(SpvExecutionMode.LocalSize), lx, ly, lz]
        emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
        delete em
    }
    if (stage == ShaderStage.Mesh) {
        // OutputVertices / OutputPrimitivesEXT counts + the output topology come from the [mesh_shader]
        // annotation (defaults 64 / 64 / triangles). Fail-closed: a non-positive count can't size an
        // output array, so reject it here at the emit gate; clamp the (then-discarded) blob to 1 so it
        // still disassembles cleanly.
        var mv = mesh_config.max_vertices
        var mp = mesh_config.max_primitives
        if (mv <= 0) {
            errors |> push("mesh_shader: max_vertices must be > 0 (got {mv})")
            mv = 1
        }
        if (mp <= 0) {
            errors |> push("mesh_shader: max_primitives must be > 0 (got {mp})")
            mp = 1
        }
        emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(SpvExecutionMode.OutputVertices), uint(mv))
        emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(SpvExecutionMode.OutputPrimitivesEXT), uint(mp))
        var topo_mode = SpvExecutionMode.OutputTrianglesEXT
        if (mesh_config.topology == MeshOutputTopology.lines) {
            topo_mode = SpvExecutionMode.OutputLinesEXT
        } elif (mesh_config.topology == MeshOutputTopology.points) {
            topo_mode = SpvExecutionMode.OutputPoints
        }
        emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(topo_mode))
    }
    if (stage == ShaderStage.Fragment) {
        emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(SpvExecutionMode.OriginUpperLeft))
        // gl_FragDepth referenced (detected by classify_global above; Output-only, so a reference is a
        // write) -> the shader replaces the fixed-function depth.
        if (ctx.uses_frag_depth) {
            emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(SpvExecutionMode.DepthReplacing))
        }
    }

    emit(m, SEC_FUNCS, SpvOp.Function, t_void, id_main, 0u, t_fn)
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    ctx.terminated = false

    // Function-storage OpVariables must lead the entry block, before any body code.
    collect_locals(m, ctx, fn.body, errors)
    alloc_call_temps(m, ctx, fn)
    // Body codegen: the SpirvEmit AstVisitor walks fn.body, emitting into m.
    drive_emit(m, ctx, fn, errors)

    if (!ctx.terminated) {
        emit(m, SEC_FUNCS, SpvOp.Return)
    }
    emit(m, SEC_FUNCS, SpvOp.FunctionEnd)

    // user-defined shader functions, in stable registration order, after the entry point
    for (key in ctx.user_order) {
        let uf = ctx.user_funcs?[key] ?? UserFunc()
        if (uf.valid) {
            emit_user_function(m, ctx, uf, errors)
        }
    }

    var words <- module_words(m)
    delete m
    delete ctx
    return <- words
}
