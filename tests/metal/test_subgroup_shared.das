options gen2

require dastest/testing_boost public
require metal/metal_builtins   // nolint:STYLE029 — requiring BOTH builtins modules IS the pin
require spirv/spirv_builtins   // nolint:STYLE029 — see above

// The four gl_Subgroup* IDs are ONE declaration in shader_lingua_franca shared by both rails —
// requiring both builtins modules in one TU must stay unambiguous (each carrying its own copy
// was error[30615]). Reading and writing all four pins the single-declaration property; the
// width-1 defaults are what a CPU replay relies on.

[test]
def test_subgroup_ids_shared(t : T?) {
    t |> run("gl_Subgroup* resolve once across metal+spirv builtins") <| @(t : T?) {
        t |> equal(int(gl_SubgroupSize), 1)         // the width-1 CPU-replay default
        t |> equal(int(gl_SubgroupInvocationID), 0)
        gl_SubgroupSize = 4u
        gl_SubgroupInvocationID = 1u
        gl_NumSubgroups = 2u
        gl_SubgroupID = 1u
        t |> equal(int(gl_SubgroupSize + gl_SubgroupInvocationID + gl_NumSubgroups + gl_SubgroupID), 8)
        gl_SubgroupSize = 1u
        gl_SubgroupInvocationID = 0u
        gl_NumSubgroups = 1u
        gl_SubgroupID = 0u
    }
}
