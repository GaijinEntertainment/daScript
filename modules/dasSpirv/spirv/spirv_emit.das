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
require strings
require daslib/ast
require daslib/rtti
require daslib/ast_boost
// public: dasVulkan's host binder reaches compute_block_layout / BlockField through spirv_emit (its
// existing spelling), so re-export the shared std140 module to keep that consumer compiling unchanged.
require daslib/shader_block_layout public

// The shader stage drives the entry-point execution model + execution mode and which builtins
// are legal. Vertex needs no execution mode; Fragment needs OriginUpperLeft; Compute needs LocalSize.
// Mesh + Task (VK_EXT_mesh_shader) pull in the MeshShadingEXT capability + SPV_EXT_mesh_shader
// extension; both need LocalSize and Mesh additionally needs OutputVertices + OutputPrimitivesEXT
// + an output-topology mode (triangles / lines / points), all configurable via the [mesh_shader] args.
// RayGen + Miss + ClosestHit (VK_KHR_ray_tracing_pipeline) pull in the RayTracingKHR capability +
// SPV_KHR_ray_tracing extension (SPIR-V >= 1.4); none needs an execution mode. (AnyHit / Intersection /
// Callable follow the same rail and land with their first consumer.)
enum public ShaderStage {
    Compute
    Vertex
    Fragment
    Mesh
    Task
    RayGen
    Miss
    ClosestHit
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
    is_write_only : bool            // @writeonly storage image (NonReadable): imageLoad on it is a compile error (imageSize is a query, still allowed)
    is_read_only : bool             // @readonly storage image (NonWritable): imageStore into it is a compile error
    is_storage_image_array : bool   // a RUNTIME-SIZED descriptor array of storage images (array<image3D> etc.) -- UniformConstant OpTypeRuntimeArray of OpTypeImage; accessed via imgs[idx] -> OpAccessChain + OpLoad
    is_separate_image : bool        // a standalone sampled image (texture2D, OpTypeImage); combined at the call via OpSampledImage
    is_separate_sampler : bool      // a standalone sampler (OpTypeSampler)
    is_texel_buffer : bool          // a uniform texel buffer (textureBuffer, OpTypeImage Dim=Buffer); read via the two-argument texelFetch (no Lod: a buffer image has no mips)
    is_ssbo_array : bool            // a RUNTIME-SIZED descriptor array of SSBOs (array<array<T>>) -- StorageBuffer OpTypeRuntimeArray of the Block struct; accessed bufs[slot][i] -> ONE OpAccessChain (var, slot, 0, i)
    is_subpass_input : bool         // a subpass-input attachment (UniformConstant OpTypeImage Dim=SubpassData); used via subpassLoad
    is_sampler_array : bool         // a fixed-count descriptor array of combined image samplers (sampler2D[N], etc.) -- UniformConstant OpTypeArray of OpTypeSampledImage; accessed via samplers[idx] -> OpAccessChain
    is_mesh_output : bool           // a mesh per-vertex / per-primitive Output array (gl_MeshVerticesEXT / gl_PrimitiveTriangleIndicesEXT); a BARE Output array indexed by a single AccessChain (no member-0 SSBO indirection), pointee from elem_type
    is_stage_io_array : bool        // a bare @in / @out stage-I/O ARRAY (e.g. a @pervertex PerVertexKHR input int[3]); indexed by a single AccessChain off the variable, pointee via emit_type(element)
    arr_count  : int                // descriptor-array element count (when is_sampler_array; > 0); 0 for non-array
    elem_type  : uint               // SSBO runtime-array element type-id (vec/struct elements can't go through emit_type) OR descriptor-array element type-id (sampled-image / image / sampler)
    is_constant : bool              // a module-scope `let X = <const>` folded to an OpConstant; references resolve to const_id directly (no OpVariable / OpLoad)
    const_id   : uint               // the OpConstant result-id (when is_constant)
    is_accel   : bool               // an accelerationStructureEXT descriptor (UniformConstant OpTypeAccelerationStructureKHR); consumed by traceRayEXT
    is_accel_array : bool           // a RUNTIME-SIZED descriptor array of accelerationStructureEXT (array<accelerationStructureEXT>); UniformConstant OpTypeRuntimeArray of OpTypeAccelerationStructureKHR, indexed accels[idx] -> OpAccessChain + OpLoad before traceRayEXT
    is_ray_payload : bool           // a @ray_payload / @incoming_ray_payload variable; traceRayEXT's payload operand names its var_id
}

// A memory-backed local: an OpVariable in Function storage. Mutable locals (`var x`) and loop
// induction variables get one; reads become OpLoad, writes OpStore. The driver mem2reg's them,
// so we never hand-roll OpPhi. Immutable value-lets (`let x`) stay pure SSA ids (ctx.locals).
struct LocalVar {
    ptr_id     : uint    // OpVariable result-id (Function storage)
    value_type : uint    // SPIR-V type-id of the pointee
    is_opaque  : bool    // an opaque handle (rayQueryEXT): declared but never zero-initialized -- no OpStore of an OpConstantNull (opaque types have no constants)
    storage    : SpvStorageClass = SpvStorageClass.Function     // almost always Function; PhysicalStorageBuffer for a cm2 decode function's block-pointer parameter (chains + loads through it are PSB-classed and Aligned)
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
    sio16_cap : bool                            // OpCapability StorageInputOutput16 already emitted (16-bit varyings)
    input_attachment_cap : bool                 // OpCapability InputAttachment already emitted (subpassLoad)
    draw_parameters_cap : bool                  // OpCapability DrawParameters already emitted (gl_DrawID)
    descriptor_indexing_cap : bool              // OpCapability ShaderNonUniform + OpExtension "SPV_EXT_descriptor_indexing" already emitted (nonuniformEXT, descriptor arrays)
    sampled_image_nu_cap : bool                 // OpCapability SampledImageArrayNonUniformIndexing already emitted (NonUniform-indexed sampler descriptor array)
    mesh_shading_cap : bool                     // OpCapability MeshShadingEXT + OpExtension "SPV_EXT_mesh_shader" already emitted (mesh / task stages)
    ray_tracing_cap : bool                      // OpCapability RayTracingKHR + OpExtension "SPV_KHR_ray_tracing" already emitted (raygen / miss / closest-hit stages)
    ray_query_cap : bool                        // OpCapability RayQueryKHR + OpExtension "SPV_KHR_ray_query" already emitted (a compute-shader inline ray query: rayQueryEXT + OpRayQuery*)
    group_non_uniform_cap : bool                // OpCapability GroupNonUniform already emitted (any subgroup op or subgroup builtin)
    subgroup_feature_caps : table<uint>         // the per-family GroupNonUniform* capabilities already emitted (vote / ballot / arithmetic / shuffle / clustered)
    integer_dot_cap : bool                      // OpCapability DotProduct + DotProductInput4x8BitPacked + OpExtension "SPV_KHR_integer_dot_product" already emitted (sdot4)
    storage_image_ext_cap : bool                // OpCapability StorageImageExtendedFormats already emitted (a non-core storage-image @format, e.g. rgba16)
    image_cube_array_cap : bool                 // OpCapability ImageCubeArray already emitted (an imageCubeArray storage image)
    sampled_cube_array_cap : bool               // OpCapability SampledCubeArray already emitted (a samplerCubeArray)
    sampled_buffer_cap : bool                   // OpCapability SampledBuffer already emitted (a textureBuffer uniform texel buffer)
    runtime_descriptor_array_cap : bool         // OpCapability RuntimeDescriptorArray already emitted (a runtime-sized descriptor array)
    fragment_barycentric_cap : bool             // OpCapability FragmentBarycentricKHR + OpExtension "SPV_KHR_fragment_shader_barycentric" already emitted (gl_BaryCoordEXT / a @pervertex input)
    atomic_float_add_cap : bool                 // OpCapability AtomicFloat32AddEXT + OpExtension "SPV_EXT_shader_atomic_float_add" already emitted (a float atomicAdd)
    mesh_max_vertices : int                     // mesh-output array sizing (from MeshOutputConfig): gl_MeshVerticesEXT length
    mesh_max_primitives : int                   // mesh-output array sizing: gl_PrimitiveTriangleIndicesEXT length
    mesh_topology : MeshOutputTopology          // declared output topology; the index-array builtin must match it (triangles -> gl_PrimitiveTriangleIndicesEXT)
    local_size : int3                           // the declared workgroup size (LocalSize execution mode); gl_WorkGroupSize folds to it
    task_payload_id : uint                      // the @task_payload TaskPayloadWorkgroupEXT variable id (0 = none); EmitMeshTasksEXT appends it as the 4th operand
    nonuniform_ids : table<uint>                // set of result ids that carry the NonUniform decoration (nonuniformEXT marker + cascade through OpAccessChain / OpLoad on descriptor arrays)
    spec_ids      : table<uint>                 // SpecId decorations already used (@constant_id globals + local_size_*_id axes) -> duplicate ids rejected
    user_funcs    : table<uint64; UserFunc>     // intptr(Function) -> emittable user-defined shader function
    user_order    : array<uint64>               // user_funcs keys in stable emission order (collect_dependencies order)
    call_temps    : table<uint64; CallTemp>     // intptr(ref-arg Expression) -> its copy-in/out Function-storage temp
    decode_funcs  : table<uint64; DecodeFn>     // intptr(Function) -> registered cm2 decode function ([spirv_decode], referenced via @@ from coopmatLoadTensorDecode)
    decode_order  : array<uint64>               // decode_funcs keys in stable emission order (first-reference order)
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

// A cm2 DECODE function ([spirv_decode], SPV_NV_cooperative_matrix2): emitted as a real OpFunction
// with the rigid (PhysicalStorageBuffer block pointer, uint[2], uint[2]) signature and referenced by
// id as OpCooperativeMatrixLoadTensorNV's DecodeFunc operand. Ids pre-allocate at registration so the
// load arm can reference the not-yet-emitted body (bodies follow the user functions).
struct DecodeFn {
    fn      : Function const?
    fn_id   : uint          // OpFunction result id (0 when !valid)
    type_id : uint          // OpTypeFunction id
    ret_type : uint         // the tile component type (f16 / f32 / s8)
    block_type : uint       // the std430-packed Block struct the PSB pointer points at
    block_ptr_type : uint   // OpTypePointer PhysicalStorageBuffer <block_type>
    valid   : bool
}

// break -> branch to `merge`, continue -> branch to `cont`. Pushed per active loop.
struct LoopTargets {
    merge : uint
    cont  : uint
}

// ===== per-control-flow-node label ids (visitor side-maps; allocated to match the hand-walker's
// linear id order so the emitted blob stays byte-identical) =====
struct IteIds {
    merge     : uint
    then_l    : uint
    else_l    : uint   // == merge when there is no else branch (no separate else label emitted)
    then_term : bool   // then-branch ended terminated (recorded when the else block opens)
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
    delete c.spec_ids
    delete c.subgroup_feature_caps
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
    // subgroup. gl_SubgroupID / gl_NumSubgroups place the subgroup inside a WORKGROUP, so req=Compute
    // (widened to mesh / task by workgroup_builtin) is exactly their legal set. gl_SubgroupSize and
    // gl_SubgroupInvocationID have no execution-model restriction -- subgroup_any_stage_builtin lifts
    // the req check for those two.
    if (name == "gl_SubgroupSize") return (ok = true, bi = SpvBuiltIn.SubgroupSize, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_SubgroupInvocationID") return (ok = true, bi = SpvBuiltIn.SubgroupLocalInvocationId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_SubgroupID") return (ok = true, bi = SpvBuiltIn.SubgroupId, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
    if (name == "gl_NumSubgroups") return (ok = true, bi = SpvBuiltIn.NumSubgroups, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
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
    // capability on the fragment module (handled at classification). ALSO valid in a closest-hit shader
    // (the hit triangle's index within its geometry) -- rt_builtin_stage_ok widens the check there.
    if (name == "gl_PrimitiveID") return (ok = true, bi = SpvBuiltIn.PrimitiveId, storage = SpvStorageClass.Input, req = ShaderStage.Fragment)
    if (name == "gl_BaryCoordEXT") return (ok = true, bi = SpvBuiltIn.BaryCoordKHR, storage = SpvStorageClass.Input, req = ShaderStage.Fragment)
    // ray tracing (VK_KHR_ray_tracing_pipeline). `req` lists the PRIMARY stage; several are legal in
    // more than one RT stage -- rt_builtin_stage_ok encodes the full per-name stage sets.
    if (name == "gl_LaunchIDEXT") return (ok = true, bi = SpvBuiltIn.LaunchIdKHR, storage = SpvStorageClass.Input, req = ShaderStage.RayGen)
    if (name == "gl_LaunchSizeEXT") return (ok = true, bi = SpvBuiltIn.LaunchSizeKHR, storage = SpvStorageClass.Input, req = ShaderStage.RayGen)
    if (name == "gl_WorldRayOriginEXT") return (ok = true, bi = SpvBuiltIn.WorldRayOriginKHR, storage = SpvStorageClass.Input, req = ShaderStage.ClosestHit)
    if (name == "gl_WorldRayDirectionEXT") return (ok = true, bi = SpvBuiltIn.WorldRayDirectionKHR, storage = SpvStorageClass.Input, req = ShaderStage.ClosestHit)
    if (name == "gl_HitTEXT") return (ok = true, bi = SpvBuiltIn.RayTmaxKHR, storage = SpvStorageClass.Input, req = ShaderStage.ClosestHit)
    if (name == "gl_InstanceCustomIndexEXT") return (ok = true, bi = SpvBuiltIn.InstanceCustomIndexKHR, storage = SpvStorageClass.Input, req = ShaderStage.ClosestHit)
    return (ok = false, bi = SpvBuiltIn.Position, storage = SpvStorageClass.Input, req = ShaderStage.Compute)
}

// gl_SubgroupSize and gl_SubgroupInvocationID describe the invocation's place in its SUBGROUP, a
// notion every execution model has, so SPIR-V puts no stage restriction on them: any stage may read
// them under GroupNonUniform. Their builtin_info req is nominal -- this lifts it.
def private subgroup_any_stage_builtin(name : string) : bool {
    return name == "gl_SubgroupSize" || name == "gl_SubgroupInvocationID"
}

// The multi-stage validity sets for the ray-tracing builtins (and gl_PrimitiveID's closest-hit reuse).
// Returns ok=true when `name` is one of the multi-stage names AND `stage` is in its set; a name not
// listed here falls back to classify_global's exact `req == stage` check.
def private rt_builtin_stage_ok(name : string; stage : ShaderStage) : bool {
    // the launch grid: every ray-pipeline stage sees it
    if (name == "gl_LaunchIDEXT" || name == "gl_LaunchSizeEXT") {
        return is_rt_stage(stage)
    }
    // the world-space ray: miss + closest-hit (raygen has no current ray)
    if (name == "gl_WorldRayOriginEXT" || name == "gl_WorldRayDirectionEXT") {
        return stage == ShaderStage.Miss || stage == ShaderStage.ClosestHit
    }
    // hit-only state; gl_PrimitiveID doubles as the closest-hit triangle index (its fragment role
    // stays on the req == stage path)
    if (name == "gl_HitTEXT" || name == "gl_InstanceCustomIndexEXT" || name == "gl_PrimitiveID") {
        return stage == ShaderStage.ClosestHit
    }
    return false
}

def private stage_name(stage : ShaderStage) : string {
    if (stage == ShaderStage.Vertex) return "vertex"
    if (stage == ShaderStage.Fragment) return "fragment"
    if (stage == ShaderStage.Mesh) return "mesh"
    if (stage == ShaderStage.Task) return "task"
    if (stage == ShaderStage.RayGen) return "raygen"
    if (stage == ShaderStage.Miss) return "miss"
    if (stage == ShaderStage.ClosestHit) return "closest-hit"
    return "compute"
}

def private is_rt_stage(stage : ShaderStage) : bool {
    return stage == ShaderStage.RayGen || stage == ShaderStage.Miss || stage == ShaderStage.ClosestHit
}

// Pull the StorageBuffer8/16BitAccess capability for a sub-32-bit member reachable through an
// @ssbo (std430) layout. std140 blocks never see sub-32 members (compute_block_layout rejects
// them), so std140 calls are no-ops by construction.
def private ensure_member_storage_caps(var m : SpirvModule; bt : Type; rules : BlockLayoutRules) {
    if (rules != BlockLayoutRules.std430) return
    let w = scalar_width(bt)
    if (w == 16) {
        ensure_storage_buffer_16(m)
    } elif (w == 8) {
        ensure_storage_buffer_8(m)
    }
}

// Build a laid-out interface struct from a daslang struct's fields. Supported member shapes:
// scalar / vector / matrix; fixed-array of scalar / vector / nested struct (ArrayStride decoration
// on the OpTypeArray); nested struct (recursive non-Block OpTypeStruct with its own Offset cascade).
// `rules` picks the packing: std140 (@uniform / @push_constant — 32-bit members only) or std430
// (@ssbo element structs — tight strides, 8/16-bit members legal; the matching StorageBuffer8/16BitAccess
// capability is pulled per member width). `as_block` toggles the Block decoration on the *outer*
// struct (true for @uniform/@push_constant blocks, false for an SSBO runtime-array element struct);
// recursively-built nested structs are always emitted non-Block. Returns the OpTypeStruct id plus
// the laid-out size (offset past the last member) and the struct's max-member alignment (which the
// SSBO caller rounds the struct size up to before using it as the OpTypeRuntimeArray's stride).
def private build_block_struct(var m : SpirvModule; sname : string; st : Structure?; as_block : bool; var errors : array<string>; rules : BlockLayoutRules = BlockLayoutRules.std140) : tuple<ok : bool; type_id : uint; size : int; align : int> {
    var fields : array<BlockField>
    let layout = compute_block_layout(sname, st, fields, errors, rules)
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
                let inner = build_block_struct(m, "{sname}.{f.name}[]", et.structType, false, errors, rules)
                if (!inner.ok) {
                    delete members; delete offsets; delete mat_strides; delete fields
                    return (ok = false, type_id = 0u, size = 0, align = 0)
                }
                elem_type = inner.type_id
            } else {
                ensure_member_storage_caps(m, et.baseType, rules)
                elem_type = emit_type(m, et)
            }
            members |> push(type_array_strided(m, elem_type, f.array_count, f.array_stride))
            offsets |> push(f.offset)
            mat_strides |> push(0)
            fi++
            continue
        }
        if (f.is_struct) {
            let inner = build_block_struct(m, "{sname}.{f.name}", fld._type.structType, false, errors, rules)
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
        ensure_member_storage_caps(m, fld._type.baseType, rules)
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
// ArrayStride. A scalar/vector element strides by its std430 base size rounded up to its base
// alignment — width-aware, so 8/16-bit elements pack tight (int8 -> 1, half -> 2, half4 -> 8) and
// pull the matching StorageBuffer8/16BitAccess capability; a struct element is laid out non-Block
// under std430 rules and strides by its size rounded to the struct alignment. Matrix and
// nested-array elements are rejected (wrap a matrix in a struct).
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
        let bs = build_block_struct(m, "{sname} element", et.structType, false, errors, BlockLayoutRules.std430)
        if (!bs.ok) return (ok = false, elem_type = 0u, stride = 0)
        return (ok = true, elem_type = bs.type_id, stride = round_up(bs.size, bs.align))
    }
    // scalar or 2/3/4 vector, any lattice width (scalar_class rejects bool, non-numeric base
    // types, and the CPU-only 8/16-lane forms)
    if (component_count(bt) >= 1 && scalar_class(bt) >= 0) {
        ensure_member_storage_caps(m, bt, BlockLayoutRules.std430)
        return (ok = true, elem_type = emit_type(m, et),
            stride = round_up(block_scalar_size(bt), block_scalar_align(bt, BlockLayoutRules.std430)))
    }
    errors |> push("ssbo '{sname}' element type {bt} is not supported (an int/uint/float scalar of any lattice width, a 2/3/4-lane vector of those, or a struct of those)")
    return (ok = false, elem_type = 0u, stride = 0)
}

// The single SpirvStageFlags bit for one shader stage. A shader module is one stage, so every
// binding/range it reflects carries this single bit; the host ORs across stages when merging.
def private stage_flag(stage : ShaderStage) : SpirvStageFlags {
    if (stage == ShaderStage.Vertex) return SpirvStageFlags.vertex
    if (stage == ShaderStage.Fragment) return SpirvStageFlags.fragment
    if (stage == ShaderStage.Mesh) return SpirvStageFlags.mesh
    if (stage == ShaderStage.Task) return SpirvStageFlags.task
    if (stage == ShaderStage.RayGen) return SpirvStageFlags.raygen
    if (stage == ShaderStage.Miss) return SpirvStageFlags.miss
    if (stage == ShaderStage.ClosestHit) return SpirvStageFlags.closest_hit
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
    // samplerCubeArray: a LAYERED cubemap, sampled by a float4 (direction.xyz, layer). Its Arrayed=1
    // is what makes it need the SampledCubeArray capability, exactly as imageCubeArray needs ImageCubeArray.
    if (n == "samplerCubeArray") return (ok = true, dim = SpvDim.Cube, arrayed = 1u, depth = 0u)
    // sampler2DShadow: a 2D DEPTH-comparison sampler (Depth=1). Sampled only via textureCompare (Dref).
    if (n == "sampler2DShadow") return (ok = true, dim = SpvDim._2D, arrayed = 0u, depth = 1u)
    return (ok = false, dim = SpvDim._2D, arrayed = 0u, depth = 0u)
}

// Storage-image globals are recognized by their opaque marker struct name (image2D, ...). Returns the
// SPIR-V image dimensionality + Arrayed flag so classify_global can build the read-write OpTypeImage
// (sampled=2). The arrayed forms are addressed with an int3 whose last lane is the layer, exactly as
// GLSL spells them; a cube-arrayed storage image additionally needs the ImageCubeArray capability.
def private storage_image_info(t : TypeDecl?) : tuple<ok : bool; dim : SpvDim; arrayed : uint> {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return (ok = false, dim = SpvDim._2D, arrayed = 0u)
    let n = "{t.structType.name}"
    if (n == "image2D") return (ok = true, dim = SpvDim._2D, arrayed = 0u)
    if (n == "image3D") return (ok = true, dim = SpvDim._3D, arrayed = 0u)
    if (n == "image2DArray") return (ok = true, dim = SpvDim._2D, arrayed = 1u)
    if (n == "imageCubeArray") return (ok = true, dim = SpvDim.Cube, arrayed = 1u)
    return (ok = false, dim = SpvDim._2D, arrayed = 0u)
}

// Map a GLSL-style storage-image format qualifier (`@format = "r32f"`) to its SpvImageFormat. Only the
// float-sampled formats are supported -- integer formats (r32i/r32ui/...) would need int/uint
// imageLoad/imageStore overloads, a separate scope. ok=false -> caller emits a fail-closed error.
// `extended` marks a format outside SPIR-V's core storage-image set, which needs the
// StorageImageExtendedFormats capability; rgba16 (unorm) is one such extended format.
def private parse_storage_image_format(s : string) : tuple<ok : bool; fmt : SpvImageFormat; extended : bool> {
    if (s == "" || s == "rgba8") return (ok = true, fmt = SpvImageFormat.Rgba8, extended = false)
    if (s == "rgba8_snorm") return (ok = true, fmt = SpvImageFormat.Rgba8Snorm, extended = false)
    if (s == "rgba16f") return (ok = true, fmt = SpvImageFormat.Rgba16f, extended = false)
    if (s == "rgba32f") return (ok = true, fmt = SpvImageFormat.Rgba32f, extended = false)
    if (s == "r32f") return (ok = true, fmt = SpvImageFormat.R32f, extended = false)
    if (s == "rgba16") return (ok = true, fmt = SpvImageFormat.Rgba16, extended = true)
    return (ok = false, fmt = SpvImageFormat.Rgba8, extended = false)
}

// subpassInput is the opaque marker for a Vulkan subpass input attachment. Same recognition pattern
// as sampler/image globals -- a tStructure named "subpassInput" -> Dim=SubpassData OpTypeImage in
// UniformConstant storage (sampled=2, no format), decorated with InputAttachmentIndex, sampled via
// subpassLoad -> OpImageRead with an int2(0,0) coord.
def private is_subpass_input_type(t : TypeDecl?) : bool {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return false
    return "{t.structType.name}" == "subpassInput"
}

// Parse a WORKGROUP-scope cm2 marker name: coopmatWg{A|B|Acc}_{f16|f32|s8|s32}_{R}x{C}. The caller
// already verified the "coopmatWg" prefix. Name-parsed (not a table) so adding a tile shape is one
// struct declaration in spirv_builtins.das. One peek_data pass; `good` fails the parse closed.
def private coopmat_wg_info(n : string) : tuple<ok : bool; is_float : bool; signed : bool; comp_width : uint; rows : uint; cols : uint; use : uint> {
    var ok = false
    var isf = false
    var w = 0
    var r = 0
    var cc = 0
    var use = 0u
    peek_data(n) $(bytes) {
        let ln = length(bytes)
        var i = 9                      // past "coopmatWg"
        var good = true
        if (i + 2 < ln && bytes[i] == uint8('A') && bytes[i + 1] == uint8('c') && bytes[i + 2] == uint8('c')) {
            use = uint(SpvCooperativeMatrixUse.MatrixAccumulatorKHR)
            i += 3
        } elif (i < ln && bytes[i] == uint8('A')) {
            use = uint(SpvCooperativeMatrixUse.MatrixAKHR)
            i++
        } elif (i < ln && bytes[i] == uint8('B')) {
            use = uint(SpvCooperativeMatrixUse.MatrixBKHR)
            i++
        } else {
            good = false
        }
        if (good && i < ln && bytes[i] == uint8('_')) {
            i++
        } else {
            good = false
        }
        if (good && i < ln) {
            isf = bytes[i] == uint8('f')
            if (!isf && bytes[i] != uint8('s')) {
                good = false
            }
            i++
        } else {
            good = false
        }
        while (good && i < ln && bytes[i] != uint8('_')) {
            let c = int(bytes[i]) - '0'
            if (c < 0 || c > 9) {
                good = false
            } else {
                w = w * 10 + c
                i++
            }
        }
        if (good && i < ln && bytes[i] == uint8('_') && (w == 8 || w == 16 || w == 32)) {
            i++
        } else {
            good = false
        }
        while (good && i < ln && bytes[i] != uint8('x')) {
            let c = int(bytes[i]) - '0'
            if (c < 0 || c > 9) {
                good = false
            } else {
                r = r * 10 + c
                i++
            }
        }
        if (good && i < ln && bytes[i] == uint8('x') && r > 0) {
            i++
        } else {
            good = false
        }
        var any = false
        while (good && i < ln) {
            let c = int(bytes[i]) - '0'
            if (c < 0 || c > 9) {
                good = false
            } else {
                cc = cc * 10 + c
                i++
                any = true
            }
        }
        ok = good && any && cc > 0
    }
    return (ok = ok, is_float = isf, signed = !isf, comp_width = uint(w), rows = uint(r), cols = uint(cc), use = use)
}

// A cooperative-matrix local is recognized by its marker struct name (coopmatA_f16_16x16, ...). Returns
// the OpTypeCooperativeMatrixKHR parameters declare_local_var + the coopmat call arms need: the component
// type (float vs signed int + width), the tile's rows/cols/use (SpvCooperativeMatrixUse), and the SCOPE —
// Subgroup for the KHR v1 tiles below, Workgroup for the name-parsed cm2 coopmatWg* markers (whole-wg
// tiles, driver-managed staging). v1 tiles: f16xf16->f32 (16x16x16) and the native s8xs8->s32 int path
// (16x16x32, where A is 16x32 / B is 32x16 / accumulator 16x16 — the Q8-weight path, no dequant).
def private coopmat_info(t : TypeDecl?) : tuple<ok : bool; is_float : bool; signed : bool; comp_width : uint; rows : uint; cols : uint; use : uint; scope : uint> {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return (ok = false, is_float = false, signed = false, comp_width = 0u, rows = 0u, cols = 0u, use = 0u, scope = 0u)
    let n = "{t.structType.name}"
    let sg = uint(SpvScope.Subgroup)
    if (n == "coopmatA_f16_16x16") return (ok = true, is_float = true, signed = false, comp_width = 16u, rows = 16u, cols = 16u, use = uint(SpvCooperativeMatrixUse.MatrixAKHR), scope = sg)
    if (n == "coopmatB_f16_16x16") return (ok = true, is_float = true, signed = false, comp_width = 16u, rows = 16u, cols = 16u, use = uint(SpvCooperativeMatrixUse.MatrixBKHR), scope = sg)
    if (n == "coopmatAcc_f32_16x16") return (ok = true, is_float = true, signed = false, comp_width = 32u, rows = 16u, cols = 16u, use = uint(SpvCooperativeMatrixUse.MatrixAccumulatorKHR), scope = sg)
    if (n == "coopmatAcc_f16_16x16") return (ok = true, is_float = true, signed = false, comp_width = 16u, rows = 16u, cols = 16u, use = uint(SpvCooperativeMatrixUse.MatrixAccumulatorKHR), scope = sg)
    if (n == "coopmatA_s8_16x32") return (ok = true, is_float = false, signed = true, comp_width = 8u, rows = 16u, cols = 32u, use = uint(SpvCooperativeMatrixUse.MatrixAKHR), scope = sg)
    if (n == "coopmatB_s8_32x16") return (ok = true, is_float = false, signed = true, comp_width = 8u, rows = 32u, cols = 16u, use = uint(SpvCooperativeMatrixUse.MatrixBKHR), scope = sg)
    if (n == "coopmatAcc_s32_16x16") return (ok = true, is_float = false, signed = true, comp_width = 32u, rows = 16u, cols = 16u, use = uint(SpvCooperativeMatrixUse.MatrixAccumulatorKHR), scope = sg)
    if (n |> starts_with("coopmatWg")) {
        let wg = coopmat_wg_info(n)
        if (wg.ok) return (ok = true, is_float = wg.is_float, signed = wg.signed, comp_width = wg.comp_width, rows = wg.rows, cols = wg.cols, use = wg.use, scope = uint(SpvScope.Workgroup))
    }
    return (ok = false, is_float = false, signed = false, comp_width = 0u, rows = 0u, cols = 0u, use = 0u, scope = 0u)
}

// Materialize the OpTypeCooperativeMatrixKHR for a coopmat marker type (0 if `t` is not a coopmat).
// A Workgroup-scope (cm2) tile additionally pulls the SPV_NV_cooperative_matrix2 capability set.
def private coopmat_type_of(var m : SpirvModule; t : TypeDecl?) : uint {
    let ci = coopmat_info(t)
    if (!ci.ok) return 0u
    if (ci.scope == uint(SpvScope.Workgroup)) {
        ensure_cooperative_matrix2(m)
    }
    let comp = ci.is_float ? type_float(m, ci.comp_width) : type_int(m, ci.comp_width, ci.signed ? 1u : 0u)
    return type_cooperative_matrix(m, comp, ci.scope, ci.rows, ci.cols, ci.use)
}

// cm2 tensor layout/view marker -> its OpTypeTensorLayoutNV / OpTypeTensorViewNV id (0 = not one).
// tensorLayout2D = clamp Undefined (aligned fast path), tensorLayout2DPad = clamp Constant (OOB
// reads yield zero — the guarded edge path); tensorView2Dt = the (1,0) transpose permutation.
def private tensor_object_type_of(var m : SpirvModule; t : TypeDecl?) : tuple<id : uint; is_view : bool> {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) return (id = 0u, is_view = false)
    let n = "{t.structType.name}"
    if (n == "tensorLayout2D") return (id = type_tensor_layout(m, 2u, uint(SpvTensorClampMode.Undefined)), is_view = false)
    if (n == "tensorLayout2DPad") return (id = type_tensor_layout(m, 2u, uint(SpvTensorClampMode.Constant)), is_view = false)
    if (n == "tensorView2D") {
        var p <- [0u, 1u]
        let id = type_tensor_view(m, 2u, false, p)
        delete p
        return (id = id, is_view = true)
    }
    if (n == "tensorView2Dt") {
        var p <- [1u, 0u]
        let id = type_tensor_view(m, 2u, false, p)
        delete p
        return (id = id, is_view = true)
    }
    return (id = 0u, is_view = false)
}

