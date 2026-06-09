require engine.render.shader_dsl

var {
    @color colorA = float3(0.1, 0.0, 0.3)
    @color colorB = float3(0.8, 0.4, 1.0)
    scale = 3.0
    speed = 0.2
}

[pixel_shader]
def fractal_noise(inp : PbrInput) {
    let t = g_Time * speed
    let uv = inp.uv * scale
    let n1 = noise(uv + float2(t, t * 0.7))
    let n2 = noise(uv * 2.1 + float2(t * 1.3, 0.0))
    let n = n1 * 0.6 + n2 * 0.4
    let result = lerp(colorA, colorB, n)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result * n,
        emissionStrength = lerp(0.3, 1.5, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
