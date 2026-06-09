require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    levels = 4.0
}

[pixel_shader]
def posterize(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let result = floor(c * levels) / levels
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
