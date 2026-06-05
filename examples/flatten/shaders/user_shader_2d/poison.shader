require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color poisonColor = float3(0.2, 1.0, 0.1)
    pulseSpeed = 2.0
    strength = 0.7
}

[shader(stage="pixel", type="pbr")]
def poison(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let pulse = sin(g_Time * pulseSpeed) * 0.5 + 0.5
    let n = noise(inp.uv * 4.0 + float2(g_Time * 0.3, 0.0))
    let mask = n * pulse
    let result = lerp(c, poisonColor * (0.5 + mask * 0.5), strength)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = poisonColor * mask * strength,
        emissionStrength = mask * lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
