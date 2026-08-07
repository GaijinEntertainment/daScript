options gen2

// VK-specific shader builtins, recognized by name in spirv_emit.das. The modern, backend-invariant
// surface (the compute IDs, gl_Position/gl_PointSize/gl_PointCoord, sampler2D/sampler3D/image2D,
// texture()/textureSize()/imageLoad()/imageStore()/imageSize()/barrier()/memoryBarrierShared()/
// discard()/dFdx()/dFdy()) now lives in shader_lingua_franca, re-exported below so every existing
// `require spirv/spirv_builtins` consumer still sees it unchanged. This file keeps only what is
// Vulkan/SPIR-V-specific. All declarations are AST-only: the body is never executed -- only the
// reference/call AST is read and lowered to SPIR-V (a BuiltIn-decorated variable, or an opcode).

module spirv_builtins shared public

require daslib/shader_lingua_franca public

// ===== VK-specific builtin variables =====
// vertex (the indices are Inputs)
// gl_VertexIndex (BuiltIn VertexIndex) and gl_InstanceIndex (BuiltIn InstanceIndex) live in
// shader_lingua_franca (shared); both rails speak them -- the GLSL emitter renames them to
// gl_VertexID / gl_InstanceID. The SPIR-V emitter still keys them by name. gl_DrawID stays VK-only
// (its GLSL form needs GL_ARB_shader_draw_parameters, outside the WebGL2 / GL-4.1 portable floor).
var gl_DrawID               : int       // BuiltIn DrawIndex (Input) — multi-draw draw call ordinal; needs the DrawParameters capability (in core in SPIR-V 1.3)

// fragment
// gl_FragCoord (BuiltIn FragCoord, Input) and gl_FragDepth (BuiltIn FragDepth, Output) live in
// shader_lingua_franca (shared) -- GLSL spells both identically, so they are part of the modern
// union. The SPIR-V emitter still keys them by name. gl_PrimitiveID stays VK-only (ES 3.2 GLSL,
// outside the WebGL2 / GL-4.1 portable floor).
var gl_PrimitiveID          : int       // BuiltIn PrimitiveId (Input, fragment) — the per-primitive id a mesh shader wrote via gl_MeshPrimitivesEXT[i].gl_PrimitiveID

// gl_BaryCoordEXT (BuiltIn BaryCoordKHR, Input, fragment): the perspective-correct barycentric weights
// of the fragment within its primitive (one per triangle vertex, summing to 1). Reading it pulls in the
// FragmentBarycentricKHR capability + SPV_KHR_fragment_shader_barycentric. Pairs with a `@pervertex` input
// array (PerVertexKHR) to fetch per-vertex attributes and interpolate them by hand (VK_KHR_fragment_shader_barycentric).
var gl_BaryCoordEXT         : float3     // BuiltIn BaryCoordKHR (Input, fragment) — perspective-correct barycentric coordinate

// ===== descriptor indexing =====
// nonuniformEXT marks an index value as varying across invocations within a wavefront/subgroup
// (Vulkan's VK_EXT_descriptor_indexing semantics). Required when indexing a descriptor array
// (sampler[], image[], SSBO[]) with a value the compiler can't prove uniform across the wave —
// without it the driver may serialise the access incorrectly. Lowers to an OpCopyObject (fresh
// result id) + OpDecorate <id> NonUniform on the new id; OpCapability ShaderNonUniform +
// OpExtension "SPV_EXT_descriptor_indexing" land once per module on first use. The descriptor-array
// indexing path itself (sampler2D[] etc.) is a separate slice — this just ships the marker, so the
// test exercises only the decoration + cap + extension. Identity passthrough; the stub body never
// runs, only the call AST is read.
[unused_argument(x), sideeffects]
def public nonuniformEXT(x : int) : int { return x }
[unused_argument(x), sideeffects]
def public nonuniformEXT(x : uint) : uint { return x }

// ===== combined image samplers =====
// sampler2D and the base texture(sampler2D, uv) sample live in shader_lingua_franca (shared). The
// emitter recognizes sampler2D by name in classify_global and lowers it to an OpTypeSampledImage in
// UniformConstant storage; the GL-superset `texture2D` field is ignored on this rail.
// textureLod(sampler2D) and texelFetch(sampler2D) also live in shader_lingua_franca now -- both are
// GLSL name-identical (core + ES 3.0), so they are part of the modern union; the SPIR-V emitter still
// keys them by call name (resolve_sampler_arg) and lowers them to OpImageSampleExplicitLod /
// OpImage+OpImageFetch. (Any textureLod overloads for samplerCube / sampler2DArray would join them
// in shader_lingua_franca, where those marker types now live.)

// ===== additional sampler dimensionalities =====
// samplerCube (cubemap), sampler2DArray (layered 2D) and sampler2DShadow (depth-compare 2D), with
// their texture() / textureSize() / textureCompare() overloads, all live in shader_lingua_franca now:
// each is core GLSL + GLES 3.0 / WebGL2 portable, so they belong to the modern shared surface both
// rails speak. The SPIR-V emitter still recognizes them by name (classify_global / sampler_info ->
// image Dim + Arrayed / Depth flag) and ignores their GL-superset handle field, exactly as for
// sampler2D / sampler3D / image2D.

