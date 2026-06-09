require engine.render.shader_dsl

// Capability demo: common-subexpression elimination. `dot(c, LUMA)` is spelled TWICE (in `a` and
// `b`); the general compiler and the backend don't CSE, so without flatten this emits TWO dot
// nodes. flatten value-numbers the body and shares the repeat into one `_cse_` let, so the graph
// carries exactly ONE dot node feeding both consumers.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_cse(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let a = dot(c, float3(0.299, 0.587, 0.114)) * 2.0
    let b = dot(c, float3(0.299, 0.587, 0.114)) + 0.1
    let result = float3(a, b, a)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
