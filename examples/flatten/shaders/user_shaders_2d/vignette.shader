require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    radius = 0.45
    softness = 0.55
}

[pixel_shader]
def vignette(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let uv = inp.uv - float2(0.5, 0.5)
    let dist = length(uv) * 1.6
    let vign = smooth_step(radius + softness, radius, dist)
    let result = c * vign
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