// ===== separate image + sampler =====
// texture2D is a standalone sampled image (OpTypeImage, no sampler) and sampler is a standalone
// OpTypeSampler -- two independent descriptors with their own @binding. sampleTexture combines them at
// the call site (OpSampledImage) before sampling (OpImageSampleImplicitLod, fragment-only). This is the
// Vulkan separate-image/sampler model, distinct from the combined sampler2D.
struct texture2D {}
struct sampler {}

[unused_argument(tex, smp, uv), sideeffects]
def public sampleTexture(tex : texture2D; smp : sampler; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== uniform texel buffer =====
// textureBuffer is a buffer-backed image (Dim=Buffer, no sampler): the host binds a VkBufferView
// (VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER), so the shader reads format-converted texels by element
// index rather than the raw std430 members an @ssbo exposes. Lowers to a UniformConstant OpTypeImage
// with Dim=Buffer (Sampled=1, format Unknown) + the SampledBuffer capability. SPIR-V-only: the GL rail
// spells the combined form (samplerBuffer), so the marker stays here rather than in the shared surface.
// A buffer image has NO mips, and SPIR-V forbids a Lod operand on one, which is why its texelFetch
// takes only (buffer, index) -- the two-argument form, distinct from the sampler one.
struct textureBuffer {}

[unused_argument(tb, index), sideeffects]
def public texelFetch(tb : textureBuffer; index : int) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== sampled cube array =====
// samplerCubeArray is a LAYERED cubemap: the sampled counterpart of imageCubeArray, and SPIR-V-only
// here for the same reason -- the GLSL emitter declares samplerCube / sampler2DArray but not this one.
// It samples by a float4 (direction.xyz, layer), the direction picking the face and the .w lane the
// cube, and its Arrayed=1 image pulls in the SampledCubeArray capability. The EXPLICIT-lod textureLod
// and the textureSize query ship (both forms the glslang reference carries); texture() stays out as an
// untested lowering. textureSize returns int3 = (face-width, face-height, number-of-cubes).
struct samplerCubeArray { textureCubeArray : uint }

[unused_argument(s, uvl, lod), sideeffects]
def public textureLod(s : samplerCubeArray; uvl : float4; lod : float) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, lod), sideeffects]
def public textureSize(s : samplerCubeArray; lod : int) : int3 {
    return int3(0, 0, 0)
}

// ===== storage images =====
// image2D and image3D, with imageStore/imageLoad/imageSize, live in shader_lingua_franca (shared). The
// emitter recognizes each by name in classify_global and lowers it to a UniformConstant OpTypeImage
// with a known format (default Rgba8, or @format; no StorageImageRead/WriteWithoutFormat capability
// needed; lavapipe-safe). @writeonly lowers to NonReadable and @readonly to NonWritable, both on the
// OpVariable, and the emitter enforces them: no imageLoad from a @writeonly, no imageStore into a
// @readonly. imageSize is a QUERY, not a read, so it stays legal on a @writeonly image.
//
// The ARRAYED storage images are SPIR-V-only for now -- the GLSL emitter can declare image2D / image3D
// but not these -- so they live here rather than in the shared surface. Both are addressed with an
// int3: image2DArray by (x, y, layer), imageCubeArray by (x, y, face + 6 * layer). A cube-arrayed
// storage image pulls in the ImageCubeArray capability.
struct image2DArray   { texture2DArray : uint }
struct imageCubeArray { textureCubeArray : uint }

[unused_argument(img, uvw, data), sideeffects]
def public imageStore(img : image2DArray; uvw : int3; data : float4) : void {}
[unused_argument(img, uvw, data), sideeffects]
def public imageStore(img : imageCubeArray; uvw : int3; data : float4) : void {}

