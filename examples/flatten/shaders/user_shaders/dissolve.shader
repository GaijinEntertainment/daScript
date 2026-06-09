require engine.render.shader_dsl

var {
    @color baseColor = float3(0.7, 0.7, 0.75)
    @color edgeColor = float3(2.0, 0.6, 0.1)
    @color edgeHotColor = float3(3.0, 2.5, 0.5)
    edgeWidth = 0.08
    noiseScale = 4.0
}

[pixel_shader]
def dissolve(inp : PbrInput) {
    let n = noise(inp.worldPos * noiseScale)

    let animatedAmount = remap(sin(g_Time * 0.7), float2(-1.0, 1.0), float2(0.1, 0.9))

    let cutoffMask = step(animatedAmount, n)
    let edgeMask = smooth_step(animatedAmount, animatedAmount + edgeWidth, n)
    let hotMask = smooth_step(animatedAmount, animatedAmount + edgeWidth * 0.4, n)

    let edgeGlow = lerp(edgeHotColor, edgeColor, edgeMask)
    let surface = lerp(edgeGlow, baseColor, edgeMask)

    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = surface * cutoffMask,
        alpha = cutoffMask + edgeMask * one_minus(cutoffMask),
        alphaCutoff = 0.5,
        emission = edgeGlow * one_minus(hotMask),
        emissionStrength = one_minus(hotMask) * lerp(0.8, 2.0, daylight),
        metalness = 0.0,
        roughness = 0.8,
        ao = 1.0
    )
}
