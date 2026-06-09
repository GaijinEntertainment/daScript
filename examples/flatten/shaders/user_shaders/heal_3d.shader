require engine.render.shader_dsl

var {
    @color baseColor = float3(0.1, 0.4, 0.15)
    @color healColor = float3(0.4, 2.0, 0.5)
    @color coreColor = float3(1.5, 2.5, 1.0)
    riseSpeed = 1.2
    noiseScale = 4.0
}

[pixel_shader]
def heal_3d(inp : PbrInput) {
    let t = g_Time * riseSpeed
    let n = noise(inp.worldPos * noiseScale + float3(0, -t, 0))
    let h = inp.localPos.y * 0.5 + 0.5
    let particle = step(0.72, n) * h
    let glow = smooth_step(0.55, 0.72, n) * h
    let pulse = sin(g_Time * 3.0) * 0.3 + 0.7
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = lerp(baseColor, healColor * 0.3, glow),
        emission = coreColor * particle * pulse + healColor * glow * 0.5,
        emissionStrength = (particle * pulse + glow * 0.3) * lerp(0.2, 1.5, daylight),
        metalness = 0.0,
        roughness = 0.8,
        ao = 1.0
    )
}

