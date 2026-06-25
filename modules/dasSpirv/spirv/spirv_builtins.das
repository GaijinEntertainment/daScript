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

// ===== storage images =====
// image2D + imageStore/imageLoad/imageSize live in shader_lingua_franca (shared). The emitter
// recognizes image2D by name in classify_global and lowers it to a UniformConstant OpTypeImage with
// a known Rgba8 format (no StorageImageRead/WriteWithoutFormat capability needed; lavapipe-safe).

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
[unused_argument(value), sideeffects] def public atomicAdd(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicAdd(mem : uint &; value : uint) : uint { return mem }
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
