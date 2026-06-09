require engine.render.shader_dsl

var {
    @color electricColor = float3(0.5, 0.8, 2.5)
    @color coreColor = float3(1.5, 1.8, 2.5)
    speed = 8.0
    noiseScale = 6.0
}

[pixel_shader]
def electric_3d(inp : PbrInput) {
    let t = g_Time * speed
    let n1 = noise(inp.worldPos * noiseScale + float3(t, 0, 0))
    let n2 = noise(inp.worldPos * noiseScale * 2.0 - float3(0, t, 0))
    let bolt = step(0.75, n1 * n2 * 2.0)
    let glow = smooth_step(0.5, 0.75, n1 * n2 * 2.0)
    let flicker = sin(t * 3.0) * 0.5 + 0.5
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = coreColor * bolt * flicker + electricColor * glow,
        emissionStrength = (bolt + glow * 0.5) * flicker * lerp(0.5, 2.5, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
