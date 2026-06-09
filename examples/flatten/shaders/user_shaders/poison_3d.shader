require engine.render.shader_dsl

var {
    @color baseColor = float3(0.2, 0.4, 0.15)
    @color poisonColor = float3(0.3, 1.0, 0.1)
    @color bubbleColor = float3(0.5, 1.5, 0.2)
    pulseSpeed = 2.0
    noiseScale = 4.0
}

[pixel_shader]
def poison_3d(inp : PbrInput) {
    let pulse = sin(g_Time * pulseSpeed) * 0.5 + 0.5
    let n = noise(inp.worldPos * noiseScale + float3(0, g_Time * 0.3, 0))
    let bubble = step(0.72, n) * pulse
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = lerp(baseColor, poisonColor, n * 0.6),
        emission = bubbleColor * bubble + poisonColor * (n * 0.2 * pulse),
        emissionStrength = bubble * lerp(0.8, 2.0, daylight),
        metalness = 0.0,
        roughness = 0.7,
        ao = 1.0
    )
}