[unused_argument(img, uvw), sideeffects]
def public imageLoad(img : image2DArray; uvw : int3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}
[unused_argument(img, uvw), sideeffects]
def public imageLoad(img : imageCubeArray; uvw : int3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// (width, height, layers); for imageCubeArray the layer count is 6 * cubes, matching GLSL.
[unused_argument(img), sideeffects]
def public imageSize(img : image2DArray) : int3 {
    return int3(0, 0, 0)
}
[unused_argument(img), sideeffects]
def public imageSize(img : imageCubeArray) : int3 {
    return int3(0, 0, 0)
}

// ===== subpass input attachments (Vulkan deferred-rendering rail) =====
// subpassInput is an opaque marker for a Vulkan subpass input attachment: a per-fragment read of a
// previous subpass's color or depth attachment, sampled at the SAME fragment coordinate (no UV).
// Lowers to OpTypeImage with Dim=SubpassData (Sampled=2, format=Unknown) + an InputAttachmentIndex
// decoration. The host binds it as VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT. Requires the
// InputAttachment capability. The shader declares it with `@input_attachment_index = N` alongside
// the usual `@set` / `@binding` slots:
//     var @set = 0 @binding = 0 @input_attachment_index = 0 subpass_color : subpassInput
// subpassLoad samples it at the current fragment (no coord operand on the daslang side; the int2
// (0,0) is emitted internally as the OpImageRead coordinate). Returns float4 even for depth inputs
// -- the depth value lands in .x.
struct subpassInput {}

[unused_argument(i), sideeffects]
def public subpassLoad(i : subpassInput) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== fragment realism =====
// discard(), the dFdx/dFdy partial derivatives, and fwidth (= |dFdx| + |dFdy|) all live in
// shader_lingua_franca (shared) -- GLSL and SPIR-V both speak them, so they are part of the modern
// union. The SPIR-V emitter lowers fwidth to OpFwidth by call name (module-agnostic).

// textureSize(sampler, lod) -> the mip-level dimensions in texels (OpImageQuerySizeLod) for sampler2D
// (-> int2) plus the wider samplerCube / sampler3D / sampler2DArray overloads all live in
// shader_lingua_franca now, alongside their marker types.

// ===== compute synchronization =====
// barrier() (OpControlBarrier at Workgroup scope) and memoryBarrierShared() (OpMemoryBarrier) live
// in shader_lingua_franca (shared). Compute-only; the must-have sync points for shared-memory compute.

// ===== atomics =====
// Atomic read-modify-write on a @workgroup or @ssbo lvalue, returning the OLD value (GLSL/SPIR-V
// semantics). The target is the first argument, passed by reference so the emitter recovers its
// pointer; int and uint overloads cover signed/unsigned ops (atomicMin/Max pick SMin/UMin /
// SMax/UMax by the operand's signedness). Lowered to OpAtomicIAdd/SMin/.../Exchange/CompareExchange
// at Workgroup scope (shared) or Device scope (buffer). The stub bodies never run -- only the call
// AST is read. Use as `let old = atomicAdd(counter, 1)`.
// atomicAdd also takes a float (OpAtomicFAddEXT, GLSL's GL_EXT_shader_atomic_float), which pulls in
// OpCapability AtomicFloat32AddEXT + OpExtension "SPV_EXT_shader_atomic_float_add". It is the only
// float-capable family here: the float min/max opcodes sit behind a different capability, so a float
// target on any other atomic is a compile error rather than a blob only the driver would reject.
[unused_argument(value), sideeffects] def public atomicAdd(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicAdd(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicAdd(mem : float &; value : float) : float { return mem }
[unused_argument(value), sideeffects] def public atomicMin(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicMin(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicMax(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicMax(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicAnd(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicAnd(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicOr(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicOr(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicXor(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicXor(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicExchange(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicExchange(mem : uint &; value : uint) : uint { return mem }
// atomicCompSwap(mem, compare, value): if mem == compare, store value; returns the OLD value either
// way (OpAtomicCompareExchange). The lock-free CAS primitive.
[unused_argument(compare, value), sideeffects] def public atomicCompSwap(mem : int &; compare, value : int) : int { return mem }
[unused_argument(compare, value), sideeffects] def public atomicCompSwap(mem : uint &; compare, value : uint) : uint { return mem }

// ===== subgroup operations (Vulkan 1.1 / SPIR-V 1.3 GroupNonUniform*) =====
// Cross-lane cooperation within one subgroup (warp/wave). The reduce family folds a value across all
// active lanes and every lane receives the result; broadcastFirst propagates the lowest active lane's
// value while broadcast/shuffle select a lane by id and shuffleXor exchanges across a butterfly mask
// (lane ^ mask); elect() is true on exactly one active lane (single-lane writeback guard). Lowered to
// OpGroupNonUniform* at Subgroup scope. subgroupMin/subgroupMax pick the S/U/F opcode from the
// operand's scalar class, exactly as the atomics above do. The value-taking ops are generic over the
// 32-bit scalars and their 2/3/4 vectors; any other operand type is rejected with a clean error at
// emit. The stub bodies model the one-invocation view (identity / true) and never run in a shader --
// only the call AST is read.
[sideeffects] def public subgroupAll(pred : bool) : bool { return pred }
[sideeffects] def public subgroupAny(pred : bool) : bool { return pred }
[sideeffects] def public subgroupBroadcastFirst(v : auto(TT)) : TT { return v }
[unused_argument(id), sideeffects] def public subgroupBroadcast(v : auto(TT); id : uint) : TT { return v }
[unused_argument(id), sideeffects] def public subgroupShuffle(v : auto(TT); id : uint) : TT { return v }
[unused_argument(mask), sideeffects] def public subgroupShuffleXor(v : auto(TT); mask : uint) : TT { return v }
[sideeffects] def public subgroupAdd(v : auto(TT)) : TT { return v }
[sideeffects] def public subgroupMin(v : auto(TT)) : TT { return v }
[sideeffects] def public subgroupMax(v : auto(TT)) : TT { return v }
[sideeffects] def public subgroupElect() : bool { return true }
// cluster_size partitions the subgroup into equal clusters and reduces within each. SPIR-V requires the
// ClusterSize operand to be a constant instruction that is a power of two, so the emitter demands a
// compile-time constant here rather than letting an invalid blob reach spirv-val.
[unused_argument(cluster_size), sideeffects]
def public subgroupClusteredAdd(v : auto(TT); cluster_size : uint) : TT { return v }

// The subgroup builtins (gl_Subgroup*, all uint Inputs) live in shader_lingua_franca (shared —
// MSL lowers the same names to simdgroup attributes). Emitter-enforced SPIR-V stage split:
// Size/InvocationID read anywhere; gl_SubgroupID / gl_NumSubgroups only in compute/mesh/task.

// unpack8/pack32/sdot4/usdot4 live in daslib/shader_lingua_franca (re-exported above) — the
// emitter lowers them by name: OpBitcast for the packs, OpSDot/OpSUDot for the dots.

// ===== mesh + task shader minimum intrinsics (VK_EXT_mesh_shader) =====
// SetMeshOutputsEXT(vertex_count, primitive_count): the mesh-shader prologue. The SPV_EXT_mesh_shader
// spec requires this call once per workgroup at workgroup-uniform control flow BEFORE any access to
// gl_MeshVerticesEXT / gl_PrimitiveTriangleIndicesEXT or any other per-vertex/per-primitive output --
// it tells the implementation how much output storage to allocate. Mesh-stage-only; stub body never
// runs. Per-vertex/per-primitive output array surfaces land in PR-E.
[unused_argument(vertex_count, primitive_count), sideeffects]
def public SetMeshOutputsEXT(vertex_count : uint; primitive_count : uint) : void {}

// EmitMeshTasksEXT(x, y, z): the task-shader (amplification) workgroup terminator. Dispatches the
// listed grid of mesh shader workgroups (x*y*z) downstream of THIS task workgroup. Per the spec it
// ALSO terminates the current task workgroup (behaves like OpReturn / OpKill), so anything after the
// call on the same control-flow path is unreachable. Task-stage-only; stub body never runs.
[unused_argument(x, y, z), sideeffects]
def public EmitMeshTasksEXT(x : uint; y : uint; z : uint) : void {}

// ===== mesh per-vertex / per-primitive output arrays (VK_EXT_mesh_shader) =====
// gl_MeshVerticesEXT is the per-vertex output: an array (sized by the [mesh_shader] max_vertices arg)
// of a block whose gl_Position lands in clip space. gl_PrimitiveTriangleIndicesEXT is the per-primitive
// connectivity: an array (sized by max_primitives) of uint3, each a triangle's three indices into the
// vertex array. The emitter recognizes BOTH by name in classify_global and emits the real Output arrays
// (sized from the config, BuiltIn-decorated); the daslang `array<>` decls exist only so
// `gl_MeshVerticesEXT[i].gl_Position = ...` and `gl_PrimitiveTriangleIndicesEXT[i] = uint3(...)`
// typecheck. Mesh-stage-only; never executed — only the AST is read.
struct gl_MeshPerVertexEXT {
    gl_Position : float4
}
var gl_MeshVerticesEXT             : array<gl_MeshPerVertexEXT>
var gl_PrimitiveTriangleIndicesEXT : array<uint3>

// gl_MeshPrimitivesEXT is the per-primitive builtin output block (the per-primitive analog of
// gl_MeshVerticesEXT): an array (sized by max_primitives) of a block whose gl_PrimitiveID the fragment
// can read (`in int gl_PrimitiveID`). Written `gl_MeshPrimitivesEXT[i].gl_PrimitiveID = …`. The emitter
// recognizes it by name and emits the real PerPrimitiveEXT-decorated Output array; the daslang decls
// exist only so the indexing + field access typecheck. (gl_Layer / gl_ViewportIndex / gl_CullPrimitiveEXT
// join the block in a follow-up.) gl_PrimitiveID is also a fragment Input builtin (read per fragment).
struct gl_MeshPerPrimitiveEXT {
    gl_PrimitiveID : int
}
var gl_MeshPrimitivesEXT : array<gl_MeshPerPrimitiveEXT>

// ===== ray tracing (VK_KHR_ray_tracing_pipeline) =====
// The RT-pipeline minimum surface: the launch grid + world-ray + hit-state builtins, the opaque
// top-level acceleration structure descriptor, and traceRayEXT. Stage legality (which builtin is
// visible in raygen / miss / closest-hit) is enforced at classification in spirv_emit; a payload is
// declared as a `@ray_payload` (traced FROM this stage) or `@incoming_ray_payload` (what THIS stage
// was invoked with) global, a triangle hit attribute as `@hit_attribute barycentrics : float2`.
// AnyHit / Intersection / Callable stages (and their builtins) land with their first consumer.
var gl_LaunchIDEXT             : uint3      // BuiltIn LaunchIdKHR (Input) — this invocation's cell in the vkCmdTraceRaysKHR launch grid
var gl_LaunchSizeEXT           : uint3      // BuiltIn LaunchSizeKHR (Input) — the launch grid dimensions
var gl_WorldRayOriginEXT       : float3     // BuiltIn WorldRayOriginKHR (Input, miss / closest-hit) — the current ray's world-space origin
var gl_WorldRayDirectionEXT    : float3     // BuiltIn WorldRayDirectionKHR (Input, miss / closest-hit) — the current ray's world-space direction
var gl_HitTEXT                 : float      // BuiltIn RayTmaxKHR (Input, closest-hit) — the hit distance t along the ray
var gl_InstanceCustomIndexEXT  : int        // BuiltIn InstanceCustomIndexKHR (Input, closest-hit) — the TLAS instance's 24-bit custom index

// The opaque top-level acceleration structure descriptor (VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
// on the host side). Declared with the usual `@set` / `@binding` slots:
//     var @set = 0 @binding = 0 tlas : accelerationStructureEXT
struct accelerationStructureEXT {}

// traceRayEXT: OpTraceRayKHR. Traces one ray against `accel`; the closest-hit / miss shader selected
// through the SBT (sbt_offset / sbt_stride / miss_index) writes its result into `payload`, which MUST
// be a @ray_payload / @incoming_ray_payload global (the SPIR-V operand names the payload variable).
// ray_flags: SPIR-V RayFlags bits (0x01 opaque, 0x04 terminate-on-first-hit, 0x08 skip-closest-hit, ...).
// The stub body never runs -- only the call AST is read.
[unused_argument(accel, ray_flags, cull_mask, sbt_offset, sbt_stride, miss_index, origin, tmin, direction, tmax, payload), sideeffects]
def public traceRayEXT(accel : accelerationStructureEXT; ray_flags : uint; cull_mask : uint;
                       sbt_offset : uint; sbt_stride : uint; miss_index : uint;
                       origin : float3; tmin : float; direction : float3; tmax : float;
                       var payload : auto(TT)&) : void {}

// ===== inline ray query (VK_KHR_ray_query) =====
// A ray query traces a ray against a TLAS from ANY stage (here: a compute shader), with no SBT and no
// hit/miss shaders -- the caller drives it with a loop. The rayQueryEXT is an opaque LOCAL object
// (OpTypeRayQueryKHR, Function storage), initialized against an accelerationStructureEXT and stepped
// by rayQueryProceedEXT until it returns false, after which the rayQueryGetIntersection* accessors
// read the committed hit. The emitter recognizes each name in visitExprCall and lowers it to the
// matching OpRayQuery* opcode (SPV_KHR_ray_query / OpCapability RayQueryKHR); the stub bodies never
// run -- only the call AST is read. `committed` is GLSL's bool (true = the committed intersection);
// glslang lowers it to a 32-bit int operand, which the emitter emits.
struct rayQueryEXT {}

// The GLSL ray-query enum constants (gl_RayFlags*EXT, gl_RayQueryCommittedIntersectionNoneEXT) are
// plain uints the CONSUMER declares -- a consumer that needs gl_RayFlagsOpaqueEXT defines it in its
// own modules, so declaring them here too would collide. A module-scope `let` uint reference folds to an
// OpConstant with no help from spirv_builtins.

[unused_argument(rq, accel, ray_flags, cull_mask, origin, tmin, direction, tmax), sideeffects]
def public rayQueryInitializeEXT(var rq : rayQueryEXT; accel : accelerationStructureEXT;
                                 ray_flags : uint; cull_mask : uint; origin : float3; tmin : float;
                                 direction : float3; tmax : float) : void {}
[unused_argument(rq), sideeffects]
def public rayQueryProceedEXT(var rq : rayQueryEXT) : bool { return false }
[unused_argument(rq, committed), sideeffects]
def public rayQueryGetIntersectionTypeEXT(var rq : rayQueryEXT; committed : bool) : uint { return 0u }
[unused_argument(rq, committed), sideeffects]
def public rayQueryGetIntersectionInstanceCustomIndexEXT(var rq : rayQueryEXT; committed : bool) : int { return 0 }
[unused_argument(rq, committed), sideeffects]
def public rayQueryGetIntersectionPrimitiveIndexEXT(var rq : rayQueryEXT; committed : bool) : int { return 0 }
[unused_argument(rq, committed), sideeffects]
def public rayQueryGetIntersectionBarycentricsEXT(var rq : rayQueryEXT; committed : bool) : float2 { return float2(0.0, 0.0) }
[unused_argument(rq, committed), sideeffects]
def public rayQueryGetIntersectionTEXT(var rq : rayQueryEXT; committed : bool) : float { return 0.0 }

// ===== cooperative matrices (VK_KHR_cooperative_matrix — Tensor-Core MMA) =====
// A coopmat is a subgroup-scoped MMA tile (OpTypeCooperativeMatrixKHR) held in a Function-storage LOCAL,
// unlike a texture (a descriptor). It is a whole-tile VALUE — loaded, accumulated and stored as a unit
// (so declare_local_var treats it as a loadable local, not is_opaque like rayQueryEXT). Each marker
// struct names one concrete (component, use, tile) combination the emitter recognizes by name
// (coopmat_info) -> OpTypeCooperativeMatrixKHR. v1 covers the f16xf16 -> f32 GEMM tile at 16x16: A (MxK)
// and B (KxN) inputs, an f32 accumulator (MxN). A bare `var` coopmat local is zero-initialized
// (OpConstantNull); coopmatLoad fills A/B from an SSBO, coopmatMulAdd accumulates, coopmatStore writes
// the result out. The stub bodies never run — only the call AST is read (declare_local_var + the
// visitExprCall coopmat arms do the lowering). `layout` is a compile-time constant: 0 = row-major, 1 =
// column-major (SpvCooperativeMatrixLayout).
struct coopmatA_f16_16x16 {}
struct coopmatB_f16_16x16 {}
struct coopmatAcc_f32_16x16 {}

// `src` / `dst` are untyped: the emitter reads the buffer operand STRUCTURALLY (its @ssbo or @workgroup
// global + the element index), so the param type is irrelevant to lowering and this one signature accepts
// both a dynamic @ssbo array<T> and a fixed @workgroup T[N] (shared-memory staging). dst (the coopmat
// tile) and a/b/c stay strongly typed — that is what selects the tile shape.
[unused_argument(dst, src, idx, stride, layout), sideeffects]
def public coopmatLoad(var dst : coopmatA_f16_16x16; src; idx, stride, layout : int) : void {}
[unused_argument(dst, src, idx, stride, layout), sideeffects]
def public coopmatLoad(var dst : coopmatB_f16_16x16; src; idx, stride, layout : int) : void {}
[unused_argument(mat, dst, idx, stride, layout), sideeffects]
def public coopmatStore(mat : coopmatAcc_f32_16x16; dst; idx, stride, layout : int) : void {}
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatA_f16_16x16; b : coopmatB_f16_16x16; c : coopmatAcc_f32_16x16) : coopmatAcc_f32_16x16 { return c }

// The f16-ACCUMULATOR variant (llama.cpp's NV default): halves accumulator register pressure so
// big-tile kernels (many fragments per subgroup) stay register-resident. coopmatConvert widens the
// accumulated tile to f32 (componentwise OpFConvert) for the f32 store path.
struct coopmatAcc_f16_16x16 {}

[unused_argument(mat, dst, idx, stride, layout), sideeffects]
def public coopmatStore(mat : coopmatAcc_f16_16x16; dst; idx, stride, layout : int) : void {}
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatA_f16_16x16; b : coopmatB_f16_16x16; c : coopmatAcc_f16_16x16) : coopmatAcc_f16_16x16 { return c }
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst : coopmatAcc_f32_16x16; src : coopmatAcc_f16_16x16) : void {}

// The native INT8 path (s8 x s8 -> s32), the Q8-weight tile: K=32 (double the f16 tile), A is 16x32 and
// B is 32x16, s32 accumulator (16x16). Loading Q8_0 weights as s8 coopmat tiles feeds the tensor cores
// directly, no dequant. The integer MulAdd carries a signed-components operands mask (added by the emitter).
struct coopmatA_s8_16x32 {}
struct coopmatB_s8_32x16 {}
struct coopmatAcc_s32_16x16 {}

// idx also takes uint: s8 element offsets run 4x larger than the uint-word offsets the packed
// kernels index by, so a >2GB weight arena overflows int32 while still fitting uint.
[unused_argument(dst, src, idx, stride, layout), sideeffects]
def public coopmatLoad(var dst : coopmatA_s8_16x32; src; idx : int | uint; stride, layout : int) : void {}
[unused_argument(dst, src, idx, stride, layout), sideeffects]
def public coopmatLoad(var dst : coopmatB_s8_32x16; src; idx : int | uint; stride, layout : int) : void {}
[unused_argument(mat, dst, idx, stride, layout), sideeffects]
def public coopmatStore(mat : coopmatAcc_s32_16x16; dst; idx, stride, layout : int) : void {}
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatA_s8_16x32; b : coopmatB_s8_32x16; c : coopmatAcc_s32_16x16) : coopmatAcc_s32_16x16 { return c }

// ===== cooperative matrices v2 (SPV_NV_cooperative_matrix2 — tensor addressing, workgroup scope) =====
// A cm2 tile is WORKGROUP-scoped (the whole workgroup owns one MxN tile; the driver manages staging)
// and is addressed through tensor LAYOUT objects (dims + strides + a per-load slice window + an
// optional q8-style block size) instead of a flat (idx, stride) pair. The wg markers are name-parsed:
// coopmatWg{A|B|Acc}_{f16|f32|s8|s32}_{R}x{C} — declare a new struct here and coopmat_info picks it up.
struct coopmatWgA_f16_64x16 {}
struct coopmatWgB_f16_16x16 {}
struct coopmatWgB_f16_16x32 {}
struct coopmatWgB_f16_16x64 {}
struct coopmatWgAcc_f16_64x16 {}
struct coopmatWgAcc_f16_64x32 {}
struct coopmatWgAcc_f16_64x64 {}
struct coopmatWgAcc_f32_64x16 {}
struct coopmatWgAcc_f32_64x32 {}
struct coopmatWgAcc_f32_64x64 {}
// the l-tile GEMM shapes (llama.cpp's cm2 quant "l" geometry: BM=128, BN=256, BK=64)
struct coopmatWgA_f16_128x64 {}
struct coopmatWgB_f16_64x256 {}
struct coopmatWgAcc_f16_128x256 {}
struct coopmatWgAcc_f32_128x256 {}
// the m-tile siblings (llama.cpp's cm2 quant "m": BN=128, same A / BK)
struct coopmatWgB_f16_64x128 {}
struct coopmatWgAcc_f16_128x128 {}
struct coopmatWgAcc_f32_128x128 {}

// flash-attention tiles (Br=Bc=64, head sizes 64 / 128): Q & O accumulators, Qf16/P as A, K^T/V/One as B
struct coopmatWgA_f16_64x64 {}
struct coopmatWgA_f16_64x128 {}
struct coopmatWgB_f16_64x64 {}
struct coopmatWgB_f16_128x64 {}
struct coopmatWgAcc_f32_64x128 {}
// the int8 twins (q8 x q8 -> s32 at workgroup scope; K-tile 32)
struct coopmatWgA_s8_64x32 {}
struct coopmatWgB_s8_32x16 {}
struct coopmatWgB_s8_32x32 {}
struct coopmatWgB_s8_32x64 {}
struct coopmatWgAcc_s32_64x16 {}
struct coopmatWgAcc_s32_64x32 {}
struct coopmatWgAcc_s32_64x64 {}

// wg-scope MulAdd twins (the emitter arm is shape-agnostic; these type the das call)
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x16; b : coopmatWgB_f16_16x16; c : coopmatWgAcc_f16_64x16) : coopmatWgAcc_f16_64x16 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x16; b : coopmatWgB_f16_16x32; c : coopmatWgAcc_f16_64x32) : coopmatWgAcc_f16_64x32 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x16; b : coopmatWgB_f16_16x64; c : coopmatWgAcc_f16_64x64) : coopmatWgAcc_f16_64x64 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x16; b : coopmatWgB_f16_16x16; c : coopmatWgAcc_f32_64x16) : coopmatWgAcc_f32_64x16 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x16; b : coopmatWgB_f16_16x32; c : coopmatWgAcc_f32_64x32) : coopmatWgAcc_f32_64x32 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x16; b : coopmatWgB_f16_16x64; c : coopmatWgAcc_f32_64x64) : coopmatWgAcc_f32_64x64 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_128x64; b : coopmatWgB_f16_64x256; c : coopmatWgAcc_f16_128x256) : coopmatWgAcc_f16_128x256 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_128x64; b : coopmatWgB_f16_64x256; c : coopmatWgAcc_f32_128x256) : coopmatWgAcc_f32_128x256 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_128x64; b : coopmatWgB_f16_64x128; c : coopmatWgAcc_f16_128x128) : coopmatWgAcc_f16_128x128 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_128x64; b : coopmatWgB_f16_64x128; c : coopmatWgAcc_f32_128x128) : coopmatWgAcc_f32_128x128 { return c }
// flash-attention shapes: S = Qf16 @ K^T (hsk 64 / 128), O += P @ V (hsv 64 / 128), rowsum = P @ One
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x64; b : coopmatWgB_f16_64x64; c : coopmatWgAcc_f32_64x64) : coopmatWgAcc_f32_64x64 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x128; b : coopmatWgB_f16_128x64; c : coopmatWgAcc_f32_64x64) : coopmatWgAcc_f32_64x64 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_f16_64x64; b : coopmatWgB_f16_64x128; c : coopmatWgAcc_f32_64x128) : coopmatWgAcc_f32_64x128 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_s8_64x32; b : coopmatWgB_s8_32x16; c : coopmatWgAcc_s32_64x16) : coopmatWgAcc_s32_64x16 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_s8_64x32; b : coopmatWgB_s8_32x32; c : coopmatWgAcc_s32_64x32) : coopmatWgAcc_s32_64x32 { return c }
[unused_argument(a, b), sideeffects]
def public coopmatMulAdd(a : coopmatWgA_s8_64x32; b : coopmatWgB_s8_32x64; c : coopmatWgAcc_s32_64x64) : coopmatWgAcc_s32_64x64 { return c }

