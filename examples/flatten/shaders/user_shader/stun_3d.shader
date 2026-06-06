require engine.render.shader_dsl

var {
    @color baseColor = float3(0.3, 0.25, 0.05)
    @color starColor = float3(2.5, 2.0, 0.3)
    @color dimColor = float3(0.5, 0.4, 0.1)
    speed = 2.5
    noiseScale = 5.0
}

[shader(stage="pixel", type="pbr")]
def stun_3d(inp : PbrInput) : PbrOutput {
    let t = g_Time * speed
    let n1 = noise(inp.worldPos * noiseScale + float3(t, 0, 0))
    let n2 = noise(inp.worldPos * noiseScale * 2.0 + float3(0, t * 1.3, 0))
    let star = step(0.8, n1 * n2 * 2.5)
    let glow = smooth_step(0.6, 0.8, n1 * n2 * 2.5)
    let wobble = sin(t * 4.0 + inp.worldPos.y * 8.0) * 0.5 + 0.5
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = lerp(baseColor, dimColor, wobble),
        emission = starColor * star * wobble + dimColor * glow,
        emissionStrength = (star * wobble + glow * 0.3) * lerp(0.2, 1.5, daylight),
        metalness = 0.0,
        roughness = 0.9,
        ao = 1.0
    )
}
