require engine.render.shader_dsl

// Capability demo: regroup-to-share. Reassoc canonicalizes the alpha sum `1 - mask + edge` to
// `(edge - mask) + 1`, splitting the `1 - mask` the albedo line still carries whole — two sub
// nodes where the source shares one. The regroup re-pairs the sum toward the live `1 - mask`
// twin, CSE shares it, and the backend gets ONE sub node feeding both consumers.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_regroup(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let mask = saturate(inp.uv.y * 2.0 - 0.3)
    let edge = saturate(inp.uv.x)
    return PbrOutput(
        albedo = c * (1.0 - mask),
        emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0,
        alpha = 1.0 - mask + edge
    )
}