// Tensor layout objects (Function-storage locals, like coopmat tiles). tensorLayout2D = clamp
// Undefined (the aligned fast path: every slice window must be fully in-bounds); tensorLayout2DPad =
// clamp Constant (out-of-bounds reads yield 0 — the guarded/edge path, llama.cpp's general arm).
struct tensorLayout2D {}
struct tensorLayout2DPad {}
// Tensor view objects: element permutation applied at load/store. tensorView2Dt = (1,0) TRANSPOSE
// (a KxN B-tile loading from an NxK-major panel); tensorView2D = (0,1) identity.
struct tensorView2D {}
struct tensorView2Dt {}

// The layout/view params are untyped (the emitter reads the LOCAL structurally and validates the
// marker type); dims/strides/offsets are uint <id> operands. Each setter reads the layout local,
// applies the op, and stores the new value back — author them as plain sequential statements.
[unused_argument(tl), sideeffects]
def public tensorLayoutCreate(var tl) : void {}
[unused_argument(tl, d0, d1), sideeffects]
def public tensorLayoutSetDimension(var tl; d0, d1 : uint) : void {}
[unused_argument(tl, s0, s1), sideeffects]
def public tensorLayoutSetStride(var tl; s0, s1 : uint) : void {}
[unused_argument(tl, b0, b1), sideeffects]
def public tensorLayoutSetBlockSize(var tl; b0, b1 : uint) : void {}
[unused_argument(tv), sideeffects]
def public tensorViewCreate(var tv) : void {}

