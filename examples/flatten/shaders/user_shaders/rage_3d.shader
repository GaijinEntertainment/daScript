require engine.render.shader_dsl

var {
    @color baseColor = float3(0.6, 0.05, 0.05)
    @color rageColor = float3(2.0, 0.2, 0.0)
    @color coreColor = float3(2.5, 1.0, 0.0)
    pulseSpeed = 3.0
    noiseScale = 3.5
}

[pixel_shader]
def rage_3d(inp : PbrInput) {
    let t = g_Time * pulseSpeed
    let n = noise(inp.worldPos * noiseScale + float3(0, t * 0.3, 0))
    let pulse = sin(t) * 0.5 + 0.5
    let vein = step(0.68, n)
    let glow = smooth_step(0.55, 0.68, n)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = baseColor,
        emission = coreColor * vein * pulse + rageColor * glow,
        emissionStrength = (vein * pulse + glow * 0.5) * lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 0.8,
        ao = 1.0
    )
}
