require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    grainStrength = 0.15
    grainScale = 200.0
    grainSpeed = 30.0
}

[shader(stage="pixel", type="pbr")]
def noise_grain(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let n = noise(inp.uv * grainScale + float2(g_Time * grainSpeed, 0.0))
    let result = c + float3(n - 0.5) * grainStrength
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
