require engine.render.shader_dsl

var {
    @color iceColor = float3(0.5, 0.85, 1.0)
    @color frostColor = float3(0.9, 0.97, 1.0)
    freezeAmount = 0.65
    noiseScale = 5.0
}

[pixel_shader]
def freeze_3d(inp : PbrInput) {
    let n = noise(inp.worldPos * noiseScale)
    let iceMask = smooth_step(1.0 - freezeAmount - 0.1, 1.0 - freezeAmount + 0.2, n)
    let crystal = step(0.78, n) * iceMask
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = lerp(float3(0.3, 0.3, 0.4), iceColor, iceMask),
        emission = frostColor * crystal + iceColor * iceMask * 0.2,
        emissionStrength = (crystal + iceMask * 0.3) * lerp(0.3, 1.2, daylight),
        metalness = 0.0,
        roughness = lerp(0.8, 0.05, iceMask),
        ao = 1.0
    )
}
