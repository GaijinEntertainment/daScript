require engine.render.shader_dsl

// Capability demo: constant reassociation. The source spells a scattered-const additive chain
// `0.3 + gain + 0.4` on a runtime material prop. The general compiler can't fold it (the consts
// are non-adjacent and it never reassociates), and the backend has no const-fold — so without
// flatten this emits TWO add nodes. flatten reassociates under fast-math, gathering the consts
// into `gain + 0.7`, so the emitted graph carries exactly ONE add (and the one genuine `base * k`
// multiply).

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    gain = 1.0
}

[pixel_shader]
def cap_fold_reassoc(inp : PbrInput) : PbrOutput {
    let base = tex2d(albedo_texture, inp.uv).xyz
    let k = 0.3 + gain + 0.4
    let result = base * k
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