// coopmatLoadTensor(dst, src, base, tl, r0, nr, c0, nc [, tv]): slice the (r0, nr, c0, nc) window off
// layout `tl` (OpTensorLayoutSliceNV — the base layout local is NOT modified; slicing per K-tile IS
// the loop), then OpCooperativeMatrixLoadTensorNV from &src[base]. `src` is an @ssbo global; `base`
// an element index. The optional trailing view permutes (transposed B panels). The DECODE form takes
// `@@fn` of a [spirv_decode] function as its last argument: src elements are then opaque BLOCKS
// (e.g. gguf q8_0 {f16 d; u16 qs[16]}) decoded in-load — pair it with tensorLayoutSetBlockSize.
[unused_argument(dst, src, base, tl, r0, nr, c0, nc), sideeffects]
def public coopmatLoadTensor(var dst; src; base : int | uint; var tl; r0, nr, c0, nc : uint) : void {}
[unused_argument(dst, src, base, tl, r0, nr, c0, nc, tv), sideeffects]
def public coopmatLoadTensor(var dst; src; base : int | uint; var tl; r0, nr, c0, nc : uint; var tv) : void {}
[unused_argument(dst, src, base, tl, r0, nr, c0, nc, decode), sideeffects]
def public coopmatLoadTensorDecode(var dst; src; base : int | uint; var tl; r0, nr, c0, nc : uint; decode) : void {}
[unused_argument(mat, dst, base, tl, r0, nr, c0, nc), sideeffects]
def public coopmatStoreTensor(mat; var dst; base : int | uint; var tl; r0, nr, c0, nc : uint) : void {}
[unused_argument(mat, dst, base, tl, r0, nr, c0, nc, tv), sideeffects]
def public coopmatStoreTensor(mat; var dst; base : int | uint; var tl; r0, nr, c0, nc : uint; var tv) : void {}