// Exactly the type set emit_type lowers without panicking: a scalar/vector (scalar_class >= 0), bool,
// a matrix, or a fixed array of those. Lets the @workgroup path reject an unsupported shared type with
// a clean diagnostic instead of crashing in emit_type (struct/handle/dynamic-array shared vars).
def private is_emittable_value_type(t : TypeDecl?) : bool {
    if (t == null) return false
    if (matrix_info(t).ok) return true
    let bt = value_base_type(t)
    if (bt == Type.tFixedArray && t.firstType != null) return is_emittable_value_type(t.firstType)
    // The 16/32-bit numerics and the 64-bit float carry a value; arith_width_ok is the one place that
    // line is drawn, so the value rail and the arithmetic rail never disagree. A 64-bit INT lowers as
    // an interface-block MEMBER only (no arithmetic, no local): admitting it here would give it an
    // OpVariable no opcode can consume.
    let ni = numeric_info(bt)
    return (ni.ok && arith_width_ok(ni.cls, ni.width)) || bt == Type.tBool
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
        errors |> push("plain struct '{sname}' must be a struct with at least one field")
        return 0u
    }
    var members : array<uint>
    for (fld in st.fields) {
        // Scalar / vector / matrix / fixed-array-of-those all lower plainly via emit_type (no Block /
        // Offset / ArrayStride). Nested-struct fields would need a recursive PLAIN struct (build_block_struct
        // adds offsets, which TaskPayloadWorkgroupEXT must not have), so reject those fail-closed for now.
        if (!is_emittable_value_type(fld._type)) {
            errors |> push("plain struct '{sname}': field '{fld.name}' must be a scalar / vector / matrix / fixed array of those (nested-struct fields are not yet supported)")
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

// The undecorated OpTypeStruct behind a struct VALUE -- a struct-typed function parameter, result, or
// local. Deduped by Structure identity, which build_plain_struct itself does not do: a payload emits
// its type once, but a value struct is named by every signature and every construction that mentions
// it, and each would otherwise mint a fresh (and, to spirv-val, distinct) OpTypeStruct.
def private value_struct_type(var m : SpirvModule; st : Structure?; var errors : array<string>) : uint {
    if (st == null) {
        errors |> push("struct value: missing structure type")
        return 0u
    }
    let key = "vstruct_{intptr(st)}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = build_plain_struct(m, "{st.name}", st, errors)
    if (id != 0u) {
        m.type_pool |> insert(key, id)
    }
    return id
}

// emit_type, plus the struct case it panics on. Only the sites that legitimately carry a struct VALUE
// (function signatures, struct construction) go through this; everywhere else keeps emit_type, so a
// struct still cannot slip into a std140 block layout or a @workgroup variable.
def private emit_value_type(var m : SpirvModule; t : TypeDecl?; var errors : array<string>) : uint {
    if (t != null && t.baseType == Type.tStructure) {
        return value_struct_type(m, t.structType, errors)
    }
    return emit_type(m, t)
}

// ===== named layout-index constant resolution: @binding / @set / @location /
// @input_attachment_index accept a module-scope `let` int constant by name, not only a literal =====

// Fold a module-scope `let NAME = <int/uint const>` to its value. The layout annotations store an
// identifier argument as a STRING, so `@binding = NAME` is resolved here by scanning the compiling
// program's globals for a constant `let` of that name. The returned `found` is false when no such
// int-constant global exists, letting the caller error instead of silently using the fallback.
def private const_int_by_name(name : string) : tuple<found : bool; value : int> {
    var result = 0
    var got = false
    program_for_each_module(compiling_program()) $(mod) {
        for_each_global(mod) $(gv) {
            if (!got && gv.name == name && gv._type != null && gv._type.flags.constant && gv.init != null) {
                let ci = gv.init ?as ExprConstInt
                if (ci != null) {
                    result = ci.value
                    got = true
                } else {
                    let cu = gv.init ?as ExprConstUInt
                    if (cu != null) {
                        result = int(cu.value)
                        got = true
                    } else {
                        // A bool const folds to 0/1 -- lets a named boolean `let` (a config flag
                        // in its das form) resolve where an int constant is expected.
                        let cb = gv.init ?as ExprConstBool
                        if (cb != null) {
                            result = cb.value ? 1 : 0
                            got = true
                        }
                    }
                }
            }
        }
    }
    return (found = got, value = result)
}

// Read an integer annotation arg. A literal int/uint is used as-is; an identifier (stored as a string
// arg) resolves to a module-scope constant `let` of that name; an absent or wrong-typed arg yields
// `deflt` (the historical `?as tInt ?? deflt` fallback). An unresolvable name is a hard error.
// Used by the layout indices (@binding / @set / @location / @input_attachment_index / @enabled) and by
// the [<stage>_shader] macro for local_size_{x,y,z}, so a shader names a platform-configured workgroup
// size instead of hardcoding it. Public for spirv_shader.
def public annotation_int(ann : AnnotationArgumentList; argn : string; deflt : int; var errors : array<string>) : int {
    let rv = ann |> find_arg(argn)
    if (rv is tInt) {
        return rv as tInt
    }
    if (rv is tUInt) {
        return int(rv as tUInt)
    }
    if (rv is tBool) {              // a literal bool arg (e.g. @enabled = false) -> 0/1
        return (rv as tBool) ? 1 : 0
    }
    if (rv is tString) {
        let nm = rv as tString
        let r = const_int_by_name(nm)
        if (!r.found) {
            errors |> push("@{argn} = {nm}: not an integer constant `let` in scope")
            return deflt
        }
        return r.value
    }
    return deflt
}

// The workgroup dims the LocalSize execution mode declares (and gl_WorkGroupSize folds to). An
// unset dim is 1, so a shader that names only local_size_x still declares a well-formed 3D size.
def private local_size_dims(local_size : int3) : uint3 {
    return uint3(local_size.x > 0 ? uint(local_size.x) : 1u,
        local_size.y > 0 ? uint(local_size.y) : 1u,
        local_size.z > 0 ? uint(local_size.z) : 1u)
}

// ===== global variable classification + emission =====
def private classify_global(var m : SpirvModule; var ctx : EmitCtx; v : VariablePtr; stage : ShaderStage; var reflection : SpirvReflection; var errors : array<string>) {
    let vname = "{v.name}"
    // gl_WorkGroupSize is the one workgroup builtin that is a CONSTANT, not a dispatch input: it is the
    // shader's own local_size. SPIR-V spells it as a BuiltIn WorkgroupSize-decorated uvec3
    // OpConstantComposite -- no OpVariable, no entry-point interface entry (glslang emits exactly this,
    // and folds every read of it) -- so it classifies as a constant global, ahead of builtin_info's
    // Input/Output variable path. It is what lets a shared module that several shaders require (each
    // compiled with its own local_size) read the size of whichever shader is compiling it.
    if (vname == "gl_WorkGroupSize") {
        if (stage != ShaderStage.Compute && stage != ShaderStage.Mesh && stage != ShaderStage.Task) {
            errors |> push("builtin '{vname}' is not available in the {stage_name(stage)} stage")
            return
        }
        if (v._type == null || v._type.baseType != Type.tUInt3) {
            errors |> push("builtin '{vname}' must be declared uint3")
            return
        }
        let wgt = emit_type(m, v._type)
        let ls = local_size_dims(ctx.local_size)
        var parts <- [const_uint(m, ls.x), const_uint(m, ls.y), const_uint(m, ls.z)]
        let cid = const_composite(m, wgt, parts)
        delete parts
        emit(m, SEC_DECOR, SpvOp.Decorate, cid, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.WorkgroupSize))
        ctx.globals |> insert(intptr(v), GlobalInfo(value_type = wgt, is_constant = true, const_id = cid))
        return
    }
    let bi = builtin_info(vname)
    if (bi.ok) {
        // The workgroup-grid builtins (gl_GlobalInvocationID / gl_LocalInvocationID / gl_WorkGroupID /
        // gl_NumWorkGroups / gl_LocalInvocationIndex, all req=Compute in builtin_info) are equally valid
        // in MESH and TASK shaders -- those stages are workgroup-dispatched (LocalSize) too, and a real
        // mesh/task shader needs them to index its slice of the work (e.g. the payload survivor-list).
        // The ray-tracing builtins are legal in per-name SETS of RT stages (rt_builtin_stage_ok).
        let workgroup_builtin = bi.req == ShaderStage.Compute
        let stage_ok = (bi.req == stage ||
            (workgroup_builtin && (stage == ShaderStage.Mesh || stage == ShaderStage.Task)) ||
            subgroup_any_stage_builtin(vname) ||
            rt_builtin_stage_ok(vname, stage))
        if (!stage_ok) {
            errors |> push("builtin '{vname}' is not available in the {stage_name(stage)} stage")
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
        // The four subgroup builtins are not part of the base Shader cap: reading any of them declares
        // GroupNonUniform, with no per-family capability (that is the ops' business).
        if (bi.bi == SpvBuiltIn.SubgroupSize || bi.bi == SpvBuiltIn.SubgroupLocalInvocationId ||
            bi.bi == SpvBuiltIn.SubgroupId || bi.bi == SpvBuiltIn.NumSubgroups) {
            ensure_group_non_uniform(m, ctx)
        }
        // gl_PrimitiveID in a fragment of a mesh pipeline: the id is not fixed-function-generated, so
        // reading it pulls in MeshShadingEXT + SPV_EXT_mesh_shader (→ SPIR-V 1.4), same as the mesh stage.
        // In a CLOSEST-HIT shader the same builtin is the hit triangle's index, and the capability is
        // RayTracingKHR, already enabled by the stage itself -- so this extra is fragment-gated.
        if (bi.bi == SpvBuiltIn.PrimitiveId && stage == ShaderStage.Fragment) {
            ensure_per_primitive(m, ctx)
        }
        // gl_BaryCoordEXT -> BuiltIn BaryCoordKHR pulls in FragmentBarycentricKHR + the extension.
        if (bi.bi == SpvBuiltIn.BaryCoordKHR) {
            ensure_fragment_barycentric(m, ctx)
        }
        // An integer-typed fragment Input cannot be interpolated, so SPIR-V requires it to be Flat
        // (VUID-StandaloneSpirv-Flat-04744) -- gl_PrimitiveID and the subgroup builtins are the
        // int/uint-typed fragment Inputs emitted here. Flat is a fragment-interface rule: the same
        // builtins in a closest-hit or compute stage take no decoration.
        let builtin_cls = scalar_class(v._type.baseType)
        if (stage == ShaderStage.Fragment && bi.storage == SpvStorageClass.Input &&
            builtin_cls >= 0 && builtin_cls <= 1) {
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Flat))
        }
        // The subgroup builtins are the GroupNonUniform feature's data plane, not part of the base
        // Shader capability.
        if (bi.bi == SpvBuiltIn.SubgroupSize || bi.bi == SpvBuiltIn.SubgroupLocalInvocationId ||
                bi.bi == SpvBuiltIn.NumSubgroups || bi.bi == SpvBuiltIn.SubgroupId) {
            ensure_group_non_uniform(m, ctx)
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
        // @enabled = <compile-time int / bool const>: a stage I/O global with @enabled = false is
        // EXCLUDED from the interface at compile time -- the das mirror of GLSL's `#if COND` around a
        // `layout(location = N) in/out ...` declaration.  Absent -> enabled.  The matching read/write
        // is gated by `static_if (COND)` in the body (dropped before name resolution), so no reference
        // to the global survives when it is excluded, and no OpVariable / Location / interface entry is
        // emitted for it -- the blob is byte-for-byte what the COND-off GLSL produces.
        if (annotation_int(v.annotation, "enabled", 1, errors) == 0) {
            return
        }
        let loc = annotation_int(v.annotation, "location", -1, errors)
        if (loc < 0) {
            errors |> push("stage variable '{vname}' must specify a non-negative @location (e.g. @in @location = 0)")
            return
        }
        let storage = is_in ? SpvStorageClass.Input : SpvStorageClass.Output
        let is_per_prim = v.annotation |> find_arg("per_primitive") is tBool
        let is_pervertex = v.annotation |> find_arg("pervertex") is tBool
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
        // varying type validation (emit_type would panic on e.g. a double varying); 16-bit
        // varyings pull StorageInputOutput16; 8-bit varyings do not exist in Vulkan
        // (SPV_KHR_8bit_storage has no Input/Output storage class)
        if (!is_emittable_value_type(v._type)) {
            errors |> push("stage variable '{vname}': type {v._type.baseType} is not a supported varying type")
            return
        }
        let vw = scalar_width(v._type.baseType)
        if (vw == 8) {
            errors |> push("stage variable '{vname}': 8-bit varyings do not exist in Vulkan (SPV_KHR_8bit_storage has no Input/Output class)")
            return
        }
        if (vw == 16) {
            ensure_storage_input_output_16(m, ctx)
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
        // pervertexEXT (GL_EXT_fragment_shader_barycentric): a PerVertexKHR-decorated fragment input ARRAY
        // whose [i] fetches vertex i's raw per-vertex value (NOT interpolated), read alongside
        // gl_BaryCoordEXT to interpolate attributes by hand. Pulls in FragmentBarycentricKHR + the
        // extension. Per-vertex inputs carry no interpolation mode, so they must NOT be Flat (suppressed below).
        if (is_pervertex) {
            ensure_fragment_barycentric(m, ctx)
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.PerVertexKHR))
        }
        // Flat decoration. Vulkan mandates Flat on any integer/uint varying (no valid interpolation
        // mode exists for integers — the driver returns garbage without it), so emit it automatically
        // for those. Floats default to perspective-correct smooth interpolation; honour an explicit
        // @flat annotation when the user wants flat-shaded floats (low-poly stylised look, provoking-
        // vertex picks per primitive). Reflection bit not added — Flat is purely a SPIR-V decoration,
        // it doesn't change the descriptor or pipeline layout. A @pervertex input is never Flat.
        let bt = v._type.baseType
        let is_int_kind = scalar_class(bt) >= 0 && scalar_class(bt) <= 1
        let want_flat = (is_int_kind || (v.annotation |> find_arg("flat") is tBool)) && !is_pervertex
        if (want_flat) {
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Flat))
        }
        // A bare @in / @out ARRAY (e.g. the @pervertex int[3]) is indexed by a single AccessChain off the
        // variable -- record it so visitExprAt takes the bare-array path, not the SSBO member-0 path.
        let is_io_array = v._type != null && v._type.baseType == Type.tFixedArray
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = storage, value_type = vt, is_ssbo = false, is_stage_io_array = is_io_array))
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
            if (esi.dim == SpvDim.Cube && esi.arrayed == 1u) {
                ensure_sampled_cube_array(m, ctx)
            }
            let img = type_image(m, type_float(m, 32u), esi.dim, esi.arrayed, 1u, SpvImageFormat.Unknown, esi.depth)
            let simg = type_sampled_image(m, img)
            let arr = type_array(m, simg, n)
            let pt = type_pointer(m, SpvStorageClass.UniformConstant, arr)
            let vid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
            let set = annotation_int(v.annotation, "set", 0, errors)
            let binding = annotation_int(v.annotation, "binding", 0, errors)
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
        if (si.dim == SpvDim.Cube && si.arrayed == 1u) {
            ensure_sampled_cube_array(m, ctx)
        }
        let img = type_image(m, type_float(m, 32u), si.dim, si.arrayed, 1u, SpvImageFormat.Unknown, si.depth)
        let simg = type_sampled_image(m, img)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, simg)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
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
    // runtime-sized descriptor array of storage images (`array<image3D>`): the texture-pool surface,
    // one descriptor per pool slot, indexed by a per-dispatch slot id. Lowers to a UniformConstant
    // OpTypeRuntimeArray of OpTypeImage, read/written as imgs[idx] -> OpAccessChain + OpLoad. An opaque
    // element has no byte layout, so the array carries NO ArrayStride. The access qualifiers apply to
    // the whole array, exactly as GLSL's `uniform writeonly image3D pool[]` spells them.
    if (v._type != null && v._type.baseType == Type.tArray && v._type.firstType != null) {
        // runtime-sized descriptor array of top-level acceleration structures (`array<accelerationStructureEXT>`,
        // GLSL `uniform accelerationStructureEXT tlas[]`): one TLAS per slot, indexed by a per-lod accel id.
        // A UniformConstant OpTypeRuntimeArray of OpTypeAccelerationStructureKHR, traced via
        // traceRayEXT(tlas[idx], ...) -> OpAccessChain + OpLoad. RT-pipeline stages only.
        let aet = v._type.firstType
        let aestn = aet.baseType == Type.tStructure && aet.structType != null ? "{aet.structType.name}" : ""
        if (aestn == "accelerationStructureEXT") {
            if (!is_rt_stage(stage) && stage != ShaderStage.Compute) {
                errors |> push("accelerationStructureEXT array '{vname}' is only valid in a raygen / miss / closest-hit shader, or a compute shader that ray-queries it")
                return
            }
            // a compute stage reaches the TLAS through an INLINE ray query (SPV_KHR_ray_query), so it
            // declares RayQueryKHR itself -- the RT stages get RayTracingKHR from generate_spirv, but a
            // compute shader has no such call.
            if (stage == ShaderStage.Compute) {
                ensure_ray_query(m, ctx)
            }
            let aaset = annotation_int(v.annotation, "set", 0, errors)
            let aabinding = annotation_int(v.annotation, "binding", 0, errors)
            if (aaset < 0 || aabinding < 0) {
                errors |> push("accelerationStructureEXT array '{vname}' set/binding must be non-negative (got set={aaset}, binding={aabinding})")
                return
            }
            ensure_runtime_descriptor_array(m, ctx)
            let aavt = type_acceleration_structure(m)
            let aaarr = type_runtime_array_opaque(m, aavt)
            let aapt = type_pointer(m, SpvStorageClass.UniformConstant, aaarr)
            let aavid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, aapt, aavid, uint(SpvStorageClass.UniformConstant))
            emit(m, SEC_DECOR, SpvOp.Decorate, aavid, uint(SpvDecoration.DescriptorSet), uint(aaset))
            emit(m, SEC_DECOR, SpvOp.Decorate, aavid, uint(SpvDecoration.Binding), uint(aabinding))
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = aavid, storage = SpvStorageClass.UniformConstant,
                value_type = aavt, is_accel = true, is_accel_array = true, elem_type = aavt))
            // count = 0 marks a variable-descriptor-count binding: the host sizes the pool, not the shader.
            reflection.bindings |> push(SpirvDescriptorBinding(set = aaset, binding = aabinding,
                kind = SpirvDescriptorKind.acceleration_structure, count = 0, stages = stage_flag(stage)))
            return
        }
        // runtime-sized descriptor array of combined image samplers (`array<samplerCubeArray>`, GLSL
        // `uniform samplerCubeArray pool[]`): one sampled texture per pool slot, indexed by a
        // per-dispatch slot id.  The fixed-count form below is the same surface with a count; here the
        // host sizes the pool, so it is an OpTypeRuntimeArray + RuntimeDescriptorArray, and an opaque
        // element carries no ArrayStride.  Read via textureLod(pool[idx], ...) -> OpAccessChain + OpLoad.
        let rsi = sampler_info(v._type.firstType)
        if (rsi.ok) {
            let rset = annotation_int(v.annotation, "set", 0, errors)
            let rbinding = annotation_int(v.annotation, "binding", 0, errors)
            if (rset < 0 || rbinding < 0) {
                errors |> push("sampler descriptor array '{vname}' set/binding must be non-negative (got set={rset}, binding={rbinding})")
                return
            }
            if (rsi.dim == SpvDim.Cube && rsi.arrayed == 1u) {
                ensure_sampled_cube_array(m, ctx)
            }
            ensure_runtime_descriptor_array(m, ctx)
            let rimg = type_image(m, type_float(m, 32u), rsi.dim, rsi.arrayed, 1u, SpvImageFormat.Unknown, rsi.depth)
            let rsimg = type_sampled_image(m, rimg)
            let rarr = type_runtime_array_opaque(m, rsimg)
            let rpt = type_pointer(m, SpvStorageClass.UniformConstant, rarr)
            let rvid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, rpt, rvid, uint(SpvStorageClass.UniformConstant))
            emit(m, SEC_DECOR, SpvOp.Decorate, rvid, uint(SpvDecoration.DescriptorSet), uint(rset))
            emit(m, SEC_DECOR, SpvOp.Decorate, rvid, uint(SpvDecoration.Binding), uint(rbinding))
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = rvid, storage = SpvStorageClass.UniformConstant,
                value_type = rarr, is_sampler_array = true, arr_count = 0, elem_type = rsimg))
            let rkind = rsi.depth != 0u ? SpirvDescriptorKind.comparison_sampler : SpirvDescriptorKind.combined_image_sampler
            // count = 0 marks a variable-descriptor-count binding: the host sizes the pool, not the shader.
            reflection.bindings |> push(SpirvDescriptorBinding(set = rset, binding = rbinding,
                kind = rkind, count = 0, stages = stage_flag(stage)))
            return
        }
        let eii = storage_image_info(v._type.firstType)
        if (eii.ok) {
            let afmt_str = v.annotation |> find_arg("format") ?as tString ?? "rgba8"
            let apf = parse_storage_image_format(afmt_str)
            if (!apf.ok) {
                errors |> push("storage image array '{vname}': unsupported @format '{afmt_str}' (supported: rgba8, rgba8_snorm, rgba16, rgba16f, rgba32f, r32f)")
                return
            }
            if (apf.extended) {
                ensure_storage_image_extended(m, ctx)
            }
            if (eii.dim == SpvDim.Cube && eii.arrayed == 1u) {
                ensure_image_cube_array(m, ctx)
            }
            ensure_runtime_descriptor_array(m, ctx)
            let aimg = type_image(m, type_float(m, 32u), eii.dim, eii.arrayed, 2u, apf.fmt)
            let aarr = type_runtime_array_opaque(m, aimg)
            let apt = type_pointer(m, SpvStorageClass.UniformConstant, aarr)
            let avid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, apt, avid, uint(SpvStorageClass.UniformConstant))
            let aset = annotation_int(v.annotation, "set", 0, errors)
            let abinding = annotation_int(v.annotation, "binding", 0, errors)
            if (aset < 0 || abinding < 0) {
                errors |> push("storage image array '{vname}' set/binding must be non-negative (got set={aset}, binding={abinding})")
                return
            }
            emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.DescriptorSet), uint(aset))
            emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.Binding), uint(abinding))
            let awrite_only = v.annotation |> find_arg("writeonly") is tBool
            let aread_only = v.annotation |> find_arg("readonly") is tBool
            if (awrite_only && aread_only) {
                errors |> push("storage image array '{vname}' cannot be both @readonly and @writeonly")
                return
            }
            if (awrite_only) {
                emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.NonReadable))
            }
            if (aread_only) {
                emit(m, SEC_DECOR, SpvOp.Decorate, avid, uint(SpvDecoration.NonWritable))
            }
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = avid, storage = SpvStorageClass.UniformConstant,
                value_type = aarr, is_storage_image_array = true, is_write_only = awrite_only,
                is_read_only = aread_only, elem_type = aimg))
            // count = 0 marks a variable-descriptor-count binding: the host sizes the pool, not the shader.
            reflection.bindings |> push(SpirvDescriptorBinding(set = aset, binding = abinding,
                kind = SpirvDescriptorKind.storage_image, count = 0, stages = stage_flag(stage)))
            return
        }
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
            errors |> push("storage image '{vname}': unsupported @format '{fmt_str}' (supported: rgba8, rgba8_snorm, rgba16, rgba16f, rgba32f, r32f)")
            return
        }
        if (pf.extended) {
            ensure_storage_image_extended(m, ctx)
        }
        if (ii.dim == SpvDim.Cube && ii.arrayed == 1u) {
            ensure_image_cube_array(m, ctx)
        }
        let img = type_image(m, type_float(m, 32u), ii.dim, ii.arrayed, 2u, pf.fmt)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, img)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
        if (set < 0 || binding < 0) {
            errors |> push("storage image '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        // @writeonly -> NonReadable, @readonly -> NonWritable, both on the VARIABLE. A write-only
        // storage image is what GLSL's `writeonly image3D` declares, and dropping the qualifier would
        // silently widen the access the driver is allowed to assume.
        let write_only = v.annotation |> find_arg("writeonly") is tBool
        let read_only = v.annotation |> find_arg("readonly") is tBool
        if (write_only && read_only) {
            errors |> push("storage image '{vname}' cannot be both @readonly and @writeonly")
            return
        }
        if (write_only) {
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.NonReadable))
        }
        if (read_only) {
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.NonWritable))
        }
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant, value_type = img, is_storage_image = true,
            is_write_only = write_only, is_read_only = read_only))
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
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
        let iax = annotation_int(v.annotation, "input_attachment_index", -1, errors)
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
    // top-level acceleration structure (VK_KHR_ray_tracing_pipeline): the opaque accelerationStructureEXT
    // marker -> a UniformConstant OpTypeAccelerationStructureKHR descriptor, traced via traceRayEXT.
    // RT-pipeline stages only (ray QUERY from compute/fragment is a different extension and a separate
    // slice -- fail closed here rather than emit a blob spirv-val rejects).
    if (stn == "accelerationStructureEXT") {
        if (!is_rt_stage(stage) && stage != ShaderStage.Compute) {
            errors |> push("accelerationStructureEXT '{vname}' is only valid in a raygen / miss / closest-hit shader, or a compute shader that ray-queries it")
            return
        }
        // a compute stage reaches the TLAS through an INLINE ray query (SPV_KHR_ray_query); it must
        // declare RayQueryKHR itself (generate_spirv gives RayTracingKHR to RT stages only).
        if (stage == ShaderStage.Compute) {
            ensure_ray_query(m, ctx)
        }
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
        if (set < 0 || binding < 0) {
            errors |> push("accelerationStructureEXT '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        let vt = type_acceleration_structure(m)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant,
            value_type = vt, is_accel = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.acceleration_structure, count = 1, stages = stage_flag(stage)))
        return
    }
    // uniform texel buffer: the opaque textureBuffer marker -> a UniformConstant OpTypeImage with
    // Dim=Buffer (Sampled=1, no format), read by element index via the two-argument texelFetch. The
    // host binds a VkBufferView, not an image view, so this is a descriptor kind of its own.
    if (stn == "textureBuffer") {
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
        if (set < 0 || binding < 0) {
            errors |> push("textureBuffer '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        ensure_sampled_buffer(m, ctx)
        let vt = type_image(m, type_float(m, 32u), SpvDim.Buffer)
        let pt = type_pointer(m, SpvStorageClass.UniformConstant, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.UniformConstant))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.UniformConstant,
            value_type = vt, is_texel_buffer = true))
        reflection.bindings |> push(SpirvDescriptorBinding(set = set, binding = binding,
            kind = SpirvDescriptorKind.uniform_texel_buffer, count = 1, stages = stage_flag(stage)))
        return
    }
    if (stn == "texture2D" || stn == "sampler") {
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
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
        // single-struct SSBO block: @ssbo struct -> StorageBuffer + Block-decorated struct (GLSL
        // `buffer Block { S s; }`), member reads via ExprField -> OpAccessChain by field index, like a
        // UBO but writable and in StorageBuffer storage. Reuses build_block_struct's std140 layout,
        // which equals std430 for a struct of 32-bit scalars/vectors.
        if (v._type.baseType == Type.tStructure && v._type.structType != null) {
            let sbs = build_block_struct(m, vname, v._type.structType, true, errors)
            if (!sbs.ok) return
            let spt = type_pointer(m, SpvStorageClass.StorageBuffer, sbs.type_id)
            let svid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, spt, svid, uint(SpvStorageClass.StorageBuffer))
            let sset = annotation_int(v.annotation, "set", 0, errors)
            let sbinding = annotation_int(v.annotation, "binding", 0, errors)
            if (sset < 0 || sbinding < 0) {
                errors |> push("ssbo '{vname}' set/binding must be non-negative (got set={sset}, binding={sbinding})")
                return
            }
            emit(m, SEC_DECOR, SpvOp.Decorate, svid, uint(SpvDecoration.DescriptorSet), uint(sset))
            emit(m, SEC_DECOR, SpvOp.Decorate, svid, uint(SpvDecoration.Binding), uint(sbinding))
            let sro = v.annotation |> find_arg("readonly") is tBool
            let smro = v.annotation |> find_arg("maybe_readonly") is tBool
            let sis_render = stage == ShaderStage.Vertex || stage == ShaderStage.Fragment
            if (sro || (smro && sis_render)) {
                emit(m, SEC_DECOR, SpvOp.Decorate, svid, uint(SpvDecoration.NonWritable))
            }
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = svid, storage = SpvStorageClass.StorageBuffer, value_type = sbs.type_id, is_ssbo = false, is_block = true))
            reflection.bindings |> push(SpirvDescriptorBinding(set = sset, binding = sbinding, kind = SpirvDescriptorKind.storage_buffer, count = 1, stages = stage_flag(stage)))
            return
        }
        if (v._type.baseType != Type.tArray || v._type.firstType == null) {
            errors |> push("ssbo '{vname}' must be an array<T> or a struct")
            return
        }
        // array<array<T>> is a runtime-sized DESCRIPTOR ARRAY of SSBOs (GLSL `buffer Block { T d[]; }
        // g[]`): one buffer per slot, indexed g[slot][i]. The OUTER runtime array is over the Block
        // struct and carries NO ArrayStride -- a descriptor slot has no byte layout -- while the inner
        // one strides std430 like any SSBO. Access is a single OpAccessChain (var, slot, 0, i).
        let dat = v._type.firstType
        if (dat.baseType == Type.tArray && dat.firstType != null) {
            let del = ssbo_element_layout(m, vname, dat.firstType, errors)
            if (!del.ok) return
            let dset = annotation_int(v.annotation, "set", 0, errors)
            let dbinding = annotation_int(v.annotation, "binding", 0, errors)
            if (dset < 0 || dbinding < 0) {
                errors |> push("ssbo array '{vname}' set/binding must be non-negative (got set={dset}, binding={dbinding})")
                return
            }
            ensure_runtime_descriptor_array(m, ctx)
            let drta = type_runtime_array(m, del.elem_type, del.stride)
            var dmembers <- [drta]
            var doffsets <- [0]
            let dst = type_struct_block(m, dmembers, doffsets)
            delete dmembers
            delete doffsets
            let douter = type_runtime_array_opaque(m, dst)
            let dpt = type_pointer(m, SpvStorageClass.StorageBuffer, douter)
            let dvid = alloc_id(m)
            emit(m, SEC_TYPES, SpvOp.Variable, dpt, dvid, uint(SpvStorageClass.StorageBuffer))
            emit(m, SEC_DECOR, SpvOp.Decorate, dvid, uint(SpvDecoration.DescriptorSet), uint(dset))
            emit(m, SEC_DECOR, SpvOp.Decorate, dvid, uint(SpvDecoration.Binding), uint(dbinding))
            let dread_only = v.annotation |> find_arg("readonly") is tBool
            let dmaybe_read_only = v.annotation |> find_arg("maybe_readonly") is tBool
            let dis_render = stage == ShaderStage.Vertex || stage == ShaderStage.Fragment
            if (dread_only || (dmaybe_read_only && dis_render)) {
                emit(m, SEC_DECOR, SpvOp.Decorate, dvid, uint(SpvDecoration.NonWritable))
            }
            ctx.globals |> insert(intptr(v), GlobalInfo(var_id = dvid, storage = SpvStorageClass.StorageBuffer,
                value_type = douter, is_ssbo_array = true, elem_type = del.elem_type))
            // count = 0 marks a variable-descriptor-count binding: the host sizes the pool, not the shader.
            reflection.bindings |> push(SpirvDescriptorBinding(set = dset, binding = dbinding,
                kind = SpirvDescriptorKind.storage_buffer, count = 0, stages = stage_flag(stage)))
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
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
        if (set < 0 || binding < 0) {
            errors |> push("ssbo '{vname}' set/binding must be non-negative (got set={set}, binding={binding})")
            return
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.DescriptorSet), uint(set))
        emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.Binding), uint(binding))
        // @readonly -> NonWritable on the VARIABLE, not the block member: the
        // block struct type is deduped, so a member decoration would leak to a
        // writable SSBO sharing the same element type.
        // @maybe_readonly is the GLSL MAYBE_READONLY macro: NonWritable only in a rendering (vertex /
        // fragment) stage, writable in compute / RT -- mirrors `readonly` expanding only under the GLSL
        // RENDERING define, so a buffer shared by both rails matches each stage's glslang reference.
        let ssbo_read_only = v.annotation |> find_arg("readonly") is tBool
        let ssbo_maybe_read_only = v.annotation |> find_arg("maybe_readonly") is tBool
        let ssbo_is_render = stage == ShaderStage.Vertex || stage == ShaderStage.Fragment
        if (ssbo_read_only || (ssbo_maybe_read_only && ssbo_is_render)) {
            emit(m, SEC_DECOR, SpvOp.Decorate, vid, uint(SpvDecoration.NonWritable))
        }
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
        let set = annotation_int(v.annotation, "set", 0, errors)
        let binding = annotation_int(v.annotation, "binding", 0, errors)
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
    // per-invocation private memory: @private_global -> Private storage (a plain module-scope global in
    // GLSL, one copy per invocation). Not a descriptor and not in the <= 1.3 entry interface (Private is
    // not Input/Output), so no set/binding and nothing reflected. Valid in ANY stage (unlike @workgroup).
    // Read/written via OpLoad/OpStore; a fixed-array private global is indexed by visitExprAt's bare-array
    // branch. (`private` is a reserved keyword, so the annotation is spelled @private_global.)
    if (v.annotation |> find_arg("private_global") is tBool) {
        if (!is_emittable_value_type(v._type)) {
            errors |> push("private variable '{vname}': @private_global type must be a scalar, vector, matrix, or fixed array of those")
            return
        }
        let vt = emit_type(m, v._type)
        let pt = type_pointer(m, SpvStorageClass.Private, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(SpvStorageClass.Private))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = SpvStorageClass.Private, value_type = vt, is_ssbo = false))
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
    // ray payloads + hit attributes (VK_KHR_ray_tracing_pipeline) -- three logical storage classes,
    // all plain-laid-out like @task_payload:
    //   @ray_payload           RayPayloadKHR          the OUTGOING payload a stage passes to traceRayEXT
    //                                                 (raygen; also miss / closest-hit for recursive rays)
    //   @incoming_ray_payload  IncomingRayPayloadKHR  the payload THIS stage was invoked with
    //                                                 (miss / closest-hit; they write the result here)
    //   @hit_attribute         HitAttributeKHR        the triangle barycentrics (float2 on the fixed-function
    //                                                 triangle path), closest-hit read-only
    // Payloads and hit attributes carry NO Location decoration: glslang emits none on a rayPayloadEXT /
    // rayPayloadInEXT / hitAttributeEXT variable (OpTraceRayKHR names the payload by id and the SBT links
    // incoming to outgoing -- payloads are not location-interface-matched), so an @location on a payload is
    // accepted but ignored, matching the glslang reference. Struct payloads lay out plainly (build_plain_struct,
    // member access via the is_block path); scalar / vector / matrix payloads emit directly and load/store
    // whole.
    let is_ray_payload = v.annotation |> find_arg("ray_payload") is tBool
    let is_incoming_payload = v.annotation |> find_arg("incoming_ray_payload") is tBool
    let is_hit_attribute = v.annotation |> find_arg("hit_attribute") is tBool
    if (is_ray_payload || is_incoming_payload || is_hit_attribute) {
        let ann_name = is_ray_payload ? "@ray_payload" : (is_incoming_payload ? "@incoming_ray_payload" : "@hit_attribute")
        if (is_ray_payload && !is_rt_stage(stage)) {
            errors |> push("{ann_name} '{vname}' is only valid in a raygen / miss / closest-hit shader")
            return
        }
        if (is_incoming_payload && stage != ShaderStage.Miss && stage != ShaderStage.ClosestHit) {
            errors |> push("{ann_name} '{vname}' is only valid in a miss or closest-hit shader")
            return
        }
        if (is_hit_attribute && stage != ShaderStage.ClosestHit) {
            errors |> push("{ann_name} '{vname}' is only valid in a closest-hit shader")
            return
        }
        var storage = SpvStorageClass.RayPayloadKHR
        if (is_incoming_payload) {
            storage = SpvStorageClass.IncomingRayPayloadKHR
        } elif (is_hit_attribute) {
            storage = SpvStorageClass.HitAttributeKHR
        }
        var vt : uint
        var as_block = false
        if (v._type.baseType == Type.tStructure && v._type.structType != null) {
            vt = build_plain_struct(m, vname, v._type.structType, errors)
            if (vt == 0u) return
            as_block = true
        } elif (is_emittable_value_type(v._type)) {
            vt = emit_type(m, v._type)
        } else {
            errors |> push("{ann_name} '{vname}' must be a struct or a scalar / vector / matrix / fixed array of those")
            return
        }
        let pt = type_pointer(m, storage, vt)
        let vid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, pt, vid, uint(storage))
        ctx.globals |> insert(intptr(v), GlobalInfo(var_id = vid, storage = storage, value_type = vt,
            is_block = as_block, is_ray_payload = !is_hit_attribute))
        // not pushed into interface_ids here: RT modules are SPIR-V 1.4, whose every-referenced-global
        // interface pass appends these (they are not Input/Output, so the <= 1.3 rule never applies)
        return
    }
    // specialization constant: a module-scope 32-bit scalar with `@constant_id = N`. Emitted as a
    // fresh OpSpecConstant -- NEVER through the const pool: two spec constants with equal defaults
    // must stay distinct ids, and a spec constant must not alias the plain OpConstant of its default
    // -- plus OpDecorate SpecId. References resolve like a folded constant (is_constant/const_id);
    // the host overrides the default at pipeline-create time via VkSpecializationInfo.
    if (v.annotation |> find_arg("constant_id") is tInt) {
        let cid = v.annotation |> find_arg("constant_id") ?as tInt ?? 0
        if (cid < 0) {
            errors |> push("spec constant '{vname}': @constant_id must be non-negative (got {cid})")
            return
        }
        if (v._type == null || (v._type.baseType != Type.tUInt && v._type.baseType != Type.tInt &&
                                v._type.baseType != Type.tFloat && v._type.baseType != Type.tBool)) {
            errors |> push("spec constant '{vname}' must be a 32-bit scalar (uint, int, float, or bool)")
            return
        }
        if (key_exists(ctx.spec_ids, uint(cid))) {
            errors |> push("spec constant '{vname}': duplicate @constant_id {cid}")
            return
        }
        var ie = v.init
        let r2v = ie ?as ExprRef2Value
        if (r2v != null) {
            ie = r2v.subexpr
        }
        var has_default = false
        var bits = 0u
        var bool_default = false
        if (ie != null) {
            let cu = ie ?as ExprConstUInt
            let ci = ie ?as ExprConstInt
            let cf = ie ?as ExprConstFloat
            let cb = ie ?as ExprConstBool
            if (cu != null) {
                bits = cu.value
                has_default = true
            } elif (ci != null) {
                bits = uint(ci.value)
                has_default = true
            } elif (cf != null) {
                bits = unsafe(reinterpret<uint>(cf.value))
                has_default = true
            } elif (cb != null) {
                bool_default = cb.value
                has_default = true
            }
            // depending on fold state `-3` can arrive as ExprOp1("-", const) rather than a folded
            // negative literal (same shape emit_const handles) — fold it here so both parse states
            // produce the same default bits
            if (!has_default) {
                let neg = ie ?as ExprOp1
                if (neg != null && "{neg.op}" == "-") {
                    let ir2v = neg.subexpr ?as ExprRef2Value
                    let sub = ir2v != null ? ir2v.subexpr : neg.subexpr
                    let nci = sub ?as ExprConstInt
                    let ncf = sub ?as ExprConstFloat
                    if (nci != null) {
                        bits = uint(-nci.value)
                        has_default = true
                    } elif (ncf != null) {
                        bits = unsafe(reinterpret<uint>(-ncf.value))
                        has_default = true
                    }
                }
            }
        }
        if (!has_default) {
            errors |> push("spec constant '{vname}' needs a literal scalar initializer (its default value)")
            return
        }
        let tid = emit_type(m, v._type)
        let sid = alloc_id(m)
        if (v._type.baseType == Type.tBool) {
            emit(m, SEC_TYPES, bool_default ? SpvOp.SpecConstantTrue : SpvOp.SpecConstantFalse, tid, sid)
        } else {
            emit(m, SEC_TYPES, SpvOp.SpecConstant, tid, sid, bits)
        }
        emit(m, SEC_DECOR, SpvOp.Decorate, sid, uint(SpvDecoration.SpecId), uint(cid))
        ctx.spec_ids |> insert(uint(cid))
        ctx.globals |> insert(intptr(v), GlobalInfo(value_type = tid, is_constant = true, const_id = sid))
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
    errors |> push("unsupported global '{vname}' (expected gl_* builtin, @ssbo, @uniform, @push_constant, @workgroup, @ray_payload / @incoming_ray_payload / @hit_attribute, a constant `let`, or an opaque type e.g. sampler2D / image2D / subpassInput / accelerationStructureEXT)")
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

// The LOCAL/argument twin of root_global_of: peel to the underlying ExprVar and return its Variable
// when it is a local, argument, or block var (null otherwise). Lets root_storage_of read a memory
// local's storage class — a cm2 decode function's block parameter is PhysicalStorageBuffer, not Function.
def private root_local_var_of(e : ExpressionPtr) : Variable const? {
    if (e == null) return null
    let ev = e ?as ExprVar
    if (ev != null) {
        if (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block) return ev.variable
        return null
    }
    let at = e ?as ExprAt
    if (at != null) return root_local_var_of(at.subexpr)
    let fld = e ?as ExprField
    if (fld != null) return root_local_var_of(fld.value)
    let sw = e ?as ExprSwizzle
    if (sw != null) return root_local_var_of(sw.value)
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return root_local_var_of(r2v.subexpr)
    return null
}

// ===== atomic call name + operand class -> SPIR-V opcode =====
// cls: 0 = signed int, 1 = unsigned int, 2 = float. atomicMin/Max pick the signed (SMin/SMax) or
// unsigned (UMin/UMax) opcode by the operand's signedness; the rest are sign-agnostic. atomicAdd is
// the one float-capable family (OpAtomicFAddEXT, VK_EXT_shader_atomic_float); every other name stays
// int-only and the caller rejects a float target. compswap flags the OpAtomicCompareExchange shape
// (an extra comparator operand + a second semantics id).
def private atomic_info(name : string; cls : int) : tuple<ok : bool; op : SpvOp; compswap : bool> {
    if (name == "atomicAdd") return (ok = true, op = cls == 2 ? SpvOp.AtomicFAddEXT : SpvOp.AtomicIAdd, compswap = false)
    if (name == "atomicMin") return (ok = true, op = cls == 1 ? SpvOp.AtomicUMin : SpvOp.AtomicSMin, compswap = false)
    if (name == "atomicMax") return (ok = true, op = cls == 1 ? SpvOp.AtomicUMax : SpvOp.AtomicSMax, compswap = false)
    if (name == "atomicAnd") return (ok = true, op = SpvOp.AtomicAnd, compswap = false)
    if (name == "atomicOr") return (ok = true, op = SpvOp.AtomicOr, compswap = false)
    if (name == "atomicXor") return (ok = true, op = SpvOp.AtomicXor, compswap = false)
    if (name == "atomicExchange") return (ok = true, op = SpvOp.AtomicExchange, compswap = false)
    if (name == "atomicCompSwap") return (ok = true, op = SpvOp.AtomicCompareExchange, compswap = true)
    return (ok = false, op = SpvOp.AtomicIAdd, compswap = false)
}

// ===== subgroup call name + operand class -> SPIR-V opcode, operand shape, capability =====
// The operand shape after the usual <result-type> <result-id> <Subgroup scope>:
//   Vote      a bool predicate                                 (OpGroupNonUniformAll / Any)
//   Elect     nothing further                                  (OpGroupNonUniformElect)
//   Unary     a value                                          (OpGroupNonUniformBroadcastFirst)
//   ValueId   a value + an invocation id or lane mask          (OpGroupNonUniformBroadcast / Shuffle / ShuffleXor)
//   Reduce    GroupOperation Reduce, then a value              (the arithmetic reductions)
//   Clustered GroupOperation ClusteredReduce, a value, a size  (subgroupClusteredAdd)
enum private SubgroupForm {
    Vote
    Elect
    Unary
    ValueId
    Reduce
    Clustered
}

// cls: 0 = signed int, 1 = unsigned int, 2 = float -- the arithmetic reductions pick their S/U/F opcode
// from it, mirroring atomic_info. Vote / Elect / Unary / ValueId are class-agnostic.
def private subgroup_info(name : string; cls : int) : tuple<ok : bool; op : SpvOp; form : SubgroupForm; cap : SpvCapability> {
    if (name == "subgroupAll") return (ok = true, op = SpvOp.GroupNonUniformAll, form = SubgroupForm.Vote, cap = SpvCapability.GroupNonUniformVote)
    if (name == "subgroupAny") return (ok = true, op = SpvOp.GroupNonUniformAny, form = SubgroupForm.Vote, cap = SpvCapability.GroupNonUniformVote)
    if (name == "subgroupElect") return (ok = true, op = SpvOp.GroupNonUniformElect, form = SubgroupForm.Elect, cap = SpvCapability.GroupNonUniform)
    if (name == "subgroupBroadcastFirst") return (ok = true, op = SpvOp.GroupNonUniformBroadcastFirst, form = SubgroupForm.Unary, cap = SpvCapability.GroupNonUniformBallot)
    if (name == "subgroupBroadcast") return (ok = true, op = SpvOp.GroupNonUniformBroadcast, form = SubgroupForm.ValueId, cap = SpvCapability.GroupNonUniformBallot)
    if (name == "subgroupShuffle") return (ok = true, op = SpvOp.GroupNonUniformShuffle, form = SubgroupForm.ValueId, cap = SpvCapability.GroupNonUniformShuffle)
    if (name == "subgroupShuffleXor") return (ok = true, op = SpvOp.GroupNonUniformShuffleXor, form = SubgroupForm.ValueId, cap = SpvCapability.GroupNonUniformShuffle)
    if (name == "subgroupAdd") return (ok = true, op = cls == 2 ? SpvOp.GroupNonUniformFAdd : SpvOp.GroupNonUniformIAdd, form = SubgroupForm.Reduce, cap = SpvCapability.GroupNonUniformArithmetic)
    if (name == "subgroupMin") return (ok = true, op = cls == 2 ? SpvOp.GroupNonUniformFMin : (cls == 1 ? SpvOp.GroupNonUniformUMin : SpvOp.GroupNonUniformSMin), form = SubgroupForm.Reduce, cap = SpvCapability.GroupNonUniformArithmetic)
    if (name == "subgroupMax") return (ok = true, op = cls == 2 ? SpvOp.GroupNonUniformFMax : (cls == 1 ? SpvOp.GroupNonUniformUMax : SpvOp.GroupNonUniformSMax), form = SubgroupForm.Reduce, cap = SpvCapability.GroupNonUniformArithmetic)
    if (name == "subgroupClusteredAdd") return (ok = true, op = cls == 2 ? SpvOp.GroupNonUniformFAdd : SpvOp.GroupNonUniformIAdd, form = SubgroupForm.Clustered, cap = SpvCapability.GroupNonUniformClustered)
    return (ok = false, op = SpvOp.GroupNonUniformAll, form = SubgroupForm.Vote, cap = SpvCapability.GroupNonUniform)
}

// A compile-time unsigned constant, for an operand SPIR-V requires to be a constant instruction (the
// ClusterSize of a clustered reduction). Peels the load marker; a non-negative int literal counts.
def private const_uint_value(e : ExpressionPtr) : tuple<ok : bool; value : uint> {
    let r2v = e ?as ExprRef2Value
    if (r2v != null) return const_uint_value(r2v.subexpr)
    let cu = e ?as ExprConstUInt
    if (cu != null) return (ok = true, value = cu.value)
    let ci = e ?as ExprConstInt
    if (ci != null && ci.value >= 0) return (ok = true, value = uint(ci.value))
    return (ok = false, value = 0u)
}

// ===== arithmetic opcode selection =====

// A scalar type-name used as a conversion call (float(x)/int8(x)/float16(x)/...). Returns the
// destination scalar class (0 signed / 1 unsigned / 2 float) + bit width, or cls=-1 if `name`
// is not a scalar type name. The vecN constructor names are handled by vector_ctor.
def private scalar_type_name_class(name : string) : tuple<cls : int; width : int> {
    if (name == "int") return (cls = 0, width = 32)
    if (name == "uint") return (cls = 1, width = 32)
    if (name == "float") return (cls = 2, width = 32)
    if (name == "float16") return (cls = 2, width = 16)
    if (name == "int16") return (cls = 0, width = 16)
    if (name == "uint16") return (cls = 1, width = 16)
    if (name == "int8") return (cls = 0, width = 8)
    if (name == "uint8") return (cls = 1, width = 8)
    if (name == "double") return (cls = 2, width = 64)
    return (cls = -1, width = 0)
}

// SPIR-V opcode for a conversion between (class, width) pairs — scalar or lane-matched vector
// (callers handle the same-class-same-width no-op). int<->uint at the SAME width is a bit
// reinterpret (OpBitcast — daslang two's-complement cast semantics); same-class width changes
// are FConvert (f16<->f32) / SConvert / UConvert (SPIR-V narrowing keeps low bits = the das
// C-truncation); class changes are value conversions (width-agnostic opcodes). A cross-sign
// cross-width int cast is supported only when it NARROWS; widening fails closed.
def private convert_op(src_cls, src_w, dst_cls, dst_w : int) : tuple<ok : bool; code : SpvOp> {
    if (src_cls == dst_cls) {
        if (src_w == dst_w) return (ok = false, code = SpvOp.Nop)   // no-op — caller's case
        if (src_cls == 2) return (ok = true, code = SpvOp.FConvert)
        if (src_cls == 0) return (ok = true, code = SpvOp.SConvert)
        if (src_cls == 1) return (ok = true, code = SpvOp.UConvert)
    }
    // int -> int across signedness AND width is one instruction only when it NARROWS: SConvert and
    // UConvert both keep the low bits there, which is exactly the C truncation daslang's narrowing
    // ctor and GLSL's uint16_t(int) both mean, and the opcode is picked by the DESTINATION's
    // signedness because that is what SPIR-V's Result Type demands. A cross-class WIDENING is NOT:
    // C sign-extends an int16 into a uint32 where OpUConvert zero-extends, so it would need a
    // widen-then-bitcast pair and fails closed rather than silently zero-extending a negative.
    if (src_cls <= 1 && dst_cls <= 1 && dst_w < src_w) {
        return (ok = true, code = dst_cls == 0 ? SpvOp.SConvert : SpvOp.UConvert)
    }
    if (dst_cls == 2) {                                             // -> float
        if (src_cls == 0) return (ok = true, code = SpvOp.ConvertSToF)
        if (src_cls == 1) return (ok = true, code = SpvOp.ConvertUToF)
    } elif (dst_cls == 0) {                                         // -> signed int
        if (src_cls == 2) return (ok = true, code = SpvOp.ConvertFToS)
        if (src_cls == 1 && src_w == dst_w) return (ok = true, code = SpvOp.Bitcast)
    } elif (dst_cls == 1) {                                         // -> unsigned int
        if (src_cls == 2) return (ok = true, code = SpvOp.ConvertFToU)
        if (src_cls == 0 && src_w == dst_w) return (ok = true, code = SpvOp.Bitcast)
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
    // bitwise family: int/uint only; `>>` splits arithmetic/logical by signedness (daslang C semantics)
    let is_int = cls == 0 || cls == 1
    if (op == "&" && is_int) return (ok = true, code = SpvOp.BitwiseAnd)
    if (op == "|" && is_int) return (ok = true, code = SpvOp.BitwiseOr)
    if (op == "^" && is_int) return (ok = true, code = SpvOp.BitwiseXor)
    if (op == "<<" && is_int) return (ok = true, code = SpvOp.ShiftLeftLogical)
    if (op == ">>") {
        if (cls == 0) return (ok = true, code = SpvOp.ShiftRightArithmetic)
        if (cls == 1) return (ok = true, code = SpvOp.ShiftRightLogical)
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

// vector constructor name -> lane count + component class (0 signed, 1 unsigned, 2 float) +
// component width, else ok=false. The (class, width) pair routes 1-arg component-kind
// conversions through convert_op (the das_sv_cvt lane-matched convert surface).
def private vector_ctor(name : string) : tuple<ok : bool; n : int; cls : int; width : int> {
    if (name == "float2") return (ok = true, n = 2, cls = 2, width = 32)
    if (name == "float3") return (ok = true, n = 3, cls = 2, width = 32)
    if (name == "float4") return (ok = true, n = 4, cls = 2, width = 32)
    if (name == "int2") return (ok = true, n = 2, cls = 0, width = 32)
    if (name == "int3") return (ok = true, n = 3, cls = 0, width = 32)
    if (name == "int4") return (ok = true, n = 4, cls = 0, width = 32)
    if (name == "uint2") return (ok = true, n = 2, cls = 1, width = 32)
    if (name == "uint3") return (ok = true, n = 3, cls = 1, width = 32)
    if (name == "uint4") return (ok = true, n = 4, cls = 1, width = 32)
    if (name == "half2") return (ok = true, n = 2, cls = 2, width = 16)
    if (name == "half3") return (ok = true, n = 3, cls = 2, width = 16)
    if (name == "half4") return (ok = true, n = 4, cls = 2, width = 16)
    if (name == "short2") return (ok = true, n = 2, cls = 0, width = 16)
    if (name == "short3") return (ok = true, n = 3, cls = 0, width = 16)
    if (name == "short4") return (ok = true, n = 4, cls = 0, width = 16)
    if (name == "ushort2") return (ok = true, n = 2, cls = 1, width = 16)
    if (name == "ushort3") return (ok = true, n = 3, cls = 1, width = 16)
    if (name == "ushort4") return (ok = true, n = 4, cls = 1, width = 16)
    if (name == "byte2") return (ok = true, n = 2, cls = 0, width = 8)
    if (name == "byte3") return (ok = true, n = 3, cls = 0, width = 8)
    if (name == "byte4") return (ok = true, n = 4, cls = 0, width = 8)
    if (name == "ubyte2") return (ok = true, n = 2, cls = 1, width = 8)
    if (name == "ubyte3") return (ok = true, n = 3, cls = 1, width = 8)
    if (name == "ubyte4") return (ok = true, n = 4, cls = 1, width = 8)
    return (ok = false, n = 0, cls = -1, width = 0)
}

// matrix constructor name (float4x4/float3x3/float3x4) -> column-vector width + column count, else
// ok=false. GLSL's `matCxR(col0, col1, ...)` column form takes `cols` column vectors, each a
// floatW. daslang matrices are column-major (m[i] is column i), so the constituents map straight
// onto OpCompositeConstruct's column operands. Mirrors matrix_info's name->dims table.
def private matrix_ctor(name : string) : tuple<ok : bool; width : int; cols : int> {
    if (name == "float4x4") return (ok = true, width = 4, cols = 4)
    if (name == "float3x3") return (ok = true, width = 3, cols = 3)
    if (name == "float3x4") return (ok = true, width = 3, cols = 4)
    return (ok = false, width = 0, cols = 0)
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

// 16-bit stage I/O (a half/short/ushort varying) requires StorageInputOutput16 — core in
// SPIR-V 1.3, cap only (SPV_KHR_16bit_storage needs no OpExtension at our baseline).
def private ensure_storage_input_output_16(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.sio16_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.StorageInputOutput16))
    ctx.sio16_cap = true
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

// sdot4 (OpSDot on packed 4x8-bit operands) requires the DotProduct + DotProductInput4x8BitPacked
// capabilities and the SPV_KHR_integer_dot_product extension (core at SPIR-V 1.6 — above our 1.3/1.4
// target, so the OpExtension is unconditional). Emit-once + ctx-flag dedupe.
def private ensure_integer_dot_product(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.integer_dot_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.DotProduct))
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.DotProductInput4x8BitPacked))
    var nm <- string_words("SPV_KHR_integer_dot_product")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.integer_dot_cap = true
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

// Fragment-shader barycentrics (VK_KHR_fragment_shader_barycentric): OpCapability FragmentBarycentricKHR
// + OpExtension "SPV_KHR_fragment_shader_barycentric". Emitted by gl_BaryCoordEXT and by a @pervertex
// (PerVertexKHR) input. No version bump -- the extension is valid from SPIR-V 1.0. Emit-once + ctx-flag dedupe.
def private ensure_fragment_barycentric(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.fragment_barycentric_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.FragmentBarycentricKHR))
    var nm <- string_words("SPV_KHR_fragment_shader_barycentric")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.fragment_barycentric_cap = true
}

