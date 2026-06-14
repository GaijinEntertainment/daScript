options gen2

// Shader builtin globals, recognized by name in the emitter (mirrors dasGlsl's glsl_common
// gl_* declarations). They exist only so authored shader code typechecks; the shader body is
// never executed in daslang — only its AST is read and lowered to SPIR-V. Each maps to a
// BuiltIn-decorated variable in spirv_emit.das — Input for read-only builtins (the compute IDs,
// gl_VertexIndex/gl_InstanceIndex, gl_FragCoord), Output for gl_Position (the one written builtin).

module spirv_builtins shared public

// compute
var gl_NumWorkGroups        : uint3     // OpEntryPoint NumWorkgroups
var gl_WorkGroupID          : uint3     // WorkgroupId
var gl_LocalInvocationID    : uint3     // LocalInvocationId
var gl_GlobalInvocationID   : uint3     // GlobalInvocationId
var gl_LocalInvocationIndex : uint      // LocalInvocationIndex

// vertex (gl_Position is the one Output builtin; the indices are Inputs)
var gl_Position             : float4    // BuiltIn Position (Output)
var gl_VertexIndex          : int       // BuiltIn VertexIndex (Input)
var gl_InstanceIndex        : int       // BuiltIn InstanceIndex (Input)

// fragment
var gl_FragCoord            : float4    // BuiltIn FragCoord (Input)

// ===== combined image samplers =====
// sampler2D is an opaque marker type: a global of this type lowers to an OpTypeSampledImage in
// UniformConstant storage (the emitter recognizes it by name in classify_global). The struct has no
// fields — shader code never reads it directly, only passes it to texture(). texture() is likewise a
// stub: its body is never executed (only the AST is read), and the call lowers to OpImageSampleImplicitLod.
struct sampler2D {}

[unused_argument(s, uv)]
def public texture(s : sampler2D; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}