// Per-element clamp of a float accumulator tile to [lo, hi] — the f16-accumulate overflow guard
// (llama.cpp clamps to ±65504 after the K loop). Emits the whole Length/AccessChain/FClamp loop.
[unused_argument(mat, lo, hi), sideeffects]
def public coopmatClamp(var mat; lo, hi : float) : void {}

// wg-scope f16 -> f32 accumulator widening (whole-tile OpFConvert, same arm as the subgroup form)
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst : coopmatWgAcc_f32_64x16; src : coopmatWgAcc_f16_64x16) : void {}
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst : coopmatWgAcc_f32_64x32; src : coopmatWgAcc_f16_64x32) : void {}
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst : coopmatWgAcc_f32_64x64; src : coopmatWgAcc_f16_64x64) : void {}
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst : coopmatWgAcc_f32_128x256; src : coopmatWgAcc_f16_128x256) : void {}
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst : coopmatWgAcc_f32_128x128; src : coopmatWgAcc_f16_128x128) : void {}
// The generic form covers every other float width/use conversion pair (validated at emission): a
// same-use width change is a plain componentwise convert; a USE-changing pair (f32 accumulator ->
// f16 A, the flash-attention Q/P path) additionally requires the conversions device feature.
[unused_argument(dst, src), sideeffects]
def public coopmatConvert(var dst; src) : void {}

