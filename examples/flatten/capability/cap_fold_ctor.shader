require engine.render.shader_dsl

// Capability demo: a constant vector constructor. The typer folds scalar const
// arithmetic during re-infer but leaves const CONSTRUCTORS — `float3(0.5)` stays a
// 3-arg ctor call, never a const. flatten's fold collapses it to a single const, so
// the graph carries a `float3Const` node (proof the ctor folded) instead of a vector
// constructor fed by three floatConst inputs.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_fold_ctor(inp : PbrInput) : PbrOutput {
    let base = tex2d(albedo_texture, inp.uv).xyz
    let result = base + float3(0.5)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
