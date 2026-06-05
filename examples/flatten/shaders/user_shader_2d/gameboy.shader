require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color darkColor = float3(0.06, 0.22, 0.06)
    @color lightColor = float3(0.47, 0.73, 0.24)
}

[shader(stage="pixel", type="pbr")]
def gameboy(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let gray = dot(c, float3(0.299, 0.587, 0.114))
    let level = floor(gray * 4.0) / 4.0
    let result = lerp(darkColor, lightColor, level)
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