// A float atomicAdd (VK_EXT_shader_atomic_float): OpCapability AtomicFloat32AddEXT + OpExtension
// "SPV_EXT_shader_atomic_float_add". No version bump -- the extension is valid from SPIR-V 1.0.
// Only the 32-bit ADD is wired; the float min/max opcodes sit behind a different capability.
// Emit-once + ctx-flag dedupe.
def private ensure_atomic_float_add(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.atomic_float_add_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.AtomicFloat32AddEXT))
    var nm <- string_words("SPV_EXT_shader_atomic_float_add")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.atomic_float_add_cap = true
}

// Ray-tracing stages (VK_KHR_ray_tracing_pipeline): OpCapability RayTracingKHR + OpExtension
// "SPV_KHR_ray_tracing". Emit-once + ctx-flag dedupe; the cap gates the RayGenerationKHR / MissKHR /
// ClosestHitKHR execution models, the RT storage classes, OpTypeAccelerationStructureKHR, and
// OpTraceRayKHR. SPV_KHR_ray_tracing requires SPIR-V >= 1.4 (set by the caller alongside this).
def private ensure_ray_tracing(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.ray_tracing_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.RayTracingKHR))
    var nm <- string_words("SPV_KHR_ray_tracing")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.ray_tracing_cap = true
}

