require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color glowColor = float3(0.5, 1.5, 2.5)
    speed = 3.0
    intensity = 0.8
}

[shader(stage="pixel", type="pbr")]
def teleport(inp : PbrInput) : PbrOutput {
    let t = g_Time * speed
    let shake = sin(t * 13.0) * 0.008 * intensity
    let uv = inp.uv + float2(shake, sin(t * 7.0) * 0.005 * intensity)
    let c = tex2d(albedo_texture, uv).xyz
    let scanline = step(0.7, sin(inp.uv.y * 80.0 + t * 4.0) * 0.5 + 0.5)
    let flicker = sin(t * 23.0) * 0.5 + 0.5
    let result = c * (1.0 - scanline * 0.4 * intensity) * lerp(0.7, 1.0, flicker)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result + glowColor * scanline * intensity * 0.3,
        emissionStrength = lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