// ===== cm2 reductions / per-element ops (flash attention: online softmax) =====
// coopmatReduce(dst, src, mask, @@fn): OpCooperativeMatrixReduceNV — combine `src`'s elements along
// rows (COOPMAT_REDUCE_ROW), columns (COOPMAT_REDUCE_COLUMN), or both, with a [spirv_combine]
// function ((T, T) -> T, T = the component type); the reduced value is broadcast across the result.
// A ROW reduce may RESIZE columns (dst cols free — llama.cpp's smear-reduce Br x Bc -> Br x HSV),
// and symmetrically a COLUMN reduce may resize rows. `mask` is a compile-time constant.
let public COOPMAT_REDUCE_ROW = 1u
let public COOPMAT_REDUCE_COLUMN = 2u
[unused_argument(dst, src, mask, fn), sideeffects]
def public coopmatReduce(var dst; src; mask : uint; fn) : void {}

// coopmatPerElement(dst, src, @@fn, extras...): OpCooperativeMatrixPerElementOpNV — dst[r,c] =
// fn(r, c, src[r,c], extras...) with a [spirv_per_element] function. A coopmat extra must match
// src's shape and arrives as its (r,c) component; a scalar extra arrives as-is. dst and src are
// the same tile type (dst may alias src).
[unused_argument(dst, src, fn), sideeffects]
def public coopmatPerElement(var dst; src; fn) : void {}
[unused_argument(dst, src, fn, e0), sideeffects]
def public coopmatPerElement(var dst; src; fn; e0) : void {}
[unused_argument(dst, src, fn, e0, e1), sideeffects]
def public coopmatPerElement(var dst; src; fn; e0, e1) : void {}
[unused_argument(dst, src, fn, e0, e1, e2), sideeffects]
def public coopmatPerElement(var dst; src; fn; e0, e1, e2) : void {}
[unused_argument(dst, src, fn, e0, e1, e2, e3), sideeffects]
def public coopmatPerElement(var dst; src; fn; e0, e1, e2, e3) : void {}