// An INLINE ray query (SPV_KHR_ray_query): OpCapability RayQueryKHR + OpExtension "SPV_KHR_ray_query".
// Emit-once + ctx-flag dedupe; the cap gates OpTypeRayQueryKHR and the OpRayQuery* opcodes. Unlike
// SPV_KHR_ray_tracing it carries NO SPIR-V version floor (valid from 1.0), so this does not touch
// m.version -- the module's version comes from whatever else it uses (any runtime descriptor
// arrays already raise it to 1.5 via ensure_runtime_descriptor_array, matching glslang).
def private ensure_ray_query(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.ray_query_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.RayQueryKHR))
    var nm <- string_words("SPV_KHR_ray_query")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    ctx.ray_query_cap = true
}

// A storage-image @format outside SPIR-V's core set (rgba16 and the rest of the extended list) needs
// the StorageImageExtendedFormats capability. Emit-once + ctx-flag dedupe.
def private ensure_storage_image_extended(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.storage_image_ext_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.StorageImageExtendedFormats))
    ctx.storage_image_ext_cap = true
}

// A cube-ARRAYED image needs the ImageCubeArray capability (the storage-image counterpart of
// SampledCubeArray, which covers the sampled ones). Emit-once + ctx-flag dedupe.
def private ensure_image_cube_array(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.image_cube_array_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.ImageCubeArray))
    ctx.image_cube_array_cap = true
}

// A cube-ARRAYED SAMPLED image (a samplerCubeArray) needs the SampledCubeArray capability -- the
// sampled counterpart of ImageCubeArray, which covers the storage ones. Emit-once + ctx-flag dedupe.
def private ensure_sampled_cube_array(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.sampled_cube_array_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.SampledCubeArray))
    ctx.sampled_cube_array_cap = true
}

// A Dim=Buffer SAMPLED image (a textureBuffer uniform texel buffer) needs the SampledBuffer capability;
// the storage counterpart (imageBuffer) would need ImageBuffer instead. Emit-once + ctx-flag dedupe.
def private ensure_sampled_buffer(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.sampled_buffer_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.SampledBuffer))
    ctx.sampled_buffer_cap = true
}

// A runtime-sized descriptor array needs the RuntimeDescriptorArray capability. Before SPIR-V 1.5 that
// capability lives in SPV_EXT_descriptor_indexing; 1.5 promoted the whole extension into core. Raise
// the module rather than emit the OpExtension, because glslang -- which targets Vulkan 1.2, hence
// SPIR-V 1.5 -- emits the capability with no extension, and a port is judged against its blob.
// Nothing here implies ShaderNonUniform: a uniform index needs no NonUniform decoration.
def private ensure_runtime_descriptor_array(var m : SpirvModule; var ctx : EmitCtx) {
    if (m.version < SPV_VERSION_1_5) {
        m.version = SPV_VERSION_1_5
    }
    if (ctx.runtime_descriptor_array_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.RuntimeDescriptorArray))
    ctx.runtime_descriptor_array_cap = true
}

// Every subgroup op and every subgroup builtin declares the base GroupNonUniform capability.
// Emit-once + ctx-flag dedupe.
def private ensure_group_non_uniform(var m : SpirvModule; var ctx : EmitCtx) {
    if (ctx.group_non_uniform_cap) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.GroupNonUniform))
    ctx.group_non_uniform_cap = true
}

// An op family (vote / ballot / arithmetic / shuffle / clustered) declares its own capability ON TOP of
// the base one. SPIR-V says a capability implies the ones it depends on, but glslang spells both out and
// a port is judged against the glslang reference -- so emit both. Deduped on the emitted-feature set.
def private ensure_subgroup_feature(var m : SpirvModule; var ctx : EmitCtx; cap : SpvCapability) {
    ensure_group_non_uniform(m, ctx)
    // subgroupElect names the base capability as its own family; re-emitting it here would duplicate
    // the OpCapability the line above already wrote.
    if (cap == SpvCapability.GroupNonUniform || key_exists(ctx.subgroup_feature_caps, uint(cap))) return
    emit(m, SEC_CAPS, SpvOp.Capability, uint(cap))
    ctx.subgroup_feature_caps |> insert(uint(cap))
}

