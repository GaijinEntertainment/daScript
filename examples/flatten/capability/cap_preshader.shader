require engine.render.shader_dsl

// Capability demo: preshader extraction. `factor` depends only on material props (brightness,
// tint) and literals — it is UNIFORM, identical for every pixel of a draw. The general compiler
// evaluates it per-pixel; flatten hoists the whole subtree to a top-of-body `_preshader_` let the
// backend routes to the per-draw preshader, so the per-pixel graph carries only `base * factor`.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    brightness = 1.5
    tint = float3(0.3, 0.6, 0.1)
}

[pixel_shader]
def cap_preshader(inp : PbrInput) : PbrOutput {
    let base = tex2d(albedo_texture, inp.uv).xyz
    let factor = brightness * 2.0 + dot(tint, float3(0.299, 0.587, 0.114))
    let result = base * factor
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
