options gen2

// Shader builtin globals, recognized by name in the emitter (mirrors dasGlsl's glsl_common
// gl_* declarations). They exist only so authored shader code typechecks; the shader body is
// never executed in daslang — only its AST is read and lowered to SPIR-V. Each maps to a
// BuiltIn-decorated Input variable in spirv_emit.das.

module spirv_builtins shared public

// compute
var gl_NumWorkGroups        : uint3     // OpEntryPoint NumWorkgroups
var gl_WorkGroupID          : uint3     // WorkgroupId
var gl_LocalInvocationID    : uint3     // LocalInvocationId
var gl_GlobalInvocationID   : uint3     // GlobalInvocationId
var gl_LocalInvocationIndex : uint      // LocalInvocationIndex