// daslang math builtin -> GLSL.std.450 ext-inst opcode. abs/min/max/clamp/sign pick the F/S/U
// variant by operand class; the rest are float-only. Operand class comes from arg 0 (vectors
// classify by lane kind, so length/normalize/cross/etc. read as float-class). ok=false => not a
// GLSL.std.450 builtin (or wrong class) -> caller emits a clean error. NOTE: keyed on the DASLANG name,
// which differs from the GLSL spelling for two: daslang `lerp` -> FMix, daslang `rsqrt` -> InverseSqrt.
// Only dependency-free builtin `math` functions appear here (no dead branches). Excluded on purpose:
// daslang has no mix.
def private glsl_ext_op(name : string; cls : int) : tuple<ok : bool; op : GLSLstd450> {
    if (name == "packHalf2x16") {
        return (ok = true, op = GLSLstd450.PackHalf2x16)     // float2 -> uint (RNE per lane)
    }
    if (name == "unpackHalf2x16") {
        return (ok = true, op = GLSLstd450.UnpackHalf2x16)   // uint -> float2 (exact)
    }
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
    // unpackHalf2x16 is the one op classified by an operand that is NOT a float: it takes the uint
    // side of the pack and yields a float2, so it has to be named above the float-only cutoff.
    if (name == "unpackHalf2x16") return (ok = true, op = GLSLstd450.UnpackHalf2x16)
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
    if (name == "packHalf2x16") return (ok = true, op = GLSLstd450.PackHalf2x16)   // float2 -> uint (two 16-bit halfs)
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

// An enum constant names its ENTRY; the integer behind it is that entry's own initializer, which
// already carries the enum's underlying type -- so re-entering emit_const yields a constant of
// exactly the type emit_type lowered the enum to, without transcribing that width/signedness rule
// twice.
def private emit_const_enum(var m : SpirvModule; cen : ExprConstEnumeration?; var errors : array<string>) : tuple<ok : bool; id : uint> {
    if (cen.enumType == null) return (ok = false, id = 0u)
    for (ee in cen.enumType.list) {
        if (ee.name == cen.value) return emit_const(m, ee.value, errors)
    }
    errors |> push("enum '{cen.enumType.name}' has no entry '{cen.value}'")
    return (ok = false, id = 0u)
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
    let cd = e ?as ExprConstDouble
    if (cd != null) return (ok = true, id = const_double(m, cd.value))
    let cb = e ?as ExprConstBool
    if (cb != null) return (ok = true, id = const_bool(m, cb.value))
    // 16/8-bit scalar constants (lattice VECTOR constants don't exist as nodes — they arrive
    // as all-constant ctor calls and fold through the composite path below)
    let ch = e ?as ExprConstFloat16
    if (ch != null) return (ok = true, id = const_float16(m, float_to_half_bits(float(ch.value))))
    let ci16 = e ?as ExprConstInt16
    if (ci16 != null) return (ok = true, id = const_int16(m, int(ci16.value)))
    let cu16 = e ?as ExprConstUInt16
    if (cu16 != null) return (ok = true, id = const_uint16(m, uint(cu16.value)))
    let ci8 = e ?as ExprConstInt8
    if (ci8 != null) return (ok = true, id = const_int8(m, int(ci8.value)))
    let cu8 = e ?as ExprConstUInt8
    if (cu8 != null) return (ok = true, id = const_uint8(m, uint(cu8.value)))
    let cen = e ?as ExprConstEnumeration
    if (cen != null) return emit_const_enum(m, cen, errors)
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
    // all-constant vector / matrix constructor -> OpConstantComposite
    let call = e ?as ExprCall
    if (call != null) {
        // matCxR(col0, ...) of constant columns: an OpConstantComposite of the column constants,
        // which is what glslang folds a `const mat4` global to. Gated on the argument count like
        // the emit path's matrix ctor, so a matrix-RETURNING user function stays not-a-constant.
        let mc = matrix_ctor("{call.name}")
        if (mc.ok && length(call.arguments) == mc.cols) {
            var cols : array<uint>
            cols |> reserve(mc.cols)
            for (a in call.arguments) {
                if (component_count(a._type.baseType) != mc.width || scalar_class(a._type.baseType) != 2) {
                    errors |> push("{call.name}: constant column of type {a._type.baseType} is not a float{mc.width}")
                    delete cols
                    return (ok = false, id = 0u)
                }
                let cr = emit_const(m, a, errors)
                if (!cr.ok) {
                    errors |> push("{call.name}: constructor column is not a compile-time constant")
                    delete cols
                    return (ok = false, id = 0u)
                }
                cols |> push(cr.id)
            }
            let mid = const_composite(m, emit_type(m, e._type), cols)
            delete cols
            return (ok = true, id = mid)
        }
        let vc = vector_ctor("{call.name}")
        if (!vc.ok) return (ok = false, id = 0u)
        var total = 0
        var parts : array<uint>
        parts |> reserve(length(call.arguments))
        for (a in call.arguments) {
            let cc = component_count(a._type.baseType)
            let a_cls = scalar_class(a._type.baseType)
            let a_w = scalar_width(a._type.baseType)
            if (cc == 0 || a_cls != vc.cls) {
                errors |> push("{call.name}: constant constructor constituent of type {a._type.baseType} has the wrong component kind")
                delete parts
                return (ok = false, id = 0u)
            }
            total += cc
            if (a_w != vc.width && cc == 1) {
                // narrow the constant LANE at compile time (das per-lane ctors take int32/float
                // args and C-truncate / RNE-round — mirror that exactly on the constant)
                var narrowed = 0u
                var got = false
                let nci = a ?as ExprConstInt
                if (nci != null && vc.width == 16) {
                    narrowed = const_int16(m, int(int16(nci.value)))
                    got = true
                } elif (nci != null && vc.width == 8) {
                    narrowed = const_int8(m, int(int8(nci.value)))
                    got = true
                }
                let ncu = a ?as ExprConstUInt
                if (ncu != null && vc.width == 16) {
                    narrowed = const_uint16(m, uint(uint16(ncu.value)))
                    got = true
                } elif (ncu != null && vc.width == 8) {
                    narrowed = const_uint8(m, uint(uint8(ncu.value)))
                    got = true
                }
                let ncf = a ?as ExprConstFloat
                if (ncf != null && vc.width == 16) {
                    narrowed = const_float16(m, float_to_half_bits(ncf.value))
                    got = true
                }
                if (!got) {
                    errors |> push("{call.name}: constant constituent of type {a._type.baseType} cannot narrow to the component width")
                    delete parts
                    return (ok = false, id = 0u)
                }
                parts |> push(narrowed)
                continue
            }
            if (a_w != vc.width) {
                errors |> push("{call.name}: constant constructor constituent of type {a._type.baseType} has the wrong component width")
                delete parts
                return (ok = false, id = 0u)
            }
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

// The vector type a scalar OPERAND splats to: `n` lanes of that operand's own component type, NOT of
// the binop's result type. The two agree for arithmetic, but a shift's Shift operand only has to
// match Base's component COUNT -- `uint4 >> 8` (the only vector-shift spelling daslang has, the
// shift count being an int) would otherwise build a v4uint out of int constituents, which compiles
// and fails spirv-val.
def private splat_type(var m : SpirvModule; scalar : TypeDecl?; n : int) : uint {
    return type_vector(m, emit_type(m, scalar), n)
}

// ===== `*` over matrices / vectors / scalars =====
// SPIR-V splits multiplication by operand shape: OpMatrixTimesMatrix / OpMatrixTimesVector /
// OpVectorTimesScalar. Plain component-wise FMul/IMul (the scalar binop path) only works when both
// operands have the SAME shape, so the mixed cases are routed here first. Returns handled=false for
// same-shape operands (let binop_code emit FMul/IMul); handled=true with id=0 on an unsupported mix
// (error already pushed). Operands l/r are pre-emitted result-ids. Handled: matrix*matrix,
// matrix*vector, vector*matrix, float vector*scalar (OpVectorTimesScalar, either order), and integer
// vector*scalar (splat + OpIMul, either order). daslang has no matrix*scalar operator, so that
// SPIR-V op is intentionally absent.
def private emit_mul(var m : SpirvModule; op2 : ExprOp2?; e : ExpressionPtr; l, r : uint; var errors : array<string>) : tuple<handled : bool; id : uint> {
    let lmat = matrix_info(op2.left._type)
    let rmat = matrix_info(op2.right._type)
    let lcc = arith_lanes(op2.left._type.baseType)
    let rcc = arith_lanes(op2.right._type.baseType)
    let lcls = arith_class(op2.left._type.baseType)
    let rcls = arith_class(op2.right._type.baseType)
    // same-shape (scalar*scalar or equal-width vector*vector): component-wise, defer to binop_code
    if (!lmat.ok && !rmat.ok && lcc == rcc) return (handled = false, id = 0u)
    let rt = emit_type(m, e._type)
    let res = alloc_id(m)
    if (lmat.ok && rmat.ok) {
        emit(m, SEC_FUNCS, SpvOp.MatrixTimesMatrix, rt, res, l, r)
    } elif (lmat.ok && rcc > 1) {
        emit(m, SEC_FUNCS, SpvOp.MatrixTimesVector, rt, res, l, r)
    } elif (rmat.ok && lcc > 1) {
        emit(m, SEC_FUNCS, SpvOp.VectorTimesMatrix, rt, res, l, r)
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
    // A REFERENCE local aliases storage that already has an address, so it gets no OpVariable of its
    // own: visitExprLetVariable binds it to the pointer its initializer lowered to. Declaring one
    // here would make every write through the alias land in that private copy and leave the aliased
    // element untouched. The inliner emits these whenever an inlined call's result is assigned to an
    // lvalue (`var _inl_pre : float& = p[i]; … ; _inl_pre = _inl_res`), which is the shape a
    // helper reached across a require arrives in.
    if (key_exists(ctx.local_vars, intptr(v)) || v._type.flags.ref) return
    let bt = v._type.baseType
    // rayQueryEXT: an opaque ray-query OBJECT (SPV_KHR_ray_query) lives in a Function-storage local,
    // NOT a descriptor. It is declared here, initialized by rayQueryInitializeEXT and stepped by
    // rayQueryProceedEXT (both by pointer, in visitExprCall), and never holds a loadable value -- so it
    // is flagged is_opaque to skip both the value guard below (no OpConstantNull of an opaque type
    // exists) and the zero-init store in visitExprLetVariable.
    if (bt == Type.tStructure && v._type.structType != null &&
        "{v._type.structType.name}" == "rayQueryEXT") {
        ensure_ray_query(m, ctx)
        let rqt = type_ray_query(m)
        let rqpt = type_pointer(m, SpvStorageClass.Function, rqt)
        let rqid = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.Variable, rqpt, rqid, uint(SpvStorageClass.Function))
        ctx.local_vars |> insert(intptr(v), LocalVar(ptr_id = rqid, value_type = rqt, is_opaque = true))
        return
    }
    // A cm2 tensor LAYOUT/VIEW object (tensorLayout2D, tensorView2Dt, ...): a Function-storage local
    // holding the opaque address-mapping value. Like rayQueryEXT it has no zero constant (is_opaque
    // skips the OpConstantNull init) — tensorLayoutCreate / tensorViewCreate stores the first value.
    if (bt == Type.tStructure && v._type.structType != null) {
        let to = tensor_object_type_of(m, v._type)
        if (to.id != 0u) {
            let tpt = type_pointer(m, SpvStorageClass.Function, to.id)
            let tid = alloc_id(m)
            emit(m, SEC_FUNCS, SpvOp.Variable, tpt, tid, uint(SpvStorageClass.Function))
            ctx.local_vars |> insert(intptr(v), LocalVar(ptr_id = tid, value_type = to.id, is_opaque = true))
            return
        }
    }
    // A cooperative-matrix OBJECT (coopmatA_f16_16x16, ...): a subgroup tile in a Function-storage local,
    // recognized by its marker name. Unlike rayQueryEXT it holds a LOADABLE value (coopmatLoad fills it,
    // coopmatMulAdd reads it and produces one), so it is NOT is_opaque -- the normal value rail applies and
    // OpConstantNull zero-init of a bare `var acc : coopmatAcc_f32_16x16` accumulator is legal for a coopmat.
    if (bt == Type.tStructure && v._type.structType != null && coopmat_info(v._type).ok) {
        let cmt = coopmat_type_of(m, v._type)
        let cmpt = type_pointer(m, SpvStorageClass.Function, cmt)
        let cmid = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.Variable, cmpt, cmid, uint(SpvStorageClass.Function))
        ctx.local_vars |> insert(intptr(v), LocalVar(ptr_id = cmid, value_type = cmt))
        return
    }
    // a matrix is a tHandle, so numeric_info does not see it -- emit_type lowers it to OpTypeMatrix,
    // which is a perfectly good Function-storage pointee (glslang gives a local `mat4` an OpVariable too)
    if (!is_emittable_value_type(v._type) && bt != Type.tFixedArray && bt != Type.tStructure &&
        !matrix_info(v._type).ok) {
        // emit_type would panic on an unsupported base type; reject with a clean error instead.
        if (cpu_only_lattice_width(bt)) {
            errors |> push("local '{v.name}': type {bt} is CPU-only (8/16-lane lattice forms do not exist in shaders — vectors cap at 4 lanes)")
        } else {
            errors |> push("local '{v.name}': type {bt} is not supported yet (only int/uint/float scalars+vectors+matrices, bool, fixed arrays, and structs of those)")
        }
        return
    }
    // a struct local takes the same undecorated type id its by-value signatures use, so passing one
    // to a helper needs no relayout; emit_value_type returns 0 when a field has no lowering.
    let vt = emit_value_type(m, v._type, errors)
    if (vt == 0u) return
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
    e2align : table<uint64; uint>        // intptr(Expression) -> Aligned literal for the OpLoad (set only for PhysicalStorageBuffer-rooted chains — PSB loads REQUIRE the Aligned memory operand)
    ite_ids : table<uint64; IteIds>      // intptr(ExprIfThenElse) -> its {merge, then, else} labels
    loop_ids : table<uint64; LoopIds>    // intptr(ExprWhile/ExprFor) -> its 5 loop-skeleton labels
    for_src : table<uint64>              // for-source range() call ptrs to intercept in visitExprCall (6.3)
    copy_dst : uint64                    // intptr(ExprCopy.left) while that destination subtree is walked
    suppress_bounds_arg : bool = false   // next node is a `.[]` call's __context__/__lineinfo__ tail — elide
    suppress_fn_addr : bool = false      // next node is coopmatLoadTensorDecode's trailing @@decode — read structurally by the cm2 load arm, not lowered
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
            if (key_exists(e2align, k)) {
                // a PhysicalStorageBuffer-rooted pointer: the load must carry Aligned <literal>
                var lops <- [pty, res, pid, uint(SpvMemoryAccess.Aligned), e2align?[k] ?? 4u]
                emit_n(bm, SEC_FUNCS, SpvOp.Load, lops)
                delete lops
            } else {
                emit(bm, SEC_FUNCS, SpvOp.Load, pty, res, pid)
            }
            return res
        }
        errs |> push("internal: no rvalue available for {e.__rtti}")
        return 0u
    }
    // Does this expression name MEMORY -- something an OpAccessChain can chain a component off?
    // A `var` local (its OpVariable), an already-chained pointer (an ssbo element, a block member),
    // or a global with an OpVariable. A `let` local, a parameter and a call result are SSA VALUES,
    // and a folded module-scope `let` global is an OpConstant: none of them has a pointer, so a
    // component of one has to be extracted rather than chained.
    def is_addressable(e : ExpressionPtr) : bool {
        if (key_exists(e2ptr, intptr(e))) return true
        let gv = global_var_of(e)
        if (gv != null) return !(ctx.globals?[intptr(gv)] ?? GlobalInfo()).is_constant
        let ev = e ?as ExprVar
        if (ev != null && (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block)) {
            return (ctx.local_vars?[intptr(ev.variable)] ?? LocalVar()).ptr_id != 0u
        }
        return false
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
    // Resolve a storage-image argument to a LOADED image SSA id. Handles both the scalar form
    // `imageStore(img, …)` (gi.is_storage_image) and the runtime descriptor-array form
    // `imageStore(pool[idx], …)` (gi.is_storage_image_array): OpAccessChain into the array, then
    // OpLoad. Returns the ROOT global's GlobalInfo so the caller can enforce @readonly / @writeonly on
    // the array as a whole -- the mirror of resolve_sampler_arg for the read-write image rail.
    def resolve_image_arg(arg : ExpressionPtr; intrinsic_name : string) : tuple<gi : GlobalInfo; img : uint; ok : bool> {
        var bm & = unsafe(*m)
        let direct = global_var_of(arg)
        if (direct != null) {
            let gi = ctx.globals?[intptr(direct)] ?? GlobalInfo()
            if (gi.is_storage_image_array) {
                errs |> push("{intrinsic_name}: '{direct.name}' is a descriptor array; index into it with [idx]")
                return (gi = GlobalInfo(), img = 0u, ok = false)
            }
            if (!gi.is_storage_image) {
                errs |> push("{intrinsic_name}: '{direct.name}' is not a storage image")
                return (gi = GlobalInfo(), img = 0u, ok = false)
            }
            let img_id = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, img_id, gi.var_id)
            return (gi = gi, img = img_id, ok = true)
        }
        let at = arg ?as ExprAt
        if (at == null) {
            errs |> push("{intrinsic_name}: first argument must be a storage-image global or a descriptor-array element (pool[idx])")
            return (gi = GlobalInfo(), img = 0u, ok = false)
        }
        let arr_g = global_var_of(at.subexpr)
        let agi = arr_g != null ? (ctx.globals?[intptr(arr_g)] ?? GlobalInfo()) : GlobalInfo()
        if (arr_g == null || !agi.is_storage_image_array) {
            errs |> push("{intrinsic_name}: descriptor-array index must target a storage-image array global")
            return (gi = GlobalInfo(), img = 0u, ok = false)
        }
        let idx = value_of(at.index)
        if (idx == 0u) return (gi = GlobalInfo(), img = 0u, ok = false)
        let elem_ptr_t = type_pointer(bm, SpvStorageClass.UniformConstant, agi.elem_type)
        let elem_ptr = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.AccessChain, elem_ptr_t, elem_ptr, agi.var_id, idx)
        let img_id = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, agi.elem_type, img_id, elem_ptr)
        return (gi = agi, img = img_id, ok = true)
    }
    // The storage class of the memory an addressable expression chains into: the root global's for a
    // global-rooted chain; for one rooted at a local it is that local's OWN storage — Function for every
    // ordinary `var` local, PhysicalStorageBuffer for a cm2 decode function's block parameter.
    // An OpAccessChain result type must name the storage class it chains INTO, so a field of a struct
    // local and a field of an SSBO element cannot share one default.
    def root_storage_of(e : ExpressionPtr) : SpvStorageClass {
        let rv = root_global_of(e)
        let rgi = rv != null ? (ctx.globals?[intptr(rv)] ?? GlobalInfo()) : GlobalInfo()
        if (rgi.var_id != 0u) return rgi.storage
        let lv = root_local_var_of(e)
        if (lv != null) return (ctx.local_vars?[intptr(lv)] ?? LocalVar()).storage
        return SpvStorageClass.Function
    }

    def ptr_of(e : ExpressionPtr) : tuple<id : uint; pty : uint> {
        let k = intptr(e)
        if (key_exists(e2ptr, k)) return (id = e2ptr?[k] ?? 0u, pty = e2pty?[k] ?? 0u)
        // ONE component of a vector that is itself addressable (ssbo[i].x, block.field.y, shared[i].z):
        // chain a component index off the base pointer. Emitted HERE rather than in visitExprSwizzle
        // because only a pointer CONSUMER (a store, a compound assign, an atomic target) needs it --
        // a read stays the CompositeExtract off the loaded vector that visitExprSwizzle already emits,
        // so no body that compiles today gains an instruction.
        let sw = e ?as ExprSwizzle
        if (sw != null && length(sw.fields) == 1 && key_exists(e2ptr, intptr(sw.value))) {
            var bm & = unsafe(*m)
            let base = ptr_of(sw.value)
            let storage = root_storage_of(sw.value)
            let pointee = emit_type(bm, e._type)
            let ptr_t = type_pointer(bm, storage, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, base.id, const_uint(bm, uint(sw.fields[0])))
            e2ptr[k] = res
            e2pty[k] = pointee
            return (id = res, pty = pointee)
        }
        // a swizzle of a VALUE used as a pointer: the base has no OpVariable to chain a component off
        // (a swizzle of a swizzle, a call result). A multi-lane assignment target lands in
        // emit_write_swizzle instead, and reports the same thing there.
        if (e is ExprSwizzle) {
            errs |> push("cannot assign through a swizzle of a value — a write-swizzle target must be addressable (a global, a `var` local, or an SSBO / UBO element)")
            return (id = 0u, pty = 0u)
        }
        // a module-scope constant (`let` fold or @constant_id spec constant) used as a write target:
        // constants have no OpVariable, so there is no pointer to store through. Name the actual
        // mistake instead of the generic internal message.
        let wg = global_var_of(e)
        if (wg != null && (ctx.globals?[intptr(wg)] ?? GlobalInfo()).is_constant) {
            errs |> push("cannot assign to '{wg.name}' — module-scope constants (incl. @constant_id spec constants) are read-only in a shader")
            return (id = 0u, pty = 0u)
        }
        errs |> push("internal: {e.__rtti} is not an lvalue")
        return (id = 0u, pty = 0u)
    }

    // Resolve an accelerationStructureEXT argument to a LOADED acceleration-structure id: a single
    // descriptor global -> OpLoad; an element of a descriptor array (tlas[idx]) -> OpAccessChain +
    // OpLoad. Shared by traceRayEXT (RT pipeline) and rayQueryInitializeEXT (inline ray query). The
    // emission (alloc + AccessChain/Load) is the last thing it does, so a caller that runs it AFTER
    // its other operand loads keeps the same instruction order. Pushes an error and returns 0 on a bad
    // operand.
    def load_accel_operand(arg : ExpressionPtr; who : string) : uint {
        var bm & = unsafe(*m)
        var agi = GlobalInfo()
        var accel_idx = 0u
        let accel_g = global_var_of(arg)
        if (accel_g != null) {
            agi = ctx.globals?[intptr(accel_g)] ?? GlobalInfo()
            if (!agi.is_accel || agi.is_accel_array) {
                errs |> push("{who}: the acceleration-structure argument must be an accelerationStructureEXT global (index a descriptor array as tlas[idx])")
                return 0u
            }
        } else {
            let accel_at = arg ?as ExprAt
            let arr_g = accel_at != null ? global_var_of(accel_at.subexpr) : null
            agi = arr_g != null ? (ctx.globals?[intptr(arr_g)] ?? GlobalInfo()) : GlobalInfo()
            if (arr_g == null || !agi.is_accel_array) {
                errs |> push("{who}: the acceleration-structure argument must be an accelerationStructureEXT global or a descriptor-array element (tlas[idx])")
                return 0u
            }
            accel_idx = value_of(accel_at.index)
            if (accel_idx == 0u) {
                errs |> push("{who}: could not evaluate the acceleration-structure array index")
                return 0u
            }
        }
        let accel_id = alloc_id(bm)
        if (agi.is_accel_array) {
            let elem_ptr_t = type_pointer(bm, SpvStorageClass.UniformConstant, agi.value_type)
            let elem_ptr = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, elem_ptr_t, elem_ptr, agi.var_id, accel_idx)
            emit(bm, SEC_FUNCS, SpvOp.Load, agi.value_type, accel_id, elem_ptr)
        } else {
            emit(bm, SEC_FUNCS, SpvOp.Load, agi.value_type, accel_id, agi.var_id)
        }
        return accel_id
    }

    // The rayQueryEXT argument to a ray-query intrinsic: a Function-storage LOCAL named by its pointer
    // (never loaded), so peel any defensive ExprRef2Value and take its lvalue pointer.
    def ray_query_ptr(arg : ExpressionPtr) : uint {
        let r2v = arg ?as ExprRef2Value
        let lv = r2v != null ? r2v.subexpr : arg
        return ptr_of(lv).id
    }

    // A coopmat local argument: peel a defensive ExprRef2Value, then take its Function pointer + coopmat
    // pointee type (coopmatLoad's dst) or load its value (coopmatMulAdd's a/b/c, coopmatStore's mat).
    def coop_local_ptr(arg : ExpressionPtr) : tuple<id : uint; pty : uint> {
        let r2v = arg ?as ExprRef2Value
        let lv = r2v != null ? r2v.subexpr : arg
        return ptr_of(lv)
    }
    def coop_local_val(arg : ExpressionPtr) : uint {
        let r2v = arg ?as ExprRef2Value
        let lv = r2v != null ? r2v.subexpr : arg
        return value_of(lv)
    }
    // Resolve a coopmat Load/Store buffer operand (an @ssbo OR @workgroup array global) + an element index
    // to the OpAccessChain pointer at buf[idx] -- the Pointer operand coopMat Load/Store wants. An @ssbo is
    // a Block struct wrapping a runtime array (member 0, then the element index); a @workgroup shared array
    // is a bare fixed array (a single element index). A real tiled GEMM stages into @workgroup and loads
    // the tiles from SHARED -- loading straight from @ssbo global is memory-bound (no reuse).
    def coop_buf_ptr(buf_arg : ExpressionPtr; idx : uint; who : string) : uint {
        var bm & = unsafe(*m)
        let g = global_var_of(buf_arg)
        let gi = g != null ? (ctx.globals?[intptr(g)] ?? GlobalInfo()) : GlobalInfo()
        if (g != null && gi.is_ssbo) {
            let ptr_t = type_pointer(bm, gi.storage, gi.elem_type)
            let res = alloc_id(bm)
            var ops <- [ptr_t, res, gi.var_id, const_uint(bm, 0u), idx]
            emit_n(bm, SEC_FUNCS, SpvOp.AccessChain, ops)
            delete ops
            return res
        }
        if (g != null && gi.storage == SpvStorageClass.Workgroup && g._type != null && g._type.firstType != null) {
            let pointee = emit_type(bm, g._type.firstType)   // the shared array's element type
            let ptr_t = type_pointer(bm, gi.storage, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, idx)
            return res
        }
        errs |> push("{who}: the buffer argument must be an @ssbo or @workgroup array")
        return 0u
    }
    // cm2: slice a (r0, nr, c0, nc) window off a tensor-layout local for one tensor load/store. Loads
    // the CURRENT layout value and emits OpTensorLayoutSliceNV — the base local is NOT modified, so
    // per-K-tile slicing inside a loop reuses one layout. Returns the sliced layout value id (0 = error).
    def cm2_sliced_layout(tl_arg : ExpressionPtr; a_r0, a_nr, a_c0, a_nc : ExpressionPtr; who : string) : uint {
        var bm & = unsafe(*m)
        let to = tensor_object_type_of(bm, tl_arg._type)
        if (to.id == 0u || to.is_view) {
            errs |> push("{who}: the layout argument must be a tensorLayout2D / tensorLayout2DPad local")
            return 0u
        }
        let lp = coop_local_ptr(tl_arg)
        if (lp.id == 0u) {
            errs |> push("{who}: could not resolve the tensor-layout local")
            return 0u
        }
        let cur = alloc_id(bm)
        emit(bm, SEC_FUNCS, SpvOp.Load, to.id, cur, lp.id)
        let v0 = value_of(a_r0)
        let v1 = value_of(a_nr)
        let v2 = value_of(a_c0)
        let v3 = value_of(a_nc)
        if (v0 == 0u || v1 == 0u || v2 == 0u || v3 == 0u) return 0u
        let res = alloc_id(bm)
        var ops <- [to.id, res, cur, v0, v1, v2, v3]
        emit_n(bm, SEC_FUNCS, SpvOp.TensorLayoutSliceNV, ops)
        delete ops
        return res
    }

    // The `committed` flag of a ray-query accessor: GLSL's bool literal (true = the committed hit).
    // It must be a compile-time constant so the emitter can fold it to the int operand SPIR-V wants.
    def committed_bool_arg(arg : ExpressionPtr) : tuple<ok : bool; value : bool> {
        let r2v = arg ?as ExprRef2Value
        let a = r2v != null ? r2v.subexpr : arg
        let cb = a ?as ExprConstBool
        if (cb == null) return (ok = false, value = false)
        return (ok = true, value = cb.value)
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
    def override visitExprConstDouble(var expr : ExprConstDouble?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_double(bm, expr.value)
        return expr
    }
    def override visitExprConstBool(var expr : ExprConstBool?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_bool(bm, expr.value)
        return expr
    }
    // daslang folds a narrowing convert of a literal (`int16(64)`, `2.0h`) to a 16-bit constant NODE,
    // so it never reaches the ExprCall convert path -- it is a leaf here. The lattice VECTORS have no
    // such leaf by design, so half4(2.0h) stays an OpCompositeConstruct splat of this one constant.
    def override visitExprConstFloat16(var expr : ExprConstFloat16?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_float16(bm, float_to_half_bits(float(expr.value)))
        return expr
    }
    def override visitExprConstInt16(var expr : ExprConstInt16?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_int16(bm, int(expr.value))
        return expr
    }
    def override visitExprConstUInt16(var expr : ExprConstUInt16?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_uint16(bm, uint(expr.value))
        return expr
    }
    def override visitExprConstInt8(var expr : ExprConstInt8?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_int8(bm, int(expr.value))
        return expr
    }
    def override visitExprConstUInt8(var expr : ExprConstUInt8?) : ExpressionPtr {
        var bm & = unsafe(*m)
        e2id[intptr(expr)] = const_uint8(bm, uint(expr.value))
        return expr
    }
    // An enum value is an integer constant of the enum's underlying type. The failure this reports is
    // the 64-bit-based enum: its entry constant has no shader lowering, exactly as a bare int64 has none.
    def override visitExprConstEnumeration(var expr : ExprConstEnumeration?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let cr = emit_const(bm, expr, errs)
        if (!cr.ok) {
            errs |> push("unsupported enum constant '{expr.value}' ({expr._type.baseType})")
            return expr
        }
        e2id[intptr(expr)] = cr.id
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
        // an assignment DESTINATION whose base is addressable: ptr_of (one lane) or emit_write_swizzle
        // (several) chains the component pointer off that base. Emitting a value here would be an
        // OpLoad of the whole destination that nothing reads.
        if (k == copy_dst && key_exists(e2ptr, intptr(expr.value))) return expr
        let gv = global_var_of(expr.value)
        // single-component global -> AccessChain to that one component (keeps it an lvalue: g.x = …).
        // A CONSTANT global (gl_WorkGroupSize, a module-scope `let`) has no OpVariable to chain
        // through, so it takes the value path below: CompositeExtract off the folded OpConstant.
        if (gv != null && length(expr.fields) == 1 &&
            !(ctx.globals?[intptr(gv)] ?? GlobalInfo()).is_constant) {
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
    // Shared by visitExprAt (a raw a[i]) and visitExprCall's `.[]` operator form (the -jit / no-opt
    // lowering of a bounds-checked a[i]): result_expr keys the side-map + gives the element type, so
    // both spellings emit the identical instruction.
    def emit_index_read(base_expr : ExpressionPtr; index_expr : ExpressionPtr; result_expr : ExpressionPtr) : void {
        var bm & = unsafe(*m)
        let k = intptr(result_expr)
        // A matrix COLUMN read (m[i], GLSL's mat[i]): a matrix is an emittable VALUE here, not an
        // addressable composite, so there is no OpVariable to chain through -- extract the column off
        // the loaded matrix. OpCompositeExtract takes LITERAL indices, so the column must be a
        // compile-time constant; a dynamic one is rejected rather than lowered into a blob that only
        // spirv-val would catch. Must precede the local-variable branch below: a value-`let` matrix is
        // an ExprVar with no ptr_id, which that branch would reject as non-indexable.
        let mi = matrix_info(base_expr._type)
        if (mi.ok) {
            let mci = index_expr ?as ExprConstInt
            if (mci == null) {
                errs |> push("indexing a matrix with a non-constant column index is not supported")
                return
            }
            let mbase = value_of(base_expr)
            if (mbase == 0u) return   // operand failed to lower (error already pushed)
            let mrt = emit_type(bm, result_expr._type)
            let mres = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.CompositeExtract, mrt, mres, mbase, uint(mci.value))
            e2id[k] = mres
            return
        }
        // A vector component under an index (GLSL's v[i]) on a base that is a pure VALUE -- a call
        // result, a `let` local, a parameter, a folded constant global -- has no OpVariable to chain a
        // component pointer off, so the component comes off the LOADED vector: OpVectorExtractDynamic,
        // which is exactly what glslang gives `textureLod(...)[i]`. A vector that IS addressable (a
        // `var` local, an ssbo element, a block member) keeps the pointer path below, which is what
        // glslang gives `h_min[i]` -- so this must test addressability, not merely vector-ness.
        // A constant index takes OpCompositeExtract, the literal-operand form, like the swizzle read.
        if (arith_lanes(base_expr._type.baseType) >= 2 && !is_addressable(base_expr)) {
            let vbase = value_of(base_expr)
            if (vbase == 0u) return   // operand failed to lower (error already pushed)
            let vrt = emit_type(bm, result_expr._type)
            let vres = alloc_id(bm)
            let vci = index_expr ?as ExprConstInt
            if (vci != null) {
                emit(bm, SEC_FUNCS, SpvOp.CompositeExtract, vrt, vres, vbase, uint(vci.value))
            } else {
                let vidx = value_of(index_expr)
                if (vidx == 0u) return   // index failed to lower (error already pushed)
                emit(bm, SEC_FUNCS, SpvOp.VectorExtractDynamic, vrt, vres, vbase, vidx)
            }
            e2id[k] = vres
            return
        }
        let lev = base_expr ?as ExprVar
        if (lev != null && (lev.varFlags.local || lev.varFlags.argument || lev.varFlags._block)) {
            let lv = ctx.local_vars?[intptr(lev.variable)] ?? LocalVar()
            if (lv.ptr_id == 0u) {
                errs |> push("indexing local '{lev.name}': only fixed-array `var`/`let` locals are indexable")
                return
            }
            let idx = value_of(index_expr)
            if (idx == 0u) return   // index failed to lower (error already pushed) — no 0-operand access chain
            let pointee = emit_type(bm, result_expr._type)
            let ptr_t = type_pointer(bm, SpvStorageClass.Function, pointee)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, lv.ptr_id, idx)
            e2ptr[k] = res
            e2pty[k] = pointee
            return
        }
        let bv = global_var_of(base_expr)
        if (bv == null) {
            // descriptor array of SSBOs: bufs[slot][i]. The inner ExprAt (bufs[slot]) emits nothing --
            // a lone descriptor slot is not an addressable value -- so the OUTER one reads the pair
            // structurally and emits ONE OpAccessChain (var, slot, member 0, i), exactly as glslang
            // chains `g[slot].data[i]`.
            let dinner = base_expr ?as ExprAt
            if (dinner != null) {
                let dav = global_var_of(dinner.subexpr)
                if (dav != null) {
                    let dgi = ctx.globals?[intptr(dav)] ?? GlobalInfo()
                    if (dgi.is_ssbo_array) {
                        let slot = value_of(dinner.index)
                        let dix = value_of(index_expr)
                        if (slot == 0u || dix == 0u) return
                        let dpointee = dgi.elem_type
                        let dptr_t = type_pointer(bm, SpvStorageClass.StorageBuffer, dpointee)
                        let dres = alloc_id(bm)
                        var dops <- [dptr_t, dres, dgi.var_id, slot, const_uint(bm, 0u), dix]
                        emit_n(bm, SEC_FUNCS, SpvOp.AccessChain, dops)
                        delete dops
                        e2ptr[k] = dres
                        e2pty[k] = dpointee
                        return
                    }
                }
            }
            // Indexing a fixed-array field inside a Block (ubo.lights[i] / ubo.material.kernel[i]) or
            // inside a struct local (s.arr[i]). The subexpr is a chained ExprField whose e2ptr is a
            // pointer to the array. Chain a single index off it, in the storage class the chain roots
            // in -- a Function-storage struct local's array must not be chained as StorageBuffer. For a
            // struct element, plain emit_type would panic (it doesn't lower tStructure) and would
            // also not return the laid-out (deduped) type — so re-acquire it via build_block_struct,
            // which is idempotent under the type/decoration dedup pools.
            if (key_exists(e2ptr, intptr(base_expr))) {
                let idx = value_of(index_expr)
                if (idx == 0u) return   // index failed to lower (error already pushed)
                let base = ptr_of(base_expr)
                let rv = root_global_of(base_expr)
                let storage = root_storage_of(base_expr)
                let rname = rv != null ? string(rv.name) : "(?)"
                let pointee = field_pointee_type(bm, result_expr._type, "{rname}[]", storage)
                if (pointee == 0u) return
                let ptr_t = type_pointer(bm, storage, pointee)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, base.id, idx)
                e2ptr[k] = res
                e2pty[k] = pointee
                if (storage == SpvStorageClass.PhysicalStorageBuffer && scalar_class(result_expr._type.baseType) >= 0) {
                    // the Aligned literal must be a POWER OF TWO — the std430 alignment, not the size
                    // (a 3-lane vector's size is 6/12, its alignment 8/16)
                    e2align[k] = uint(block_scalar_align(result_expr._type.baseType, BlockLayoutRules.std430))
                }
                return
            }
            errs |> push("indexing is only supported on a global ssbo array, a local fixed array, or an array field inside a @uniform/@push_constant/@ssbo block")
            return
        }
        let gi = ctx.globals?[intptr(bv)] ?? GlobalInfo()
        // BARE array global (not wrapped in a Block struct), so the access chain is a single index off
        // the variable -- no member-0 indirection like an SSBO. Two flavours: @workgroup shared arrays
        // and the mesh per-vertex / per-primitive Output arrays (gl_MeshVerticesEXT[i] /
        // gl_PrimitiveTriangleIndicesEXT[i]), plus @private_global per-invocation arrays. The mesh
        // per-vertex element is a Block struct that emit_type can't lower, so use the classified
        // elem_type; @workgroup / @private_global scalar/vector elements lower fine via emit_type.
        if (gi.storage == SpvStorageClass.Workgroup || gi.storage == SpvStorageClass.Private || gi.is_mesh_output || gi.is_stage_io_array) {
            let idx = value_of(index_expr)
            if (idx == 0u) return   // index failed to lower (error already pushed)
            let pointee = (gi.is_mesh_output && gi.elem_type != 0u) ? gi.elem_type : emit_type(bm, result_expr._type)
            let res = alloc_id(bm)
            let ptr_t = type_pointer(bm, gi.storage, pointee)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, gi.var_id, idx)
            e2ptr[k] = res
            e2pty[k] = pointee
            return
        }
        // descriptor array of samplers (sampler2D[N], etc.): the ExprAt is read STRUCTURALLY by the
        // texture/textureLod/textureCompare/textureSize/texelFetch handler (resolve_sampler_arg),
        // which emits the OpAccessChain + OpLoad and cascades NonUniform when warranted. Skip
        // emission here -- leaving no e2id/e2ptr entry is fine because the call-site walks the AST
        // directly; any other consumer (which doesn't exist for an opaque sampler value) would
        // surface a clean "no rvalue available" fail-closed.
        // ...and a descriptor array of storage images (array<image3D>, ...) is the same deal: imageLoad
        // / imageStore / imageSize read the ExprAt structurally (resolve_image_arg) and emit the
        // OpAccessChain + OpLoad themselves.
        // ...and a descriptor array of acceleration structures (array<accelerationStructureEXT>) too:
        // traceRayEXT reads the tlas[idx] ExprAt structurally and emits its own OpAccessChain + OpLoad.
        // ...and a descriptor array of SSBOs (array<array<T>>) too: bufs[slot] alone addresses nothing,
        // so the OUTER ExprAt (bufs[slot][i]) reads this one structurally and emits the whole chain.
        if (gi.is_sampler_array || gi.is_storage_image_array || gi.is_accel_array || gi.is_ssbo_array) {
            return
        }
        if (!gi.is_ssbo) {
            errs |> push("only ssbo, @workgroup, or sampler / storage-image descriptor-array indexing is supported for now")
            return
        }
        let idx = value_of(index_expr)
        if (idx == 0u) return   // index failed to lower (error already pushed) — no 0-operand access chain
        // use the classified element type-id: a struct element can't go through emit_type
        let pointee = gi.elem_type != 0u ? gi.elem_type : emit_type(bm, result_expr._type)
        let res = alloc_id(bm)
        let ptr_t = type_pointer(bm, gi.storage, pointee)
        var ops <- [ptr_t, res, gi.var_id, const_uint(bm, 0u), idx]
        emit_n(bm, SEC_FUNCS, SpvOp.AccessChain, ops)
        delete ops
        e2ptr[k] = res
        e2pty[k] = pointee
    }
    def override visitExprAt(var expr : ExprAt?) : ExpressionPtr {
        emit_index_read(expr.subexpr, expr.index, expr)
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
            // scalar_class, NOT arith_class: the increment is const_one, which only ever emits a
            // 32-bit constant, so a 16-bit ++ would build OpIAdd(%short, %int_1). Fail it closed.
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
        let uo = unop_code(op, arith_class(expr.subexpr._type.baseType))
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
                    let bo = binop_code(compound_base(op), arith_class(expr.left._type.baseType))
                    if (!bo.ok) {
                        errs |> push("unsupported compound assignment '{expr.op}'")
                        return expr
                    }
                    // vector <op>= scalar: splat the scalar rhs (SPIR-V arithmetic operands must match)
                    var rhs2 = rhs
                    let dn = arith_lanes(expr.left._type.baseType)
                    if (dn > 1 && arith_lanes(expr.right._type.baseType) == 1) {
                        rhs2 = splat_to_vec(bm, splat_type(bm, expr.right._type, dn), rhs, dn)
                    }
                    res = alloc_id(bm)
                    emit(bm, SEC_FUNCS, bo.code, p.pty, res, cur, rhs2)
                }
                emit(bm, SEC_FUNCS, SpvOp.Store, p.id, res)
            }
            return expr
        }
        let lbt = value_base_type(expr.left._type)
        let cls = arith_class(lbt)
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
            // Whole-vector `==` / `!=` yields a SCALAR bool in daslang (and GLSL), but the SPIR-V
            // component-wise compare op produces a bvecN — reduce it to the scalar with OpAll
            // (`==`: all lanes equal) / OpAny (`!=`: any lane differs), matching what glslang emits.
            // A scalar-bool-typed OpFOrdEqual on vector operands is invalid SPIR-V (drivers reject it).
            let dn = arith_lanes(lbt)
            if (dn > 1) {
                let bvec = type_vector(bm, bt, dn)
                let cmp = alloc_id(bm)
                emit(bm, SEC_FUNCS, cc.code, bvec, cmp, l, r)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, op == "!=" ? SpvOp.Any : SpvOp.All, bt, res, cmp)
                e2id[intptr(expr)] = res
                return expr
            }
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
        // SPIR-V arithmetic operands must match the result type: splat a scalar operand against a
        // vector one (daslang broadcasts vec <op> scalar; `*` already went through emit_mul above)
        var lid = l
        var rid = r
        let rn = arith_lanes(expr._type.baseType)
        if (rn > 1) {
            if (arith_lanes(expr.left._type.baseType) == 1) {
                lid = splat_to_vec(bm, splat_type(bm, expr.left._type, rn), l, rn)
            }
            if (arith_lanes(expr.right._type.baseType) == 1) {
                rid = splat_to_vec(bm, splat_type(bm, expr.right._type, rn), r, rn)
            }
        }
        let res = alloc_id(bm)
        emit(bm, SEC_FUNCS, bo.code, rt, res, lid, rid)
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
        let n = component_count(value_base_type(expr._type))
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
    // The destination subtree is marked while it is walked, so a swizzle target emits no value: it is
    // a pointer its consumer builds, and the value path would spend an OpLoad of the whole destination
    // that nothing reads. glslang emits neither.
    def override preVisitExprCopy(var expr : ExprCopy?) : void {
        copy_dst = intptr(expr.left)
    }
    def override preVisitExprCopyRight(var expr : ExprCopy?; right : ExpressionPtr) : void {
        copy_dst = 0ul
    }
    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let sw = expr.left ?as ExprSwizzle
        if (sw != null && length(sw.fields) > 1) {
            emit_write_swizzle(sw, expr.right)
            return expr
        }
        let p = ptr_of(expr.left)
        let val = value_of(expr.right)
        if (p.id != 0u && val != 0u) {
            emit(bm, SEC_FUNCS, SpvOp.Store, p.id, val)
        }
        return expr
    }

    // GLSL's write swizzle (`buf[i].xyz = pos`). SPIR-V has no partial store, and glslang
    // does NOT load-shuffle-store it: it chains ONE component pointer per lane and stores the matching
    // lane of the rhs. Lower it the same way, so the port comes out instruction for instruction.
    // daslang only takes a CONTIGUOUS ascending mask as a reference (`.xz` is error[30915]), so the
    // lanes are distinct and each is written exactly once -- the order of the stores cannot matter.
    def emit_write_swizzle(sw : ExprSwizzle?; rhs : ExpressionPtr) : void {
        var bm & = unsafe(*m)
        if (!key_exists(e2ptr, intptr(sw.value))) {
            errs |> push("cannot assign through a swizzle of a value — a write-swizzle target must be addressable (a global, a `var` local, or an SSBO / UBO element)")
            return
        }
        let val = value_of(rhs)
        if (val == 0u) return
        let base = ptr_of(sw.value)
        if (base.id == 0u) return
        let storage = root_storage_of(sw.value)
        let comp = emit_component_type(bm, sw._type)
        let ptr_t = type_pointer(bm, storage, comp)
        for (i in range(length(sw.fields))) {
            let cptr = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, cptr, base.id, const_uint(bm, uint(sw.fields[i])))
            let lane = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.CompositeExtract, comp, lane, val, uint(i))
            emit(bm, SEC_FUNCS, SpvOp.Store, cptr, lane)
        }
    }

    // ----- let: memory local -> store init; value-let -> bind SSA id; reference local -> alias -----
    def override visitExprLetVariable(cexpr : ExprLet?; var arg : VariablePtr; lastArg : bool) : VariablePtr {
        var bm & = unsafe(*m)
        // A reference local IS the pointer its initializer lowered to, so it binds that pointer
        // rather than storing through one of its own -- reads and writes through the alias then
        // reach the aliased element like any other addressable local.
        if (arg._type.flags.ref) {
            if (arg.init == null) {
                errs |> push("reference local '{arg.name}' has no initializer to bind")
                return arg
            }
            let rp = ptr_of(arg.init)
            if (rp.id == 0u) {
                errs |> push("reference local '{arg.name}' must bind an addressable expression (a global, a `var` local, or an SSBO / UBO element)")
                return arg
            }
            ctx.local_vars |> insert(intptr(arg), LocalVar(ptr_id = rp.id, value_type = rp.pty))
            return arg
        }
        if (arg.init == null) {
            // daslang DROPS an initializer it proves dead -- a `var` that every path assigns before
            // it is ever read (`var l = 0.0; if (..) { l = a } else { l = b }`) reaches us with no
            // init at all. das still zero-initializes such a local, so lower it to that zero instead
            // of refusing it; a value-let cannot get here (it has nowhere to store one).
            let vk0 = intptr(arg)
            if (!key_exists(ctx.local_vars, vk0)) {
                errs |> push("uninitialized local '{arg.name}' is not supported (every let/var must have an initializer)")
                return arg
            }
            let lv0 = ctx.local_vars?[vk0] ?? LocalVar()
            // an opaque handle (rayQueryEXT) has no zero value: OpConstantNull of an opaque type is
            // illegal. It is initialized by its intrinsic (rayQueryInitializeEXT), never by a store.
            if (lv0.is_opaque) return arg
            emit(bm, SEC_FUNCS, SpvOp.Store, lv0.ptr_id, const_null(bm, lv0.value_type))
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
        // TaskPayloadWorkgroupEXT (a @task_payload) and Function (a struct local) are PLAIN logical
        // storage classes -- no Block / Offset / ArrayStride -- so a field type lowers via
        // emit_value_type: the SAME id build_plain_struct gave the member, so the OpAccessChain types
        // match. (Block storages below take the std140/std430 strided path.)
        if (storage == SpvStorageClass.TaskPayloadWorkgroupEXT || storage == SpvStorageClass.Function) {
            return emit_value_type(bm, ft, errs)
        }
        // The packing rules follow the root global's storage class: an @ssbo member access rebuilds
        // its member types under std430 (tight, width-aware strides), everything else under std140.
        // MUST stay in lockstep with build_block_struct — the stride is part of the type-dedup key,
        // so a rules mismatch here yields a DIFFERENT OpTypeArray than the struct's member type and
        // the OpAccessChain result type no longer matches (spirv-val failure). A cm2 decode function's
        // PhysicalStorageBuffer block is laid out std430 too (register_decode_func builds it that way).
        let rules = ((storage == SpvStorageClass.StorageBuffer || storage == SpvStorageClass.PhysicalStorageBuffer)
            ? BlockLayoutRules.std430 : BlockLayoutRules.std140)
        // matrices come back through emit_type -> type_matrix correctly; only tStructure and
        // tFixedArray need the layout-aware path.
        if (ft.baseType == Type.tStructure && ft.structType != null) {
            let inner = build_block_struct(bm, sname, ft.structType, false, errs, rules)
            if (!inner.ok) return 0u
            return inner.type_id
        }
        if (ft.baseType == Type.tFixedArray && ft.firstType != null) {
            let et = ft.firstType
            let nelem = ft.fixedDim
            var elem_type : uint = 0u
            if (et.baseType == Type.tStructure && et.structType != null) {
                let inner = build_block_struct(bm, "{sname}[]", et.structType, false, errs, rules)
                if (!inner.ok) return 0u
                elem_type = inner.type_id
            } else {
                ensure_member_storage_caps(bm, et.baseType, rules)
                elem_type = emit_type(bm, et)
            }
            // element stride: scalar/vector rounds up to vec4 under std140 / packs tight under
            // std430; struct element strides by its laid-out size rounded to the struct alignment.
            // The struct-element probe routes its diagnostics through the visitor's `errs` so a
            // layout failure here surfaces the same detailed message a direct classify_global path
            // would (was a local array that swallowed errors).
            var stride = 0
            if (et.baseType == Type.tStructure && et.structType != null) {
                var probe : array<BlockField>
                let pl = compute_block_layout("{sname}[]", et.structType, probe, errs, rules)
                delete probe
                if (!pl.ok) return 0u
                let salign = rules == BlockLayoutRules.std140 ? round_up(pl.align, 16) : pl.align
                stride = round_up(pl.size, salign)
            } else {
                stride = block_array_element_stride(et.baseType, rules)
            }
            return type_array_strided(bm, elem_type, nelem, stride)
        }
        // a terminal sub-32 member type still needs the storage cap (e.g. reading `blk.d : float16`
        // directly through an access chain)
        ensure_member_storage_caps(bm, ft.baseType, rules)
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
                // chain the field pointer in the storage class the chain roots in (StorageBuffer for an
                // SSBO, Uniform for a UBO, PushConstant for @push_constant, Function for a struct local).
                let storage = root_storage_of(expr.value)
                let pointee = field_pointee_type(bm, expr._type, "{est.name}.{expr.name}", storage)
                if (pointee == 0u) return expr
                let ptr_t = type_pointer(bm, storage, pointee)
                let res = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.AccessChain, ptr_t, res, base.id, const_uint(bm, uint(midx)))
                e2ptr[k] = res
                e2pty[k] = pointee
                if (storage == SpvStorageClass.PhysicalStorageBuffer && scalar_class(expr._type.baseType) >= 0) {
                    // power-of-two Aligned literal — std430 alignment, not size (see the ExprAt twin)
                    e2align[k] = uint(block_scalar_align(expr._type.baseType, BlockLayoutRules.std430))
                }
                return expr
            }
            // member of a local struct VALUE (e.g. `let inst = instances[i]; inst.pos_scale`): the let
            // holds the loaded composite (value_of in visitExprLetVariable), so extract the member by
            // index -- the value-path twin of the e2ptr access-chain branch above. Reads only (a `let`).
            // Gate on the base having actually lowered to a value id (e2id): a struct-typed base that did
            // NOT lower (e.g. an unsupported struct-returning call `foo().field`) falls through to the
            // clean "not supported" error below, rather than a confusing "internal: no rvalue" out of value_of.
            let lest = expr.value._type.structType
            if (lest != null && key_exists(e2id, intptr(expr.value))) {
                let lmidx = field_index_by_name(lest, "{expr.name}")
                if (lmidx < 0) {
                    errs |> push("member access '.{expr.name}': no such field on '{lest.name}'")
                    return expr
                }
                // A nested struct / fixed-array member would need its laid-out type to match the loaded
                // composite -- not handled here; fail closed rather than emit a mismatched extract.
                if (expr._type != null && (expr._type.baseType == Type.tStructure || expr._type.baseType == Type.tFixedArray)) {
                    errs |> push("member access '.{expr.name}': struct/array members of a local struct value are not yet supported; read the member directly from its source block element/field instead of copying the struct into a local")
                    return expr
                }
                let lbase = value_of(expr.value)
                if (lbase == 0u) return expr   // base failed to lower (error already pushed)
                let lrt = emit_type(bm, expr._type)
                let lres = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.CompositeExtract, lrt, lres, lbase, uint(lmidx))
                e2id[k] = lres
                return expr
            }
            errs |> push("member access '.{expr.name}' is only supported on a @uniform/@ssbo/@push_constant block global, an indexed ssbo struct element, or a local struct value for now")
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
            let is_ref = i < length(f.arguments) && is_out_param(f.arguments[i]._type)
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

    // Flag EXACTLY a `.[]` runtime-index call's __context__ / __lineinfo__ tail (args 2/3) by node
    // identity, so the ExprFakeContext / ExprFakeLineInfo rejects elide them; scoping to those nodes
    // (not the whole `.[]` subtree) keeps an explicit __context__ in the base / index failing closed.
    def override preVisitExprCallArgument(expr : ExprCall?; arg : ExpressionPtr; last : bool) : void {
        suppress_bounds_arg = (expr.name == ".[]" && length(expr.arguments) > 3 &&
            (arg == expr.arguments[2] || arg == expr.arguments[3]))
        // the trailing @@decode argument of coopmatLoadTensorDecode is read STRUCTURALLY by the cm2
        // load arm (registered as a DecodeFunc operand) — it never lowers to a value, so the @@
        // fail-closed guard must not fire on it. Set ONLY when the argument actually is an ExprAddr
        // (a non-@@ trailing arg gets the arm's own clean error, and the flag can never leak onto an
        // unrelated later @@). Generic stub -> match the ORIGIN name. Assigned per argument, like
        // suppress_bounds_arg above, so it also self-clears on the next visited argument.
        suppress_fn_addr = false
        if (last && expr.func != null) {
            let r2v = arg ?as ExprRef2Value
            let da = r2v != null ? r2v.subexpr : arg
            if (da is ExprAddr) {
                var co = expr.func
                while (co.fromGeneric != null) {
                    co = co.fromGeneric
                }
                if ("{co.name}" == "coopmatLoadTensorDecode") {
                    suppress_fn_addr = true
                }
            }
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
        // A bounds-checked a[i] lowers to a `.[](base, index, __context__, __lineinfo__)` OPERATOR call
        // under -jit / no-opt (a const index folds to a raw ExprAt), so route base/index through the
        // same emit_index_read the ExprAt path uses. The __context__ / __lineinfo__ tail (a trapping
        // out-of-range panic in the interpreter) is elided in preVisitExprCallArgument; SPIR-V has no
        // trap. The unchecked form carries just (base, index).
        if (name == ".[]") {
            let argc = length(expr.arguments)
            if (argc == 2 || argc == 4) {
                emit_index_read(expr.arguments[0], expr.arguments[1], expr)
                return expr
            }
        }
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
        // traceRayEXT(accel, ray_flags, cull_mask, sbt_offset, sbt_stride, miss_index, origin, tmin,
        // direction, tmax, payload): OpTraceRayKHR. The accel operand is an OpLoad of the
        // accelerationStructureEXT descriptor; the payload operand names the @ray_payload /
        // @incoming_ray_payload VARIABLE itself (a pointer, per SPV_KHR_ray_tracing), so the last
        // argument must be a payload global, not an arbitrary expression. RT-pipeline stages only.
        // The stub is GENERIC over the payload type, so the resolved call carries the mangled
        // instance name — match on the ORIGIN generic's name instead of the ExprCall name.
        var origin_name = name
        if (expr.func != null) {
            var co = expr.func
            while (co.fromGeneric != null) {
                co = co.fromGeneric
            }
            origin_name = "{co.name}"
        }
        if (origin_name == "traceRayEXT") {
            if (!is_rt_stage(ctx.stage)) {
                errs |> push("traceRayEXT is only valid in a raygen / miss / closest-hit shader")
                return expr
            }
            if (length(expr.arguments) != 11) {
                errs |> push("traceRayEXT expects (accel, ray_flags, cull_mask, sbt_offset, sbt_stride, miss_index, origin, tmin, direction, tmax, payload)")
                return expr
            }
            // the payload operand names the @ray_payload / @incoming_ray_payload VARIABLE itself (a
            // pointer, per SPV_KHR_ray_tracing), so the last argument must be a payload global.
            let payload_g = global_var_of(expr.arguments[10])
            let pgi = payload_g != null ? (ctx.globals?[intptr(payload_g)] ?? GlobalInfo()) : GlobalInfo()
            if (payload_g == null || !pgi.is_ray_payload) {
                errs |> push("traceRayEXT: the payload argument must be a @ray_payload / @incoming_ray_payload global")
                return expr
            }
            var vals : array<uint>
            vals |> reserve(9)
            for (ai in range(1, 10)) {
                let vv = value_of(expr.arguments[ai])
                if (vv == 0u) {
                    errs |> push("traceRayEXT: could not evaluate operand {ai}")
                    delete vals
                    return expr
                }
                vals |> push(vv)
            }
            // the accel operand -- a single descriptor OpLoad, or an array element (tlas[idx]) ->
            // OpAccessChain + OpLoad -- is emitted LAST, after the value operands (unchanged order).
            let accel_id = load_accel_operand(expr.arguments[0], "traceRayEXT")
            if (accel_id == 0u) {
                delete vals
                return expr
            }
            var ops <- [accel_id]
            ops |> push_from(vals)
            ops |> push(pgi.var_id)
            emit_n(bm, SEC_FUNCS, SpvOp.TraceRayKHR, ops)
            delete ops
            delete vals
            return expr
        }
        // Inline ray query (SPV_KHR_ray_query), the compute-shader analogue of traceRayEXT: the rq
        // argument is a Function-storage rayQueryEXT LOCAL named by its pointer (never loaded); the
        // accel is loaded exactly as traceRayEXT's is. Non-generic, so matched on the bare call name.
        // OpRayQueryInitializeKHR(rq, accel, ray_flags, cull_mask, origin, tmin, direction, tmax): 8
        // operands, no result. The value operands (ray_flags..tmax) load before the accel, so the
        // accel Load is emitted last -- the same order traceRayEXT uses.
        if (name == "rayQueryInitializeEXT") {
            if (length(expr.arguments) != 8) {
                errs |> push("rayQueryInitializeEXT expects (rq, accel, ray_flags, cull_mask, origin, tmin, direction, tmax)")
                return expr
            }
            let rqp = ray_query_ptr(expr.arguments[0])
            if (rqp == 0u) {
                errs |> push("rayQueryInitializeEXT: the first argument must be a rayQueryEXT local")
                return expr
            }
            var vals : array<uint>
            vals |> reserve(6)
            for (ai in range(2, 8)) {
                let vv = value_of(expr.arguments[ai])
                if (vv == 0u) {
                    errs |> push("rayQueryInitializeEXT: could not evaluate operand {ai}")
                    delete vals
                    return expr
                }
                vals |> push(vv)
            }
            let accel_id = load_accel_operand(expr.arguments[1], "rayQueryInitializeEXT")
            if (accel_id == 0u) {
                delete vals
                return expr
            }
            var ops <- [rqp, accel_id]
            ops |> push_from(vals)
            emit_n(bm, SEC_FUNCS, SpvOp.RayQueryInitializeKHR, ops)
            delete ops
            delete vals
            return expr
        }
        // OpRayQueryProceedKHR(rq) -> bool: steps the traversal, returning true while candidates remain
        // (GLSL drives it as `while (rayQueryProceedEXT(rq)) ;`).
        if (name == "rayQueryProceedEXT") {
            if (length(expr.arguments) != 1) {
                errs |> push("rayQueryProceedEXT expects (rq)")
                return expr
            }
            let rqp = ray_query_ptr(expr.arguments[0])
            if (rqp == 0u) {
                errs |> push("rayQueryProceedEXT: the argument must be a rayQueryEXT local")
                return expr
            }
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.RayQueryProceedKHR, rt, res, rqp)
            e2id[k] = res
            return expr
        }
        // The ray-query accessors: OpRayQueryGetIntersection{Type,InstanceCustomIndex,PrimitiveIndex,
        // Barycentrics,T}KHR(rq, committed) -> a scalar/vec2 read of the chosen (candidate/committed)
        // intersection. Result type comes from the intrinsic's own return type; the `committed` flag
        // is a 32-bit SIGNED-int constant (0/1), which is what glslang lowers GLSL's `true` to.
        if (name == "rayQueryGetIntersectionTypeEXT" ||
            name == "rayQueryGetIntersectionInstanceCustomIndexEXT" ||
            name == "rayQueryGetIntersectionPrimitiveIndexEXT" ||
            name == "rayQueryGetIntersectionBarycentricsEXT" ||
            name == "rayQueryGetIntersectionTEXT") {
            if (length(expr.arguments) != 2) {
                errs |> push("{name} expects (rq, committed)")
                return expr
            }
            let rqp = ray_query_ptr(expr.arguments[0])
            if (rqp == 0u) {
                errs |> push("{name}: the first argument must be a rayQueryEXT local")
                return expr
            }
            let cb = committed_bool_arg(expr.arguments[1])
            if (!cb.ok) {
                errs |> push("{name}: the committed argument must be a bool literal (true / false)")
                return expr
            }
            let committed_id = const_int(bm, cb.value ? 1 : 0)
            var op = SpvOp.RayQueryGetIntersectionTypeKHR
            if (name == "rayQueryGetIntersectionInstanceCustomIndexEXT") {
                op = SpvOp.RayQueryGetIntersectionInstanceCustomIndexKHR
            } elif (name == "rayQueryGetIntersectionPrimitiveIndexEXT") {
                op = SpvOp.RayQueryGetIntersectionPrimitiveIndexKHR
            } elif (name == "rayQueryGetIntersectionBarycentricsEXT") {
                op = SpvOp.RayQueryGetIntersectionBarycentricsKHR
            } elif (name == "rayQueryGetIntersectionTEXT") {
                op = SpvOp.RayQueryGetIntersectionTKHR
            }
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, op, rt, res, rqp, committed_id)
            e2id[k] = res
            return expr
        }
        // Cooperative matrices (SPV_KHR_cooperative_matrix). All compute-only (the tile scope is the
        // subgroup). coopmatLoad(dst, src, idx, stride, layout): OpCooperativeMatrixLoadKHR from an @ssbo
        // element pointer into the dst tile local (result-type = dst's coopmat type), then OpStore.
        // coopmatStore(mat, dst, idx, stride, layout): OpCooperativeMatrixStoreKHR of the loaded tile.
        // coopmatMulAdd(a, b, c): OpCooperativeMatrixMulAddKHR (float accumulate -> no operands mask).
        // `layout` is a compile-time-constant 0 (row-major) / 1 (column-major); `stride` is an id operand.
        // coopmatLoad/coopmatStore have an untyped buffer param, so they are GENERIC — match the origin
        // name (name is the mangled instance name), like the coopmat markers this file also matches.
        if (origin_name == "coopmatLoad" || origin_name == "coopmatStore") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("{origin_name} is only valid in a compute shader")
                return expr
            }
            if (length(expr.arguments) != 5) {
                errs |> push("{origin_name} expects ({origin_name == "coopmatLoad" ? "dst, src" : "mat, dst"}, idx, stride, layout)")
                return expr
            }
            let idx = value_of(expr.arguments[2])
            let stride_id = value_of(expr.arguments[3])
            let lay = const_uint_value(expr.arguments[4])
            if (idx == 0u || stride_id == 0u) return expr
            if (!lay.ok || lay.value > 1u) {
                errs |> push("{origin_name}: layout must be a constant 0 (row-major) or 1 (column-major)")
                return expr
            }
            let layout_id = const_uint(bm, lay.value)
            if (origin_name == "coopmatLoad") {
                let dst = coop_local_ptr(expr.arguments[0])
                if (dst.id == 0u || dst.pty == 0u) {
                    errs |> push("coopmatLoad: the first argument must be a coopmat local")
                    return expr
                }
                let sptr = coop_buf_ptr(expr.arguments[1], idx, "coopmatLoad")
                if (sptr == 0u) return expr
                let res = alloc_id(bm)
                var ld <- [dst.pty, res, sptr, layout_id, stride_id]
                emit_n(bm, SEC_FUNCS, SpvOp.CooperativeMatrixLoadKHR, ld)
                delete ld
                emit(bm, SEC_FUNCS, SpvOp.Store, dst.id, res)
            } else {
                let obj = coop_local_val(expr.arguments[0])
                if (obj == 0u) {
                    errs |> push("coopmatStore: the first argument must be a coopmat value")
                    return expr
                }
                let dptr = coop_buf_ptr(expr.arguments[1], idx, "coopmatStore")
                if (dptr == 0u) return expr
                var st <- [dptr, obj, layout_id, stride_id]
                emit_n(bm, SEC_FUNCS, SpvOp.CooperativeMatrixStoreKHR, st)
                delete st
            }
            return expr
        }
        if (name == "coopmatMulAdd") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("coopmatMulAdd is only valid in a compute shader")
                return expr
            }
            if (length(expr.arguments) != 3) {
                errs |> push("coopmatMulAdd expects (a, b, c)")
                return expr
            }
            let va = coop_local_val(expr.arguments[0])
            let vb = coop_local_val(expr.arguments[1])
            let vc = coop_local_val(expr.arguments[2])
            if (va == 0u || vb == 0u || vc == 0u) {
                errs |> push("coopmatMulAdd: could not evaluate an operand")
                return expr
            }
            let ci = coopmat_info(expr._type)
            let rt = coopmat_type_of(bm, expr._type)
            if (rt == 0u) {
                errs |> push("coopmatMulAdd: result must be a coopmat accumulator type")
                return expr
            }
            let res = alloc_id(bm)
            // integer MulAdd carries a CooperativeMatrixOperands mask declaring operand signedness (A/B/C/
            // Result signed); float accumulate takes no mask. The mask is an inline bitmask LITERAL.
            var ops <- [rt, res, va, vb, vc]
            if (!ci.is_float && ci.signed) {
                ops |> push(uint(SpvCooperativeMatrixOperands.MatrixASignedComponentsKHR)
                    | uint(SpvCooperativeMatrixOperands.MatrixBSignedComponentsKHR)
                    | uint(SpvCooperativeMatrixOperands.MatrixCSignedComponentsKHR)
                    | uint(SpvCooperativeMatrixOperands.MatrixResultSignedComponentsKHR))
            }
            emit_n(bm, SEC_FUNCS, SpvOp.CooperativeMatrixMulAddKHR, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        // coopmatConvert(dst_f32_acc, src_f16_acc): componentwise OpFConvert over the tile — the
        // f16-accumulate path's widening before an f32 store (llama.cpp's cast-then-coopMatStore)
        if (name == "coopmatConvert") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("coopmatConvert is only valid in a compute shader")
                return expr
            }
            if (length(expr.arguments) != 2) {
                errs |> push("coopmatConvert expects (dst, src)")
                return expr
            }
            let di = coopmat_info(expr.arguments[0]._type)
            let si = coopmat_info(expr.arguments[1]._type)
            if (!di.ok || !si.ok || !di.is_float || !si.is_float || di.comp_width != 32u || si.comp_width != 16u
                    || di.use != si.use || di.rows != si.rows || di.cols != si.cols) {
                errs |> push("coopmatConvert expects (f32 accumulator dst, f16 accumulator src) of one tile shape")
                return expr
            }
            let dst = coop_local_ptr(expr.arguments[0])
            let vs = coop_local_val(expr.arguments[1])
            if (dst.id == 0u || dst.pty == 0u || vs == 0u) {
                errs |> push("coopmatConvert: both arguments must be coopmat locals")
                return expr
            }
            let res = alloc_id(bm)
            var cops <- [dst.pty, res, vs]
            emit_n(bm, SEC_FUNCS, SpvOp.FConvert, cops)
            delete cops
            var sops <- [dst.id, res]
            emit_n(bm, SEC_FUNCS, SpvOp.Store, sops)
            delete sops
            return expr
        }
        // ----- cm2 (SPV_NV_cooperative_matrix2 + SPV_NV_tensor_addressing): tensor layout/view objects
        // and tensor-addressed coopmat load/store. Compute-only, like the KHR coopmat arms above. The
        // layout/view stubs have untyped params, so they are GENERIC — match the origin name.
        if (origin_name == "tensorLayoutCreate" || origin_name == "tensorViewCreate") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("{origin_name} is only valid in a compute shader")
                return expr
            }
            if (length(expr.arguments) != 1) {
                errs |> push("{origin_name} expects (dst)")
                return expr
            }
            let want_view = origin_name == "tensorViewCreate"
            let to = tensor_object_type_of(bm, expr.arguments[0]._type)
            if (to.id == 0u || to.is_view != want_view) {
                errs |> push("{origin_name}: the argument must be a {want_view ? "tensorView2D / tensorView2Dt" : "tensorLayout2D / tensorLayout2DPad"} local")
                return expr
            }
            let dst = coop_local_ptr(expr.arguments[0])
            if (dst.id == 0u) {
                errs |> push("{origin_name}: could not resolve the destination local")
                return expr
            }
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, want_view ? SpvOp.CreateTensorViewNV : SpvOp.CreateTensorLayoutNV, to.id, res)
            emit(bm, SEC_FUNCS, SpvOp.Store, dst.id, res)
            return expr
        }
        if (origin_name == "tensorLayoutSetDimension" || origin_name == "tensorLayoutSetStride"
                || origin_name == "tensorLayoutSetBlockSize") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("{origin_name} is only valid in a compute shader")
                return expr
            }
            if (length(expr.arguments) != 3) {
                errs |> push("{origin_name} expects (layout, v0, v1)")
                return expr
            }
            let to = tensor_object_type_of(bm, expr.arguments[0]._type)
            if (to.id == 0u || to.is_view) {
                errs |> push("{origin_name}: the first argument must be a tensorLayout2D / tensorLayout2DPad local")
                return expr
            }
            let dst = coop_local_ptr(expr.arguments[0])
            if (dst.id == 0u) {
                errs |> push("{origin_name}: could not resolve the tensor-layout local")
                return expr
            }
            let v0 = value_of(expr.arguments[1])
            let v1 = value_of(expr.arguments[2])
            if (v0 == 0u || v1 == 0u) return expr
            let cur = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, to.id, cur, dst.id)
            let op = (origin_name == "tensorLayoutSetDimension" ? SpvOp.TensorLayoutSetDimensionNV
                : (origin_name == "tensorLayoutSetStride" ? SpvOp.TensorLayoutSetStrideNV : SpvOp.TensorLayoutSetBlockSizeNV))
            let res = alloc_id(bm)
            var tops <- [to.id, res, cur, v0, v1]
            emit_n(bm, SEC_FUNCS, op, tops)
            delete tops
            emit(bm, SEC_FUNCS, SpvOp.Store, dst.id, res)
            return expr
        }
        // coopmatLoadTensor(dst, src, base, tl, r0, nr, c0, nc [, view]) / …Decode(…, @@fn) /
        // coopmatStoreTensor(mat, dst, base, tl, r0, nr, c0, nc [, view]): slice the window off the
        // layout, then OpCooperativeMatrix{Load,Store}TensorNV at &buf[base]. The Object operand of a
        // load is the tile's CURRENT value (elements a clamped/skipped load leaves in place). The
        // decode form registers the @@-referenced [spirv_decode] function and passes it as the
        // DecodeFunc tensor-addressing operand — dequant-in-load for block-quantized sources.
        if (origin_name == "coopmatLoadTensor" || origin_name == "coopmatLoadTensorDecode"
                || origin_name == "coopmatStoreTensor") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("{origin_name} is only valid in a compute shader")
                return expr
            }
            let is_store = origin_name == "coopmatStoreTensor"
            let is_decode = origin_name == "coopmatLoadTensorDecode"
            let argc = length(expr.arguments)
            if (is_decode ? argc != 9 : (argc != 8 && argc != 9)) {
                errs |> push("{origin_name} expects ({is_store ? "mat, dst" : "dst, src"}, base, layout, r0, nr, c0, nc{is_decode ? ", @@decode" : " [, view]"})")
                return expr
            }
            let ci = coopmat_info(expr.arguments[0]._type)
            if (!ci.ok) {
                errs |> push("{origin_name}: the first argument must be a coopmat local")
                return expr
            }
            let idx = value_of(expr.arguments[2])
            if (idx == 0u) return expr
            let sptr = coop_buf_ptr(expr.arguments[1], idx, origin_name)
            if (sptr == 0u) return expr
            let sliced = cm2_sliced_layout(expr.arguments[3], expr.arguments[4], expr.arguments[5],
                expr.arguments[6], expr.arguments[7], origin_name)
            if (sliced == 0u) return expr
            var ta_mask = 0u
            var ta_extra = 0u
            if (is_decode) {
                let da0 = expr.arguments[8]
                let r2v = da0 ?as ExprRef2Value
                let da = r2v != null ? r2v.subexpr : da0
                let fa = da ?as ExprAddr
                if (fa == null || fa.func == null) {
                    errs |> push("coopmatLoadTensorDecode: the last argument must be @@<decode function>")
                    return expr
                }
                if (!function_has_annotation(fa.func, "spirv_decode")) {
                    errs |> push("coopmatLoadTensorDecode: '{fa.func.name}' must be annotated [spirv_decode]")
                    return expr
                }
                let fid = register_decode_func(bm, *ctx, fa.func, errs)
                if (fid == 0u) return expr
                ta_mask = uint(SpvTensorAddressingOperands.DecodeFunc)
                ta_extra = fid
            } elif (argc == 9) {
                let vo = tensor_object_type_of(bm, expr.arguments[8]._type)
                if (vo.id == 0u || !vo.is_view) {
                    errs |> push("{origin_name}: the trailing argument must be a tensorView2D / tensorView2Dt local")
                    return expr
                }
                let vp = coop_local_ptr(expr.arguments[8])
                if (vp.id == 0u) return expr
                let vv = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, vo.id, vv, vp.id)
                ta_mask = uint(SpvTensorAddressingOperands.TensorView)
                ta_extra = vv
            }
            if (is_store) {
                let obj = coop_local_val(expr.arguments[0])
                if (obj == 0u) {
                    errs |> push("coopmatStoreTensor: the first argument must be a coopmat value")
                    return expr
                }
                var ops <- [sptr, obj, sliced, uint(SpvMemoryAccess.None), ta_mask]
                if (ta_extra != 0u) {
                    ops |> push(ta_extra)
                }
                emit_n(bm, SEC_FUNCS, SpvOp.CooperativeMatrixStoreTensorNV, ops)
                delete ops
            } else {
                let dst = coop_local_ptr(expr.arguments[0])
                if (dst.id == 0u || dst.pty == 0u) {
                    errs |> push("{origin_name}: the first argument must be a coopmat local")
                    return expr
                }
                let cur_obj = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Load, dst.pty, cur_obj, dst.id)
                let res = alloc_id(bm)
                var ops <- [dst.pty, res, sptr, cur_obj, sliced, uint(SpvMemoryAccess.None), ta_mask]
                if (ta_extra != 0u) {
                    ops |> push(ta_extra)
                }
                emit_n(bm, SEC_FUNCS, SpvOp.CooperativeMatrixLoadTensorNV, ops)
                delete ops
                emit(bm, SEC_FUNCS, SpvOp.Store, dst.id, res)
            }
            return expr
        }
        // coopmatClamp(mat, lo, hi): per-element FClamp over a float accumulator tile — the
        // f16-accumulate ±65504 overflow guard. OpCooperativeMatrixLengthKHR bounds a hand-emitted
        // structured loop that AccessChains each element of the Function-storage tile local.
        if (origin_name == "coopmatClamp") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("coopmatClamp is only valid in a compute shader")
                return expr
            }
            if (length(expr.arguments) != 3) {
                errs |> push("coopmatClamp expects (mat, lo, hi)")
                return expr
            }
            let ci = coopmat_info(expr.arguments[0]._type)
            if (!ci.ok || !ci.is_float) {
                errs |> push("coopmatClamp: the first argument must be a float coopmat local")
                return expr
            }
            let dst = coop_local_ptr(expr.arguments[0])
            if (dst.id == 0u || dst.pty == 0u) {
                errs |> push("coopmatClamp: could not resolve the coopmat local")
                return expr
            }
            let lo32 = value_of(expr.arguments[1])
            let hi32 = value_of(expr.arguments[2])
            if (lo32 == 0u || hi32 == 0u) return expr
            let comp_t = type_float(bm, ci.comp_width)
            var lo = lo32
            var hi = hi32
            if (ci.comp_width != 32u) {
                lo = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.FConvert, comp_t, lo, lo32)
                hi = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.FConvert, comp_t, hi, hi32)
            }
            let glsl = ensure_glsl_ext(bm, *ctx)
            let ut = type_uint(bm)
            let len = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.CooperativeMatrixLengthKHR, ut, len, dst.pty)
            // structured loop: pre -> hdr (phi i) -> body -> cont -> hdr; merge exits. The fresh `pre`
            // block exists so the phi has a KNOWN entry predecessor (the current block's label id is
            // not tracked); `inext` is forward-referenced by the phi from the back edge, which OpPhi allows.
            let pre = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Branch, pre)
            emit(bm, SEC_FUNCS, SpvOp.Label, pre)
            let hdr = alloc_id(bm)
            let body = alloc_id(bm)
            let cont = alloc_id(bm)
            let merge = alloc_id(bm)
            let iv = alloc_id(bm)
            let inext = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Branch, hdr)
            emit(bm, SEC_FUNCS, SpvOp.Label, hdr)
            var phi <- [ut, iv, const_uint(bm, 0u), pre, inext, cont]
            emit_n(bm, SEC_FUNCS, SpvOp.Phi, phi)
            delete phi
            let bt = type_bool(bm)
            let cnd = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ULessThan, bt, cnd, iv, len)
            emit(bm, SEC_FUNCS, SpvOp.LoopMerge, merge, cont, 0u)
            emit(bm, SEC_FUNCS, SpvOp.BranchConditional, cnd, body, merge)
            emit(bm, SEC_FUNCS, SpvOp.Label, body)
            let ept = type_pointer(bm, SpvStorageClass.Function, comp_t)
            let ep = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.AccessChain, ept, ep, dst.id, iv)
            let ev = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, comp_t, ev, ep)
            let cl = alloc_id(bm)
            var cops2 <- [comp_t, cl, glsl, uint(GLSLstd450.FClamp), ev, lo, hi]
            emit_n(bm, SEC_FUNCS, SpvOp.ExtInst, cops2)
            delete cops2
            emit(bm, SEC_FUNCS, SpvOp.Store, ep, cl)
            emit(bm, SEC_FUNCS, SpvOp.Branch, cont)
            emit(bm, SEC_FUNCS, SpvOp.Label, cont)
            emit(bm, SEC_FUNCS, SpvOp.IAdd, ut, inext, iv, const_uint(bm, 1u))
            emit(bm, SEC_FUNCS, SpvOp.Branch, hdr)
            emit(bm, SEC_FUNCS, SpvOp.Label, merge)
            return expr
        }
        // barrier(): control + shared-memory barrier (OpControlBarrier at Workgroup execution+memory
        // scope, WorkgroupMemory|AcquireRelease semantics). memoryBarrierShared(): the memory-only
        // variant (OpMemoryBarrier, same scope/semantics). memoryBarrierBuffer(): the SSBO variant
        // (OpMemoryBarrier at Device scope, UniformMemory|AcquireRelease — glslang's lowering) for
        // cross-invocation buffer handoffs, where barrier()'s WorkgroupMemory semantics do not
        // reach; pair as `memoryBarrierBuffer(); barrier();`. All compute-only; not terminators.
        if (name == "barrier" || name == "memoryBarrierShared" || name == "memoryBarrierBuffer") {
            if (ctx.stage != ShaderStage.Compute) {
                errs |> push("{name}() is only valid in a compute shader")
                return expr
            }
            if (!empty(expr.arguments)) {
                errs |> push("{name}() takes no arguments")
                return expr
            }
            if (name == "memoryBarrierBuffer") {
                let dscope = const_uint(bm, uint(SpvScope.Device))
                let bsem = const_uint(bm, uint(SpvMemorySemantics.UniformMemory) | uint(SpvMemorySemantics.AcquireRelease))
                emit(bm, SEC_FUNCS, SpvOp.MemoryBarrier, dscope, bsem)
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
            if (acls != 0 && acls != 1 && !(acls == 2 && name == "atomicAdd")) {
                errs |> push("{name}: atomic target must be int or uint (float is atomicAdd-only)")
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
            if (acls == 2) {
                ensure_atomic_float_add(bm, *ctx)
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
        // subgroup ops: cross-invocation reduction / broadcast at Subgroup scope (OpGroupNonUniform*).
        // The value-taking stubs are GENERIC, so the resolved call carries the mangled instance name --
        // match on the ORIGIN generic's name, exactly as traceRayEXT above does. A vote takes a bool
        // predicate; elect takes nothing; every other form takes a 32-bit scalar or a vector of those.
        // No stage gate: unlike barrier(), a subgroup op carries no execution-model restriction.
        let sgcls = empty(expr.arguments) ? -1 : scalar_class(expr.arguments[0]._type.baseType)
        let sg = subgroup_info(origin_name, sgcls)
        if (sg.ok) {
            let nargs = (sg.form == SubgroupForm.Elect ? 0
                : ((sg.form == SubgroupForm.ValueId || sg.form == SubgroupForm.Clustered) ? 2 : 1))
            if (length(expr.arguments) != nargs) {
                errs |> push("{origin_name} expects {nargs} argument(s)")
                return expr
            }
            if (sg.form == SubgroupForm.Vote) {
                if (expr.arguments[0]._type.baseType != Type.tBool) {
                    errs |> push("{origin_name}: the predicate must be a bool")
                    return expr
                }
            } elif (sg.form != SubgroupForm.Elect && sgcls < 0) {
                errs |> push("{origin_name}: operand must be a 32-bit int / uint / float, or a vector of those")
                return expr
            }
            var v = 0u
            if (sg.form != SubgroupForm.Elect) {
                v = value_of(expr.arguments[0])
                if (v == 0u) return expr
            }
            ensure_subgroup_feature(bm, *ctx, sg.cap)
            let scope = const_uint(bm, uint(SpvScope.Subgroup))
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- [rt, res, scope]
            if (sg.form == SubgroupForm.Reduce || sg.form == SubgroupForm.Clustered) {
                ops |> push(uint(sg.form == SubgroupForm.Reduce ? SpvGroupOperation.Reduce : SpvGroupOperation.ClusteredReduce))
            }
            if (sg.form != SubgroupForm.Elect) {
                ops |> push(v)
            }
            if (sg.form == SubgroupForm.ValueId) {
                let iid = value_of(expr.arguments[1])
                if (iid == 0u) {
                    delete ops
                    return expr
                }
                // Before SPIR-V 1.5, OpGroupNonUniformBroadcast's Id had to be a CONSTANT instruction;
                // 1.5 relaxed it to merely dynamically uniform. A dynamic lane index (a loop counter,
                // gl_SubgroupInvocationID) is the natural spelling and what glslang emits, so raise the
                // module version instead of rejecting it. OpGroupNonUniformShuffle never carried the
                // restriction, and a constant Id stays valid at the default version.
                if (sg.op == SpvOp.GroupNonUniformBroadcast && !const_uint_value(expr.arguments[1]).ok &&
                    bm.version < SPV_VERSION_1_5) {
                    bm.version = SPV_VERSION_1_5
                }
                ops |> push(iid)
            }
            if (sg.form == SubgroupForm.Clustered) {
                // SPIR-V requires ClusterSize to be a constant instruction, a power of two, at least 1.
                // Demand it here rather than let an invalid blob reach the driver.
                let cs = const_uint_value(expr.arguments[1])
                if (!cs.ok || cs.value == 0u || (cs.value & (cs.value - 1u)) != 0u) {
                    errs |> push("{origin_name}: cluster_size must be a compile-time constant power of two")
                    delete ops
                    return expr
                }
                ops |> push(const_uint(bm, cs.value))
            }
            emit_n(bm, SEC_FUNCS, sg.op, ops)
            delete ops
            e2id[k] = res
            return expr
        }
        // unpack8 / pack32: reinterpret one 32-bit scalar as four 8-bit lanes (and back) — a single
        // OpBitcast; the byte4/ubyte4 type factory pulls the Int8 capability. Widening the unpacked
        // lanes (`int4(b4)`) rides the existing OpSConvert path, giving sign extension for free.
        if (name == "unpack8" || name == "pack32") {
            if (length(expr.arguments) != 1) {
                errs |> push("{name} expects 1 argument")
                return expr
            }
            let v = value_of(expr.arguments[0])
            if (v == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Bitcast, rt, res, v)
            e2id[k] = res
            return expr
        }
        // sdot4/usdot4: packed 4x8-bit integer dot product (dp4a) — OpSDot/OpSUDot on two uint
        // words with the PackedVectorFormat4x8Bit LITERAL as the trailing operand (not an id).
        // Pulls the DotProduct + DotProductInput4x8BitPacked caps + SPV_KHR_integer_dot_product.
        // usdot4(a unsigned, b signed) swaps into OpSUDot's (V1 signed, V2 unsigned) operand order.
        if (name == "sdot4" || name == "usdot4") {
            if (length(expr.arguments) != 2) {
                errs |> push("{name} expects 2 arguments")
                return expr
            }
            let wa = value_of(expr.arguments[0])
            let wb = value_of(expr.arguments[1])
            if (wa == 0u || wb == 0u) return expr
            ensure_integer_dot_product(bm, *ctx)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var ops <- (name == "usdot4"
                ? [rt, res, wb, wa, uint(SpvPackedVectorFormat.PackedVectorFormat4x8Bit)]
                : [rt, res, wa, wb, uint(SpvPackedVectorFormat.PackedVectorFormat4x8Bit)])
            emit_n(bm, SEC_FUNCS, name == "usdot4" ? SpvOp.SUDot : SpvOp.SDot, ops)
            delete ops
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
        // texelFetch(textureBuffer, index): the two-argument form. The global IS the image (no sampler
        // to extract from), and a buffer image carries no mips -- SPIR-V forbids a Lod operand on
        // Dim=Buffer -- so it fetches with the bare index and no image operands.
        if (name == "texelFetch" && length(expr.arguments) == 2) {
            let tb = global_var_of(expr.arguments[0])
            if (tb == null) {
                errs |> push("texelFetch: the two-argument form expects a textureBuffer global")
                return expr
            }
            let gi = ctx.globals?[intptr(tb)] ?? GlobalInfo()
            if (!gi.is_texel_buffer) {
                errs |> push("texelFetch: '{tb.name}' is not a textureBuffer")
                return expr
            }
            let index = value_of(expr.arguments[1])
            if (index == 0u) return expr
            let img = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Load, gi.value_type, img, gi.var_id)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageFetch, rt, res, img, index)
            e2id[k] = res
            return expr
        }
        if (name == "texelFetch") {
            if (length(expr.arguments) != 3) {
                errs |> push("texelFetch expects (sampler, coord, lod) or (textureBuffer, index)")
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
            let ri = resolve_image_arg(expr.arguments[0], "imageStore")
            if (!ri.ok) return expr
            if (ri.gi.is_read_only) {
                errs |> push("imageStore: target is @readonly (NonWritable)")
                return expr
            }
            let coord = value_of(expr.arguments[1])
            let texel = value_of(expr.arguments[2])
            if (coord == 0u || texel == 0u) return expr
            emit(bm, SEC_FUNCS, SpvOp.ImageWrite, ri.img, coord, texel)
            return expr
        }
        // imageLoad(image, coord) : float4: OpLoad the image + OpImageRead
        if (name == "imageLoad") {
            if (length(expr.arguments) != 2) {
                errs |> push("imageLoad expects (image, coord)")
                return expr
            }
            let ri = resolve_image_arg(expr.arguments[0], "imageLoad")
            if (!ri.ok) return expr
            // GLSL forbids reading a writeonly image. imageSize is a QUERY, not a read, and stays legal.
            if (ri.gi.is_write_only) {
                errs |> push("imageLoad: target is @writeonly (NonReadable)")
                return expr
            }
            let coord = value_of(expr.arguments[1])
            if (coord == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageRead, rt, res, ri.img, coord)
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
            // imageSize is a QUERY, legal on a @writeonly image (unlike imageLoad).
            let ri = resolve_image_arg(expr.arguments[0], "imageSize")
            if (!ri.ok) return expr
            ensure_image_query(bm, *ctx)
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.ImageQuerySize, rt, res, ri.img)
            e2id[k] = res
            return expr
        }
        // scalar numeric conversion: float(int/uint), float16(float), int8(float), ... Same class
        // AND width is a no-op passthrough; anything else emits OpConvert*/OpF/S/UConvert/OpBitcast
        // per convert_op. Scalar only (lane-matched vector converts ride the ctor path below).
        let dst = scalar_type_name_class(name)
        if (dst.cls >= 0 && length(expr.arguments) == 1) {
            let a0 = expr.arguments[0]
            let sn = numeric_info(value_base_type(a0._type))
            if (!sn.ok || sn.lanes != 1) {
                errs |> push("{name}: cannot convert from {a0._type.baseType} (numeric scalar only)")
                return expr
            }
            let aid = value_of(a0)
            if (aid == 0u) return expr
            let src_cls = sn.cls
            let src_w = sn.width
            if (src_cls == dst.cls && src_w == dst.width) {  // same class+width no-op, e.g. float(someFloat)
                e2id[k] = aid
                return expr
            }
            let cc = convert_op(src_cls, src_w, dst.cls, dst.width)
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
                let src_cls = scalar_class(a0._type.baseType)
                let src_w = scalar_width(a0._type.baseType)
                if (src_cls != vc.cls || src_w != vc.width) {
                    // lane-matched component conversion: float2(uint2) -> OpConvertUToF,
                    // half4(float4) -> OpFConvert, short4(int4) -> OpSConvert, ...
                    // (OpConvert*/OpF/S/UConvert/OpBitcast are all vector-valid)
                    if (cc == vc.n && src_cls >= 0) {
                        let cv = convert_op(src_cls, src_w, vc.cls, vc.width)
                        if (cv.ok) {
                            let aid = value_of(a0)
                            if (aid == 0u) return expr
                            let res = alloc_id(bm)
                            emit(bm, SEC_FUNCS, cv.code, rt, res, aid)
                            e2id[k] = res
                            return expr
                        }
                    }
                    // splat-with-convert: half4(floatScalar) same-class narrows, float3(intScalar)
                    // crosses class (ConvertSToF) — convert the scalar once (a single rounding, matching
                    // the CPU ctor), then splat. convert_op covers both, so any numeric scalar splats.
                    if (cc == 1 && src_cls >= 0) {
                        let cv = convert_op(src_cls, src_w, vc.cls, vc.width)
                        if (cv.ok) {
                            let aid = value_of(a0)
                            if (aid == 0u) return expr
                            let et = emit_component_type(bm, expr._type)
                            let conv = alloc_id(bm)
                            emit(bm, SEC_FUNCS, cv.code, et, conv, aid)
                            let res = alloc_id(bm)
                            var cons <- [rt, res]
                            cons |> reserve(2 + vc.n)
                            for (_l in range(vc.n)) {
                                cons |> push(conv)
                            }
                            emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
                            delete cons
                            e2id[k] = res
                            return expr
                        }
                    }
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
                let a_cls = scalar_class(a._type.baseType)
                let a_w = scalar_width(a._type.baseType)
                let needs_conv = a_cls != vc.cls || a_w != vc.width
                // a SCALAR constituent of a different component kind/width converts to the vector's
                // component type (float2(intX, y) -> ConvertSToF the int lane); a vector constituent
                // must already match, since a per-lane vector cross-component convert is not lowered here
                if (a_cls < 0 || (needs_conv && cc != 1)) {
                    errs |> push("{name}: constituent of type {a._type.baseType} has a different component kind (conversion not supported yet)")
                    delete cons
                    return expr
                }
                total += cc
                var aid = value_of(a)
                if (aid == 0u) {
                    delete cons
                    return expr
                }
                if (needs_conv) {
                    // scalar constituent of a different class/width: convert once (the CPU per-lane
                    // registrations take int32/float args), int -> float via ConvertSToF, etc.
                    let cv = convert_op(a_cls, a_w, vc.cls, vc.width)
                    if (!cv.ok) {
                        errs |> push("{name}: constituent of type {a._type.baseType} cannot convert to the component type")
                        delete cons
                        return expr
                    }
                    let et = emit_component_type(bm, expr._type)
                    let conv = alloc_id(bm)
                    emit(bm, SEC_FUNCS, cv.code, et, conv, aid)
                    aid = conv
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
        // matrix column constructor: matCxR(col0, col1, ...) -> OpCompositeConstruct of `cols`
        // column vectors (each a floatW). Gated on the constructor NAME (not the result type), so a
        // matrix-RETURNING user function is left to the OpFunctionCall path below. Only the column
        // form lowers here; the no-arg identity / float3x4-widen forms fall through (unsupported).
        let mc = matrix_ctor(name)
        if (mc.ok && length(expr.arguments) == mc.cols) {
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            var cons <- [rt, res]
            cons |> reserve(2 + mc.cols)
            var ok_cols = true
            for (a in expr.arguments) {
                if (scalar_class(a._type.baseType) != 2 || component_count(a._type.baseType) != mc.width) {
                    errs |> push("{name}: each column must be a float{mc.width} vector (got {a._type.baseType})")
                    ok_cols = false
                    break
                }
                let aid = value_of(a)
                if (aid == 0u) {
                    ok_cols = false
                    break
                }
                cons |> push(aid)
            }
            if (ok_cols) {
                emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
                e2id[k] = res
            }
            delete cons
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
        // mod: core OpFMod (GLSL mod semantics — sign-of-DIVISOR), the twin of
        // fmod above. The two differ on a negative dividend, so a port of GLSL's
        // mod() must reach this one and never fmod.
        if (name == "mod") {
            if (length(expr.arguments) != 2) {
                errs |> push("mod expects 2 arguments")
                return expr
            }
            let a = value_of(expr.arguments[0])
            let b = value_of(expr.arguments[1])
            if (a == 0u || b == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.FMod, rt, res, a, b)
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
        // intBitsToFloat / uintBitsToFloat / floatBitsToInt / floatBitsToUint (daslib/math_bits'
        // int_bits_to_float / uint_bits_to_float / float_bits_to_int / float_bits_to_uint): a bit-pattern
        // reinterpret -> one core OpBitcast, with the result type taken from the overload's return. math_bits
        // is a builtin module (is_user_shader_func == false), so its CPU-reinterpret body is never emitted;
        // without this interception the call would fail-close as an uncallable host function below.
        if (name == "int_bits_to_float" || name == "uint_bits_to_float" ||
            name == "float_bits_to_int" || name == "float_bits_to_uint") {
            if (length(expr.arguments) != 1) {
                errs |> push("{name} expects 1 argument")
                return expr
            }
            let a = value_of(expr.arguments[0])
            if (a == 0u) return expr
            let rt = emit_type(bm, expr._type)
            let res = alloc_id(bm)
            emit(bm, SEC_FUNCS, SpvOp.Bitcast, rt, res, a)
            e2id[k] = res
            return expr
        }
        // GLSL.std.450 math: OpExtInst. arith_class, NOT scalar_class: an ext-op operand carries no
        // interface-block byte layout, so it may be narrower than 32 bits -- which is what lets
        // max(int16, int16) emit OpExtInst SMax on %short, the shape glslang gives GLSL's 16-bit max,
        // rather than failing closed. Same width-aware pair the arithmetic and compare rails use.
        let cls = empty(expr.arguments) ? -1 : arith_class(expr.arguments[0]._type.baseType)
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
            let result_n = arith_lanes(expr._type.baseType)
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

    // ----- fixed-array literal -> OpConstantComposite / OpCompositeConstruct (whole-array) -----
    // All-constant elements fold to a deduped OpConstantComposite; emit_const recurses into them
    // internally, so their own default visit emits nothing (at patch they are ExprConstFloatN leaves,
    // not sub-call nodes). Elements that are RUNTIME values instead build the array with one
    // OpCompositeConstruct of their already-emitted ids — the same shape glslang gives
    // `int[5](a, b, c, d, e)`. Either way a fixed-array `let` / `var` then Stores the id into its
    // Function-storage OpVariable.
    def override visitExprMakeArray(var expr : ExprMakeArray?) : ExpressionPtr {
        var bm & = unsafe(*m)
        // a dynamic array<T> literal is ALSO an ExprMakeArray; emit_type would panic on tArray
        if (expr._type == null || expr._type.baseType != Type.tFixedArray) {
            errs |> push("only fixed-array literals are supported (dynamic array<T> literals are not)")
            return expr
        }
        var probe : array<string>
        let r = emit_const(bm, expr, probe)
        delete probe
        if (r.ok) {
            e2id[intptr(expr)] = r.id
            return expr
        }
        var cons <- [emit_type(bm, expr._type), 0u]
        cons |> reserve(2 + length(expr.values))
        for (v in expr.values) {
            let vid = value_of(v)
            if (vid == 0u) {
                errs |> push("array literal element is neither a compile-time constant nor an emittable value")
                delete cons
                return expr
            }
            cons |> push(vid)
        }
        let res = alloc_id(bm)
        cons[1] = res
        emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, cons)
        delete cons
        e2id[intptr(expr)] = res
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
        if (key_exists(ite_ids, intptr(expr))) {
            ite_ids[intptr(expr)].then_term = ctx.terminated
        }
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.else_l)
        ctx.terminated = false
    }

    def override visitExprIfThenElse(var expr : ExprIfThenElse?) : ExpressionPtr {
        let ids = ite_ids?[intptr(expr)] ?? IteIds()
        var bm & = unsafe(*m)
        let last_term = ctx.terminated      // else-branch termination (or then's, when there is no else)
        if (!ctx.terminated) emit(bm, SEC_FUNCS, SpvOp.Branch, ids.merge)
        emit(bm, SEC_FUNCS, SpvOp.Label, ids.merge)
        if (expr.if_false != null && ids.then_term && last_term) {
            // both branches terminate (the optimizer folds trailing code into `else`, so a body can
            // END on such an if) — the merge block is unreachable: close it and stay terminated so
            // the function-end check doesn't misread this as a missing return
            emit(bm, SEC_FUNCS, SpvOp.Unreachable)
            ctx.terminated = true
        } else {
            ctx.terminated = false
        }
        return expr
    }

    // Parameterised hints take a NON-NEGATIVE int literal. A wrong type must not fall through to
    // "unknown hint" (the hint is known, the value is not), and a negative would wrap to a huge
    // uint and emit invalid SPIR-V that only fails later, at pipeline creation.
    def loop_hint_literal(a : AnnotationArgument) : uint {
        if (a.basicType != Type.tInt) {
            errs |> push("vulkan loop hint '{a.name}' takes an int value")
            return 0u
        }
        if (a.iValue < 0) {
            errs |> push("vulkan loop hint '{a.name}' must be >= 0, got {a.iValue}")
            return 0u
        }
        return uint(a.iValue)
    }

    // Vulkan-native loop hints: argument names map 1:1 onto SPIR-V LoopControl bits, so nothing is
    // translated from another backend's vocabulary and no hint can alias onto an assertion.
    def loop_control_of(args : AnnotationArgumentList; var lits : array<uint>) : uint {
        var mask = 0u
        var dep_len = 0u
        var min_it = 0u
        var max_it = 0u
        var it_mul = 0u
        var peel = 0u
        var partial = 0u
        for (a in args) {
            if (a.name == "unroll") {
                mask |= uint(SpvLoopControl.Unroll)
            } elif (a.name == "dont_unroll") {
                mask |= uint(SpvLoopControl.DontUnroll)
            } elif (a.name == "dependency_infinite") {
                mask |= uint(SpvLoopControl.DependencyInfinite)
            } elif (a.name == "dependency_length") {
                mask |= uint(SpvLoopControl.DependencyLength)
                dep_len = loop_hint_literal(a)
            } elif (a.name == "min_iterations") {
                mask |= uint(SpvLoopControl.MinIterations)
                min_it = loop_hint_literal(a)
            } elif (a.name == "max_iterations") {
                mask |= uint(SpvLoopControl.MaxIterations)
                max_it = loop_hint_literal(a)
            } elif (a.name == "iteration_multiple") {
                mask |= uint(SpvLoopControl.IterationMultiple)
                it_mul = loop_hint_literal(a)
            } elif (a.name == "peel_count") {
                mask |= uint(SpvLoopControl.PeelCount)
                peel = loop_hint_literal(a)
            } elif (a.name == "partial_count") {
                mask |= uint(SpvLoopControl.PartialCount)
                partial = loop_hint_literal(a)
            } else {
                errs |> push("unknown vulkan loop hint '{a.name}'")
            }
        }
        if ((mask & uint(SpvLoopControl.Unroll)) != 0u && (mask & uint(SpvLoopControl.DontUnroll)) != 0u) {
            errs |> push("loop hints 'unroll' and 'dont_unroll' are mutually exclusive")
        }
        // parameterised bits append their literal to OpLoopMerge in ASCENDING BIT ORDER (SPIR-V spec)
        if ((mask & uint(SpvLoopControl.DependencyLength)) != 0u) {
            lits |> push(dep_len)
        }
        if ((mask & uint(SpvLoopControl.MinIterations)) != 0u) {
            lits |> push(min_it)
        }
        if ((mask & uint(SpvLoopControl.MaxIterations)) != 0u) {
            lits |> push(max_it)
        }
        if ((mask & uint(SpvLoopControl.IterationMultiple)) != 0u) {
            lits |> push(it_mul)
        }
        if ((mask & uint(SpvLoopControl.PeelCount)) != 0u) {
            lits |> push(peel)
        }
        if ((mask & uint(SpvLoopControl.PartialCount)) != 0u) {
            lits |> push(partial)
        }
        return mask
    }

    def emit_loop_merge(var bm : SpirvModule; ids : LoopIds; args : AnnotationArgumentList) {
        var lits : array<uint>
        let mask = loop_control_of(args, lits)
        if (empty(lits)) {
            emit(bm, SEC_FUNCS, SpvOp.LoopMerge, ids.merge, ids.cont_l, mask)
        } else {
            var ops <- [ids.merge, ids.cont_l, mask]
            ops |> push_from(lits)
            emit_n(bm, SEC_FUNCS, SpvOp.LoopMerge, ops)
            delete ops
        }
        delete lits
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
        emit_loop_merge(bm, ids, expr.annotations)
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
        emit_loop_merge(bm, ids, expr.annotations)
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
        if (suppress_fn_addr) {
            suppress_fn_addr = false
            return
        }
        errs |> push("function address (@@) is not supported in SPIR-V shaders")
    }
    def override preVisitExprConstPtr(expr : ExprConstPtr?) : void {
        errs |> push("pointer constant is not supported in SPIR-V shaders")
    }
    // __context__ / __lineinfo__ ARE lowerable as a `.[]` bounds-check tail: elided when
    // preVisitExprCallArgument flagged them, but fail closed anywhere else.
    def override preVisitExprFakeContext(expr : ExprFakeContext?) : void {
        if (suppress_bounds_arg) {
            suppress_bounds_arg = false
            return
        }
        errs |> push("__context__ is not supported in SPIR-V shaders")
    }
    def override preVisitExprFakeLineInfo(expr : ExprFakeLineInfo?) : void {
        if (suppress_bounds_arg) {
            suppress_bounds_arg = false
            return
        }
        errs |> push("__lineinfo__ is not supported in SPIR-V shaders")
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
        // `with (subject) { body }` is a transparent scope: infer resolves the body's bare field names
        // to subject.field, and the optimizer's BlockFolding normally inlines the body and drops the
        // subject before this patch runs. A no-optimization compile (e.g. lint) keeps `with` alive to
        // here, so emit the body as-is. The subject expression is a struct lvalue reference; visiting it
        // emits no stray value op for a supported (block) global, and a local struct fails separately on
        // its own "struct local not supported" limitation -- so there is nothing to do here. (The GL rail
        // mirrors this; see glsl_internal preVisitExprWith.)
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
    // Struct construction (`S(a = x, b = y, ...)`) -> one OpCompositeConstruct of the
    // field values, in the struct's DECLARED order -- which is what the OpTypeStruct members are in, and
    // is NOT the order the named arguments are written in. Every field must be given: SPIR-V has no
    // partial composite, and daslang only omits a field when its declaration carries a default, which
    // would have to be materialised here; fail closed instead of constructing a short (invalid) composite.
    def override canVisitExprMakeStructBody(expr : ExprMakeStruct?) : bool {
        return true    // descend: the field initialisers are ordinary expressions and must lower first
    }
    def override visitExprMakeStruct(var expr : ExprMakeStruct?) : ExpressionPtr {
        var bm & = unsafe(*m)
        let st = expr.makeType != null ? expr.makeType.structType : null
        if (st == null) {
            errs |> push("struct construction: not a struct type")
            return expr
        }
        if (length(expr.structs) != 1) {
            errs |> push("struct construction '{st.name}': only a single struct value is supported (an array-of-struct initializer is not)")
            return expr
        }
        let sid = value_struct_type(bm, st, errs)
        if (sid == 0u) return expr
        let nf = length(st.fields)
        var vals : array<uint>
        vals |> resize(nf)
        for (fi in range(nf)) {
            vals[fi] = 0u
        }
        for (fld in *expr.structs[0]) {
            let fi = field_index_by_name(st, "{fld.name}")
            if (fi < 0) {
                errs |> push("struct construction '{st.name}': no such field '{fld.name}'")
                delete vals
                return expr
            }
            vals[fi] = value_of(fld.value)
        }
        for (fi in range(nf)) {
            if (vals[fi] == 0u) {
                errs |> push("struct construction '{st.name}': field '{st.fields[fi].name}' has no value (every field must be given)")
                delete vals
                return expr
            }
        }
        let res = alloc_id(bm)
        var ops <- [sid, res]
        ops |> push_from(vals)
        emit_n(bm, SEC_FUNCS, SpvOp.CompositeConstruct, ops)
        delete ops
        delete vals
        e2id[intptr(expr)] = res
        return expr
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
    // spirv_builtins -> texture/imageStore/traceRayEXT/...), so they must never be emitted as
    // OpFunctions. A GENERIC intrinsic stub (traceRayEXT) instantiates into the caller's module, so
    // classify by the ORIGIN generic's module, not the instance's.
    var forigin = f
    while (forigin.fromGeneric != null) {
        forigin = forigin.fromGeneric
    }
    // a [spirv_decode] function is emitted by the cm2 DECODE rail (PSB block pointer + uint[2]
    // coordinate params), never as a plain user function — the das-typed signature would register wrong
    if (function_has_annotation(forigin, "spirv_decode")) return false
    let mn = "{forigin._module.name}"
    return !(mn == "builtin" || mn == "math" || mn == "math_bits" || mn == "spirv_builtins" || mn == "shader_lingua_franca")
}

// True iff `f` (or its generic origin) carries the named function annotation (e.g. [spirv_decode]).
def private function_has_annotation(f : Function const?; nm : string) : bool {
    if (f == null) return false
    var fo = f
    while (fo.fromGeneric != null) {
        fo = fo.fromGeneric
    }
    for (ad in fo.annotations) {
        if (ad != null && ad.annotation != null && "{ad.annotation.name}" == nm) return true
    }
    return false
}

// Is this parameter WRITTEN THROUGH by the callee -- a Function pointer parameter fed by the
// copy-in/copy-out temp at the call site, rather than a by-value OpFunctionParameter?
//
// The two shapes of the test are not interchangeable. das passes a struct BY REFERENCE already, so a
// struct parameter never carries flags.ref -- `hp : HP` and `var np : NP&` differ ONLY in the const
// flag, and testing ref on one silently misreads every mutable struct param as read-only. A scalar's
// `var x : float` is a mutable LOCAL COPY (no propagation), so only the `&` form (ref AND non-const)
// is an out-param there. Hence: const alone for a struct, ref + const for everything else.
def private is_out_param(at : TypeDecl?) : bool {
    if (at == null) return false
    if (at.baseType == Type.tStructure) return !at.flags.constant
    return at.flags.ref && !at.flags.constant
}

// Validate a user function's signature, allocate its OpFunction id + OpTypeFunction, and record it.
// Only 16/32-bit scalar/vector/matrix results+params are supported (by value, or by reference for
// params); anything else is rejected here with a clean error so a later call site doesn't emit
// invalid SPIR-V.
def private register_user_func(var m : SpirvModule; var ctx : EmitCtx; f : Function const?; var errors : array<string>) {
    if (key_exists(ctx.user_funcs, intptr(f))) return
    var uf = UserFunc(fn = f, valid = true, is_void = false, fn_id = 0u, type_id = 0u, ret_type = 0u)
    if (f.result.baseType == Type.tVoid) {
        uf.is_void = true
        uf.ret_type = type_void(m)
    } elif (f.result.baseType == Type.tStructure) {
        // A struct RESULT is returned by value, exactly as glslang returns one (OpFunction %S).
        uf.ret_type = value_struct_type(m, f.result.structType, errors)
        if (uf.ret_type == 0u) {
            uf.valid = false
        }
    } elif (is_emittable_value_type(f.result) && f.result.baseType != Type.tFixedArray) {
        uf.ret_type = emit_type(m, f.result)
    } else {
        errors |> push("shader function '{f.name}': return type {f.result.baseType} is not supported (only void, a plain struct, and 16/32-bit scalar/vector/matrix results)")
        uf.valid = false
    }
    var ptypes : array<uint>
    for (arg in f.arguments) {
        let at = arg._type
        // A struct parameter is exempt from the value-type guard below: its fields are what must be
        // emittable, and emit_value_type reports any that is not. is_out_param draws the read-only /
        // written-through line for every parameter kind alike.
        let is_struct = at != null && at.baseType == Type.tStructure
        if (!is_struct && (!is_emittable_value_type(at) || at.baseType == Type.tFixedArray)) {
            errors |> push("shader function '{f.name}': parameter '{arg.name}' of type {at.baseType} is not supported (only a plain struct and 16/32-bit scalar/vector/matrix parameters, by value or by reference)")
            uf.valid = false
            continue
        }
        let vt = emit_value_type(m, at, errors)
        if (vt == 0u) {
            uf.valid = false
            continue
        }
        if (is_out_param(at)) {
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

// Validate + register a cm2 decode function (first reference wins; later loads reuse the id). The
// SPV_NV_cooperative_matrix2 DecodeFunc contract is rigid: (block : <struct>; blockCoord,
// coordInBlock : uint2) returning the tile component type. The block param lowers to a
// PhysicalStorageBuffer pointer at the std430 packing (build_block_struct, Block-decorated — a gguf
// q8_0 {d : float16; qs : uint16[16]} lands at offsets 0/2, 34 bytes); uint2 coords lower as uint[2]
// ARRAYS (the spec's shape — .x/.y reads become OpCompositeExtract, valid on arrays).
def private register_decode_func(var m : SpirvModule; var ctx : EmitCtx; f : Function const?; var errors : array<string>) : uint {
    if (key_exists(ctx.decode_funcs, intptr(f))) {
        let ex = ctx.decode_funcs?[intptr(f)] ?? DecodeFn()
        return ex.valid ? ex.fn_id : 0u
    }
    var df = DecodeFn(fn = f, valid = true)
    ensure_cooperative_matrix2(m)
    let rbt = f.result.baseType
    if (rbt == Type.tFloat16) {
        df.ret_type = type_float(m, 16u)
    } elif (rbt == Type.tFloat) {
        df.ret_type = type_float(m, 32u)
    } elif (rbt == Type.tInt8) {
        df.ret_type = type_int(m, 8u, 1u)
    } else {
        errors |> push("decode function '{f.name}': result must be float16, float, or int8 (the tile's component type)")
        df.valid = false
    }
    if (length(f.arguments) != 3) {
        errors |> push("decode function '{f.name}': signature must be (block : <struct>; blockCoord, coordInBlock : uint2)")
        df.valid = false
    } else {
        let a0 = f.arguments[0]._type
        if (a0 == null || a0.baseType != Type.tStructure || a0.structType == null) {
            errors |> push("decode function '{f.name}': the first parameter must be the block struct")
            df.valid = false
        } else {
            let blk = build_block_struct(m, "{f.name} block", a0.structType, true, errors, BlockLayoutRules.std430)
            if (!blk.ok) {
                df.valid = false
            } else {
                df.block_type = blk.type_id
                df.block_ptr_type = type_pointer(m, SpvStorageClass.PhysicalStorageBuffer, blk.type_id)
            }
        }
        for (i in range(1, 3)) {
            let at = f.arguments[i]._type
            if (at == null || at.baseType != Type.tUInt2) {
                errors |> push("decode function '{f.name}': parameter {i + 1} must be uint2 (the tensor coordinates)")
                df.valid = false
            }
        }
    }
    if (df.valid) {
        df.fn_id = alloc_id(m)
        let arr2 = type_array(m, type_uint(m), 2)
        var ps <- [df.block_ptr_type, arr2, arr2]
        df.type_id = type_function(m, df.ret_type, ps)
        delete ps
    }
    ctx.decode_funcs |> insert(intptr(f), df)
    ctx.decode_order |> push(intptr(f))
    return df.valid ? df.fn_id : 0u
}

// Emit one registered decode function: OpFunction with the PSB block pointer decorated Aliased
// (PhysicalStorageBuffer function parameters require Aliased or Restrict; the same block is decoded
// by many invocations), the two uint[2] coordinate params bound as SSA values, then the das body via
// the normal walk — reads through the block param chain as PhysicalStorageBuffer and load Aligned
// (LocalVar.storage + e2align carry that through ptr_of/value_of).
def private emit_decode_function(var m : SpirvModule; var ctx : EmitCtx; df : DecodeFn; var errors : array<string>) {
    let f = unsafe(reinterpret<FunctionPtr>(df.fn))   // read-only emission; strip pointee const
    emit(m, SEC_FUNCS, SpvOp.Function, df.ret_type, df.fn_id, 0u, df.type_id)
    let p0 = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.FunctionParameter, df.block_ptr_type, p0)
    emit(m, SEC_DECOR, SpvOp.Decorate, p0, uint(SpvDecoration.Aliased))
    ctx.local_vars |> insert(intptr(f.arguments[0]), LocalVar(ptr_id = p0, value_type = df.block_type,
        is_opaque = true, storage = SpvStorageClass.PhysicalStorageBuffer))
    let arr2 = type_array(m, type_uint(m), 2)
    var coord_pids : uint[2]
    for (i in range(1, 3)) {
        let pid = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.FunctionParameter, arr2, pid)
        coord_pids[i - 1] = pid
    }
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    // The das body types the coords as uint2, but the spec-mandated params are uint[2] ARRAYS —
    // rebuild each as a uvec2 SSA value at entry and bind THAT, so any vector op in the body
    // (not just .x/.y extracts) lowers against the type the AST believes in.
    let ut = type_uint(m)
    let uv2 = type_vector(m, ut, 2)
    for (i in range(1, 3)) {
        let e0 = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.CompositeExtract, ut, e0, coord_pids[i - 1], 0u)
        let e1 = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.CompositeExtract, ut, e1, coord_pids[i - 1], 1u)
        let vid = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.CompositeConstruct, uv2, vid, e0, e1)
        ctx.locals |> insert(intptr(f.arguments[i]), vid)
    }
    ctx.terminated = false
    collect_locals(m, ctx, f.body, errors)
    alloc_call_temps(m, ctx, f, errors)
    drive_emit(m, ctx, f, errors)
    if (!ctx.terminated) {
        errors |> push("decode function '{f.name}': not every control-flow path returns a value")
        emit(m, SEC_FUNCS, SpvOp.Unreachable)
    }
    emit(m, SEC_FUNCS, SpvOp.FunctionEnd)
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
    errs : array<string>
    def override preVisitExprCall(expr : ExprCall?) : void {
        let f = expr.func
        if (f == null || !key_exists(ctx.user_funcs, intptr(f))) return
        let uf = ctx.user_funcs?[intptr(f)] ?? UserFunc()
        if (!uf.valid) return
        var bm & = unsafe(*m)
        for (arg, i in expr.arguments, count()) {
            if (i >= length(f.arguments)) break
            let pt = f.arguments[i]._type
            if (is_out_param(pt)) {
                if (key_exists(ctx.call_temps, intptr(arg))) continue
                let vt = emit_value_type(bm, pt, errs)
                let ptr_t = type_pointer(bm, SpvStorageClass.Function, vt)
                let pid = alloc_id(bm)
                emit(bm, SEC_FUNCS, SpvOp.Variable, ptr_t, pid, uint(SpvStorageClass.Function))
                ctx.call_temps |> insert(intptr(arg), CallTemp(ptr_id = pid, value_type = vt))
            }
        }
    }
}

def private alloc_call_temps(var m : SpirvModule; var ctx : EmitCtx; f : FunctionPtr; var errors : array<string>) {
    var v = new SpirvTempAlloc(m = unsafe(addr(m)), ctx = unsafe(addr(ctx)))
    make_visitor(*v) $(mv_adapter) {
        v.adapter = mv_adapter
        visit(f.body, v.adapter)
    }
    errors |> push_from(v.errs)
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
        let vt = emit_value_type(m, at, errors)
        let pid = alloc_id(m)
        if (is_out_param(at)) {
            // written through: the pointer binds as a memory local, so a field of it access-chains
            // and a bare write stores -- exactly like a `var` local declared in the body.
            let pt = type_pointer(m, SpvStorageClass.Function, vt)
            emit(m, SEC_FUNCS, SpvOp.FunctionParameter, pt, pid)
            ctx.local_vars |> insert(intptr(arg), LocalVar(ptr_id = pid, value_type = vt))
        } else {
            // read-only: binds an SSA id, off which a field read is an OpCompositeExtract like any
            // other struct value.
            emit(m, SEC_FUNCS, SpvOp.FunctionParameter, vt, pid)
            ctx.locals |> insert(intptr(arg), pid)
        }
    }
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    ctx.terminated = false
    collect_locals(m, ctx, f.body, errors)
    alloc_call_temps(m, ctx, f, errors)
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
    if (stage == ShaderStage.RayGen) return SpvExecutionModel.RayGenerationKHR
    if (stage == ShaderStage.Miss) return SpvExecutionModel.MissKHR
    if (stage == ShaderStage.ClosestHit) return SpvExecutionModel.ClosestHitKHR
    return SpvExecutionModel.GLCompute
}

[macro_function]
def generate_spirv(fn : FunctionPtr; var errors : array<string>; stage : ShaderStage; local_size : int3; local_size_ids : int3; mesh_config : MeshOutputConfig; early_fragment_tests : bool; var reflection : SpirvReflection) : array<uint> {
    var m = make_module()
    var ctx : EmitCtx
    ctx.stage = stage

    // Fully overwrite the out-param: a caller that reuses a SpirvReflection (or retries after a
    // failed attempt) must not accumulate stale bindings / ranges from a previous call.
    reflection.stage = stage_flag(stage)
    reflection.entry_point = "main"
    // Mesh + Task workgroups are sized like compute via LocalSize, so the reflected local_size
    // mirrors the actual workgroup dispatch dims, not (1,1,1).
    let is_workgroupy = stage == ShaderStage.Compute || stage == ShaderStage.Mesh || stage == ShaderStage.Task
    reflection.local_size = is_workgroupy ? local_size : int3(1, 1, 1)
    reflection.bindings |> clear()
    reflection.push_constants |> clear()

    // RT stages declare RayTracingKHR (below), which implicitly declares Shader; glslang emits NO
    // explicit Shader capability for a raygen / miss / closest-hit module, so match it -- a bare Shader
    // here would diverge from the glslang reference surface.
    if (!is_rt_stage(stage)) {
        emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
    }
    // OpMemoryModel is emitted LATE (just before the entry point), not here: a cooperative-matrix body
    // pulls the VulkanMemoryModel capability, which forces the Vulkan memory model, and that is only
    // known once the body has been walked. Every other shader stays Logical GLSL450.
    // Mesh / task stages pull in the mesh-shading capability + the SPV_EXT_mesh_shader extension.
    // Do this BEFORE the entry point so the capabilities section is closed and the extension is
    // visible by the time spirv-val checks the entry point's execution model. SPV_EXT_mesh_shader
    // requires SPIR-V >= 1.4, so the module header version is bumped from the 1.3 default; the other
    // stages stay at 1.3 (a 1.4 header would force every global into the entry-point interface).
    if (stage == ShaderStage.Mesh || stage == ShaderStage.Task) {
        m.version = SPV_VERSION_1_4
        ensure_mesh_shading(m, ctx)
    }
    // Ray-tracing stages pull in the RayTracingKHR capability + SPV_KHR_ray_tracing, which likewise
    // requires SPIR-V >= 1.4 (the every-referenced-global interface rule below then applies).
    if (is_rt_stage(stage)) {
        m.version = SPV_VERSION_1_4
        ensure_ray_tracing(m, ctx)
    }
    // Mesh per-vertex / per-primitive Output arrays (gl_MeshVerticesEXT, gl_PrimitiveTriangleIndicesEXT)
    // are sized from the [mesh_shader] output config; classify_global reads these off ctx when it
    // emits the array types. Only the mesh stage has these outputs (task dispatches, it doesn't emit
    // geometry).
    ctx.mesh_max_vertices = mesh_config.max_vertices
    ctx.mesh_max_primitives = mesh_config.max_primitives
    ctx.mesh_topology = mesh_config.topology
    // gl_WorkGroupSize folds to this, so it has to be on ctx before the globals are classified.
    ctx.local_size = local_size

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

    // Compute pins the workgroup size; Fragment fixes the framebuffer origin; Vertex needs none.
    // Mesh + Task stages need LocalSize too; Mesh additionally needs OutputVertices,
    // OutputPrimitivesEXT, and an output-topology mode (OutputTrianglesEXT / OutputLinesEXT /
    // OutputPoints) — all three are MANDATORY per the SPV_EXT_mesh_shader spec.
    if (stage == ShaderStage.Compute || stage == ShaderStage.Mesh || stage == ShaderStage.Task) {
        let ls = local_size_dims(local_size)
        var em <- [id_main, uint(SpvExecutionMode.LocalSize), ls.x, ls.y, ls.z]
        emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
        delete em
        // A local_size_*_id annotation arg makes that workgroup axis specializable: an
        // OpSpecConstantComposite uint3 decorated BuiltIn WorkgroupSize, each constituent an
        // OpSpecConstant (SpecId = the axis id, default = the LocalSize value) or the pooled
        // OpConstant default for id-less axes. Per SPIR-V 2.16.2 the WorkgroupSize builtin takes
        // precedence over the LocalSize execution mode (kept above -- glslang parity). WorkgroupSize
        // is deprecated only at SPIR-V >= 1.6 (LocalSizeId), fine at our 1.3 header.
        if (local_size_ids.x >= 0 || local_size_ids.y >= 0 || local_size_ids.z >= 0) {
            let t_u32 = type_int(m, 32u, 0u)
            var axis_ids : array<uint>
            for (aid, adefault in fixed_array(local_size_ids.x, local_size_ids.y, local_size_ids.z), fixed_array(ls.x, ls.y, ls.z)) {
                if (aid >= 0) {
                    if (key_exists(ctx.spec_ids, uint(aid))) {
                        errors |> push("local_size_*_id {aid} duplicates another SpecId in this shader")
                    }
                    ctx.spec_ids |> insert(uint(aid))
                    let sid = alloc_id(m)
                    emit(m, SEC_TYPES, SpvOp.SpecConstant, t_u32, sid, adefault)
                    emit(m, SEC_DECOR, SpvOp.Decorate, sid, uint(SpvDecoration.SpecId), uint(aid))
                    axis_ids |> push(sid)
                } else {
                    axis_ids |> push(const_uint(m, adefault))
                }
            }
            let t_uv3 = type_vector(m, t_u32, 3)
            let wgs_id = alloc_id(m)
            var wo <- [t_uv3, wgs_id]
            wo |> push_from(axis_ids)
            emit_n(m, SEC_TYPES, SpvOp.SpecConstantComposite, wo)
            delete wo
            delete axis_ids
            emit(m, SEC_DECOR, SpvOp.Decorate, wgs_id, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.WorkgroupSize))
        }
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
        // early_fragment_tests: run depth/stencil BEFORE the shader, so an occluded or stencil-killed
        // fragment never executes its body (discard / side effects included). GLSL layout(early_fragment_tests).
        if (early_fragment_tests) {
            emit(m, SEC_EXECMODE, SpvOp.ExecutionMode, id_main, uint(SpvExecutionMode.EarlyFragmentTests))
        }
    }

    emit(m, SEC_FUNCS, SpvOp.Function, t_void, id_main, 0u, t_fn)
    let id_label = alloc_id(m)
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    ctx.terminated = false

    // Function-storage OpVariables must lead the entry block, before any body code.
    collect_locals(m, ctx, fn.body, errors)
    alloc_call_temps(m, ctx, fn, errors)
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
    // cm2 decode functions (registered by the coopmatLoadTensorDecode arm during the body walk)
    for (key in ctx.decode_order) {
        let df = ctx.decode_funcs?[key] ?? DecodeFn()
        if (df.valid) {
            emit_decode_function(m, ctx, df, errors)
        }
    }

    // The EntryPoint lands in its own section, so it is emitted LAST, once every body has been walked.
    // Two things are only final by now: the module version (a body may raise it -- an
    // OpGroupNonUniformBroadcast with a dynamic Id forces SPIR-V 1.5) and, through it, the interface
    // rule below, which the version selects. Emitting it earlier read a version that had not settled.
    //
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

    // The memory model, now that the body is walked: a cooperative-matrix module pulled
    // VulkanMemoryModel (spirv-val requires it once CooperativeMatrixKHR is declared), which mandates the
    // Vulkan memory model; every other shader keeps Logical GLSL450. A cm2 module (tensor addressing /
    // decode functions — the "cap_cm2" pool key, which also declared PhysicalStorageBufferAddresses)
    // additionally switches the ADDRESSING model to PhysicalStorageBuffer64, exactly as glslang does.
    let mem_model = (m.type_pool?["cap_coopmat"] ?? 0u) != 0u ? SpvMemoryModel.Vulkan : SpvMemoryModel.GLSL450
    let addr_model = (m.type_pool?["cap_cm2"] ?? 0u) != 0u ? SpvAddressingModel.PhysicalStorageBuffer64 : SpvAddressingModel.Logical
    emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(addr_model), uint(mem_model))

    var ep <- [uint(execution_model(stage)), id_main]
    var nm <- string_words("main")
    ep |> push_from(nm)
    delete nm
    ep |> push_from(ctx.interface_ids)
    emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
    delete ep

    var words <- module_words(m)
    delete m
    delete ctx
    return <- words
}
