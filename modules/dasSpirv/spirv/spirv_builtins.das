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
