expect 50501

options gen2

require glsl/glsl_opengl

// the subgroup IDs live in shader_lingua_franca for the SPIR-V/MSL rails; the GLSL rail has no
// subgroup lowering and must reject them by name instead of emitting an undeclared identifier
var @out fc_subgroup_color : float4

[fragment_program(name="fc_subgroup_ids_glsl")]
def fc_subgroup_ids_fragment {
    fc_subgroup_color = float4(float(gl_SubgroupInvocationID), float(gl_SubgroupSize),
        float(gl_NumSubgroups), float(gl_SubgroupID))
}

[export]
def main {
    print("{fc_subgroup_ids_glsl}\n")
}
