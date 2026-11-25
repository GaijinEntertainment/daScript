// Fail-closed fixture: coopmatElem reads a signed 32-bit accumulator.
// A module overload over a float accumulator passes the front end,
// reaches the intrinsic arm by name, and must be rejected at emit
// time with a clean diagnostic.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_cet_out : array<int>

[unused_argument(mat, e), sideeffects]
def coopmatElem(mat : coopmatAcc_f32_16x16; e : uint) : int => 0

[compute_shader(local_size_x=32, name="fc_coopmat_elem_type_spv")]
def fc_coopmat_elem_type {
    var acc : coopmatAcc_f32_16x16
    fc_cet_out[gl_LocalInvocationID.x] = coopmatElem(acc, 0u)
}