// ===== componentwise coopmat arithmetic (SPV_KHR_cooperative_matrix extends the scalar opcodes) =====
// dst = a OP b, all three the same float tile type (dst may alias a or b). coopmatMulElem is the
// COMPONENTWISE (Hadamard) product — matrix multiply stays coopmatMulAdd.
[unused_argument(dst, a, b), sideeffects]
def public coopmatAdd(var dst; a; b) : void {}
[unused_argument(dst, a, b), sideeffects]
def public coopmatSub(var dst; a; b) : void {}
[unused_argument(dst, a, b), sideeffects]
def public coopmatMulElem(var dst; a; b) : void {}
// mat *= s (OpMatrixTimesScalar; s converts to the component width)
[unused_argument(mat, s), sideeffects]
def public coopmatScale(var mat; s : float) : void {}
// dst = src, same tile type (plain das assignment does not move coopmat locals — the markers are
// empty phantom structs; llama.cpp's `Mold = M` snapshot before the online-softmax max update)
[unused_argument(dst, src), sideeffects]
def public coopmatCopy(var dst; src) : void {}
// every element of `mat` = v (a `var` tile declaration zero-fills; this is the non-zero fill)
[unused_argument(mat, v), sideeffects]
def public coopmatFill(var mat; v : float) : void {}
